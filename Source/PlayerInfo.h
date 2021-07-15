#pragma once
#include <BWAPI.h>

#include <set>
#include <memory>

#include "PlayerState.h"

struct UnitInfo;

struct PlayerInfo
{
public:
  PlayerInfo();
  PlayerInfo(BWAPI::Player newPlayer) : bwPlayer(newPlayer) {}

  void addUnit(std::shared_ptr<UnitInfo> info) { unitList.insert(info); }
  BWAPI::Player getPlayer() { return bwPlayer; }
  PlayerState getPlayerState() { return playerState; }
  int getSupply() { return supply; }
  std::set<std::shared_ptr<UnitInfo>>& getUnits() { return unitList; }
  bool isSelf() { return playerState == PlayerState::Self; }
  void update();

private:
  BWAPI::Player bwPlayer = nullptr;
  PlayerState playerState = PlayerState::None;
  int supply = 0;
  std::set<std::shared_ptr<UnitInfo>> unitList;
};