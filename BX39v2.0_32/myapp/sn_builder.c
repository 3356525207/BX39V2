#include "sn_builder.h"

/* 模型前缀：与 bt_transparent.c 的 TELEM_MODEL("TW-04") 同值。
 * 此处内部重定义同值常量，避免跨文件耦合；model 字段仍由
 * bt_transparent.c 的 TELEM_MODEL 输出，二者取值一致。 */
#define SN_MODEL_PREFIX   "TW-04"

/* 大写十六进制字符表 */
static const char k_hex_chars[] = "0123456789ABCDEF";

/* SN 字符串缓存：布局 [ "TW-04" ][ '-' ][ 24 hex ][ '\0' ] = 31 字节。
 * Init 之前为空字符串 ""，保证 Get 在 Init 前返回合法非空指针。 */
static char g_sn[SN_BUF_SIZE] = {0};

/**
 * 上电初始化：读取 UID@UID_BASE 的 12 字节, 生成并缓存 SN 字符串。
 *
 * - 以 UID_BASE(0x1FFFF4FC) 为起点, 按字节地址低->高逐字节读取
 *   UID_LENGTH(12) 字节, 避免端序歧义 (需求 1.1、1.2)。
 * - 每字节先输出高半字节再输出低半字节, 映射到大写 hex, 共 24 字符 (需求 1.2)。
 * - 拼装 SN_MODEL_PREFIX + '-' + 24 hex + '\0' 写入 g_sn (需求 1.3、1.4)。
 * - 兜底: UID 全 0xFF / 全 0x00 不做特殊跳变, hex 转换天然产出确定性合法
 *   非空字符串 (TW-04-FFFF.../TW-04-0000...)。此为已知可接受输出, 不引入
 *   随机/时间因子以保持可重现性 (需求 2.2、2.3)。
 */
void SN_Builder_Init(void)
{
    const __IO uint8_t *uid = (const __IO uint8_t *)UID_BASE;
    char *p = g_sn;
    uint32_t i;

    /* 写入模型前缀 "TW-04" + '-' */
    {
        const char *prefix = SN_MODEL_PREFIX "-";
        while (*prefix)
        {
            *p++ = *prefix++;
        }
    }

    /* 逐字节读取 UID, 每字节高半字节在前、低半字节在后 */
    for (i = 0; i < UID_LENGTH; i++)
    {
        uint8_t b = uid[i];
        *p++ = k_hex_chars[(b >> 4) & 0x0F];
        *p++ = k_hex_chars[b & 0x0F];
    }

    *p = '\0';
}

/**
 * 返回缓存的 SN 字符串 (以 '\0' 结尾)。
 * 纯读、可重入安全 (无写操作)。Init 之前返回空字符串 ""。
 */
const char *SN_Builder_Get(void)
{
    return g_sn;
}
