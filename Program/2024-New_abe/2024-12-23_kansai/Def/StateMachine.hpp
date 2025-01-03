#pragma once

enum class State{
  LineTrace,
  KeeperDash,
  BackToGoal_Weak,
  BackToGoal_Strong
};

extern State state;