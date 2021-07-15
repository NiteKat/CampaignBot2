#pragma once

struct BuildingManager
{
public:
  int getQueuedGas() { return queuedGas; }
  int getQueuedMinerals() { return queuedMinerals; }
  void onFrame();
private:
  void queueBuildings();

  int queuedGas = 0;
  int queuedMinerals = 0;
};