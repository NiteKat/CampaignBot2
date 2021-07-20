#include "CampaignBot.h"

CommandInfo CommandInfo::AttackMove(BWAPI::Position newTarget)
{
  CommandInfo c{ CommandType::AttackMove };
  c.target = newTarget;
  return c;
}

CommandInfo CommandInfo::Move(BWAPI::Position newTarget, BWAPI::UnitType unitType)
{
  CommandInfo c{ CommandType::Move };
  c.target = newTarget;
  c.unitTypes = unitType;
  return c;
}

CommandInfo CommandInfo::UseTech(BWAPI::TechType techType, BWAPI::UnitType unitType)
{
  CommandInfo c{ CommandType::UseTech };
  c.techType = techType;
  c.unitTypes = unitType;
  return c;
}

CommandInfo CommandInfo::Wait(int newTime, BWAPI::UnitType unitType)
{
  CommandInfo c{ CommandType::Wait };
  c.time = newTime;
  c.unitTypes = unitType;
  return c;
}