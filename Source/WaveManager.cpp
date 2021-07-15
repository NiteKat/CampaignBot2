#include "CampaignBot.h"

#include <queue>
// ------------------ PUBLIC FUNCTIONS ------------------ //
void WaveManager::onFrame()
{
  removeEmptyWaves();
  updateWaves();
}

void WaveManager::onStart()
{
  auto startWave = std::make_shared<WaveInfo>();
  waveList.insert(startWave);
}

// ------------------ PRIVATE FUNCTIONS ----------------- //

BWAPI::Region WaveManager::findFirstUnexploredRegion(BWAPI::Region startRegion)
{
  struct node
  {
    node() : myRegion(nullptr), parent(nullptr), cost(-1) {}
    node(BWAPI::Region region, node* parent, int cost) : myRegion(region), parent(parent), cost(cost) {}
    BWAPI::Region myRegion;
    node* parent;
    int cost;
  };
  
  std::queue<node> Q;
  std::map<BWAPI::Region, node> discovered;
  Q.push(node{ startRegion, nullptr, 0 });
  discovered[startRegion] = Q.front();
  while (Q.size())
  {
    auto v = Q.front();
    Q.pop();
    if (!BWAPI::Broodwar->isExplored(BWAPI::TilePosition(v.myRegion->getCenter())))
      return v.myRegion;
    for (auto& neighbor : v.myRegion->getNeighbors())
    {
      if (!discovered[neighbor].myRegion)
      {
        if (neighbor->getRegionGroupID() == v.myRegion->getRegionGroupID()) // this is for ground only
        {
          discovered[neighbor] = v;
          Q.push(node{ neighbor, &discovered[neighbor], v.cost + 1 });
        }
      }
    }
  }
  return nullptr;
}

void WaveManager::removeEmptyWaves()
{
  auto itr = waveList.begin();
  while (itr != waveList.end())
  {
    if (!(*itr)->getUnitCount() && (*itr)->isActive()) // remove if empty and active
      itr = waveList.erase(itr);
    else
      itr++;
  }
}

void WaveManager::updateWaves()
{
  bool needNewWave = false;
  for (auto& wave : waveList)
  {
    // Update wave information
    wave->updateWave();
    // Check if the wave has a target currently
    if (!wave->getTarget() && wave->isActive())
    {
      // See if any enemy structures are known.
      BWAPI::Region bestRegion = nullptr;
      double bestDist = DBL_MAX;
      for (auto& unit : bot->getUnitManager().getUnits(PlayerState::Enemy))
      {
        if (!unit->getType().isBuilding())
          continue;

        auto dist = unit->getDistance(wave->getCentroid());
        if (dist < bestDist)
        {
          bestDist = dist;
          bestRegion = BWAPI::Broodwar->getRegionAt(unit->getPosition());
        }
      }
      if (bestRegion) // Found a region with an enemy building in it.
        wave->setTarget(bestRegion);
      else // Unable to find a region with an enemy building in it, need to explore.
      {
        // Will use BFS to find a region whose center is not revealed.
        auto startRegion = BWAPI::Broodwar->getRegionAt(wave->getCentroid());
        if (!startRegion) // Somehow our centroid is not in a start region. If waves stall out will need a solution for this. Maybe fallback to start location?
          startRegion = BWAPI::Broodwar->getRegionAt(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()));
        wave->setTarget(findFirstUnexploredRegion(startRegion));
      }
    }
    else if (!wave->isActive())
    {
      // Check for units to add to the wave.
      auto& buildOrder = bot->getBuildOrder();
      for (auto& unit : bot->getUnitManager().getUnits(PlayerState::Self))
      {
        // Only consider combat units.
        if (unit->getRole() != Roles::Combat
          || unit->hasWave())
          continue;
        
        if (wave->getTypeCount(unit->getType()) < buildOrder.getComposition(unit->getType()))
          wave->addUnit(*unit);
      }
      // Are we ready to launch this wave?
      bool ready = true;
      for (auto& [type, count] : buildOrder.getComposition())
      {
        if (!count)
          continue;

        if (count != wave->getTypeCount(type))
          ready = false;

        if (!ready)
          break;
      }
      if (ready)
      {
        wave->setActive(true);
        needNewWave = true;

        // Should we advance to the next wave?
        switch (buildOrder.getWave())
        {
        case 1:
          if (BWAPI::Broodwar->self()->isUnitAvailable(BWAPI::UnitTypes::Terran_Factory)
              && BWAPI::Broodwar->self()->isUnitAvailable(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode))
            buildOrder.nextWave();
          break;
        case 2:
          buildOrder.nextWave();
          break;
        case 3:
          if (BWAPI::Broodwar->self()->isUnitAvailable(BWAPI::UnitTypes::Terran_Starport)
              && BWAPI::Broodwar->self()->isUnitAvailable(BWAPI::UnitTypes::Terran_Wraith))
            buildOrder.nextWave();
          break;
        case 4:
          if (BWAPI::Broodwar->self()->isUnitAvailable(BWAPI::UnitTypes::Terran_Battlecruiser))
            buildOrder.nextWave();
          break;
        default:
          break;
        }
      }
    }
  }
  if (needNewWave)
  {
    auto newWave = std::make_shared<WaveInfo>();
    waveList.insert(newWave);
  }
}