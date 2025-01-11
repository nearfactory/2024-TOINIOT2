#pragma once

#define COMPILE 32

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
  Oshikomi,
  Test
};

extern State state;
extern State state_prev;