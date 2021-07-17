#pragma once
#include <memory>

struct WaveInfo : std::enable_shared_from_this<WaveInfo>
{
public:
  void addUnit(UnitInfo& unit);
  BWAPI::Position getCentroid() { return centroid; }
  std::shared_ptr<UnitInfo> getFirstUnit() { return (*(unitList.begin())).lock(); }
  int getGatherTimer() { return gatherTimer; }
  BWAPI::Region getOldTarget() { return oldTarget; }
  BWAPI::Region getTarget() { return target; }
  double getTypeCount(BWAPI::UnitType type) { return unitCounts[type]; }
  std::map<BWAPI::UnitType, double>& getTypeCounts() { return unitCounts; }
  int getUnitCount() { return unitList.size(); }
  bool isActive() { return active; }
  bool isGathering() { return gathering; }
  void removeUnit(UnitInfo&);
  void setActive(bool newActive) { active = newActive; }
  void setOldTarget(BWAPI::Region newOldTarget) { oldTarget = newOldTarget; }
  void setTarget(BWAPI::Region newTarget) { target = newTarget; }
  void updateWave();
private:
  bool active = false;
  BWAPI::Position centroid;
  bool gathering;
  int gatherTimer = 500;
  BWAPI::Region oldTarget = nullptr;
  BWAPI::Region target = nullptr;
  std::map<BWAPI::UnitType, double> unitCounts;
  std::set<std::weak_ptr<UnitInfo>> unitList;
};