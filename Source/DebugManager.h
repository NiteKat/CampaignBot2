#pragma once
#include <string>

struct DebugManager
{
public:
  void onFrame();
private:
  void drawMapName();
  void drawTextMap(BWAPI::Position, std::string);
  void drawTextScreen(BWAPI::Position, std::string);
  void drawTownPanel();
  void drawUnitPanel();
  void drawUnitRoles();
  void drawWavePanel();
  const std::string roleTable[5] = {"None", "Worker", "Defender", "Production", "Combat"};
};