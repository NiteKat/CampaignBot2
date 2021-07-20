#pragma once
#include <memory>

struct WaveInfo : std::enable_shared_from_this<WaveInfo>
{
public:
  void addUnit(UnitInfo& unit);
  BWAPI::Position getBeaconTarget() { return beaconTarget; }
  BWAPI::Position getCentroid() { return centroid; }
  std::shared_ptr<UnitInfo> getFirstUnit() { return (*(unitList.begin())).lock(); }
  int getGatherTimer() { return gatherTimer; }
  BWAPI::Region getOldTarget() { return oldTarget; }
  BWAPI::Region getTarget() { return target; }
  BWAPI::Position getTargetPosition() { return targetPosition; }
  double getTypeCount(BWAPI::UnitType type) { return unitCounts[type]; }
  std::map<BWAPI::UnitType, double>& getTypeCounts() { return unitCounts; }
  int getUnitCount() { return unitList.size(); }
  bool getUseCommandQueue() { return useCommandQueue; }
  bool isActive() { return active; }
  bool isGathering() { return gathering; }
  void removeUnit(UnitInfo&);
  void setActive(bool newActive) { active = newActive; }
  void setBeaconTarget(BWAPI::Position newBeaconTarget) { beaconTarget = newBeaconTarget; }
  void setOldTarget(BWAPI::Region newOldTarget) { oldTarget = newOldTarget; }
  void setTarget(BWAPI::Region newTarget) { target = newTarget; }
  void setTargetPosition(BWAPI::Position newTargetPosition) { targetPosition = newTargetPosition; }
  void setUseCommandQueue(bool newUseCommandQueue) { useCommandQueue = newUseCommandQueue; }
  void updateWave();
private:
  bool active = false;
  BWAPI::Position beaconTarget = BWAPI::Positions::None;
  BWAPI::Position centroid;
  bool useCommandQueue = false;
  bool gathering;
  int gatherTimer = 500;
  BWAPI::Region oldTarget = nullptr;
  BWAPI::Region target = nullptr;
  BWAPI::Position targetPosition = BWAPI::Positions::None;
  std::map<BWAPI::UnitType, double> unitCounts;
  std::set<std::weak_ptr<UnitInfo>> unitList;
};