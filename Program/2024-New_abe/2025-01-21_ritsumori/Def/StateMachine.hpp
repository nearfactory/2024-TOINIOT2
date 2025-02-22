#pragma once

enum class State : int{
  KickOff,
  LineTrace,
  KeeperDash,
  BackToGoal_Weak,
  BackToGoal_Strong,
  Center,
  Test
};

extern State state;