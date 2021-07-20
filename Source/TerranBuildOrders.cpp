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
  else if (currentMap == "Untitled Scenario")
    T4JacobsInstalation();
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
  buildOrder.setMaxWave(5);
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

void TerranBuildOrders::setArmyCompositionInsane()
{
  auto& buildOrder = bot->getBuildOrder();
  buildOrder.setMaxWave(9);
  switch (buildOrder.getWave())
  {
  case 1:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Marine, 20.0);
    break;
  case 2:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Marine, 20.0);
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Vulture, 10.0);
    break;
  case 3:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Marine, 20.0);
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode, 6.0);
    break;
  case 4:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Wraith, 12.0);
    break;
  case 5:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Goliath, 12.0);
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode, 6.0);
    break;
  case 6:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Wraith, 6.0);
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Battlecruiser, 6.0);
    break;
  case 7:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Battlecruiser, 10.0);
    break;
  case 8:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Marine, 12.0);
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Goliath, 6.0);
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Siege_Tank_Tank_Mode, 4.0);
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Ghost, 2.0);
    break;
  case 9:
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Wraith, 8.0);
    buildOrder.setComposition(BWAPI::UnitTypes::Terran_Battlecruiser, 6.0);
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

void TerranBuildOrders::T2BackwaterStationInsane()
{
  auto& buildQueue = bot->getBuildOrder().getBuildQueue();
  auto s = bot->getBuildOrder().getSupply();

  buildQueue[BWAPI::UnitTypes::Terran_Command_Center] = 1;
  buildQueue[BWAPI::UnitTypes::Terran_Supply_Depot] = std::min(25, int(s * 2 >= BWAPI::Broodwar->self()->supplyTotal() - 2) + bot->getUnitManager().getMyCompleted(BWAPI::UnitTypes::Terran_Supply_Depot));
  buildQueue[BWAPI::UnitTypes::Terran_Barracks] = 1 + int(s >= 18);
  buildQueue[BWAPI::UnitTypes::Terran_Refinery] = int(s >= 20);
  buildQueue[BWAPI::UnitTypes::Terran_Engineering_Bay] = int(s >= 20);

  // Army Composition
  setArmyCompositionInsane();
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

void TerranBuildOrders::T3DesperateAllianceInsane()
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
  buildQueue[BWAPI::UnitTypes::Terran_Factory] = (bot->getBuildOrder().getWave() - 1) * 2 * int(s >= 34);

  // Army Composition
  setArmyCompositionInsane();
}

void TerranBuildOrders::T4JacobsInstalation()
{
  auto& commandQueue = bot->getBuildOrder().getCommandQueue();
  if (!commandQueue.size())
  {
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(3007, 3487)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(2303, 3872)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(3007, 3487)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(2623, 3296)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(3390, 2592)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(3599, 2529))); // Teleport Beacon
    commandQueue.push_back(CommandInfo::Wait(60));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(2542, 1055))); // Camera Beacon
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(2766, 1185))); // Teleport Beacon
    commandQueue.push_back(CommandInfo::Wait(60));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(2593, 3293)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(1985, 2915)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(1215, 2496)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(576, 2175)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(1088, 1888)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(1488, 2047))); // Defense Beacon
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(942, 1344))); // Door Beacon
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(512, 1185)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(1952, 1408)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(1466, 1152)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(2400, 641)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(1826, 352)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(1407, 545)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(1056, 354)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(974, 190))); // Teleport Beacon
    commandQueue.push_back(CommandInfo::Wait(60));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(3008, 738)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(3840, 1185)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(3104, 1537)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(2719, 1794)));
    commandQueue.push_back(CommandInfo::AttackMove(BWAPI::Position(2447, 2018))); // Victory Beacon
  }
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