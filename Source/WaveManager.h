#pragma once
#include <set>

struct WaveManager
{
public:
  void onFrame();
  void onStart();
private:
  BWAPI::Region findFirstUnexploredRegion(BWAPI::Region startRegion);
  void removeEmptyWaves();
  void updateWaves();

  int currentWave = 1;
  std::set<std::shared_ptr<WaveInfo>> waveList;
};