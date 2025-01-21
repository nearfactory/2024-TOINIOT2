#pragma once

enum class State : int{
  KickOff,
  LineTrace,
  KeeperDash,
  BackToGoal_Weak,
  BackToGoal_Strong
};

extern State state;