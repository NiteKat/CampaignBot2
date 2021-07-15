#include "CampaignBot.h"
// ------------------ PUBLIC FUNCTIONS ------------------ //
void CombatManager::onFrame()
{
  updateWaves();
  updateUnits();
}
// ------------------ PRIVATE FUNCTIONS ----------------- //
void CombatManager::updateDecision(UnitInfo& unit)
{
  if (!unit.getUnit() || !unit.getUnit()->exists()
    || unit.getUnit()->isLoaded()
    || unit.isLockedDown() || unit.isMaelstrommed() || unit.isStasised() || !unit.isCompleted())
    return;

  // Check if worker is still under durress or should we switch it back to a worker.
  if (unit.getType().isWorker())
  {
    updateRole(unit);
    return;
  }
  updateTargets(unit);
  if (unit.getTargetRegion()
    && unit.getRegion() != unit.getTargetRegion()
    && unit.isIdle())
    unit.attack(unit.getTargetRegion()->getCenter());
}

void CombatManager::updateRole(UnitInfo& unit)
{
  // Don't look at non-combat workers.
  if (unit.getRole() != Roles::Combat)
    return;

  for (auto& u : bot->getUnitManager().getUnits(PlayerState::Enemy))
  {
    if (u->getUnit()->getTarget() == unit.getUnit()
      && unit.isUnderAttack())
      return;
  }

  if (!unit.getUnit()->isMoving())
    unit.setRole(Roles::Worker);

}

void CombatManager::updateTargets(UnitInfo& unit)
{
  // Check if currently part of an attack wave. If not, we're a guard.
  if (!unit.getWave() || !unit.getWave()->isActive())
  {
    auto& unitManager = bot->getUnitManager();
    BWAPI::Region bestRegion = nullptr;
    double bestDist = DBL_MAX;
    double bestScore = DBL_MAX;
    bool enemy = false;
    for (auto& u : unitManager.getUnits(PlayerState::Self))
    {
      if (!u->getType().isBuilding())
        continue;

      auto region = u->getRegion();
      if (region == bestRegion)
        continue;

      auto enemyUnits = unitManager.getUnitsInRegion(PlayerState::Enemy, region);
      if (enemyUnits.size())
      {
        auto dist = unit.getDistance(region->getCenter());
        if (dist < bestDist || !enemy)
        {
          bestDist = dist;
          bestRegion = region;
          enemy = true;
        }
      }
      else if (!enemy && !unit.getTargetRegion()) // Not currently assigned to guard a region.
      {
        auto score = 0.0;
        for (auto& uir : unitManager.getUnitsInRegion(PlayerState::Self, region))
        {
          if (!uir->getType().isWorker())
            score += uir->getGroundDamage() + uir->getAirDamage();
        }

        if (score < bestScore)
        {
          bestRegion = region;
          bestScore = score;
        }
      }
    }
    if (bestRegion)
      unit.setTargetRegion(bestRegion);
  }
  else if (unit.getWave() && unit.getWave()->isActive()) // wave is active
  {
    unit.setTargetRegion(unit.getWave()->getTarget());
  }
}

void CombatManager::updateUnits()
{
  for (auto& u : bot->getUnitManager().getUnits(PlayerState::Self))
  {
    if (u->getRole() == Roles::Combat)
      updateDecision(*u);
  }
}

void CombatManager::updateWaves()
{

}