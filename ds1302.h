#ifndef _DS1302_H_
#define _DS1302_H_

#include "stm32f0xx.h"
#include <inttypes.h>






#define DS1302_SEC			0x80
#define DS1302_MIN			0x82
#define DS1302_HOUR			0x84
#define DS1302_DATE			0x86
#define DS1302_MONTH		0x88
#define DS1302_DAY			0x8a
#define DS1302_YEAR			0x8c
#define DS1302_CONTROL		0x8e
#define DS1302_CHARGER		0x90
#define DS1302_CLKBURST		0xbe
#define DS1302_RAMBURST 	0xfe
#define RAMSIZE 			0x31
#define DS1302_RAMSTART		0xc0

#define HEX2BCD(v)	((v) % 10 + (v) / 10 * 16)
#define BCD2HEX(v)	((v) % 16 + (v) / 16 * 10)

typedef struct
{
	int8_t second;
	int8_t minute;
	int8_t hour;
	int8_t day;
	int8_t month;
	int8_t year;
	int8_t pin_SDA;
	int8_t pin_SCK;
	int8_t pin_RST;
	GPIO_TypeDef *PORT_SDA;
	GPIO_TypeDef *PORT_SCK;
	GPIO_TypeDef *PORT_RST;
} ds1302_type;

void init_ds1302(ds1302_type *in,
		GPIO_TypeDef *port_sda, uint8_t pin_sda,
		GPIO_TypeDef *port_sck, uint8_t pin_sck,
		GPIO_TypeDef *port_rst, uint8_t pin_rst);


void write_byte_ds1302(uint8_t addr, uint8_t in);
void write_cmd_ds1302(uint8_t in);
uint8_t read_byte_ds1302(uint8_t addr);
void read_time_ds1302(uint8_t *buff);
void write_time_ds1302(uint8_t *buff);
void write_ram_ds1302(uint8_t addr, uint8_t val);
uint8_t read_ram_ds1302(uint8_t addr);
void clear_ram_ds1302(void);


void SDA_ds1302(uint8_t in);
void SCK_ds1302(uint8_t in);
void RST_ds1302(uint8_t in);

void SDA_is_output(void);
void SDA_is_input(void);

void __attribute__((optimize("O0")))delay(void);

#endif
