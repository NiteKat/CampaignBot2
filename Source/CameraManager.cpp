#include "CampaignBot.h"
// ------------------ PUBLIC FUNCTIONS ------------------ //
void CameraManager::onFrame()
{
  updateUnits();
  updateFollowUnit();
  updateCamera();
}

void CameraManager::onStart()
{
  screenPosition = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation()) - BWAPI::Position(340, 240);
}

// ------------------ PRIVATE FUNCTIONS ----------------- //
void CameraManager::updateCamera()
{
  if (!followingUnit || !followingUnit->exists())
    return;

  auto& target = followingUnit->getPosition();
  // Check if unit is centered
  auto diff = target - (screenPosition + BWAPI::Position(320, 240));
  if (diff.x != 0 || diff.y != 0)
  {
    auto sgn = [](int num) {
      return (0 < num) - (num < 0);
    };
    auto newPos = screenPosition;
    if (std::abs(diff.x) > 10)
      newPos += BWAPI::Position(10 * sgn(diff.x), 0);
    else
      newPos += BWAPI::Position(diff.x, 0);
    if (std::abs(diff.y) > 10)
      newPos += BWAPI::Position(0, 10 * sgn(diff.y));
    else
      newPos += BWAPI::Position(0, diff.y);
    newPos.makeValid();
    BWAPI::Broodwar->setScreenPosition(newPos);
    screenPosition = newPos;
  }
}

void CameraManager::updateFollowUnit()
{
  // Check if we've been focusing the camera for more than 30 second
  // and that the unit we were following is still alive.
  if (BWAPI::Broodwar->getFrameCount() - frameChanged < 720 &&
      followingUnit && followingUnit->exists())
    return;

  // find last most interesting event to happen since we've followed this unit.
  BWAPI::Unit bestUnit = nullptr;
  int bestFrame = INT_MAX;
  for (auto& [unit, frame] : pastEvents)
  {
    if (frame < bestFrame && unit->exists())
    {
      bestUnit = unit;
      bestFrame = frame;
    }
  }
  if (bestUnit)
    followingUnit = bestUnit;
  pastEvents.clear();
}
void CameraManager::updateUnits()
{
  // Clear following unit if it no longer exists.
  if (followingUnit && !followingUnit->exists())
    followingUnit = nullptr;

  for (auto& unit : bot->getUnitManager().getUnits(PlayerState::Self))
  {
    if (unit->isUnderAttack() || unit->getUnit()->isAttacking())
    {
      // Get distance from screen position to see if it's off screen.
      auto diff = unit->getPosition() - BWAPI::Broodwar->getScreenPosition() / 16;
      if (diff.x < 0 || diff.y < 0 || 640 < diff.x || 480 < diff.y) // off screen
        pastEvents[unit->getUnit()] = BWAPI::Broodwar->getFrameCount();
    }
  }
}