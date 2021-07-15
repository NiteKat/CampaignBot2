#pragma once

struct ProductionManager
{
public:
  void onFrame();
  void updateProduction();
private:
  bool isAffordable(BWAPI::UnitType);
  bool isAffordable(BWAPI::UpgradeType);
  bool isCreatable(BWAPI::Unit building, BWAPI::UnitType type);
  bool isCreatable(BWAPI::UpgradeType);
  bool isSuitable(BWAPI::UnitType);
  bool isSuitable(BWAPI::UpgradeType);
  bool produce(UnitInfo&);
  int scoreUnit(BWAPI::UnitType);
  bool upgrade(UnitInfo&);

  int lastTrainFrame = 0;
  std::map<BWAPI::UnitType, int> trainedThisFrame;
};