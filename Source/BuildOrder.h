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
  int getSupply() { return supply; }
  std::set<BWAPI::UnitType>& getUnlockedTypes() { return unlockedTypes; }
  int getWave() { return wave; }
  void nextWave() { wave++; }
  bool isUnitUnlocked(BWAPI::UnitType);
  void onFrame();
  void setComposition(BWAPI::UnitType type, double count) { armyComposition[type] = count; }

private:
  void updateBuild();

  std::map<BWAPI::UnitType, double> armyComposition;
  std::map<BWAPI::UnitType, int> buildQueue;
  std::string currentBuildOrder;
  std::string mapName;
  int supply = 0;
  std::set<BWAPI::UnitType> unlockedTypes;
  int wave = 1;


  TerranBuildOrders terran;
};