#include "CampaignBot.h"

// ------------------ PUBLIC FUNCTIONS ------------------ //
void WorkerManager::onFrame()
{
  updateUnits();
  updateDecision();
}
// ------------------ PRIVATE FUNCTIONS ----------------- //
void WorkerManager::updateDecision()
{
  for (auto& unit : bot->getUnitManager().getUnits(PlayerState::Self))
  {
    if (unit->getRole() != Roles::Worker)
      continue;

    if (unit->isConstructing())
    {
      if (unit->getUnit()->getBuildUnit())
      {
        unit->setBuildTarget(BWAPI::TilePositions::None);
        unit->setBuildType(BWAPI::UnitTypes::None);
      }
    }
    else if (unit->hasBuildTarget()) // build something
    {
      if (!bot->getTownManager().shouldMoveToBuild(unit->getBuildType()))
        continue;
      auto distance = unit->getDistance(BWAPI::Position(unit->getBuildTarget()));
      if (distance > 100)
        unit->move(BWAPI::Position(unit->getBuildTarget()));
      else
        unit->build(unit->getBuildType(), unit->getBuildTarget());
    }
    else if (unit->isUnderAttack() && unit->isGathering()) // actively being threatened.
    {
      unit->setBuildTarget(BWAPI::TilePositions::None);
      unit->setBuildType(BWAPI::UnitTypes::None);
      unit->setRole(Roles::Combat);
      unit->stop();
    }
    else // check if we need to do anything with resources
    {
      auto& resource = unit->getResource();
      if (!resource)
        continue;

      if ((unit->isGatheringMinerals() || unit->isIdle()) && resource->getType().isRefinery())
      {
        unit->gather(*resource);
        // Reset these so we don't have any hanging resources we are reserving.
        unit->setBuildTarget(BWAPI::TilePositions::None);
        unit->setBuildType(BWAPI::UnitTypes::None);
      }

      else if ((unit->isGatheringGas() || unit->isIdle()) && resource->getType().isMineralField())
      {
        unit->gather(*resource);
        // Reset these so we don't have any hanging resources we are reserving.
        unit->setBuildTarget(BWAPI::TilePositions::None);
        unit->setBuildType(BWAPI::UnitTypes::None);
      }
    }
  }
}

void WorkerManager::updateResources(UnitInfo& unit)
{ 
  if (unit.hasBuildTarget())
    return;
  
  auto& town = unit.getTown();
  const auto isGasWorker = unit.hasResource() && unit.getResource()->getType().isRefinery();
  const auto isMineralWorker = unit.hasResource() && unit.getResource()->getType().isMineralField();
  const auto sat = town->isGasSaturated();
  const auto enoughWorkers = town->getTownWorkerCount() - town->getGasWorkerCount() > town->getMineralCount() - 2;
  const auto minerals = town->getMinerals().size();
  const auto needGas = ( !sat && enoughWorkers) || minerals == 0;
  const auto needMinerals = town->getTownWorkerCount() - town->getGasWorkerCount() < town->getMineralCount() - 2 && town->getMinerals().size() > 0;
  
  if (!needGas && isMineralWorker)
    return;

  if (needGas && !needMinerals && !isGasWorker)
  {
    auto& geyser = town->getUnsaturatedGas();
    if (!geyser)
      return;

    if (unit.hasResource())
    {
      auto oldResource = unit.getResource();
      oldResource->removeWorker(unit);
    }
    unit.setResource(&(*geyser));
    geyser->addWorker(unit);
  }
  else if ((needMinerals && !isMineralWorker) || unit.isIdle())
  {
    if (unit.hasResource())
    {
      auto oldResource = unit.getResource();
      oldResource->removeWorker(unit);
    }
    auto& mineral = town->getLeastTargetedMineral();
    unit.setResource(&(*mineral));
    mineral->addWorker(unit);
  }
}

void WorkerManager::updateUnits()
{
  for (auto& t : bot->getTownManager().getTownList())
  {
    for (auto& w : t->getTownWorkers())
    {
      auto& worker = w.lock();
      if (!worker)
        continue;

      updateResources(*worker);
    }
  }
}