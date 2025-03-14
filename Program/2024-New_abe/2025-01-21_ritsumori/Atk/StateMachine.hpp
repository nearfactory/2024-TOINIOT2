#pragma once

#define COMPILE 76

enum class State : int{
  KickOff,      // キックオフ
  Damaged,      // 故障復帰
  Follow,       // 回り込み
  Dribble,      // ドリブル
  Shoot,        // (未使用)シュート
  AvoidKeeper,  // (未使用)敵避け
  Pushing,      // 押し合い
  NoBall,       // ボールなし
  Neutral,      // (未使用)中立点から
  Oshikomi,     // 押し込み
  Ritsumori,
  Test          // テスト
};

extern State state;
extern State state_prev;