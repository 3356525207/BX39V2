#include "pairing_store.h"
#include "n32g003_flash.h"

/**
 * 上电初始化: 读取 Flash 标志并缓存有效性
 * 判定仍以每次实时读 Flash 为准, 此处缓存仅作上电一次性核对
 */
void Pairing_Store_Init(void)
{
    /* 读取专用页首字, 与 magic 比较缓存有效性 */
    (void)(*(__IO uint32_t *)PAIR_FLASH_PAGE == PAIR_DONE_MAGIC);
}

/**
 * 返回当前 firstPairing 判定
 * 每次实时读 Flash, 确保遥测反映最新值:
 *   Flash 有有效标志(== PAIR_DONE_MAGIC) → 0 (false, 已登记)
 *   Flash 未编程(读出 0xFFFFFFFF)        → 1 (true,  首次配对)
 */
uint8_t Pairing_IsFirst(void)
{
    return (*(__IO uint32_t *)PAIR_FLASH_PAGE == PAIR_DONE_MAGIC) ? 0 : 1;
}

/**
 * 持久化"已完成配对登记": 将 PAIR_DONE_MAGIC 写入 Flash
 *
 * 幂等判定置于屏蔽中断之前: 先读 Flash 比对 magic, 若已有效则立即返回,
 * 不重复擦写, 以缩短临界区(绝大多数情况下根本不进入临界区)。
 *
 * 擦写临界区用保存/恢复 PRIMASK 屏蔽中断。理由: 芯片 Flash 驱动擦/写
 * 不屏蔽中断、无 RWW(读写不能同时)保障; 屏蔽中断可彻底杜绝
 *   (a) 与 TIM1 中断中 AddrStore_Save 的嵌套 Flash 操作;
 *   (b) 擦写期间 ISR 仍从同一片 Flash 取指造成的取指冲突。
 * 可接受副作用: 擦写期间(约数毫秒)TIM1 1ms tick 被挂起, g_ms_tick 少计
 * 若干拍轻微滞后, 属业界标准代价。
 *
 * 擦/写失败(非 FLASH_EOP)时不写入, 标志保持无效, 不产生半写脏数据。
 * (__get_PRIMASK/__disable_irq/__set_PRIMASK 经 main.h/n32g003.h 间接包含)
 */
void Pairing_MarkDone(void)
{
    uint32_t primask;

    /* 幂等判定: 已为有效标志则不擦写, 直接返回(临界区之外, 缩短临界区) */
    if (*(__IO uint32_t *)PAIR_FLASH_PAGE == PAIR_DONE_MAGIC)
        return;

    /* 进入擦写临界区: 保存并屏蔽中断 */
    primask = __get_PRIMASK();
    __disable_irq();

    FLASH_Unlock();

    /* 擦除专用页(512 字节), 仅当擦除成功(FLASH_EOP)才编程 */
    if (FLASH_EOP == FLASH_One_Page_Erase(PAIR_FLASH_PAGE))
    {
        FLASH_Word_Program(PAIR_FLASH_PAGE, PAIR_DONE_MAGIC);
    }

    FLASH_Lock();

    /* 退出临界区: 恢复原中断使能状态 */
    __set_PRIMASK(primask);
}
