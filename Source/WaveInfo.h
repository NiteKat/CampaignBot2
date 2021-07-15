#pragma once
#include <memory>

struct WaveInfo : std::enable_shared_from_this<WaveInfo>
{
public:
  void addUnit(UnitInfo& unit);
  BWAPI::Position getCentroid() { return centroid; }
  BWAPI::Region getTarget() { return target; }
  double getTypeCount(BWAPI::UnitType type) { return unitCounts[type]; }
  std::map<BWAPI::UnitType, double>& getTypeCounts() { return unitCounts; }
  int getUnitCount() { return unitList.size(); }
  bool isActive() { return active; }
  void removeUnit(UnitInfo&);
  void setActive(bool newActive) { active = newActive; }
  void setTarget(BWAPI::Region newTarget) { target = newTarget; }
  void updateWave();
private:
  bool active = false;
  BWAPI::Position centroid;
  BWAPI::Region target = nullptr;
  std::map<BWAPI::UnitType, double> unitCounts;
  std::set<std::weak_ptr<UnitInfo>> unitList;
};