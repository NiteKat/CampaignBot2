#pragma once
#include <BWAPI.h>

enum struct CommandType
{
  None,
  AttackMove,
  Cloak,
  Move,
  Wait
};

struct CommandInfo
{
public:
  CommandInfo() {};
  CommandInfo(CommandType newType) : type(newType) {}
  static CommandInfo AttackMove(BWAPI::Position);
  static CommandInfo Wait(int);
  CommandType type;
  BWAPI::UnitType unitTypes = BWAPI::UnitTypes::None;
  BWAPI::Position target = BWAPI::Positions::None;
  BWAPI::TechType techType = BWAPI::TechTypes::None;
  int time;
private:
};