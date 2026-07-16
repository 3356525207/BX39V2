#ifndef __SN_BUILDER_H__
#define __SN_BUILDER_H__

#include "main.h"

/* model("TW-04")=5 + '-'=1 + 24 hex + '\0'=1 = 31 字节，满足需求 1.4 (≤31)
 * 预留对齐余量，缓冲区取 32，实际占用 31 字节 */
#define SN_BUF_SIZE   32

/* 上电初始化：读取 UID@UID_BASE 的 12 字节，生成并缓存 SN 字符串。
 * 必须在首次 BT_SendTelemetry 之前调用一次。 */
void SN_Builder_Init(void);

/* 返回缓存的 SN 字符串（以 '\0' 结尾，长度 ≤ 30 字符）。
 * 在 SN_Builder_Init 之前调用返回空字符串 ""。 */
const char *SN_Builder_Get(void);

#endif
