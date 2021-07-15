#pragma once

struct CombatManager
{
public:
  void onFrame();
private:
  void updateDecision(UnitInfo&);
  void updateRole(UnitInfo&);
  void updateTargets(UnitInfo&);
  void updateUnits();
  void updateWaves();
};