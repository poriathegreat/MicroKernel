
#ifndef _UNIQUE_ID_H
#define _UNIQUE_ID_H

#include <main.h>
#include <stdint.h>

#define UUID_SIZE_BYTE          (12)

#define STM32F0_UUID_ADDRESS    ((uint8_t *)0x1FFFF7AC)
#define STM32F1_UUID_ADDRESS    ((uint8_t *)0x1FFFF7E8)
#define STM32F2_UUID_ADDRESS    ((uint8_t *)0x1FFF7A10)
#define STM32F3_UUID_ADDRESS    ((uint8_t *)0x1FFFF7AC)
#define STM32F4_UUID_ADDRESS    ((uint8_t *)0x1FFF7A10)
#define STM32F7_UUID_ADDRESS    ((uint8_t *)0x1FF0F420)


#ifdef STM32F0
#define UUID_BASE_ADDRESS STM32F0_UUID_ADDRESS
#endif

#ifdef STM32F1
#define UUID_BASE_ADDRESS STM32F1_UUID_ADDRESS
#endif

#ifdef STM32F2
#define UUID_BASE_ADDRESS STM32F2_UUID_ADDRESS
#endif

#ifdef STM32F3
#define UUID_BASE_ADDRESS STM32F3_UUID_ADDRESS
#endif

#ifdef STM32F4
#define UUID_BASE_ADDRESS STM32F4_UUID_ADDRESS
#endif

#ifdef STM32F7
#define UUID_BASE_ADDRESS STM32F7_UUID_ADDRESS
#endif


int32_t read_mcu_unique_id( uint8_t * uuid );

#endif //_UNIQUE_ID_H
