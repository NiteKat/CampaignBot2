#include "CampaignBot.h"

void PlayerInfo::update()
{
  auto self = BWAPI::Broodwar->self();
  if (bwPlayer->getID() == self->getID())
    playerState = PlayerState::Self;
  else if (bwPlayer->isEnemy(self))
    playerState = PlayerState::Enemy;
  else if (bwPlayer->isNeutral())
    playerState = PlayerState::Neutral;
  else if (bwPlayer->isAlly(self))
    playerState = PlayerState::Ally;
  else
    playerState = PlayerState::None;

  supply = 0;
  for (auto& u : unitList)
    supply += u->getType().supplyRequired();
}