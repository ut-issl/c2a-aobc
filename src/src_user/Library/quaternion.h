/**
* @file   quaternion.h
* @brief  quaternion関連の関数群の定義
* @note   全関数の末尾で規格化を行うため，ライブラリ内の演算結果は正規化が保たれる見込み
* @note   スカラー部の符号強制 (正に固定) は行わない (暫定仕様)
*/

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "stdint.h"
#include "physical_constants.h"
#include "c2a_math.h"
#include "euler_angle.h"

/**
 * @struct Quaternion
 * @brief  quaternion構造体
 *
 *         実部，虚部の配列上位置の定義が世の中に複数あって面倒くさい+ライブラリ実装上の簡便さから，最初から実部，虚部を分ける
 */
typedef struct
{
  float vector_part[PHYSICAL_CONST_THREE_DIM];
  float scalar_part;
} Quaternion;

/**
 * @enum   QUATERNION_SCALAR_POSITION
 * @brief  要素4配列中におけるquaternionスカラー部位置の指定
 * @note   uint8_tを想定 (0か3のどちらかのみ想定)
 */
typedef enum
{
  QUATERNION_SCALAR_POSITION_FIRST = 0,
  QUATERNION_SCALAR_POSITION_LAST  = 3
} QUATERNION_SCALAR_POSITION;


// Quaternion生成関数
// 生成関数のうち，正規化条件の崩れた引数を渡されうるものについてはC2A_MATH_ERRORを返す

/**
 * @brief  float型 array[PHYSICAL_CONST_QUATERNION_DIM]からQuaternion生成
 * @param[out]  *q_out     : Quaternion格納先
 * @param[in]   array      : 元の配列
 * @param[in]   position   : 配列中のスカラー部位置 (0か3のどちらかのみ想定)
 * @return C2A_MATH_ERROR
 * @note   入力が規格化されていないこととゼロ割を生じる値であることを区別してエラー出力
 * @note   入力がゼロ割を生じる場合はunitq uaternionを返し，入力が規格化されていない場合は規格化する
 */
C2A_MATH_ERROR QUATERNION_make_from_array(Quaternion* q_out,
                                          const float array[PHYSICAL_CONST_QUATERNION_DIM],
                                          const QUATERNION_SCALAR_POSITION position);

/**
 * @brief  float型 array[PHYSICAL_CONST_QUATERNION_DIM]からQuaternion生成（ただし，規格化を行わない）
 * @param[out]  *q_out     : Quaternion格納先
 * @param[in]   array      : 元の配列
 * @param[in]   position   : 配列中のスカラー部位置 (0か3のどちらかのみ想定)
 * @return C2A_MATH_ERROR
 * @note   positionに不正な値が入ったときのみエラー出力
 * TODO_L: Quaternion伝播を行う際に規格化を伴わないQuaternion生成関数が必要になって定義しているが，規格化ありの関数とどう組み合わせるか要相談
 */
C2A_MATH_ERROR QUATERNION_make_from_array_unnormalizing(Quaternion* q_out,
                                                        const float array[PHYSICAL_CONST_QUATERNION_DIM],
                                                        const QUATERNION_SCALAR_POSITION position);

/**
 * @brief  Euler軸と角度からのQuaternion生成
 * @param[out]  *q_out     : Quaternion格納先
 * @param[in]   axis       : 回転軸ベクトル
 * @param[in]   angle_rad  : axis周りの回転角 [rad]
 * @return C2A_MATH_ERROR
 * @note   入力軸が規格化されていないこととゼロ割を生じる値であることを区別してエラー出力
 * @note   入力がゼロ割を生じる場合はunitq uaternionを返し，入力軸が規格化されていない場合は規格化する
 */
C2A_MATH_ERROR QUATERNION_make_from_axis_angle(Quaternion* q_out,
                                               const float axis[PHYSICAL_CONST_THREE_DIM],
                                               const float angle_rad);

/**
 * @brief  座標変換行列からのQuaternion生成
 * @param[out]  *q_out    : Quaternion格納先
 * @param[in]   matrix     : 元の行列 (2次元配列)
 * @return C2A_MATH_ERROR
 * @note   元行列が規格化されていないこととゼロ割を生じる値であることを区別してエラー出力
 * @note   入力がゼロ割を生じる場合はunitq uaternionを返し，元行列が規格化されていない場合は規格化する
 */
