#pragma once
#include <BWAPI.h>

enum struct CommandType
{
  None,
  AttackMove,
  Cloak,
  Move
};

struct CommandInfo
{
public:
  CommandType type;
  BWAPI::UnitType unitTypes = BWAPI::UnitTypes::None;
  BWAPI::Position target = BWAPI::Positions::None;
  BWAPI::TechType techType = BWAPI::TechTypes::None;
private:
};