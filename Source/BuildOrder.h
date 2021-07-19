#pragma once
#include <BWAPI.h>

struct TerranBuildOrders;

struct BuildOrder
{
public:
  std::map<BWAPI::UnitType, int>& getBuildQueue() { return buildQueue; }
  double getComposition(BWAPI::UnitType type) { return armyComposition[type]; }
  std::map<BWAPI::UnitType, double>& getComposition() { return armyComposition; }
  std::string getCurrentBuildOrder() { return currentBuildOrder; }
  int getMaxWave() { return maxWave; }
  std::vector<BWAPI::Position>& getPositionQueue() { return positionQueue; }
  int getPositionQueueSpot() { return positionQueueSpot; }
  int getSupply() { return supply; }
  std::set<BWAPI::UnitType>& getUnlockedTypes() { return unlockedTypes; }
  int getWave() { return wave; }
  void nextPositionQueueSpot() { positionQueueSpot = std::max(0, std::min(int(positionQueue.size() - 1), positionQueueSpot + 1)); }
  void nextWave() { wave++; }
  bool isUnitUnlocked(BWAPI::UnitType);
  void onFrame();
  void setComposition(BWAPI::UnitType type, double count) { armyComposition[type] = count; }
  void setMaxWave(int newMaxWave) { maxWave = newMaxWave; }

private:
  void updateBuild();

  std::map<BWAPI::UnitType, double> armyComposition;
  std::map<BWAPI::UnitType, int> buildQueue;
  std::string currentBuildOrder;
  bool insaneScript = false;
  std::string mapName;
  int maxWave = 0;
  std::vector<BWAPI::Position> positionQueue;
  int positionQueueSpot = 0;
  int supply = 0;
  std::set<BWAPI::UnitType> unlockedTypes;
  int wave = 1;


  TerranBuildOrders terran;
};