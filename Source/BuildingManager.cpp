#include "CampaignBot.h"

// ------------------ PUBLIC FUNCTIONS ------------------ //
void BuildingManager::onFrame()
{
  queueBuildings();
}
// ------------------ PRIVATE FUNCTIONS ----------------- //
void BuildingManager::queueBuildings()
{
  queuedMinerals = 0;
  queuedGas = 0;
  auto& buildQueue = bot->getBuildOrder().getBuildQueue();

  for (auto& [building, count] : buildQueue)
  {
    int queuedCount = 0;

    if (building.isAddon() || !building.isBuilding())
      continue;

    auto visible = bot->getUnitManager().getMyVisible(building);
    auto queued = bot->getUnitManager().getMyQueued(building);

    queuedMinerals += building.mineralPrice() * queued;
    queuedMinerals += building.mineralPrice() * std::max(0, count - queued - visible);
    queuedGas += building.gasPrice() * queued;
    queuedGas += building.gasPrice() * std::max(0, count - queued - visible);

    while (count > queuedCount + queued + visible)
    {
      queuedCount++;

      for (auto& town : bot->getTownManager().getTownList())
      {
        if (town->getTownWorkerCount() > 0 && town->hasTownHall())
          town->queueBuilding(building);
      }
    }
  }
}