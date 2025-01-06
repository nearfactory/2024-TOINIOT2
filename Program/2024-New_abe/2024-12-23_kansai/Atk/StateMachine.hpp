#pragma once

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
};

extern State state;
extern State state_prev;