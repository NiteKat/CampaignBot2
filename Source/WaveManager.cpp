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

std::vector<BWAPI::Region> WaveManager::findFirstUnexploredRegion(BWAPI::Region startRegion)
{
  struct node
  {
    node() : myRegion(nullptr), parent(nullptr), cost(-1) {}
    node(BWAPI::Region region, node* parent, int cost) : myRegion(region), parent(parent), cost(cost) {}
    BWAPI::Region myRegion;
    node* parent;
    int cost;
  };
  std::vector<BWAPI::Region> path;
  
  std::queue<node> Q;
  std::map<BWAPI::Region, node> discovered;
  Q.push(node{ startRegion, nullptr, 0 });
  discovered[startRegion] = Q.front();
  auto returnPath = [&](node v) {
    path.push_back(v.myRegion);
    while (v.parent)
    {
      v = *v.parent;
      path.push_back(v.myRegion);
    }
    return path;
  };

  while (Q.size())
  {
    auto v = Q.front();
    Q.pop();
    if (!BWAPI::Broodwar->isExplored(BWAPI::TilePosition(v.myRegion->getCenter())))
      return returnPath(v);
    for (auto& neighbor : v.myRegion->getNeighbors())
    {
      if (!discovered[neighbor].myRegion)
      {
        if (neighbor->getRegionGroupID() == v.myRegion->getRegionGroupID()
          && BWAPI::Broodwar->hasPath(startRegion->getCenter(), neighbor->getCenter())) // this is for ground only
        {
          discovered[neighbor] = v;
          Q.push(node{ neighbor, &discovered[neighbor], v.cost + 1 });
        }
      }
    }
  }
  return path;
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
  // Check if we need to make turn the defenders into combatants and a wave because we have no buildings.
  // We will need to be able to disable this wave once we get buildings, but will fix that later.
  if (BWAPI::Broodwar->getFrameCount() == 0
    && !bot->getBuildings())
  {
    std::shared_ptr<WaveInfo> wave;
    auto itr = waveList.begin();
    if (itr == waveList.end())
      wave = std::make_shared<WaveInfo>();
    else
      wave = *itr;

    for (auto& unit : bot->getUnitManager().getUnits(PlayerState::Self))
    {
      wave->addUnit(*unit);
      unit->setRole(Roles::Combat);
    }

    wave->setActive(true);
  }
  if (BWAPI::Broodwar->getFrameCount() > 0
    && !bot->getBuildings())
  {
    auto& wave = *waveList.begin();
    for (auto& unit : bot->getUnitManager().getUnits(PlayerState::Self))
    {
      if (!unit->getWave())
      {
        wave->addUnit(*unit);
        unit->setRole(Roles::Combat);
      }
    }
  }
  for (auto& wave : waveList)
  {
    // Update wave information
    wave->updateWave();
    // Check if the wave has a target currently
    if (!bot->getBuildOrder().getPositionQueue().size())
    {
      auto& beacons = bot->getUnitManager().getBeacons();
      if (beacons.size() > 0 && wave->isActive())
      {
        BWAPI::Position bestBeacon = BWAPI::Positions::None;
        double bestDist = DBL_MAX;
        for (auto& unit : beacons)
        {
          if (unit->getBeaconFlag())
            continue;

          auto dist = unit->getDistance(wave->getCentroid());
          if (dist < bestDist && BWAPI::Broodwar->hasPath(wave->getCentroid(), unit->getPosition()))
          {
            bestDist = dist;
            bestBeacon = unit->getPosition();
          }
        }
        if (bestBeacon != wave->getBeaconTarget())
        {
          wave->setBeaconTarget(bestBeacon);
          wave->setOldTarget(wave->getTarget());
          wave->setTarget(nullptr);
        }
      }
    }
    if (!wave->getTarget() && wave->getBeaconTarget() == BWAPI::Positions::None && wave->isActive())
    {
      // Installation map with a position queue to work through
      if (bot->getBuildOrder().getPositionQueue().size())
      {
        wave->setTarget(BWAPI::Broodwar->getRegionAt(bot->getBuildOrder().getPositionQueue()[bot->getBuildOrder().getPositionQueueSpot()]));
        wave->setTargetPosition(bot->getBuildOrder().getPositionQueue()[bot->getBuildOrder().getPositionQueueSpot()]);
        bot->getBuildOrder().nextPositionQueueSpot();
      }
      else
      {
        // See if any enemy structures are known.
        BWAPI::Region bestRegion = nullptr;
        double bestDist = DBL_MAX;
        std::shared_ptr<UnitInfo> beacon = nullptr;
        for (auto& unit : bot->getUnitManager().getUnits(PlayerState::Enemy))
        {
          if (!unit->getType().isBuilding()
            || unit->getType().isBeacon())
            continue;

          auto dist = unit->getDistance(wave->getCentroid());
          if (dist < bestDist)
          {
            bestDist = dist;
            bestRegion = BWAPI::Broodwar->getRegionAt(unit->getPosition());
          }
        }
        if (bestRegion) // Found a region with an enemy building in it.
        {
          wave->setTarget(bestRegion);
        }
        else // Unable to find a region with an enemy building in it, need to explore.
        {
          // Will use BFS to find a region whose center is not revealed.
          auto startRegion = BWAPI::Broodwar->getRegionAt(wave->getCentroid());
          if (!startRegion) // Somehow our centroid is not in a start region. If waves stall out will need a solution for this. Maybe fallback to start location?
            startRegion = BWAPI::Broodwar->getRegionAt(BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()));
          if (startRegion->getRegionGroupID() != wave->getFirstUnit()->getRegion()->getRegionGroupID())
            startRegion = wave->getFirstUnit()->getRegion();
          auto path = findFirstUnexploredRegion(startRegion);
          if (path.size() >= 2)
          {
            if (!wave->getOldTarget())
              wave->setOldTarget(path[1]);
            wave->setTarget(path[0]);
          }
        }
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
        if (buildOrder.getMaxWave() == 5)
        {
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
        else
        {
          switch (buildOrder.getWave())
          {
          case 1:
            if (BWAPI::Broodwar->self()->isUnitAvailable(BWAPI::UnitTypes::Terran_Factory)
              && BWAPI::Broodwar->self()->isUnitAvailable(BWAPI::UnitTypes::Terran_Vulture))
              buildOrder.nextWave();
            break;
          case 2:
            if (BWAPI::Broodwar->self()->isUnitAvailable(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode))
              buildOrder.nextWave();
            break;
          case 3:
            if (BWAPI::Broodwar->self()->isUnitAvailable(BWAPI::UnitTypes::Terran_Starport)
              && BWAPI::Broodwar->self()->isUnitAvailable(BWAPI::UnitTypes::Terran_Wraith))
              buildOrder.nextWave();
            break;
          case 4:
            if (BWAPI::Broodwar->self()->isUnitAvailable(BWAPI::UnitTypes::Terran_Goliath))
              buildOrder.nextWave();
            break;
          case 5:
            if (BWAPI::Broodwar->self()->isUnitAvailable(BWAPI::UnitTypes::Terran_Battlecruiser))
              buildOrder.nextWave();
            break;
          case 6:
            buildOrder.nextWave();
            break;
          case 7:
            if (BWAPI::Broodwar->self()->isUnitAvailable(BWAPI::UnitTypes::Terran_Ghost))
              buildOrder.nextWave();
            break;
          case 8:
            buildOrder.nextWave();
            break;
          default:
            break;
          }
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