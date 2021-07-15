#include "CampaignBotModule.h"
#include "CampaignBot.h"

void CampaignBotModule::onStart()
{
  bot = std::make_unique<CampaignBot>();
  bot->onStart();
}

void CampaignBotModule::onEnd(bool isWinner)
{

}

void CampaignBotModule::onFrame()
{
  bot->onFrame();
}

void CampaignBotModule::onSendText(std::string text)
{
  bot->onSendText(text);
}

void CampaignBotModule::onReceiveText(BWAPI::Player player, std::string text)
{

}

void CampaignBotModule::onPlayerLeft(BWAPI::Player player)
{

}

void CampaignBotModule::onNukeDetect(BWAPI::Position target)
{

}

void CampaignBotModule::onUnitDiscover(BWAPI::Unit unit)
{

}
void CampaignBotModule::onUnitEvade(BWAPI::Unit unit)
{

}

void CampaignBotModule::onUnitShow(BWAPI::Unit unit)
{

}

void CampaignBotModule::onUnitHide(BWAPI::Unit unit)
{

}

void CampaignBotModule::onUnitCreate(BWAPI::Unit unit)
{

}

void CampaignBotModule::onUnitDestroy(BWAPI::Unit unit)
{
  bot->onUnitDestroy(unit);
}

void CampaignBotModule::onUnitMorph(BWAPI::Unit unit)
{

}

void CampaignBotModule::onUnitRenegade(BWAPI::Unit unit)
{
  bot->onUnitRenegade(unit);
}

void CampaignBotModule::onSaveGame(std::string gameName)
{

}

void CampaignBotModule::onUnitComplete(BWAPI::Unit unit)
{
  bot->onUnitComplete(unit);
}