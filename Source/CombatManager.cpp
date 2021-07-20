#include "CampaignBot.h"
// ------------------ PUBLIC FUNCTIONS ------------------ //
void CombatManager::onFrame()
{
  updateWaves();
  updateUnits();
}
// ------------------ PRIVATE FUNCTIONS ----------------- //
bool CombatManager::attack(UnitInfo& unit)
{
  if (unit.getRole() == Roles::Defender
      && !unit.getType().isBuilding())
  {
    if (unit.isIdle()
      && unit.getPosition() != unit.getInitialPosition())
      unit.attack(unit.getInitialPosition());
  }
  if (unit.getRole() != Roles::Combat)
    return false;

  if (unit.getWave()
    && unit.getWave()->getUseCommandQueue())
  {
    auto& command = bot->getBuildOrder().getCommandQueue()[unit.getCommandQueuePosition()];
    if (command.type == CommandType::AttackMove
      && unit.isIdle()
      && (command.unitTypes == BWAPI::UnitTypes::None
        || command.unitTypes == unit.getType()))
    {
      unit.attack(command.target);
      return true;
    }
    if (unit.getDistance(command.target) < 100)
    {
      unit.nextCommandQueuePosition();
      return true;
    }
    return false;
  }
  if (unit.getWave()
    && unit.getWave()->getBeaconTarget() != BWAPI::Positions::None
    && unit.isIdle())
  {
    unit.attack(unit.getWave()->getBeaconTarget());
    return true;
  }
  if (unit.getTargetRegion()
    && unit.getRegion() != unit.getTargetRegion()
    && unit.isIdle())
  {
    if (unit.getTargetPosition() != BWAPI::Positions::None)
      unit.attack(unit.getTargetPosition());
    else
      unit.attack(unit.getTargetRegion()->getCenter());
    return true;
  }

  return false;
}

bool CombatManager::cast(UnitInfo& unit)
{
  if (unit.getWave()
    && unit.getWave()->getUseCommandQueue())
  {
    auto& command = bot->getBuildOrder().getCommandQueue()[unit.getCommandQueuePosition()];
    if (command.type == CommandType::UseTech
      && unit.isIdle()
      && (command.unitTypes == BWAPI::UnitTypes::None
        || command.unitTypes == unit.getType()))
    {
      if (command.techType == BWAPI::TechTypes::Personnel_Cloaking
        && !unit.isCloaked())
      {
        unit.useTech(command.techType);
        unit.nextCommandQueuePosition();
        return true;
      }
    }
  }
  // Hero Sarah Kerrigan - Lockdown / Cloak
  if (unit.getType() == BWAPI::UnitTypes::Hero_Sarah_Kerrigan)
  {
    if (unit.isUnderAttack() && unit.getEnergy() >= 100)
    {
      for (auto& enemy : bot->getUnitManager().getUnits(PlayerState::Enemy))
      {
        if (!enemy->getType().isMechanical())
          continue;

        if (enemy->getUnit()->getTarget() == unit.getUnit()
          && !enemy->isLockedDown())
        {
          unit.useTech(BWAPI::TechTypes::Lockdown, enemy->getUnit());
          return true;
        }
      }
    }
    if (unit.isUnderAttack() && unit.getEnergy() >= 30 && !unit.isCloaked())
    {
      unit.useTech(BWAPI::TechTypes::Personnel_Cloaking);
      return true;
    }
  }

  // Wraith - Cloak
  if (unit.getType() == BWAPI::UnitTypes::Terran_Wraith)
  {
    if (unit.isUnderAttack() && BWAPI::Broodwar->self()->hasResearched(BWAPI::TechTypes::Cloaking_Field) && unit.getEnergy() >= 30 && !unit.isCloaked())
    {
      unit.useTech(BWAPI::TechTypes::Cloaking_Field);
      return true;
    }
  }
  
  return false;
}

