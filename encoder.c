#include "encoder.h"

RESULT init_encoder( encoder_type *in, uint8_t *v , void (*a)(void),void (*b)(void), GPIO_TypeDef *p, uint8_t pin, GPIO_TypeDef *pa, uint8_t pina, GPIO_TypeDef *pb, uint8_t pinb){
	in->value=v;
	in->last = 0;
	in->input=0;
	in->button_counter=0;
	in->button_step=0;
	in->button_state=0;
	in->button_state_=0;
	in->PORT=p;
	in->PORTa=pa;
	in->PORTb=pb;
	in->pin = pin;
	in->pinA = pina;
	in->pinB = pinb;
	in->push=a;
	in->release=b;
	if (pa == GPIOA || pb == GPIOA )RCC->AHBENR|=RCC_AHBENR_GPIOAEN;
	if (pa == GPIOB || pb == GPIOB )RCC->AHBENR|=RCC_AHBENR_GPIOBEN;
	if (pa == GPIOC || pb == GPIOC )RCC->AHBENR|=RCC_AHBENR_GPIOCEN;
	pa->MODER&= ~(0x3<<(pina<<1));
	pb->MODER&= ~(0x3<<(pinb<<1));

	return DONE;

}

void encoder_update(encoder_type *in){

if( (in->PORTa->IDR) & (1<<in->pinA) )in->input|=1;
else in->input&=~0x1;

if( (in->PORTb->IDR) & (1<<in->pinB) )in->input|=2;
else in->input&=~0x2;
in->last&=3;
switch(in->last){
case 0:
	if(in->input==0x2)(*in->value)++;
	if(in->input==0x1)(*in->value)--;
	break;
case 1:
	if(in->input==0x0)(*in->value)++;
	if(in->input==0x3)(*in->value)--;
	break;
case 2:
	if(in->input==0x3)(*in->value)++;
	if(in->input==0x0)(*in->value)--;
	break;
case 3:
	if(in->input==0x1)(*in->value)++;
	if(in->input==0x2)(*in->value)--;
	break;

}
in->last=in->input;




if(!((in->PORT->IDR) & (1<<in->pin)))	in->button_state =1;
else in->button_state =0;
if(in->button_state && in->button_step ==0 )in->button_step =1;

switch(in->button_step){
case 0:
	break;
case 1:
	in->push();

	in->button_step =2;
	break;
case 2:

	if(!in->button_state_)in->button_counter++;
	if(in->button_counter>100){in->button_counter=0;in->button_state_=1;}
	if(in->button_state_ && in->button_state==0)in->button_step =3;

	break;
case 3:

	in->release();

	if(in->button_state_)in->button_counter++;
	if(in->button_counter>100){in->button_counter=0;in->button_state_=0;in->button_step =0;}
	break;

}


}

