#pragma once

enum class State : int{
  LineTrace,
  KeeperDash,
  BackToGoal_Weak,
  BackToGoal_Strong
};

extern State state;