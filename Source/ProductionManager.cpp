#include "CampaignBot.h"
// ------------------ PUBLIC FUNCTIONS ------------------ //
void ProductionManager::onFrame()
{
  trainedThisFrame.clear();
  for (auto& building : bot->getUnitManager().getUnits(PlayerState::Self))
  {
    updateUnits(*building);
    updateProduction(*building);
  }
}
// ------------------ PRIVATE FUNCTIONS ----------------- //
bool ProductionManager::isAffordable(BWAPI::UnitType type)
{
  auto selfMinerals = BWAPI::Broodwar->self()->minerals();
  auto selfGas = BWAPI::Broodwar->self()->gas();
  auto selfSupply = bot->getPlayerManager().getSupply(PlayerState::Self);

  for (auto& [type, count] : trainedThisFrame)
  {
    selfMinerals -= type.mineralPrice() * count;
    selfGas -= type.gasPrice() * count;
    selfSupply += type.supplyRequired() * count;
  }
  auto& buildingManager = bot->getBuildingManager();
  auto mineralAffordable = type.mineralPrice() == 0 || (selfMinerals >= type.mineralPrice() + buildingManager.getQueuedMinerals());
  auto gasAffordable = type.gasPrice() == 0 || (selfGas >= type.gasPrice() + buildingManager.getQueuedGas());
  auto supplyAffordable = type.supplyRequired() == 0 || selfSupply + type.supplyRequired() <= BWAPI::Broodwar->self()->supplyTotal();
  
  return mineralAffordable && gasAffordable && supplyAffordable;
}

bool ProductionManager::isAffordable(BWAPI::UpgradeType upgrade)
{
  return BWAPI::Broodwar->self()->minerals() >= upgrade.mineralPrice() && BWAPI::Broodwar->self()->gas() >= upgrade.gasPrice();
}

bool ProductionManager::isCreatable(BWAPI::Unit building, BWAPI::UnitType type)
{
  if (!bot->getBuildOrder().isUnitUnlocked(type))
    return false;

  switch (type)
  {
  case BWAPI::UnitTypes::Terran_Marine:
    return true;
  default:
    break;
  }

  return false;
}

bool ProductionManager::isCreatable(BWAPI::UpgradeType upgrade)
{
  if (BWAPI::Broodwar->self()->getUpgradeLevel(upgrade) < upgrade.maxRepeats() && !BWAPI::Broodwar->self()->isUpgrading(upgrade))
    return true;
  return false;
}

bool ProductionManager::isSuitable(BWAPI::UnitType)
{
  return true;
}

bool ProductionManager::isSuitable(BWAPI::UpgradeType upgrade)
{
  switch (upgrade)
  {
  case BWAPI::UpgradeTypes::U_238_Shells:
    return true;
  case BWAPI::UpgradeTypes::Terran_Infantry_Weapons:
    return BWAPI::Broodwar->self()->getUpgradeLevel(upgrade) == BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Armor);
  case BWAPI::UpgradeTypes::Terran_Infantry_Armor:
    return BWAPI::Broodwar->self()->getUpgradeLevel(upgrade) < BWAPI::Broodwar->self()->getUpgradeLevel(BWAPI::UpgradeTypes::Terran_Infantry_Weapons);
  default:
    return false;
  }
  return false;
}

bool ProductionManager::produce(UnitInfo& unit)
{
  auto bestType = BWAPI::UnitTypes::None;
  auto bestValue = 0;

  // Look through each UnitType we can train
  for (auto& type : unit.getType().buildsWhat())
  {
    if (!isCreatable(unit.getUnit(), type))
      continue;

    const auto value = scoreUnit(type);
    if (value > bestValue)
    {
      bestValue = value;
      bestType = type;
    }
  }

  if (bestType != BWAPI::UnitTypes::None)
  {
    // If we can afford it, train it
    if (isAffordable(bestType))
    {
      trainedThisFrame[bestType]++;
      unit.train(bestType);
      return true;
    }
  }
  return false;
}

int ProductionManager::scoreUnit(BWAPI::UnitType type)
{
  auto wanted = bot->getBuildOrder().getComposition(type);
  return std::max(0, int(wanted - bot->getUnitManager().getMyVisible(type)));
}

void ProductionManager::updateProduction(UnitInfo& building)
{
  trainedThisFrame.clear();

  if (!building.getUnit()
    || building.getRole() != Roles::Production
    || !building.isCompleted()
    || building.getRemainingTrainFrames() >= BWAPI::Broodwar->getLatencyFrames()
    || lastTrainFrame >= BWAPI::Broodwar->getFrameCount() - BWAPI::Broodwar->getLatencyFrames()
    || building.getType() == BWAPI::UnitTypes::Zerg_Larva)
    return;

  if (!building.getType().isResourceDepot() || building.getType().getRace() == BWAPI::Races::Zerg)
  {
    if (!produce(building))
      upgrade(building);
  }
  else
  {
    for (auto& type : building.getType().buildsWhat())
    {
      if (!BWAPI::Broodwar->self()->isUnitAvailable(type))
        continue;

      auto& town = building.getTown();
      if (!town)
        continue;

      auto workers = town->getMineralCount() + town->getGasCount() * 3;
      if (type.isWorker() && town->getTownWorkerCount() < workers && isAffordable(type))
      {
        if (building.train(type))
        {
          trainedThisFrame[type]++;
          lastTrainFrame = BWAPI::Broodwar->getFrameCount();
        }
      }
    }
  }
}

void ProductionManager::updateUnits(UnitInfo& building)
{
  // Check if we need to be repaired.
  if (building.isCompleted()
    && building.getType().isBuilding()
    && double(building.getHitPoints()) / double(building.getMaxHitPoints()) < 0.75 && !building.hasRepairTarget()
    && building.getType().getRace() == BWAPI::Races::Terran)
  {
    auto& worker = bot->getUnitManager().getClosestUnit(building.getPosition(), PlayerState::Self, [](auto& u) {
      return u->hasTown() && u->hasResource() && u->getResource()->getType().isMineralField() && !u->hasBuildTarget() && !u->getUnit()->getBuildUnit();
    });

    if (worker)
    {
      building.setRepairTarget(&(*worker));
      worker->setRepairTarget(&building);
    }
  }
  // Check if we need to replace our old builder.
  if (!building.isCompleted()
    && building.getType().isBuilding()
    && !building.getType().isAddon()
    && !building.getUnit()->getBuildUnit() && building.getType().getRace() == BWAPI::Races::Terran)
  {
    auto& worker = bot->getUnitManager().getClosestUnit(building.getPosition(), PlayerState::Self, [](auto& u) {
      return u->hasTown() && u->hasResource() && u->getResource()->getType().isMineralField() && !u->hasBuildTarget() && !u->getUnit()->getBuildUnit();
    });

    if (worker)
      worker->rightClick(building.getUnit());
  }
}

bool ProductionManager::upgrade(UnitInfo& unit)
{
  for (auto& upgrade : unit.getType().upgradesWhat())
  {
    if (isCreatable(upgrade) && isSuitable(upgrade))
    {
      if (isAffordable(upgrade))
      {
        unit.upgrade(upgrade);
        lastTrainFrame = BWAPI::Broodwar->getFrameCount();
        return true;
      }
    }
  }
  return false;
}
