#include "CampaignBot.h"
// ------------------ PUBLIC FUNCTIONS ------------------ //
void TerranBuildOrders::opener()
{
  auto currentMap = BWAPI::Broodwar->mapName();
  if (currentMap == "Terran Tutorial")
    TerranTutorial();
  else if (currentMap == "T1) Wasteland")
    T1Wasteland();
  else if (currentMap == "T2) Backwater Station")
    T2BackwaterStation();
  else if (currentMap == "T3) Desperate Alliance")
    T3DesperateAlliance();
}

void TerranBuildOrders::unlocks()
{
  auto& unlockedTypes = bot->getBuildOrder().getUnlockedTypes();
  //auto currentBuildOrder = bot->getBuildOrder().getCurrentBuildOrder();

  for (int i = 0; static_cast<BWAPI::UnitType>(i) != BWAPI::UnitTypes::None; i++)
  {
    auto type = static_cast<BWAPI::UnitType>(i);
    if (BWAPI::Broodwar->self()->isUnitAvailable(type))
      unlockedTypes.insert(type);
  }

}
// ------------------ PRIVATE FUNCTIONS ----------------- //
void TerranBuildOrders::setArmyComposition()
{
  auto& buildOrder = bot->getBuildOrder();
  switch (buildOrder.getWave())
  {
  case 1:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Marine, 12.0);
    break;
  case 2:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Marine, 12.0);
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode, 3.0);
    break;
  case 3:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Marine, 14.0);
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode, 5.0);
    break;
  case 4:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Marine, 12.0);
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode, 4.0);
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Wraith, 4.0);
    break;
  case 5:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Wraith, 10.0);
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Battlecruiser, 4.0);
    break;
  }
}

void TerranBuildOrders::T1Wasteland()
{
  auto& buildQueue = bot->getBuildOrder().getBuildQueue();
  auto s = bot->getBuildOrder().getSupply();

  // Build
  buildQueue[BWAPI::UnitTypes::Terran_Command_Center] = 1;
  buildQueue[BWAPI::UnitTypes::Terran_Barracks] = 1;
  buildQueue[BWAPI::UnitTypes::Terran_Supply_Depot] = 1 + int(s >= 16.0);

  // Army Composition
  setArmyComposition();
}

void TerranBuildOrders::T2BackwaterStation()
{
  auto& buildQueue = bot->getBuildOrder().getBuildQueue();
  auto s = bot->getBuildOrder().getSupply();

  buildQueue[BWAPI::UnitTypes::Terran_Command_Center] = 1;
  buildQueue[BWAPI::UnitTypes::Terran_Barracks] = 1 + int(s >= 18);
  buildQueue[BWAPI::UnitTypes::Terran_Refinery] = int(s >= 20);
  buildQueue[BWAPI::UnitTypes::Terran_Engineering_Bay] = int(s >= 20);

  // Army Composition
  setArmyComposition();
}

void TerranBuildOrders::T3DesperateAlliance()
{
  auto& buildQueue = bot->getBuildOrder().getBuildQueue();
  auto s = bot->getBuildOrder().getSupply();
  
  buildQueue[BWAPI::UnitTypes::Terran_Command_Center] = 1;
  if (bot->getUnitManager().getMyVisible(BWAPI::UnitTypes::Terran_Supply_Depot) >= 3)
    buildQueue[BWAPI::UnitTypes::Terran_Supply_Depot] = std::min(25, int(s * 2 >= BWAPI::Broodwar->self()->supplyTotal() - 2) + bot->getUnitManager().getMyCompleted(BWAPI::UnitTypes::Terran_Supply_Depot));
  else
    buildQueue[BWAPI::UnitTypes::Terran_Supply_Depot] = 3;
  buildQueue[BWAPI::UnitTypes::Terran_Refinery] = int(s >= 38);
  buildQueue[BWAPI::UnitTypes::Terran_Barracks] = 1 + int(s >= 34);
  buildQueue[BWAPI::UnitTypes::Terran_Engineering_Bay] = 1;
  buildQueue[BWAPI::UnitTypes::Terran_Missile_Turret] = bot->getUnitManager().getMyCompleted(BWAPI::UnitTypes::Terran_Engineering_Bay) ? 3 + int(s >= 30) + int(s >= 35) : 0;
  buildQueue[BWAPI::UnitTypes::Terran_Academy] = 1;
  buildQueue[BWAPI::UnitTypes::Terran_Bunker] = 2;
  
  // Army Composition
  setArmyComposition();
}

void TerranBuildOrders::TerranTutorial()
{
  auto& buildQueue = bot->getBuildOrder().getBuildQueue();
  auto s = bot->getBuildOrder().getSupply();

  // Build
  buildQueue[BWAPI::UnitTypes::Terran_Command_Center] = 1;
  buildQueue[BWAPI::UnitTypes::Terran_Refinery] = s >= 20.0;
  buildQueue[BWAPI::UnitTypes::Terran_Supply_Depot] = 1 + int(s >= 17.0) + int(s >= 22.0);

  // Army Composition
  setArmyComposition();
}