/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "main.h"


void przycisniecie(void);
void puszczenie(void);

void TIM3_IRQHandler(void);

int kutas = BLACK;

uint8_t data[7];
uint8_t data1[7];
int ii;

uint8_t digit1=3;



uint8_t status=1;
uint8_t status1=1;
graphic_element *one, *two, *three, *c;
char gg[] ="ABCD";
char g1[] ="AAAA";
char g2[] ="ARTUR";

RESULT enc_init_result = UNDONE;


int main(void)
{
	init_stm();

	enc_init_result = init_encoder(&enc,&encoder_value,przycisniecie,puszczenie,BUTTON_PORT,BUTTON_PIN,ENC_A_PORT,ENC_A_PIN,ENC_B_PORT,ENC_B_PIN);

	init_ds1302(&zegar,GPIOA,0,GPIOA,1,GPIOA,7);

	ili9341_init();
	ili9341_setRotation(2);
	ili9341_clear(BLACK);


	write_time_ds1302(&data1[0]);



	two = (graphic_element*)create_checkbox(60,20,20,20,&status1);
	create_next_checkbox(two,100,20,20,20,&status1);
	//create_next_textbox(return_last_graphic_element(two),10,170,4,7,g1);
	create_next_checkbox(return_last_graphic_element(two),150,20,20,20,&status1);
	create_next_checkbox(return_last_graphic_element(two),180,20,20,20,&status1);
	create_next_textbox(return_last_graphic_element(two),0,240,5,4,g2);
	create_next_digitbox(return_last_graphic_element(two),10,40,4,&digit1);
	create_next_countbox(return_last_graphic_element(two),10,80,4,&encoder_value);


	while(1){



		ili9341_settextcolour(WHITE,BLACK);

		draw_graphic_elements(two);

//		read_time_ds1302(&data[0]);

			//printf("%d:%d:%d\n",data[3],data[4],data[5]);


		}
}


void TIM3_IRQHandler(void){
	TIM3->SR&=~0x1;

	if( enc_init_result == DONE )encoder_update(&enc);

}


void przycisniecie(void){
	write_ram_ds1302(0,encoder_value);

status1=1;
kutas=GREEN;

}

void puszczenie(void){
kutas=BLACK;

status1=0;
}

int __io_putchar(char c)
{

	ili9341_write(c);
    return c;
}

