#ifndef __PAIRING_STORE_H__
#define __PAIRING_STORE_H__

#include "main.h"

/* 专用页，与 addr_store 的 0x08007400 物理隔离，512 字节对齐，
 * 避免整页擦除互相破坏 */
#define PAIR_FLASH_PAGE   ((uint32_t)0x08007200)
/* 已配对登记标志的 magic（区别于 addr_store 的 0xA5A50000） */
#define PAIR_DONE_MAGIC   ((uint32_t)0xA5A50003)

/* 上电初始化：读取 Flash 标志并缓存有效性。建议主循环遥测前调用一次。 */
void Pairing_Store_Init(void);

/* 返回当前 firstPairing 判定：
 *   Flash 未编程(读出 0xFFFFFFFF) → 返回 1 (true，首次配对)
 *   Flash 有有效标志(== PAIR_DONE_MAGIC) → 返回 0 (false，已登记) */
uint8_t Pairing_IsFirst(void);

/* 持久化"已完成配对登记"：将 PAIR_DONE_MAGIC 写入 Flash。
 * 幂等：若已为有效标志(== PAIR_DONE_MAGIC)则直接返回，不重复擦写。 */
void Pairing_MarkDone(void);

#endif
