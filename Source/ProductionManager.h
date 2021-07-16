#pragma once

struct ProductionManager
{
public:
  void onFrame();
private:
  bool isAffordable(BWAPI::UnitType);
  bool isAffordable(BWAPI::UpgradeType);
  bool isCreatable(BWAPI::Unit building, BWAPI::UnitType type);
  bool isCreatable(BWAPI::UpgradeType);
  bool isSuitable(BWAPI::UnitType);
  bool isSuitable(BWAPI::UpgradeType);
  bool produce(UnitInfo&);
  int scoreUnit(BWAPI::UnitType);
  void updateProduction(UnitInfo& building);
  void updateUnits(UnitInfo& building);
  bool upgrade(UnitInfo&);

  int lastTrainFrame = 0;
  std::map<BWAPI::UnitType, int> trainedThisFrame;
};