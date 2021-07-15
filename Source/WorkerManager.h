#pragma once

struct WorkerManager
{
public:
  void onFrame();
private:
  void updateDecision();
  void updateResources(UnitInfo&);
  void updateUnits();
};