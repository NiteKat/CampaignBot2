#include "CampaignBot.h"

void TownManager::addTown(UnitInfo& unit)
{
  if (!unit.getType().isResourceDepot())
    return;

  auto resource = bot->getUnitManager().getClosestUnit(unit.getPosition(), PlayerState::Neutral, [&](auto& u) {
    return u->getType().isResourceContainer();
  });

  if (!resource)
    return;

  bool makeNewTown = true;
  for (auto& town : townList)
  {
    if (town->getResourceGroup() == resource->getResourceGroup())
      makeNewTown = false;
  }
  if (makeNewTown)
  {
    auto newTown = std::make_shared<TownInfo>();
    newTown->addUnit(unit);
    newTown->setPosition(unit.getPosition());
    newTown->setResourceGroup(resource->getResourceGroup());

    for (auto& u : bot->getUnitManager().getUnits(PlayerState::Neutral))
    {
      if (u->getType().isResourceContainer() && u->getResourceGroup() == newTown->getResourceGroup())
        newTown->addUnit(*u);
    }
    townList.insert(newTown);
  }
}

void TownManager::init()
{
  // Look for Resource Depots that we own and and are not lifted and set up towns for them.
  for (auto& unit : bot->getUnitManager().getUnits(PlayerState::Self))
  {
    if (!unit->getType().isResourceDepot())
      continue;

    if (unit->isLifted())
      continue;

    auto newTown = std::make_shared<TownInfo>();
    newTown->addUnit(*unit);
    newTown->setPosition(unit->getPosition());

    auto resource = bot->getUnitManager().getClosestUnit(unit->getPosition(), PlayerState::Neutral, [&](auto& u) {
      return u->getType().isResourceContainer();
    });
    if (!resource)
      continue;

    newTown->setResourceGroup(resource->getResourceGroup());

    for (auto& u : bot->getUnitManager().getUnits(PlayerState::Neutral))
    {
      if (u->getType().isResourceContainer() && u->getResourceGroup() == newTown->getResourceGroup())
        newTown->addUnit(*u);
    }
    townList.insert(newTown);
  }
}

void TownManager::onFrame()
{
  for (auto& town : townList)
  {
    // find workers for any buildings we're tasked to construct, and clear the task
    // list afterwards.
    for (auto& [building, count] : town->getTownQueue())
    {
      auto assigned = 0;
      for (auto& workerPointer : town->getTownWorkers())
      {
        auto& worker = workerPointer.lock();
        if (!worker)
          continue;

        auto isAvailableToBuild = [](std::shared_ptr<UnitInfo> u) {
          return u->getRole() == Roles::Worker && !u->isConstructing() && !u->hasBuildTarget()
            && !u->isCarryingPowerup() && !u->isGatheringGas();
        };

        if (isAvailableToBuild(worker) && shouldMoveToBuild(building))
        {
          worker->setBuildType(building);
          worker->setBuildTarget(BWAPI::Broodwar->getBuildLocation(building, BWAPI::TilePosition(town->getPosition())));
          assigned++;
        }
        if (count == assigned)
          break;
      }
    }
    town->getTownQueue().clear();
  }
}

bool TownManager::shouldMoveToBuild(BWAPI::UnitType type)
{
  auto enoughGas = type.gasPrice() > 0 ? BWAPI::Broodwar->self()->gas() >= type.gasPrice() - 25 : true;
  auto enoughMinerals = type.mineralPrice() > 0 ? BWAPI::Broodwar->self()->minerals() >= type.mineralPrice() - 25 : true;

  return (enoughGas && enoughMinerals);
}