#include "CampaignBot.h"
// ------------------ PUBLIC FUNCTIONS ------------------ //
void DebugManager::onFrame()
{
  drawMapName();
  drawUnitRoles();
  drawTownPanel();
}
// ------------------ PRIVATE FUNCTIONS ----------------- //
void DebugManager::drawMapName()
{
  drawTextScreen(BWAPI::Position(10, 0), BWAPI::Broodwar->mapName());
}
void DebugManager::drawTextMap(BWAPI::Position here, std::string text)
{
  int adjust = (text.size() / 2) * 5;
  BWAPI::Broodwar->drawTextMap(here - BWAPI::Position(adjust, 0), "%s", text.c_str());
}

void DebugManager::drawTextScreen(BWAPI::Position here, std::string text)
{
  BWAPI::Broodwar->drawTextScreen(here, "%s", text.c_str());
}

void DebugManager::drawTownPanel()
{
  std::stringstream text("");
  text << "\x017Town Panel" << std::endl << "\xFNumber Towns:\x2 " << bot->getTownManager().getTownCount() << std::endl;
  for (auto& town : bot->getTownManager().getTownList())
  {
    text << "\xFTown " << town->getResourceGroup() << ": \xEMinerals:\x2 " << town->getMinerals().size()
      << " \x18Gas:\x2 " << town->getGasGeysers().size() << std::endl << "        \xFWorkers:\x2 " << town->getTownWorkers().size()
      << std::endl;
  }
  drawTextScreen(BWAPI::Position(10, 20), text.str());
}



void drawUnitPanel()
{
  const char playerColorMap[] = { '\x14', '\x1' };
  std::stringstream text("");
  text << "\x17Unit Panel" << std::endl;
  for (auto [type, count] : bot->getUnitManager().getMyCompletedCounts())
  {
    if (count)
    {
      
    }
  }
}

void DebugManager::drawUnitRoles()
{
  for (auto& p : bot->getPlayerManager().getPlayers())
  {
    if (p->isSelf())
    {
      for (auto& u : p->getUnits())
        if (u->isCompleted())
          drawTextMap(u->getPosition(), roleTable[static_cast<int>(u->getRole())]);
    }
  }
}