bool CombatManager::move(UnitInfo& unit)
{
  if (unit.getWave()
    && unit.getWave()->getUseCommandQueue())
  {
    auto& command = bot->getBuildOrder().getCommandQueue()[unit.getCommandQueuePosition()];
    if (command.type == CommandType::Move
      && unit.isIdle()
      && (command.unitTypes == BWAPI::UnitTypes::None
        || command.unitTypes == unit.getType()))
    {
      unit.move(command.target);
      return true;
    }
    if (unit.getDistance(command.target) < 100)
    {
      unit.nextCommandQueuePosition();
      return true;
    }
  }
  return false;
}

bool CombatManager::special(UnitInfo& unit)
{
  if (unit.hasWave()
    && unit.getWave()->isActive()
    && unit.getUnit()->getTransport())
  {
    unit.getUnit()->getTransport()->unload(unit.getUnit());
    return true;
  }



  if ((unit.getType() == BWAPI::UnitTypes::Terran_Marine
    || unit.getType() == BWAPI::UnitTypes::Terran_Firebat
    || unit.getType() == BWAPI::UnitTypes::Terran_Ghost)
    && bot->getUnitManager().getMyCompleted(BWAPI::UnitTypes::Terran_Bunker) > 0)
  {
    auto& bunker = bot->getUnitManager().getClosestUnit(unit.getPosition(), PlayerState::Self, [](auto& u) {
      return u->isCompleted() && u->getType() == BWAPI::UnitTypes::Terran_Bunker && u->getSpaceRemaining() > 0;
    });

    if (bunker)
    {
      bool load = true;
      if (unit.getRole() == Roles::Defender
        && unit.getDistance(bunker->getPosition()) > 100)
        load = false;
      else if (unit.hasWave() && unit.getWave()->isActive())
        load = false;
      if (load)
      {
        unit.rightClick(bunker->getUnit());
        return true;
      }
    }
  }
  return false;
}

bool CombatManager::wait(UnitInfo& unit)
{
  if (unit.getWave()
    && unit.getWave()->getUseCommandQueue())
  {
    auto& command = bot->getBuildOrder().getCommandQueue()[unit.getCommandQueuePosition()];
    if (command.type == CommandType::Wait)
    {
      if (!unit.getWaitTimer())
      {
        unit.setWaitTimer(command.time + 1);
        return true;
      }
      if (unit.getWaitTimer() == 1)
        unit.nextCommandQueuePosition();
      return true;
    }
  }
  return false;
}

void CombatManager::updateDecision(UnitInfo& unit)
{
  if (!unit.getUnit() || !unit.getUnit()->exists()
    || unit.isLockedDown() || unit.isMaelstrommed() || unit.isStasised() || !unit.isCompleted())
    return;

  // Check if worker is still under durress or should we switch it back to a worker.
  if (unit.getType().isWorker())
  {
    updateRole(unit);
    return;
  }
  updateTargets(unit);
  if (!wait(unit))
    if (!cast(unit))
      if (!special(unit))
        if (!attack(unit))
          if (!move(unit))
          {
            // Check if we're on a command in a command queue that isn't for us.
            if (unit.getWave()
              && unit.getWave()->getUseCommandQueue()
              && (bot->getBuildOrder().getCommandQueue()[unit.getCommandQueuePosition()].unitTypes != BWAPI::UnitTypes::None
                && bot->getBuildOrder().getCommandQueue()[unit.getCommandQueuePosition()].unitTypes != unit.getType()))
              unit.nextCommandQueuePosition();
          }
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
  // Only for combat units
  if (unit.getRole() == Roles::Defender)
    return;

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
    if (unit.getWave()->isGathering())
      unit.setTargetRegion(unit.getWave()->getOldTarget());
    else
      unit.setTargetRegion(unit.getWave()->getTarget());
    unit.setTargetPosition(unit.getWave()->getTargetPosition());
  }
}

void CombatManager::updateUnits()
{
  for (auto& u : bot->getUnitManager().getUnits(PlayerState::Self))
  {
    if (u->getRole() == Roles::Combat
      || (u->getRole() == Roles::Defender && !u->getType().isBuilding()))
      updateDecision(*u);
  }
}

void CombatManager::updateWaves()
{

}