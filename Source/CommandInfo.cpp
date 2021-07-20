#include "CampaignBot.h"

CommandInfo CommandInfo::AttackMove(BWAPI::Position newTarget)
{
  CommandInfo c{ CommandType::AttackMove };
  c.target = newTarget;
  return c;
}

CommandInfo CommandInfo::Wait(int newTime)
{
  CommandInfo c{ CommandType::Wait };
  c.time = newTime;
  return c;
}