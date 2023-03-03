#pragma section REPRO
/**
 * @file
 * @brief git revisionをコードに埋め込む
 *
 * @author 牛 佳成
 * @date 2021.01.11
 */

#include "git_revision.h"

const char GIT_REV_CORE[41] = "0000000000000000000000000000000000000000";
const uint32_t GIT_REV_CORE_SHORT = 0x0000000;
const char GIT_REV_USER[41] = "0000000000000000000000000000000000000000";
const uint32_t GIT_REV_USER_SHORT = 0x0000000;

#pragma section
