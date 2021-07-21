#include "CampaignBot.h"
#include <fstream>

// ------------------ PUBLIC FUNCTIONS ------------------ //
bool BuildOrder::isUnitUnlocked(BWAPI::UnitType type)
{
  return unlockedTypes.find(type) != unlockedTypes.end();
}

void BuildOrder::onEnd(bool isWinner)
{
  writeFile(isWinner);
}

void BuildOrder::onFrame()
{
  updateBuild();
}

void BuildOrder::onStart()
{
  readFile();
}

// ------------------ PRIVATE FUNCTIONS ----------------- //
void BuildOrder::readFile()
{
  std::ifstream readFile;
  readFile.open("bwapi-data/write/result.txt");
  if (readFile.is_open())
  {
    readFile >> insaneScript;
    readFile.close();
  }
}

void BuildOrder::updateBuild()
{
  supply = BWAPI::Broodwar->self()->supplyUsed() / 2;
  buildQueue.clear();
  mapName = BWAPI::Broodwar->mapName();

  if (BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Terran)
  {
    terran.opener();
    terran.unlocks();
  }
}

void BuildOrder::writeFile(bool isWinner)
{
  std::ofstream writeFile;
  writeFile.open("bwapi-data/write/result.txt");
  if (isWinner)
    writeFile << 0;
  else
    writeFile << 1;
  writeFile.close();
}