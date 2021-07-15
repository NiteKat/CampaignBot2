#include <BWAPI.h>
#include <BWAPI/Client.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>

#include "Source/CampaignBot.h"

void reconnect()
{
  while (!BWAPI::BWAPIClient.connect())
  {
    std::this_thread::sleep_for(std::chrono::milliseconds{ 1000 });
  }
}

int main()
{
  std::cout << "Connecting..." << std::endl;
  reconnect();
  while (true)
  {
    //std::unique_ptr<CampaignBot> bot;
    std::cout << "waiting to enter match" << std::endl;
    while (!BWAPI::Broodwar->isInGame())
    {
      BWAPI::BWAPIClient.update();
      if (!BWAPI::BWAPIClient.isConnected())
      {
        std::cout << "Reconnecting..." << std::endl;
        reconnect();
      }
    }
    std::cout << "starting match!" << std::endl;
    std::cout << "Map: " << BWAPI::Broodwar->mapName() << std::endl;
    BWAPI::Broodwar->enableFlag(BWAPI::Flag::UserInput);
    while (BWAPI::Broodwar->isInGame())
    {
      for (auto& e : BWAPI::Broodwar->getEvents())
      {
        switch (e.getType())
        {
        case BWAPI::EventType::MatchStart:
          bot = std::make_unique<CampaignBot>();
          break;
        case BWAPI::EventType::SendText:
          BWAPI::Broodwar->sendText(e.getText().c_str());
          break;
        default:
          break;
        }
      }
      auto& allUnits = BWAPI::Broodwar->getAllUnits();
      if (BWAPI::Broodwar->getFrameCount() > 0)
        bot->onFrame();
      BWAPI::BWAPIClient.update();

    }
  }
}