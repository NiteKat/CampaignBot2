#pragma once
#include <BWAPI.h>

#include <memory>

#include "Roles.h"

struct TownInfo;
struct WaveInfo;

struct UnitInfo : std::enable_shared_from_this<UnitInfo>
{
public:
  UnitInfo();
  UnitInfo(BWAPI::Unit newUnit) : bwUnit(newUnit) {}
  
  void addWorker(UnitInfo& unit) { targetedBy.push_back(unit.weak_from_this()); }
  bool attack(BWAPI::Position target, bool shiftQueueCommand = false) { return bwUnit->attack(target, shiftQueueCommand); }
  bool build(BWAPI::UnitType type, BWAPI::TilePosition target) { return bwUnit->build(type, target); }
  bool canAttackGround();
  bool canAttackAir();
  bool gather(UnitInfo& unit, bool shiftQueueCommand = false) { return bwUnit->gather(unit.getUnit(), shiftQueueCommand); }
  double getAirDamage() { return airDamage; }
  BWAPI::TilePosition getBuildTarget() { return buildTarget; }
  BWAPI::UnitType getBuildType() { return buildType; }
  int getDistance(BWAPI::Position here) { return bwUnit->getDistance(here); }
  int getGathererCount() { return int(targetedBy.size()); }
  double getGroundDamage() { return groundDamage; }
  BWAPI::Position getPosition() { return bwPosition; }
  BWAPI::Region getRegion() { return bwRegion; }
  int getRemainingTrainFrames() { return bwRemainingTrainTime; }
  std::shared_ptr<UnitInfo> getResource() { return resource.lock(); }
  int getResourceGroup() { return bwResourceGroup; }
  Roles getRole() { return role; }
  int getSpaceRemaining() { return bwSpaceRemaining; }
  std::vector<std::weak_ptr<UnitInfo>>& getTargetedBy() { return targetedBy; }
  BWAPI::Region getTargetRegion() { return targetRegion; }
  std::shared_ptr<TownInfo> getTown() { return town.lock(); }
  BWAPI::UnitType getType() { return bwType; }
  BWAPI::Unit getUnit() { return bwUnit; }
  std::shared_ptr<WaveInfo> getWave() { return wave.lock(); }
  bool hasBuildTarget() { return buildTarget != BWAPI::TilePositions::None; }
  bool hasResource() { return !resource.expired(); }
  bool hasWave() { return !wave.expired(); }
  bool isCarryingPowerup() { return bwUnit->getPowerUp(); }
  bool isCompleted() { return bwIsCompleted; }
  bool isConstructing() { return bwIsConstructing; }
  bool isGathering() { return bwIsGatheringGas || bwIsGatheringMinerals; }
  bool isGatheringGas() { return bwIsGatheringGas; }
  bool isGatheringMinerals() { return bwIsGatheringMinerals; }
  bool isIdle() { return bwIsIdle; }
  bool isLifted() { return bwIsLifted; }
  bool isLockedDown() { return bwLockdownTimer > 0; }
  bool isMaelstrommed() { return bwMaelstromTimer > 0; }
  bool isStartingUnit() { return startingUnit; }
  bool isStasised() { return bwStasisTimer > 0; }
  bool isUnderAttack() { return bwIsUnderAttack; }
  bool move(BWAPI::Position target, bool shiftQueueCommand = false) { return bwUnit->move(target, shiftQueueCommand); }
  void removeWorker(UnitInfo& unit);
  bool rightClick(BWAPI::Unit target, bool shiftQueueCommand = false) { return bwUnit->rightClick(target, shiftQueueCommand); }
  void setBuildTarget(BWAPI::TilePosition newBuildTarget) { buildTarget = newBuildTarget; }
  void setBuildType(BWAPI::UnitType newType) { buildType = newType; }
  void setResource(UnitInfo* unit) { unit ? resource = unit->weak_from_this() : resource.reset(); }
  void setRole(Roles newRole) { role = newRole; }
  void setTargetRegion(BWAPI::Region newRegion) { targetRegion = newRegion; }
  void setTown(TownInfo* newTown);
  void setWave(WaveInfo* newWave);
  bool stop(bool queueShiftCommand = false) { return bwUnit->stop(queueShiftCommand); }
  bool train(BWAPI::UnitType type) { return bwUnit->train(type); }
  void update();
  bool upgrade(BWAPI::UpgradeType upgrade) { return bwUnit->upgrade(upgrade); }

private:
  double airDamage = 0.0;
  BWAPI::TilePosition buildTarget = BWAPI::TilePositions::None;
  BWAPI::UnitType buildType = BWAPI::UnitTypes::None;
  int bwAcidSporeCount = 0;
  int bwAirWeaponCooldown = 0;
  double bwAngle = 0.0;
  int bwBottom = 0;
  BWAPI::UnitType bwBuildType = BWAPI::UnitTypes::None;
  int bwDefenseMatrixPoints = 0;
  int bwDefenseMatrixTimer = 0;
  int bwEnergy = 0;
  int bwEnsnareTimer = 0;
  bool bwExists = false;
  int bwGroundWeaponCooldown = 0;
  bool bwHasNuke = false;
  int bwHitPoints = 0;
  int bwId = -1;
  int bwInitialHitPoints = 0;
  BWAPI::Position bwInitialPosition = BWAPI::Positions::None;
  int bwInitialResources = 0;
  BWAPI::TilePosition bwInitialTilePosition = BWAPI::TilePositions::None;
  BWAPI::UnitType bwInitalType = BWAPI::UnitTypes::None;
  int bwInterceptorCount = 0;
  int bwIrradiateTimer = 0;
  bool bwIsAccelerating = false;
  bool bwIsAttackFrame = false;
  bool bwIsAttacking = false;
  bool bwIsBeingConstructed = false;
  bool bwIsBeingGathered = false;
  bool bwIsBeingHealed = false;
  bool bwIsBlind = false;
  bool bwIsBraking = false;
  bool bwIsBurrowed = false;
  bool bwIsCarryingGas = false;
  bool bwIsCarryingMinerals = false;
  bool bwIsCloaked = false;
  bool bwIsCompleted = false;
  bool bwIsConstructing = false;
  bool bwIsDetected = false;
  bool bwIsFlying = false;
  bool bwIsGatheringGas = false;
  bool bwIsGatheringMinerals = false;
  bool bwIsHallucination = false;
  bool bwIsHoldingPosition = false;
  bool bwIsIdle = false;
  bool bwIsInterruptible = false;
  bool bwIsInvincible = false;
  bool bwIsLifted = false;
  bool bwIsMorphing = false;
  bool bwIsMoving = false;
  bool bwIsParasited = false;
  bool bwIsPowered = false;
  bool bwIsRepairing = false;
  bool bwIsStartingAttack = false;
  bool bwIsStuck = false;
  bool bwIsTraining = false;
  bool bwIsUnderAttack = false;
  bool bwIsUnderDarkSwarm = false;
  bool bwIsUnderDisruptionWeb = false;
  bool bwIsUnderStorm = false;
  BWAPI::Player bwLastAttackingPlayer;
  int bwLastCommandFrame = 0;
  int bwLeft = 0;
  int bwLockdownTimer = 0;
  int bwMaelstromTimer = 0;
  BWAPI::Order bwOrder = BWAPI::Orders::None;
  int bwOrderTimer = 0;
  int bwPlagueTimer = 0;
  BWAPI::Player bwPlayer = nullptr;
  BWAPI::Position bwPosition = BWAPI::Positions::None;
  BWAPI::Region bwRegion = nullptr;
  int bwRemainingBuildTime = 0;
  int bwRemainingResearchTime = 0;
  int bwRemainingTrainTime = 0;
  int bwRemainingUpgradeTime = 0;
  int bwRemoveTimer = 0;
  int bwResourceGroup = -1;
  int bwResources = 0;
  int bwRight = 0;
  Roles role = Roles::None;
  int bwScarabCount = 0;
  BWAPI::Order bwSecondaryOrder = BWAPI::Orders::None;
  int bwShields = 0;
  int bwSpaceRemaining = 0;
  int bwSpellCooldown = 0;
  int bwSpiderMineCount = 0;
  int bwStasisTimer = 0;
  int bwStimTimer = 0;
  BWAPI::TechType bwTech = BWAPI::TechTypes::None;
  BWAPI::TilePosition bwTilePosition = BWAPI::TilePositions::None;
  int bwTop = 0;
  BWAPI::UnitType::list bwTrainingQueue;
  BWAPI::UnitType bwType = BWAPI::UnitTypes::None;
  BWAPI::Unit bwUnit = nullptr;
  BWAPI::UpgradeType bwUpgrade = BWAPI::UpgradeTypes::None;
  double bwVelocityX = 0.0;
  double bwVelocityY = 0.0;
  double groundDamage = 0.0;
  std::weak_ptr<UnitInfo> resource;
  bool startingUnit;
  std::vector<std::weak_ptr<UnitInfo>> targetedBy;
  BWAPI::Region targetRegion = nullptr;
  std::weak_ptr<TownInfo> town;
  std::weak_ptr<WaveInfo> wave;
};

bool operator< (const std::weak_ptr<UnitInfo>(l), const std::weak_ptr<UnitInfo>(r));

bool operator< (const std::shared_ptr<UnitInfo>(l), const std::shared_ptr<UnitInfo>(r));