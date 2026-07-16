#ifndef __ADDR_STORE_H__
#define __ADDR_STORE_H__

#include "main.h"

#define ADDR_FLASH_PAGE   ((uint32_t)0x08007400)
#define ADDR_VALID_MAGIC  ((uint32_t)0xA5A50000)

void AddrStore_Load(uint8_t *addr_high, uint8_t *addr_low);
void AddrStore_Save(uint8_t addr_high, uint8_t addr_low);

#endif
