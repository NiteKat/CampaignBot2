#include "CampaignBot.h"

void TownInfo::addUnit(UnitInfo& unit)
{
  if (unit.getType().isMineralField())
    minerals.insert(unit.weak_from_this());
  else if (unit.getType().isResourceContainer())
    gasGeysers.insert(unit.weak_from_this());
  else if (unit.getType().isResourceDepot() && !pTownCenter.lock())
  {
    pTownCenter = unit.weak_from_this();
    unit.setTown(this);
  }
  else if (unit.getType().isWorker())
  {
    townWorkers.insert(unit.weak_from_this());
    unit.setTown(this);
  }
}

int TownInfo::getGasWorkerCount()
{
  int workers = 0;
  for (auto& u : gasGeysers)
  {
    for (auto& u : gasGeysers)
    {
      auto& geyser = u.lock();
      if (!geyser)
        continue;

      workers += geyser->getGathererCount();
    }
  }
  return workers;
}

bool TownInfo::isGasSaturated()
{
  for (auto& u : gasGeysers)
  {
    auto& geyser = u.lock();
    if (!geyser || geyser->getGathererCount() >= 3)
      continue;
    return false;
  }
  return true;
}

void TownInfo::removeMineral(UnitInfo& unit)
{
  for (auto itr = minerals.begin(); itr != minerals.end(); itr++)
  {
    if (!itr->lock())
      continue;

    if (itr->lock()->getUnit() == unit.getUnit())
    {
      minerals.erase(itr);
      return;
    }
  }
}

void TownInfo::removeWorker(UnitInfo& unit)
{
  for (auto itr = townWorkers.begin(); itr != townWorkers.end(); itr++)
  {
    if (!itr->lock())
      continue;

    if (itr->lock()->getUnit() == unit.getUnit())
    {
      townWorkers.erase(itr);
      return;
    }
  }
}

std::shared_ptr<UnitInfo> TownInfo::getLeastTargetedMineral()
{
  int bestGatherer = INT_MAX;
  std::shared_ptr<UnitInfo> bestMineral = nullptr;
  for (auto& u : minerals)
  {
    auto& mineral = u.lock();
    if (!mineral)
      continue;

    if (mineral->getGathererCount() < bestGatherer)
    {
      bestGatherer = mineral->getGathererCount();
      bestMineral = mineral;
    }
  }
  return bestMineral;
}

std::shared_ptr<UnitInfo> TownInfo::getUnsaturatedGas()
{
  for (auto& u : gasGeysers)
  {
    auto& geyser = u.lock();
    if (!geyser)
      continue;

    if (geyser->getGathererCount() < 3)
      return geyser;
  }
  return nullptr;
}