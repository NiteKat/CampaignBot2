#pragma once
#include <set>
#include <memory>

struct TownInfo;

struct TownManager
{
public:
  void addTown(UnitInfo&);
  int getTownCount() { return townList.size(); }
  std::set<std::shared_ptr<TownInfo>>& getTownList() { return townList; }
  void init();
  void onFrame();
  bool shouldMoveToBuild(BWAPI::UnitType type);
private:
  std::set<std::shared_ptr<TownInfo>> townList;
};