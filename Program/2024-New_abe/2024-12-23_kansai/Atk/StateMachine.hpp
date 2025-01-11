#pragma once

#define COMPILE 9

enum class State : int{
  KickOff,
  Damaged,
  Follow, 
  Dribble,
  Shoot,
  AvoidKeeper,
  Pushing,
  NoBall,
  Neutral,
  Test
};

extern State state;
extern State state_prev;