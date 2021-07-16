#pragma once
#include <BWAPI.h>

#include <set>
#include <map>
#include <memory>

struct UnitInfo;

struct TownInfo : std::enable_shared_from_this<TownInfo>
{
public:
  TownInfo() {}
  TownInfo(BWAPI::Position newLocation) : location(newLocation) {}

  void addUnit(UnitInfo&);
  int getGasCount() { return gasGeysers.size(); }
  std::set<std::weak_ptr<UnitInfo>>& getGasGeysers() { return gasGeysers; }
  int getGasWorkerCount();
  int getMineralCount() { return minerals.size(); }
  std::set<std::weak_ptr<UnitInfo>>& getMinerals() { return minerals; }
  BWAPI::Position getPosition() { return location; }
  int getResourceGroup() { return resourceGroup; }
  std::map<BWAPI::UnitType, int>& getTownQueue() { return townQueue; }
  bool isGasSaturated();
  void setResourceGroup(int newGroup) { resourceGroup = newGroup; }
  std::shared_ptr<UnitInfo> getLeastTargetedMineral();
  int getTownWorkerCount() { return int(townWorkers.size()); }
  std::set<std::weak_ptr<UnitInfo>>& getTownWorkers() { return townWorkers; }
  std::shared_ptr<UnitInfo> getUnsaturatedGas();
  bool hasTownHall() { return !pTownCenter.expired(); }
  void queueBuilding(BWAPI::UnitType type) { townQueue[type]++; }
  void removeMineral(UnitInfo&);
  void removeWorker(UnitInfo&);
  void setPosition(BWAPI::Position newLocation) { location = newLocation; }

private:
  std::set<std::weak_ptr<UnitInfo>> townWorkers;
  
  int desiredWorkerCount = 0;
  int currentWorkerCount = 0;
  int resourceGroup = -1;

  BWAPI::Position location = BWAPI::Positions::None;
  std::weak_ptr<UnitInfo> pTownCenter;
  std::set<std::weak_ptr<UnitInfo>> minerals;
  std::set<std::weak_ptr<UnitInfo>> gasGeysers;

  std::map<BWAPI::UnitType, int> townQueue;
};