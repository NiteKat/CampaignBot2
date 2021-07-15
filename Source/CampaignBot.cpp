#include "CampaignBot.h"

std::unique_ptr<CampaignBot> bot;

void CampaignBot::onFrame()
{
  playerManager.onFrame();
  unitManager.onFrame();
  combatManager.onFrame();
  if (BWAPI::Broodwar->getFrameCount() == 0)
    townManager.init();
  buildOrder.onFrame();
  waveManager.onFrame();
  buildingManager.onFrame();
  townManager.onFrame();
  workerManager.onFrame();
  productionManager.onFrame();
  cameraManager.onFrame();
  
#if defined(_DEBUG)
  debugManager.onFrame();
#endif
}

void CampaignBot::onStart()
{
  cameraManager.onStart();
  waveManager.onStart();
}

void CampaignBot::onSendText(std::string text)
{
  BWAPI::Broodwar->sendText(text.c_str());
}

void CampaignBot::onUnitComplete(BWAPI::Unit unit)
{
  if (!unit->getType().isBuilding() && BWAPI::Broodwar->getFrameCount() > 0)
    cameraManager.addEvent(unit);
}

void CampaignBot::onUnitDestroy(BWAPI::Unit unit)
{
  playerManager.removeUnit(unit);
}

void CampaignBot::onUnitRenegade(BWAPI::Unit unit)
{
  if (!unit->getType().isRefinery())
  {
    playerManager.removeUnit(unit);
    playerManager.storeUnit(unit);
    cameraManager.addEvent(unit);
  }
}