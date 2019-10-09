#ifndef _ENCODER_H_
#define _ENCODER_H_
#include "stm32f0xx.h"
#include <inttypes.h>

typedef enum {UNDONE=0,DONE=1}RESULT;


typedef struct
{
	void (*push)(void);
	void (*release)(void);
	uint8_t *value;				//value pulses counted on encoder
	int8_t last;				//last data from encoder output
	int8_t pin;					//number of port connected to button
	int8_t pinA;				//number of port connected to encoder terminal a
	int8_t pinB;				//number of port connected to encoder terminal b
	uint8_t input;				//most recent data from encoder output
	uint8_t button_counter;		//counter for button delay
	uint8_t button_step;		//actual state of button routine
	uint8_t button_state;		//actual state of button routine
	uint8_t button_state_;	//last state of button routine
	GPIO_TypeDef *PORT; 		//button port
	GPIO_TypeDef *PORTa;		//encoder port a
	GPIO_TypeDef *PORTb;		//encoder port b
} encoder_type;





RESULT init_encoder( encoder_type *in, uint8_t *v ,
		void (*a)(void),void (*b)(void),
		GPIO_TypeDef *p,  uint8_t pin,
		GPIO_TypeDef *pa, uint8_t pina,
		GPIO_TypeDef *pb, uint8_t pinb
		);

void encoder_update(encoder_type *in);
int8_t ret_encoder_value(encoder_type *in);
#endif

