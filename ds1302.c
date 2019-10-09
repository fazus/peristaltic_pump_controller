#include "ds1302.h"



ds1302_type *clockownik;


void init_ds1302(ds1302_type *in,
		GPIO_TypeDef *port_sda, uint8_t pin_sda,
		GPIO_TypeDef *port_sck, uint8_t pin_sck,
		GPIO_TypeDef *port_rst, uint8_t pin_rst)

{

	uint32_t temp;
	clockownik = in;

	in->pin_SDA = pin_sda;
	in->pin_SCK = pin_sck;
	in->pin_RST = pin_rst;
	in->PORT_SDA = port_sda;
	in->PORT_SCK = port_sck;
	in->PORT_RST = port_rst;
	if (port_sda == GPIOA || port_sck == GPIOA || port_rst == GPIOA )RCC->AHBENR|=RCC_AHBENR_GPIOAEN;
	if (port_sda == GPIOB || port_sck == GPIOB || port_rst == GPIOB )RCC->AHBENR|=RCC_AHBENR_GPIOBEN;
	if (port_sda == GPIOC || port_sck == GPIOC || port_rst == GPIOC )RCC->AHBENR|=RCC_AHBENR_GPIOCEN;

	temp = (0x1<<(clockownik->pin_SDA<<1));
	in->PORT_SDA->MODER|= temp;
	temp = (0x1<<(clockownik->pin_SCK<<1));
	in->PORT_SCK->MODER|= temp;
	temp = (0x1<<(clockownik->pin_RST<<1));
	in->PORT_RST->MODER|= temp;


	temp = (0x3<<(clockownik->pin_SDA<<1));
	in->PORT_SDA->OSPEEDR|= temp;
	temp = (0x3<<(clockownik->pin_SCK<<1));
	in->PORT_SCK->OSPEEDR|= temp;
	temp = (0x3<<(clockownik->pin_RST<<1));
	in->PORT_RST->OSPEEDR|= temp;


	write_byte_ds1302(DS1302_CHARGER,0x00);
	RST_ds1302(0);
	SCK_ds1302(0);

}


void write_byte_ds1302(uint8_t addr, uint8_t in){
	uint8_t i;
	RST_ds1302(1);
	write_cmd_ds1302(addr);

	for (i = 0; i < 8; i ++)
		{
			if(in & 0x01)SDA_ds1302(1);
			else SDA_ds1302(0);
			SCK_ds1302(1);
			delay();
			SCK_ds1302(0);
			delay();
			in >>= 1;
		}
	RST_ds1302(0);
	SDA_ds1302(0);


}

void write_cmd_ds1302(uint8_t in){
		uint8_t i;
		for (i = 0; i < 8; i ++)
		{

			if(in & 0x01)SDA_ds1302(1);
			else SDA_ds1302(0);
			SCK_ds1302(1);
			delay();
			SCK_ds1302(0);
			delay();
			in >>= 1;
		}

}



uint8_t read_byte_ds1302(uint8_t addr){



	uint8_t i;
	uint8_t temp = 0;

	RST_ds1302(1);
	addr = addr | 0x01; // Generate Read Address

	write_cmd_ds1302(addr);	// Sends address

	SDA_is_input();
	for (i = 0; i < 8; i ++)
	{
		temp >>= 1;

		if(clockownik->PORT_SDA->IDR & (1<<clockownik->pin_SDA))temp |= 0x80;

		SCK_ds1302(1);
		delay();
		SCK_ds1302(0);
		delay();
	}
	SDA_is_output();

	RST_ds1302(0);
	SDA_ds1302(0);
	return temp;


}

void write_time_ds1302(uint8_t *buff)
{
	write_byte_ds1302(DS1302_CONTROL,0x00);
	delay();
	write_byte_ds1302(DS1302_SEC,0x80);
	write_byte_ds1302(DS1302_YEAR,HEX2BCD(buff[0]));
	write_byte_ds1302(DS1302_MONTH,HEX2BCD(buff[1]));
	write_byte_ds1302(DS1302_DATE,HEX2BCD(buff[2]));
	write_byte_ds1302(DS1302_HOUR,HEX2BCD(buff[3]));
	write_byte_ds1302(DS1302_MIN,HEX2BCD(buff[4]));
	write_byte_ds1302(DS1302_SEC,HEX2BCD(buff[5]));
	write_byte_ds1302(DS1302_DAY,HEX2BCD(buff[6]));
	write_byte_ds1302(DS1302_CONTROL,0x80);
	delay();
}


void read_time_ds1302(uint8_t *buff)
{
   	uint8_t tmp;

	tmp = read_byte_ds1302(DS1302_YEAR);
	buff[0] = BCD2HEX(tmp);
	tmp = read_byte_ds1302(DS1302_MONTH);
	buff[1] = BCD2HEX(tmp);
	tmp = read_byte_ds1302(DS1302_DATE);
	buff[2] = BCD2HEX(tmp);
	tmp = read_byte_ds1302(DS1302_HOUR);
	buff[3] = BCD2HEX(tmp);
	tmp = read_byte_ds1302(DS1302_MIN);
	buff[4] = BCD2HEX(tmp);
	tmp = read_byte_ds1302((DS1302_SEC))&0x7F;
	buff[5] = BCD2HEX(tmp);
	tmp = read_byte_ds1302(DS1302_DAY);
	buff[6] = BCD2HEX(tmp);
}


void write_ram_ds1302(uint8_t addr, uint8_t val) {
	write_byte_ds1302(DS1302_CONTROL,0x00);			// Disable write protection
	delay();
	if (addr >= RAMSIZE) {
		return;
	}

	write_byte_ds1302(DS1302_RAMSTART + (2 * addr), val);

	write_byte_ds1302(DS1302_CONTROL,0x80);			// Enable write protection
	delay();
}

/* Reads ram address 'addr' */
uint8_t read_ram_ds1302(uint8_t addr) {
	if (addr >= RAMSIZE) {
		return 0;
	}

	return read_byte_ds1302(DS1302_RAMSTART + (2 * addr));
}



/* Clears the entire ram writing 0 */
void clear_ram_ds1302(void) {
	uint8_t i;
	for(i=0; i< RAMSIZE; i++){
		write_ram_ds1302(i,0x00);
	}
}



void SDA_ds1302(uint8_t in){
	if(in) clockownik->PORT_SDA->BSRR = (1<<clockownik->pin_SDA);
	else clockownik->PORT_SDA->BSRR = (1<<clockownik->pin_SDA)<<16;
}
void SCK_ds1302(uint8_t in){
	if(in) clockownik->PORT_SCK->BSRR = (1<<clockownik->pin_SCK);
	else clockownik->PORT_SCK->BSRR = (1<<clockownik->pin_SCK)<<16;
}
void RST_ds1302(uint8_t in){
	if(in) clockownik->PORT_RST->BSRR = (1<<clockownik->pin_RST);
	else clockownik->PORT_RST->BSRR = (1<<clockownik->pin_RST)<<16;
}

void SDA_is_output(void){
	uint32_t temp = (0x1<<(clockownik->pin_SDA<<1));
	clockownik->PORT_SDA->MODER|= temp;

}
void SDA_is_input(void){
	uint32_t temp = ~(0x3<<(clockownik->pin_SDA<<1));
	clockownik->PORT_SDA->MODER&= temp;

}

void __attribute__((optimize("O0")))delay(void){
	int i;
	for(i=0;i<=2000;i++);

}
