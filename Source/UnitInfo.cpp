#include "CampaignBot.h"

// ------------------ PUBLIC FUNCTIONS ------------------ //

bool UnitInfo::canAttackGround()
{
  return groundDamage > 0.0;
}

bool UnitInfo::canAttackAir()
{
  return airDamage > 0.0;
}

void UnitInfo::setWave(WaveInfo* newWave)
{
  newWave ? wave = newWave->weak_from_this() : wave.reset();
}

void UnitInfo::setTown(TownInfo* newTown)
{ 
  newTown ? town = newTown->weak_from_this() : town.reset();
}

void UnitInfo::removeWorker(UnitInfo& unit)
{
  auto u = unit.weak_from_this();
  for (auto itr = targetedBy.begin(); itr != targetedBy.end(); itr++)
  {
    if ((itr)->lock() == u.lock())
    {
      targetedBy.erase(itr);
      break;
    }
  }
}

void UnitInfo::update()
{
  if (bwUnit)
  {
    if (BWAPI::Broodwar->getFrameCount() == 0)
      startingUnit = true;

    bwExists = bwUnit->exists();
    if (bwExists)
    {
      bwId = bwUnit->getID();
      bwPlayer = bwUnit->getPlayer();
      bwType = bwUnit->getType();
      bwPosition = bwUnit->getPosition();
      bwTilePosition = bwUnit->getTilePosition();
      bwAngle = bwUnit->getAngle();
      bwVelocityX = bwUnit->getVelocityX();
      bwVelocityY = bwUnit->getVelocityY();
      bwRegion = bwUnit->getRegion();
      bwLeft = bwUnit->getLeft();
      bwTop = bwUnit->getTop();
      bwRight = bwUnit->getRight();
      bwBottom = bwUnit->getBottom();
      bwHitPoints = bwUnit->getHitPoints();
      bwShields = bwUnit->getShields();
      bwEnergy = bwUnit->getEnergy();
      bwResources = bwUnit->getResources();
      bwResourceGroup = bwUnit->getResourceGroup();
      bwLastCommandFrame = bwUnit->getLastCommandFrame();
      bwLastAttackingPlayer = bwUnit->getLastAttackingPlayer();
      bwInitalType = bwUnit->getInitialType();
      bwInitialPosition = bwUnit->getInitialPosition();
      bwInitialTilePosition = bwUnit->getInitialTilePosition();
      bwInitialHitPoints = bwUnit->getInitialHitPoints();
      bwInitialResources = bwUnit->getInitialResources();
      bwAcidSporeCount = bwUnit->getAcidSporeCount();
      bwInterceptorCount = bwUnit->getInterceptorCount();
      bwScarabCount = bwUnit->getScarabCount();
      bwSpiderMineCount = bwUnit->getSpiderMineCount();
      bwGroundWeaponCooldown = bwUnit->getGroundWeaponCooldown();
      bwAirWeaponCooldown = bwUnit->getAirWeaponCooldown();
      bwSpellCooldown = bwUnit->getSpellCooldown();
      bwDefenseMatrixPoints = bwUnit->getDefenseMatrixPoints();
      bwDefenseMatrixTimer = bwUnit->getDefenseMatrixTimer();
      bwEnsnareTimer = bwUnit->getEnsnareTimer();
      bwIrradiateTimer = bwUnit->getIrradiateTimer();
      bwLockdownTimer = bwUnit->getLockdownTimer();
      bwMaelstromTimer = bwUnit->getMaelstromTimer();
      bwOrderTimer = bwUnit->getOrderTimer();
      bwPlagueTimer = bwUnit->getPlagueTimer();
      bwRemoveTimer = bwUnit->getRemoveTimer();
      bwStasisTimer = bwUnit->getStasisTimer();
      bwStimTimer = bwUnit->getStimTimer();
      bwBuildType = bwUnit->getBuildType();
      bwTrainingQueue = bwUnit->getTrainingQueue();
      bwTech = bwUnit->getTech();
      bwUpgrade = bwUnit->getUpgrade();
      bwRemainingBuildTime = bwUnit->getRemainingBuildTime();
      bwRemainingTrainTime = bwUnit->getRemainingTrainTime();
      bwRemainingResearchTime = bwUnit->getRemainingResearchTime();
      bwRemainingUpgradeTime = bwUnit->getRemainingUpgradeTime();
      bwOrder = bwUnit->getOrder();
      bwSecondaryOrder = bwUnit->getSecondaryOrder();
      bwSpaceRemaining = bwUnit->getSpaceRemaining();
      bwHasNuke = bwUnit->hasNuke();
      bwIsAccelerating = bwUnit->isAccelerating();
      bwIsAttacking = bwUnit->isAttacking();
      bwIsAttackFrame = bwUnit->isAttackFrame();
      bwIsBeingConstructed = bwUnit->isBeingConstructed();
      bwIsBeingGathered = bwUnit->isBeingGathered();
      bwIsBeingHealed = bwUnit->isBeingHealed();
      bwIsBlind = bwUnit->isBlind();
      bwIsBraking = bwUnit->isBraking();
      bwIsBurrowed = bwUnit->isBurrowed();
      bwIsCarryingGas = bwUnit->isCarryingGas();
      bwIsCarryingMinerals = bwUnit->isCarryingMinerals();
      bwIsCloaked = bwUnit->isCloaked();
      bwIsCompleted = bwUnit->isCompleted();
      bwIsConstructing = bwUnit->isConstructing();
      bwIsDetected = bwUnit->isDetected();
      bwIsFlying = bwUnit->isFlying();
      bwIsGatheringGas = bwUnit->isGatheringGas();
      bwIsGatheringMinerals = bwUnit->isGatheringMinerals();
      bwIsHallucination = bwUnit->isHallucination();
      bwIsHoldingPosition = bwUnit->isHoldingPosition();
      bwIsIdle = bwUnit->isIdle();
      bwIsInterruptible = bwUnit->isInterruptible();
      bwIsInvincible = bwUnit->isInvincible();
      bwIsLifted = bwUnit->isLifted();
      bwIsMorphing = bwUnit->isMorphing();
      bwIsMoving = bwUnit->isMoving();
      bwIsParasited = bwUnit->isParasited();
      bwIsRepairing = bwUnit->isRepairing();
      bwIsStartingAttack = bwUnit->isStartingAttack();
      bwIsStuck = bwUnit->isStuck();
      bwIsTraining = bwUnit->isTraining();
      bwIsUnderAttack = bwUnit->isUnderAttack();
      bwIsUnderDarkSwarm = bwUnit->isUnderDarkSwarm();
      bwIsUnderDisruptionWeb = bwUnit->isUnderDisruptionWeb();
      bwIsUnderStorm = bwUnit->isUnderStorm();
      bwIsPowered = bwUnit->isPowered();
      int upLevel = bwPlayer->getUpgradeLevel(bwType.airWeapon().upgradeType());
      if (bwType == BWAPI::UnitTypes::Terran_Bunker)
        airDamage = 24.0 + (4.0 * upLevel);
      else if (bwType == BWAPI::UnitTypes::Terran_Valkyrie)
        airDamage = (48.0 * 0.36) + (8.0 * upLevel);
      else
        airDamage = bwType.airWeapon().damageAmount() + (bwType.airWeapon().damageBonus() * upLevel);

      upLevel = bwPlayer->getUpgradeLevel(bwType.groundWeapon().upgradeType());
      if (bwType == BWAPI::UnitTypes::Terran_Bunker)
        groundDamage = 24.0 + (4.0 * upLevel);
      else if (bwType == BWAPI::UnitTypes::Terran_Firebat)
        groundDamage = 16.0 + (2.0 * upLevel);
      else
        groundDamage = bwType.groundWeapon().damageAmount() + (bwType.groundWeapon().damageBonus() * upLevel);

      if (bwIsConstructing || bwUnit->getDistance(BWAPI::Position(buildTarget)) < 150 && !BWAPI::Broodwar->canBuildHere(buildTarget, buildType))
      {
        buildTarget = BWAPI::TilePositions::None;
        buildType = BWAPI::UnitTypes::None;
      }
    }
    else
    {
      bwGroundWeaponCooldown = std::max(0, bwGroundWeaponCooldown - 1);
      bwAirWeaponCooldown = std::max(0, bwAirWeaponCooldown - 1);
      bwSpellCooldown = std::max(0, bwSpellCooldown - 1);
      bwDefenseMatrixPoints = bwDefenseMatrixTimer > 1 ? bwDefenseMatrixPoints : 0;
      bwDefenseMatrixTimer = std::max(0, bwDefenseMatrixTimer - 1);
      bwEnsnareTimer = std::max(0, bwEnsnareTimer - 1);
      bwIrradiateTimer = std::max(0, bwIrradiateTimer - 1);
      bwLockdownTimer = std::max(0, bwLockdownTimer - 1);
      bwMaelstromTimer = std::max(0, bwMaelstromTimer - 1);
      bwPlagueTimer = std::max(0, bwPlagueTimer - 1);
      bwRemoveTimer = std::max(0, bwRemoveTimer - 1);
      bwStasisTimer = std::max(0, bwStasisTimer - 1);
      bwStimTimer = std::max(0, bwStimTimer - 1);
      bwRemainingBuildTime = std::max(0, bwRemainingBuildTime - 1);
      bwRemainingTrainTime = std::max(0, bwRemainingTrainTime - 1);
      bwRemainingResearchTime = std::max(0, bwRemainingResearchTime - 1);
      bwRemainingUpgradeTime = std::max(0, bwRemainingUpgradeTime - 1);
    }
  }
}

// ------------------ PRIVATE FUNCTIONS ----------------- //

// ---------------------- OPERATORS --------------------- //
bool operator< (const std::weak_ptr<UnitInfo>(l), const std::weak_ptr<UnitInfo>(r))
{
  return l.lock()->getUnit() < r.lock()->getUnit();
}

bool operator< (const std::shared_ptr<UnitInfo>(l), const std::shared_ptr<UnitInfo>(r))
{
  return l->getUnit() < r->getUnit();
}