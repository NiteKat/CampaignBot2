#pragma once
#include <BWAPI.h>

enum struct CommandType
{
  None,
  AttackMove,
  UseTech,
  Move,
  Wait
};

struct CommandInfo
{
public:
  CommandInfo() {};
  CommandInfo(CommandType newType) : type(newType) {}
  static CommandInfo AttackMove(BWAPI::Position);
  static CommandInfo Move(BWAPI::Position, BWAPI::UnitType unitType = BWAPI::UnitTypes::None);
  static CommandInfo UseTech(BWAPI::TechType, BWAPI::UnitType unitType = BWAPI::UnitTypes::None);
  static CommandInfo Wait(int, BWAPI::UnitType unitType = BWAPI::UnitTypes::None);
  CommandType type;
  BWAPI::UnitType unitTypes = BWAPI::UnitTypes::None;
  BWAPI::Position target = BWAPI::Positions::None;
  BWAPI::TechType techType = BWAPI::TechTypes::None;
  int time;
private:
};