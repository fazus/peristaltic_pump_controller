#ifndef _ILI9341GFX_H_
#define _ILI9341GFX_H_


#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <stdlib.h>

typedef struct
{
	uint16_t x,y,w,h; // x,y - position w,h - dimensions
	void (*draw)(struct graphic_element *in);
	struct graphic_element *next;
} graphic_element;


typedef struct
{
	graphic_element module;
	uint8_t redraw;
	uint8_t prev_status;
	uint8_t *status;
} checkbox;

typedef struct
{
	graphic_element module;
	uint8_t l;
	uint8_t s;
	char *buff;
} textbox;

typedef struct
{
	graphic_element module;
	uint8_t prev_digit;
	uint8_t *digit;
} digitbox;

typedef struct
{
	graphic_element module;
	uint8_t prev_number_of_digits;
	uint8_t s;
	uint8_t prev_count;
	int8_t *count;
	char *buff;
} countbox;

checkbox *create_checkbox(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint8_t *status);
void create_next_checkbox(graphic_element *prev,uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint8_t *status);

textbox *create_textbox(uint16_t x,uint16_t y,uint8_t l,uint8_t s,char *text);
void create_next_textbox(graphic_element *prev,uint16_t x,uint16_t y,uint16_t l,uint16_t s,char *text);

digitbox *create_digitbox(uint16_t x,uint16_t y,uint8_t s,uint8_t *digit);
void create_next_digitbox(graphic_element *prev,uint16_t x,uint16_t y,uint8_t s,uint8_t *digit);

countbox *create_countbox(uint16_t x,uint16_t y,uint8_t s,uint8_t *count);
void create_next_countbox(graphic_element *prev,uint16_t x,uint16_t y,uint8_t s,uint8_t *count);


void draw_textbox(textbox *in);
void draw_checkbox(checkbox *in);
void draw_digitbox(digitbox *in);
void draw_countbox(countbox *in);

graphic_element *return_graphic_element(graphic_element *first, uint8_t with);
graphic_element *return_next_graphic_element(graphic_element *first);
graphic_element *return_last_graphic_element(graphic_element *first);


void draw_graphic_element(graphic_element *in);
void draw_graphic_elements(graphic_element *first);

void ili9341_drawchar(int16_t x, int16_t y, unsigned char c,uint16_t color, uint16_t bg, uint8_t size);
void ili9341_setcursor(uint16_t x,uint16_t y);
void ili9341_settextcolour(uint16_t x,uint16_t y);
void ili9341_settextsize(uint8_t s);
void ili9341_write(uint8_t c);
void backuplocationvset(void);
void backuplocationvactual(void);
void backuplocationiset(void);
void backuplocationiactual(void);
void display_init(void);

#endif
