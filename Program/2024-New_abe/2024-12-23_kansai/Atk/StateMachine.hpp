#pragma once

enum class State : int{
  KickOff,
  Follow, 
  Dribble,
  Neutral,
  AvoidKeeper,
  Pushing
};

extern State state;