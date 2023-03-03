#ifndef INITIAL_SETTINGS_H_
#define INITIAL_SETTINGS_H_

/*
2019-07-13 鈴本
とりあえず，いきなり初期シーケンスを分岐させるのは怖いので，
ここでdefineして弾く
*/


// 設定
#define INITIAL_SEQUENCE_FOR_GROUND_TEST       0
#define INITIAL_SEQUENCE_FOR_AFTER_DEPLOY      1
#define INITIAL_SEQUENCE_FOR_AFTER_RESET       2

// ↓ここをいじって初期シーケンスを切り替える！
#define INITIAL_SEQUENCE INITIAL_SEQUENCE_FOR_GROUND_TEST

// 地上試験時に衛星に対してダメージを与えうる部分をコンパイルしないためのフラグ
#define GROUND_TEST             // 1ビット通信用
#define SAP_GROUND_TEST         // HRMが動作しないように使用
#define SADA_GROUND_TEST         // SADA駆動部分が動作しないように使用

#endif
