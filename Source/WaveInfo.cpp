#include "CampaignBot.h"

// ------------------ PUBLIC FUNCTIONS ------------------ //

void WaveInfo::addUnit(UnitInfo& unit)
{
  unitList.insert(unit.weak_from_this());
  unitCounts[unit.getType()]++; 
  unit.setWave(this);
}

void WaveInfo::removeUnit(UnitInfo& unit)
{
  auto u = unit.weak_from_this();
  for (auto itr = unitList.begin(); itr != unitList.end(); itr++)
  {
    if ((itr)->lock() == u.lock())
    {
      unitList.erase(itr);
      break;
    }
  }
}

void WaveInfo::updateWave()
{
  centroid = BWAPI::Position(0, 0);
  unitCounts.clear();
  bool unitsGathered = oldTarget != nullptr;
  bool atLeastOne = false;
  for (auto& u : unitList)
  {
    auto& unit = u.lock();
    if (!unit)
      continue;

    if (beaconTarget != BWAPI::Positions::None)
    {
      auto dist = unit->getDistance(beaconTarget);
      if (dist < 150)
      {
        for (auto& beacon : bot->getUnitManager().getBeacons())
        {
          if (beacon->getPosition() == beaconTarget)
          {
            beacon->setBeaconFlag(true);
            oldTarget = BWAPI::Broodwar->getRegionAt(beaconTarget);
            beaconTarget = BWAPI::Positions::None;
            gathering = true;
            gatherTimer = 500;
          }
        }
      }
    }

    centroid += unit->getPosition();
    unitCounts[unit->getType()]++;
    if (unit->getRegion() != oldTarget)
      unitsGathered = false;
    else if (unit->getRegion() == oldTarget)
      atLeastOne = true;
  }
  if (unitsGathered)
    gathering = false;
  if (unitList.size())
    centroid = centroid / unitList.size(); // NAIEVE will be wrong if expired pointers exist
  // If we have a target, let's make sure it's still valid.
  if (target)
  {
    if (atLeastOne)
      gatherTimer = std::max(0, gatherTimer - 1);
    if (gatherTimer == 0)
      gathering = false;
    // Check if it's a target because we're exploring to it
    if (BWAPI::Broodwar->isExplored(BWAPI::TilePosition(target->getCenter())))
    {
      // Check if there's still enemy buildings in the region.
      bool buildingFound = false;
      for (auto& unit : bot->getUnitManager().getUnitsInRegion(PlayerState::Enemy, target))
      {
        if (unit->getType().isBuilding())
        {
          buildingFound = true;
          break;
        }
      }
      // need to reset target. Units will reset on next update.
      if (!buildingFound)
      {
        oldTarget = target;
        target = nullptr;
        // Since we changed targets, we should gather.
        gathering = true;
        gatherTimer = 500;
      }
    }
  }
}