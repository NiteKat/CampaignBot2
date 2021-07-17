#pragma once
#include <set>
#include <memory>

#include "UnitInfo.h"

struct UnitManager
{
public:
  template<typename F>
  std::shared_ptr<UnitInfo> getClosestUnit(BWAPI::Position here, PlayerState player, F&& pred)
  {
    auto distBest = DBL_MAX;
    auto& units = getUnits(player);
    std::shared_ptr<UnitInfo> best = nullptr;

    for (auto& u : units)
    {
      if (!u->getUnit() || !pred(u))
        continue;

      auto dist = u->getDistance(here);
      if (dist < distBest)
      {
        best = u;
        distBest = dist;
      }
    }
    return best;
  }
  int getMyCompleted(BWAPI::UnitType type) { return myCompletedTypes[type]; }
  std::map<BWAPI::UnitType, int>& getMyCompletedCounts() { return myCompletedTypes; }
  int getMyInactiveVisible(BWAPI::UnitType type) { return myInactiveVisibleTypes[type]; }
  int getMyQueued(BWAPI::UnitType type) { return myQueuedTypes[type]; }
  int getMyVisible(BWAPI::UnitType type) { return myVisibleTypes[type]; }
  std::shared_ptr<UnitInfo> getUnitInfo(BWAPI::Unit unit);
  std::set<std::shared_ptr<UnitInfo>>& getUnits(PlayerState);
  std::set<std::shared_ptr<UnitInfo>> getUnitsInRegion(PlayerState, BWAPI::Region);
  void onFrame();
  

private:
  void updateCounts();
  void updateNeutrals();
  void updateRole(UnitInfo& unit);
  void updateSelf();
  void updateUnits();

  std::set<std::shared_ptr<UnitInfo>> allyUnits;
  std::set<std::shared_ptr<UnitInfo>> enemyUnits;
  std::map<BWAPI::UnitType, int> myCompletedTypes;
  std::map<BWAPI::UnitType, int> myInactiveVisibleTypes;
  std::map<BWAPI::UnitType, int> myQueuedTypes;
  std::set<std::shared_ptr<UnitInfo>> myUnits;
  std::map<BWAPI::UnitType, int> myVisibleTypes;
  std::set<std::shared_ptr<UnitInfo>> neutralUnits;
};