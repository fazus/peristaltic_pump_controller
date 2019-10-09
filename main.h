#ifndef _MAIN_H_
#define _MAIN_H_

#include "stm32f0xx.h"
#include "ili9341.h"
#include "ili9341gfx.h"
#include "encoder.h"
#include "ds1302.h"
#include "dma_assign.h"
#include "init_stm.h"


encoder_type enc;
#define ENC_A_PORT GPIOB
#define ENC_A_PIN 1
#define ENC_B_PORT GPIOB
#define ENC_B_PIN 0
#define BUTTON_PORT GPIOA
#define BUTTON_PIN 12
int8_t encoder_value=0;


ds1302_type zegar;




#endif
