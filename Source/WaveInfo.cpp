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
  // Check if we should clear the Beacon Target if we have one.
  if (beaconTarget != BWAPI::Positions::None)
  {
    auto& unit = bot->getUnitManager().getClosestUnit(beaconTarget, PlayerState::Self, [&](auto& u) {
      return u->getWave() && u->getWave()->getBeaconTarget() == beaconTarget;
    });

    if (unit)
    {
      auto dist = unit->getDistance(beaconTarget);
      if (dist < 75)
      {
        for (auto& beacon : bot->getUnitManager().getBeacons())
        {
          if (beacon->getPosition() == beaconTarget)
          {
            beacon->setBeaconFlag(true);
            oldTarget = BWAPI::Broodwar->getRegionAt(beaconTarget);
            target = nullptr;
            beaconTarget = BWAPI::Positions::None;
            gathering = true;
            gatherTimer = 500;
            break;
          }
        }
      }
    }
  }
  // Update wave information
  for (auto& u : unitList)
  {
    auto& unit = u.lock();
    if (!unit)
      continue;

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
        if (unit->getType().isBuilding() &&
          !unit->getType().isBeacon())
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