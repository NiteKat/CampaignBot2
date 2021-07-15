#pragma once
#include <BWAPI.h>
#include <map>

struct CameraManager
{
public:
  void addEvent(BWAPI::Unit unit) { pastEvents[unit] = BWAPI::Broodwar->getFrameCount(); }
  void onFrame();
  void onStart();
private:
  void updateCamera();
  void updateFollowUnit();
  void updateUnits();

  int frameChanged = 0;
  BWAPI::Unit followingUnit = nullptr;
  std::map<BWAPI::Unit, int> pastEvents;
  BWAPI::Position screenPosition = BWAPI::Positions::None;
};