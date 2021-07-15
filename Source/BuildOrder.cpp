#include "CampaignBot.h"

// ------------------ PUBLIC FUNCTIONS ------------------ //
bool BuildOrder::isUnitUnlocked(BWAPI::UnitType type)
{
  return unlockedTypes.find(type) != unlockedTypes.end();
}

void BuildOrder::onFrame()
{
  updateBuild();
}
// ------------------ PRIVATE FUNCTIONS ----------------- //
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