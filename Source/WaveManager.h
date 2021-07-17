#pragma once
#include <set>
#include <vector>

struct WaveManager
{
public:
  std::set<std::shared_ptr<WaveInfo>>& getWaves() { return waveList; }
  void onFrame();
  void onStart();
private:
  std::vector<BWAPI::Region> findFirstUnexploredRegion(BWAPI::Region startRegion);
  void removeEmptyWaves();
  void updateWaves();

  int currentWave = 1;
  std::set<std::shared_ptr<WaveInfo>> waveList;
};