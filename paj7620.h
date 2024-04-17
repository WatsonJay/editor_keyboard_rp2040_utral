#pragma once

#include "quantum.h"
#include <stdint.h>
#include <stdbool.h>
#include "i2c_master.h"
#include "print.h"
#include "wait.h"
#include "debug.h"

#define BIT(x)  1 << x

#define PAJ7620_I2C_TIMEOUT 100

// REGISTER DESCRIPTION
#define PAJ7620_VAL(val, maskbit)		( val << maskbit )
#define PAJ7620_ADDR_BASE				0x00

// REGISTER BANK SELECT
#define PAJ7620_REGITER_BANK_SEL		(PAJ7620_ADDR_BASE + 0xEF)	//W

// DEVICE ID
#define PAJ7620_ID  ( 0x73 << 1 )

// REGISTER BANK 0
#define PAJ7620_ADDR_SUSPEND_CMD		(PAJ7620_ADDR_BASE + 0x3)	//W
#define PAJ7620_ADDR_GES_PS_DET_MASK_0		(PAJ7620_ADDR_BASE + 0x41)	//RW
#define PAJ7620_ADDR_GES_PS_DET_MASK_1		(PAJ7620_ADDR_BASE + 0x42)	//RW
#define PAJ7620_ADDR_GES_PS_DET_FLAG_0		(PAJ7620_ADDR_BASE + 0x43)	//R
#define PAJ7620_ADDR_GES_PS_DET_FLAG_1		(PAJ7620_ADDR_BASE + 0x44)	//R
#define PAJ7620_ADDR_STATE_INDICATOR	(PAJ7620_ADDR_BASE + 0x45)	//R
#define PAJ7620_ADDR_PS_HIGH_THRESHOLD	(PAJ7620_ADDR_BASE + 0x69)	//RW
#define PAJ7620_ADDR_PS_LOW_THRESHOLD	(PAJ7620_ADDR_BASE + 0x6A)	//RW
#define PAJ7620_ADDR_PS_APPROACH_STATE	(PAJ7620_ADDR_BASE + 0x6B)	//R
#define PAJ7620_ADDR_PS_RAW_DATA		(PAJ7620_ADDR_BASE + 0x6C)	//R

// REGISTER BANK 1
#define PAJ7620_ADDR_PS_GAIN			(PAJ7620_ADDR_BASE + 0x44)	//RW
#define PAJ7620_ADDR_IDLE_S1_STEP_0		(PAJ7620_ADDR_BASE + 0x67)	//RW
#define PAJ7620_ADDR_IDLE_S1_STEP_1		(PAJ7620_ADDR_BASE + 0x68)	//RW
#define PAJ7620_ADDR_IDLE_S2_STEP_0		(PAJ7620_ADDR_BASE + 0x69)	//RW
#define PAJ7620_ADDR_IDLE_S2_STEP_1		(PAJ7620_ADDR_BASE + 0x6A)	//RW
#define PAJ7620_ADDR_OP_TO_S1_STEP_0	(PAJ7620_ADDR_BASE + 0x6B)	//RW
#define PAJ7620_ADDR_OP_TO_S1_STEP_1	(PAJ7620_ADDR_BASE + 0x6C)	//RW
#define PAJ7620_ADDR_OP_TO_S2_STEP_0	(PAJ7620_ADDR_BASE + 0x6D)	//RW
#define PAJ7620_ADDR_OP_TO_S2_STEP_1	(PAJ7620_ADDR_BASE + 0x6E)	//RW
#define PAJ7620_ADDR_OPERATION_ENABLE	(PAJ7620_ADDR_BASE + 0x72)	//RW

// PAJ7620_REGITER_BANK_SEL
#define PAJ7620_BANK0		PAJ7620_VAL(0,0)
#define PAJ7620_BANK1	PAJ7620_VAL(1,0)

// PAJ7620_ADDR_SUSPEND_CMD
#define PAJ7620_I2C_WAKEUP	PAJ7620_VAL(1,0)
#define PAJ7620_I2C_SUSPEND	PAJ7620_VAL(0,0)

// PAJ7620_ADDR_OPERATION_ENABLE
#define PAJ7620_ENABLE		PAJ7620_VAL(1,0)
#define PAJ7620_DISABLE		PAJ7620_VAL(0,0)

typedef enum {
	BANK0 = 0,
	BANK1,
} bank_e;

#define GES_RIGHT_FLAG				PAJ7620_VAL(1,0)
#define GES_LEFT_FLAG				PAJ7620_VAL(1,1)
#define GES_UP_FLAG					PAJ7620_VAL(1,2)
#define GES_DOWN_FLAG				PAJ7620_VAL(1,3)
#define GES_FORWARD_FLAG			PAJ7620_VAL(1,4)
#define GES_BACKWARD_FLAG			PAJ7620_VAL(1,5)
#define GES_CLOCKWISE_FLAG			PAJ7620_VAL(1,6)
#define GES_COUNT_CLOCKWISE_FLAG	PAJ7620_VAL(1,7)
#define GES_WAVE_FLAG				PAJ7620_VAL(1,0)

#define INIT_REG_ARRAY_SIZE (sizeof(initRegisterArray)/sizeof(initRegisterArray[0]))

/**
 * 初始化paj7620
 */
uint8_t paj7620_init(void);

/**
 * Configure input/output to a given port
 */
bool paj7620_write_reg(uint8_t addr, uint8_t cmd);

/**
 * Configure input/output to a given port
 */
bool paj7620_read_reg(uint8_t addr, uint8_t qty, uint8_t data[]);

/**
 * Configure input/output to a given port
 */
void paj7620_select_bank(bank_e bank);

/**
 * 获取paj7620手势
 */
uint16_t paj7620_gesture(void);

/**
 * paj7620休眠
 */
void paj7620_suspend(void);

/**
 * paj7620唤醒
 */
void paj7620_wake_up(void);