/**
* @file   z_filter.h
* @brief  離散線形フィルタ関連の関数群の定義
*/

#ifndef Z_FILTER_H_
#define Z_FILTER_H_

#include <stdint.h>
#include <src_user/Library/c2a_math.h>


#define Z_FILTER_ORDER_MAX 2  //!< 離散フィルタ最高次数

/**
 * @struct ZFilter
 * @brief  離散線形フィルタ構造体
 * @note   センサノイズ除去，アクチュエータ出力からの構造励振成分除去等に使う線形フィルタ処理をまとめるための構造体
 * @note   用途に合わせた初期化関数 (Signal_zfilter_init_as_xx) で初期化し，Signal_zfilter_outでフィルタ出力を取得する
 * @note   フィルタ初期化時は連続系, かつ, 周波数領域 (Hz) でパラメータ指定する (内部で換算)
 */
typedef struct
{
  double input_previous[Z_FILTER_ORDER_MAX];   //!< 前回入力値
  double output_previous[Z_FILTER_ORDER_MAX];  //!< 前回出力値

  double numerator_discrete_coeff[Z_FILTER_ORDER_MAX + 1];     //!< 離散系分子多項式係数(z^-n)
  double denominator_discrete_coeff[Z_FILTER_ORDER_MAX];       //!< 離散系分母多項式係数(z^-n), 最高次係数は1で規格化

  uint8_t order; //!< フィルタ次数
} ZFilter;


/**
 * @brief  1次LPFとしZFilterを初期化
 * @param[in,out]  *filter  : 初期化対象
 * @param[in]   f_sample_Hz : サンプリング周波数 [Hz]
 * @param[in]   f_cut_Hz    : カットオフ周波数 [Hz]
 * @return C2A_MATH_ERROR
 * @note   f_sample_HzはZ_FILTER_calc_outputのcall頻度に合わせてほしい
 * @note   f_cut_Hzはf_sample_Hzに対してナイキストルールを守って使ってほしい (f_cut_Hz < 2 * f_sample_Hz)
 * @note   明らかに設定周波数が不正な場合には，設定値に依らず，フィルタはフィードスルーになる (output = input)
 * @note   内部でプリワーピングはしていないため，f_cut_Hz > f_sample_Hz / 5 程度を目安に
 *         連続系で見た所望のf_cut_Hzに対し，プリワーピングした後の値をf_cut_Hzに設定してほしい
 */
C2A_MATH_ERROR Z_FILTER_init_as_lpf_1st(ZFilter* filter, const float f_sample_Hz, const float f_cut_Hz);


/**
 * @brief  2次LPFとしてZFilterを初期化
 * @param[in,out]  *filter  : 初期化対象
 * @param[in]   f_sample_Hz : サンプリング周波数 [Hz]
 * @param[in]   f_cut_Hz    : カットオフ周波数 [Hz]
 * @param[in]   xi          : 減衰率 [-]
 * @return C2A_MATH_ERROR
 * @note   使い方はZ_FILTER_init_as_lpf_1stと同様
 */
C2A_MATH_ERROR Z_FILTER_init_as_lpf_2nd(ZFilter* filter, const float f_sample_Hz, const float f_cut_Hz, const float xi);


/**
 * @brief  (円) NotchフィルタとしてZFilterを初期化
 * @param[in,out]  *filter  : 初期化対象
 * @param[in]   f_sample_Hz : サンプリング周波数 [Hz]
 * @param[in]   f_cut_Hz    : 中心周波数 [Hz]
 * @param[in]   xi          : 減衰率 [-]
 * @param[in]   depth       : ノッチ深さ [-]
 * @return C2A_MATH_ERROR
 * @note   使い方はZ_FILTER_init_as_lpf_1stと同様
 * @note   楕円ノッチには対応していない
 */
C2A_MATH_ERROR Z_FILTER_init_as_notch(ZFilter* filter, const float f_sample_Hz, const float f_cut_Hz, const float xi, const float depth);


/**
 * @brief  ZFilterの内部状態をゼロクリアする
 * @param[in,out]  *filter  : リセット対象
 */
void Z_FILTER_reset(ZFilter* filter);


/**
 * @brief  ZFilterの出力を得る
 * @param[in,out]  *filter  : 使用するフィルタ
 * @param[in]   input       : フィルタ入力値
 * @return フィルタ出力
 */
float Z_FILTER_calc_output(ZFilter* filter, const float input);


/**
 * @brief  入出力型がdouble型の変数を扱うZFilterの出力を得る
 * @param[in,out]  *filter  : 使用するフィルタ
 * @param[in]   input       : フィルタ入力値
 * @return フィルタ出力
 */
double Z_FILTER_calc_output_double(ZFilter* filter, const double input);


#endif