C2A_MATH_ERROR QUATERNION_make_from_matrix(Quaternion* q_out,
                                           const float matrix[][PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief  Euler角からQuaternion生成
 * @param[out]  *q_out    : Quaternion格納先
 * @param[in]   angle_1_rad    : 1回目の回転軸周りの回転角 [rad]
 * @param[in]   angle_2_rad    : 2回目の回転軸周りの回転角 [rad]
 * @param[in]   angle_3_rad    : 3回目の回転軸周りの回転角 [rad]
 * @param[in]   rotation_order : EULER_ANGLE_ROTATION_ORDERに則した回転順序
 * @return C2A_MATH_ERROR
 */
C2A_MATH_ERROR QUATERNION_make_from_euler_angles(Quaternion* q_out,
                                                 const float angle_1_rad,
                                                 const float angle_2_rad,
                                                 const float angle_3_rad,
                                                 const EULER_ANGLE_ROTATION_ORDER rotation_order);

/**
 * @brief   実部と虚部を渡してQuaternion生成
 * @param[out]       q_out : Quaternion格納先
 * @param[in]  vector_part : Quaternionの虚部
 * @param[in]  scalar_part : Quaternionの実部
*/
C2A_MATH_ERROR QUATERNION_make_from_vector_scalar(Quaternion* q_out,
                                                  const float vector_part[PHYSICAL_CONST_THREE_DIM],
                                                  const float scalar_part);

/**
 * @brief  X軸周り回転角からのQuaternion生成
 * @param  angle_x_rad : X軸周りの回転角 [rad]
 * @return Quaternion
 */
Quaternion QUATERNION_make_x_rot(const float angle_around_x_rad);

/**
 * @brief  Y軸周り回転角からのQuaternion生成
 * @param  angle_y_rad : Y軸周りの回転角 [rad]
 * @return Quaternion
 */
Quaternion QUATERNION_make_y_rot(const float angle_around_y_rad);

/**
 * @brief  Z軸周り回転角からのQuaternion生成
 * @param  angle_z_rad : Z軸周りの回転角 [rad]
 * @return Quaternion
 */
Quaternion QUATERNION_make_z_rot(const float angle_around_z_rad);

/**
 * @brief  単位Quaternion生成
 * @return 単位Quaternion
 */
Quaternion QUATERNION_make_unit(void);

/**
 * @brief  逆Quaternion生成
 * @param  q_in : 元のQuaternion
 * @return 反転後のQuaternion
 */
Quaternion QUATERNION_inverse(const Quaternion q_in);


// Quaternionを別表現へ変換する関数

/**
 * @brief        QuaternionをEuler軸，回転角に変換
 * @param[out]   axis      : 取り出したEuler軸の格納先
 * @param[out]   angle_rad : 取り出した回転角の格納先 [rad]
 * @param[in]    q_in      : 計算対象
 */
void QUATERNION_to_euler_axis_angle(float axis[PHYSICAL_CONST_THREE_DIM], float* angle_rad,
                                    const Quaternion q_in);

/**
 * @brief        Quaternionを座標変換行列へ変換
 * @param[out]   axis      : 変換後の行列格納先
 * @param[in]    q_in      : 計算対象
 * @note         座標変換行列であって，ベクトル回転行列ではない (両者は回転方向が逆になる)
 */
void QUATERNION_to_rotation_matrix(float matrix[][PHYSICAL_CONST_THREE_DIM],
                                   const Quaternion q_in);

/**
 * @brief        Quaternionを要素4配列へ変換
 * @param[out]   array_out : 変換結果格納先の配列
 * @param[in]    q_in      : 変換元Quaternion
 * @param[in]    position  : 配列中のスカラー部位置 (0か3のどちらかのみ想定)
 */
void QUATERNION_to_array(float array_out[PHYSICAL_CONST_QUATERNION_DIM],
                         const Quaternion q_in,
                         const QUATERNION_SCALAR_POSITION position);


// Quaternion演算関数

/**
 * @brief  Quaternionが規格化されているかの確認
 * @param  q_in : 規格化確認対称
 * @return C2A_MATH_ERRORに準拠
 */
C2A_MATH_ERROR QUATERNION_is_normalized(const Quaternion q_in);

/**
 * @brief  Quaternionの規格化
 * @param  q_in : 規格化対象
 * @return 規格化後のQuaternion
 * @note   ノルムが小さい等の理由で規格化に失敗した場合，unit_quaternionを返す
 */
Quaternion QUATERNION_normalize(const Quaternion q_in);

/**
 * @brief       規格時の規格化エラー判定を含めたQuaternionの規格化
 * @param[out]   *q_out : 規格化後のQuaternion格納先
 * @param[in]    q_in   : 規格化対象
 * @return       C2A_MATH_ERRORに則したエラーコード
 * @note         ノルムが小さい等の理由で規格化に失敗した場合，unit_quaternionを返す
 */
C2A_MATH_ERROR QUATERNION_normalize_with_error_check(Quaternion* q_out, const Quaternion q_in);

/**
 * @brief  Quaternionノルムの取り出し
 * @param  q_in : ノルム計算対象
 * @return q_inのノルム
 */
float QUATERNION_calc_norm(const Quaternion q_in);

/**
 * @brief    QuaternionのHamilton積，q_out = q1*q2の演算
 * @param    q_in1 : q1*q2のq1
 * @param    q_in2 : q1*q2のq2
 * @return   Quaternion q_out= q1*q2
 */
Quaternion QUATERNION_product(const Quaternion q_in1, const Quaternion q_in2);

/**
 * @brief   Quaternionの内積計算
 * @param   q_in1 : 内積をとるQuaternion
 * @param   q_in2 : 内積をとるQuaternion
 * @return  演算結果
 * @note    単位Quaternion同士であれば，結果の値域は[-1, 1]となる
 */
float QUATERNION_inner_product(const Quaternion q_in1, const Quaternion q_in2);

/**
 * @brief    キネマティクス方程式に基づく，Quaternionの時間微分q_dot = 1/2 * q_in * omegaの計算
 * @param    q_in             : 入力Quaternion []
 * @param    omega_in_rad_s   : 角速度ベクトル [rad/s]
 * @return   Quaternion q_dot = 1/2 * q_in * omega [ /s]
 * @note     q_dotはノルム1である必要がないので，正規化されない
 */
Quaternion QUATERNION_calc_time_derivative(const Quaternion q_in, const float omega_in_rad_s[PHYSICAL_CONST_THREE_DIM]);

/**
 * @brief       異なる時刻のQuaternionから角速度計算
 * @param[out]  omega_rad_s   : 角速度ベクトル [rad/s]
 * @param[in]   q_previous    : 前ステップのQuaternion
 * @param[in]   q_current     : 現ステップのQuaternion
 * @param[in]   time_diff_sec : Quaterionの時刻差 [sec]
 * @return      C2A_MATH_ERRORに準拠
 */
C2A_MATH_ERROR QUATERNION_calc_angular_velocity(float omega_rad_s[PHYSICAL_CONST_THREE_DIM],
                                                const Quaternion q_previous,
                                                const Quaternion q_current,
                                                const float time_diff_sec);

/**
 * @brief    オイラー法を使った伝搬によるクオータニオンの時間伝播
 * @param    q_previous       : 前ステップのクオータニオン []
 * @param    omega_in_rad_s   : 角速度ベクトル [rad/s]
 * @param    time_step_s      : 時間ステップ [s]
 * @return   q_current        : 現在のクオータニオン
 * TODO_L: ルンゲクッタ法によるアップデートも実装する
 */
Quaternion QUATERNION_euler_propagate(const Quaternion q_previous,
                                      const float omega_in_rad_s[PHYSICAL_CONST_THREE_DIM],
                                      const float time_step_s);

/**
 * @brief    Quaternion同士の輪を取る．q_out = q_in1 + q_in2
 * @param    q_in1 : 足されるQuaternion
 * @param    q_in2 : 足されるQuaternion
 * @return   q_out : 演算結果
 * @note     q_outは正規化されないことに注意する
 */
Quaternion QUATERNION_add(const Quaternion q_in1,
                          const Quaternion q_in2);

/**
 * @brief    Quaternionのスカラー倍を与える．q_out = k * q_in
 * @param    scalar : スカラーk
 * @param    q_in   : スカラー倍されるQuaternion
 * @return   q_out  : 演算結果
 * @note     q_outは正規化されないことに注意する
 */
Quaternion QUATERNION_scalar_product(const float scalar,
                                     const Quaternion q_in);

/**
 * @brief   Quaternionの除算，q2 = q1*q_outを満たすq_outを返す
 * @param   q_in1 : q2 = q1*q_outのq1
 * @param   q_in2 : q2 = q1*q_outのq2
 * @return  Quaternion q2 = q1*q_outのq_out
 */
Quaternion QUATERNION_divide(const Quaternion q_in1, const Quaternion q_in2);

/**
 * @brief   Quaternionの球面線形補間，q1とq2の内挿値q_outを返す
 * @param   q_in1 : 始点のQuaternion
 * @param   q_in2 : 終点のQuaternion
 * @param   factor : 補間パラメータ [0, 1]で指定する
 * @return  演算結果
 * @note    factorが0に近づくほどq1に近い値となり，1に近づくほどq2に近い値となる．
 * @note    slerpはsphercial linear interpolationの略
 */
Quaternion QUATERNION_interpolate_slerp(const Quaternion q_in1, const Quaternion q_in2, const float factor);

/**
 * @brief        A座標系からB座標系への変換qABを用いて，A座標系のベクトルをB座標系で見たベクトルに変換
 * @param[out]   vector_out : vector_inをB座標系で見たベクトルに変換した結果の格納先
 * @param[in]    q_in       : A座標系からB座標系への変換qAB
 * @param[in]    vector_in  : A座標系のベクトル
 * @note         回すのは座標であって，ベクトルではない (両者は回転方向が逆になる)
 */
void QUATERNION_trans_coordinate(float vector_out[PHYSICAL_CONST_THREE_DIM],
                                 const Quaternion q_in,
                                 const float vector_in[PHYSICAL_CONST_THREE_DIM]);

#endif
