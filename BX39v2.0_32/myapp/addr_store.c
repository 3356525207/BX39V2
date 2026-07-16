#include "addr_store.h"
#include "n32g003_flash.h"

/**
 * 从 Flash 末尾页读取已配对的遥控器地址
 * 未编程时 Flash 为 0xFFFFFFFF, 返回 {0, 0}
 */
void AddrStore_Load(uint8_t *addr_high, uint8_t *addr_low)
{
    uint32_t word = *(__IO uint32_t *)ADDR_FLASH_PAGE;

    if ((word & 0xFFFF0000) == ADDR_VALID_MAGIC)
    {
        *addr_high = (uint8_t)((word >> 8) & 0xFF);
        *addr_low  = (uint8_t)(word & 0xFF);
    }
    else
    {
        *addr_high = 0;
        *addr_low  = 0;
    }
}

/**
 * 保存已配对的遥控器地址到 Flash 末尾页
 * 格式: 0xA5A5HHLL (HH=addr_high, LL=addr_low)
 */
void AddrStore_Save(uint8_t addr_high, uint8_t addr_low)
{
    uint8_t cur_high, cur_low;

    /* 检查是否需要更新 (避免不必要的擦除) */
    AddrStore_Load(&cur_high, &cur_low);
    if (cur_high == addr_high && cur_low == addr_low)
        return;

    FLASH_Unlock();

    /* 擦除页 (512 字节) */
    if (FLASH_EOP == FLASH_One_Page_Erase(ADDR_FLASH_PAGE))
    {
        uint32_t data = ADDR_VALID_MAGIC
                      | ((uint32_t)addr_high << 8)
                      | ((uint32_t)addr_low);

        FLASH_Word_Program(ADDR_FLASH_PAGE, data);
    }

    FLASH_Lock();
}
