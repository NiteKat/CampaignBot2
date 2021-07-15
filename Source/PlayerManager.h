#pragma once
#include <set>
#include <memory>

struct PlayerInfo;

struct PlayerManager
{
public:
  std::shared_ptr<PlayerInfo> getPlayerInfo(BWAPI::Player targetPlayer);
  std::set<std::shared_ptr<PlayerInfo>>& getPlayers() { return playerList; }
  int getSupply(PlayerState state);
  void onFrame();
  void removeUnit(BWAPI::Unit);
  void storeUnit(BWAPI::Unit);

private:

  void storePlayer(BWAPI::Player newPlayer);
  void updatePlayers();

  std::set<std::shared_ptr<PlayerInfo>> playerList;
};