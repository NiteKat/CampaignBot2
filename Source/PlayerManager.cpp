#include "CampaignBot.h"
// ------------------ PUBLIC FUNCTIONS ------------------ //

std::shared_ptr<PlayerInfo> PlayerManager::getPlayerInfo(BWAPI::Player targetPlayer)
{
  for (auto& player : playerList)
  {
    if (targetPlayer->getID() == player->getPlayer()->getID())
      return player;
  }
  return nullptr;
}

int PlayerManager::getSupply(PlayerState state)
{
  auto combined = 0;
  for (auto& player : playerList)
  {
    if (player->getPlayerState() == state)
      combined += player->getSupply();
  }
  return combined;
}

void PlayerManager::onFrame()
{
  for (auto& player : BWAPI::Broodwar->getPlayers())
  {
    auto p = getPlayerInfo(player);
    if (!p)
      this->storePlayer(player);
  }

  this->updatePlayers();
}

void PlayerManager::removeUnit(BWAPI::Unit bwUnit)
{
  for (auto& player : playerList)
  {
    for (auto& unit : player->getUnits())
    {
      if (unit->getUnit() == bwUnit)
      {
        if (unit->hasWave())
        {
          unit->getWave()->removeUnit(*unit);
          unit->setWave(nullptr);
        }
        if (unit->hasResource())
        {
          unit->getResource()->removeWorker(*unit);
          unit->setResource(nullptr);
        }

        if (unit->getType().isWorker())
        {
          auto& town = unit->getTown();
          if (town)
          {
            town->removeWorker(*unit);
            unit->setTown(nullptr);
          }
        }
        if (unit->getType().isMineralField())
        {
          for (auto& u : unit->getTargetedBy())
          {
            auto& worker = u.lock();
            if (!worker)
              continue;

            worker->setResource(nullptr);
          }
          unit->getTargetedBy().clear();
          for (auto& town : bot->getTownManager().getTownList())
          {
            if (town->getResourceGroup() == unit->getResourceGroup())
              town->removeMineral(*unit);
          }
        }

        player->getUnits().erase(unit);
        return;
      }
    }
  }
}

void PlayerManager::storeUnit(BWAPI::Unit unit)
{
  auto player = this->getPlayerInfo(unit->getPlayer());
  if (player)
  {
    auto info = std::make_shared<UnitInfo>(unit);
    player->addUnit(info);
    info->update();
    if (unit->getType().isResourceDepot() && player->getPlayerState() == PlayerState::Self)
      bot->getTownManager().addTown(*info);
  }
}
// ------------------ PRIVATE FUNCTIONS ----------------- //
void PlayerManager::storePlayer(BWAPI::Player newPlayer)
{
  playerList.insert(std::make_shared<PlayerInfo>(newPlayer));
}

void PlayerManager::updatePlayers()
{
  for (auto& p : this->getPlayers())
  {
    p->update();
  }
}