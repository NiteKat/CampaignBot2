#include "CampaignBot.h"

std::shared_ptr<UnitInfo> UnitManager::getUnitInfo(BWAPI::Unit unit)
{
  for (auto& p : bot->getPlayerManager().getPlayers())
  {
    for (auto& u : p->getUnits())
      if (u->getUnit()->getID() == unit->getID())
        return u;
  }
  return nullptr;
}

std::set<std::shared_ptr<UnitInfo>>& UnitManager::getUnits(PlayerState state)
{
  switch (state)
  {
  case PlayerState::Self:
  case PlayerState::None:
    return myUnits;
  case PlayerState::Neutral:
    return neutralUnits;
  case PlayerState::Enemy:
    return enemyUnits;
  }
  return myUnits;
}

std::set<std::shared_ptr<UnitInfo>> UnitManager::getUnitsInRegion(PlayerState state, BWAPI::Region region)
{
  std::set<std::shared_ptr<UnitInfo>> returnValue;
  for (auto& unit : getUnits(state))
  {
    if (unit->getUnit()->getRegion() == region)
    {
      returnValue.insert(unit);
    }
  }

  return returnValue;
}

void UnitManager::onFrame()
{
  updateUnits();
  updateCounts();
}

void UnitManager::updateCounts()
{
  enemyUnits.clear();
  myUnits.clear();
  myCompletedTypes.clear();
  myInactiveVisibleTypes.clear();
  myVisibleTypes.clear();
  myQueuedTypes.clear();
  neutralUnits.clear();

  for (auto& p : bot->getPlayerManager().getPlayers())
  {
    if (p->isSelf())
    {
      for (auto& u : p->getUnits())
      {
        myUnits.insert(u);
        if (u->getType().isBuilding())
          bot->setBuildings(true);
        if (u->getType().isBuilding())
        {
          myVisibleTypes[u->getType()]++;
          if (u->hasWave() && !u->getWave()->isActive())
            myInactiveVisibleTypes[u->getType()]++;
        }
        if (u->hasBuildTarget())
          myQueuedTypes[u->getBuildType()]++;
        if (u->isCompleted())
          myCompletedTypes[u->getType()]++;
      }
        
    }
    if (p->getPlayerState() == PlayerState::Neutral)
    {
      for (auto& u : p->getUnits())
      {
        if (u->getType().isRefinery())
        {
          if (BWAPI::Broodwar->self() == u->getUnit()->getPlayer())
          {
            myUnits.insert(u);
            myVisibleTypes[u->getType()]++;
            if (u->isCompleted())
              myCompletedTypes[u->getType()]++;
          }
          else if (BWAPI::Broodwar->self()->isEnemy(u->getUnit()->getPlayer()))
            enemyUnits.insert(u);
          else if (BWAPI::Broodwar->self()->isAlly(u->getUnit()->getPlayer()))
            allyUnits.insert(u);
          else
            neutralUnits.insert(u);
        }
        else
          neutralUnits.insert(u);
      }
    }
    if (p->getPlayerState() == PlayerState::Enemy)
    {
      for (auto& u : p->getUnits())
        enemyUnits.insert(u);
    }
  }
}

void UnitManager::updateNeutrals()
{
  for (auto& player : bot->getPlayerManager().getPlayers())
  {
    if (player->getPlayerState() == PlayerState::Neutral)
    {

      for (auto& unit : player->getUnits())
      {
        unit->update();
      }
    }
  }
}

void UnitManager::updateRole(UnitInfo& unit)
{
  // Don't assign a role to incomplete units.
  if (!unit.isCompleted() && !unit.getType().isBuilding() && unit.getType() != BWAPI::UnitTypes::Zerg_Egg)
  {
    unit.setRole(Roles::None);
    return;
  }

  // Assign default roles.
  if (unit.getRole() == Roles::None)
  {
    if (unit.getType().isWorker())
      unit.setRole(Roles::Worker);
    else if ((!unit.getType().isBuilding() 
        && BWAPI::Broodwar->getFrameCount() == 0)
        || (unit.getType().isBuilding()
        && !unit.getType().buildsWhat().size()
        && !unit.getType().supplyProvided()))
      unit.setRole(Roles::Defender);
    else if (!unit.getType().isBuilding())
      unit.setRole(Roles::Combat);
    else if (unit.getType().isBuilding() && !unit.canAttackGround() && !unit.canAttackAir())
      unit.setRole(Roles::Production);
  }
}

void UnitManager::updateSelf()
{
  for (auto& player : bot->getPlayerManager().getPlayers())
  {
    if (player->isSelf())
    {
      
      for (auto& unit : player->getUnits())
      {
        unit->update();
        this->updateRole(*unit);
        if (unit->getRole() == Roles::Worker && !unit->getTown())
        {
          auto closestDepot = getClosestUnit(unit->getPosition(), PlayerState::Self, [&](auto& u) {
            return u->getType().isResourceDepot() && u->getTown();
          });
          if (closestDepot && closestDepot->getTown())
            closestDepot->getTown()->addUnit(*unit);
        }
      }
    }
  }
}

void UnitManager::updateUnits()
{
  for (auto unit : BWAPI::Broodwar->getAllUnits())
  {
    auto info = this->getUnitInfo(unit);
    if (!info)
      bot->getPlayerManager().storeUnit(unit);
  }

  this->updateSelf();
  this->updateNeutrals();
}