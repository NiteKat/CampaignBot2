#pragma once

struct CombatManager
{
public:
  void onFrame();
private:
  bool attack(UnitInfo&);
  bool cast(UnitInfo&);
  bool move(UnitInfo&);
  bool special(UnitInfo&);
  bool wait(UnitInfo&);
  void updateDecision(UnitInfo&);
  void updateRole(UnitInfo&);
  void updateTargets(UnitInfo&);
  void updateUnits();
  void updateWaves();
};