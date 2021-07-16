#pragma once
#include "PlayerInfo.h"
#include "UnitInfo.h"
#include "TownInfo.h"
#include "WaveInfo.h"
#include "PlayerManager.h"
#include "UnitManager.h"
#include "DebugManager.h"
#include "TownManager.h"
#include "WorkerManager.h"
#include "TerranBuildOrders.h"
#include "BuildOrder.h"
#include "CombatManager.h"
#include "BuildingManager.h"
#include "ProductionManager.h"
#include "WaveManager.h"
#include "CameraManager.h"

struct CampaignBot
{
public:
  BuildingManager& getBuildingManager() { return buildingManager; }
  bool getBuildings() { return buildings; }
  BuildOrder& getBuildOrder() { return buildOrder; }
  PlayerManager& getPlayerManager() { return playerManager; }
  TownManager& getTownManager() { return townManager; }
  UnitManager& getUnitManager() { return unitManager; }
  void onFrame();
  void onStart();
  void onSendText(std::string text);
  void onUnitComplete(BWAPI::Unit);
  void onUnitDestroy(BWAPI::Unit);
  void onUnitRenegade(BWAPI::Unit);
  void setBuildings(bool newBuildings) { buildings = newBuildings; }
private:
  BuildOrder buildOrder;
  CameraManager cameraManager;
  CombatManager combatManager;
  DebugManager debugManager;
  PlayerManager playerManager;
  TownManager townManager;
  UnitManager unitManager;
  WaveManager waveManager;
  WorkerManager workerManager;
  BuildingManager buildingManager;
  ProductionManager productionManager;
  bool buildings = false;
};

extern std::unique_ptr<CampaignBot> bot;