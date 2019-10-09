#include "ili9341.h"
#include "ili9341_commands.h"
volatile uint16_t LCD_W=ILI9341_TFTWIDTH;
volatile uint16_t LCD_H=ILI9341_TFTHEIGHT;

void ili9341_hard_init(void)//init hardware
{
	RCC->AHBENR|=RCC_AHBENR_GPIOAEN|RCC_AHBENR_GPIOBEN;
	GPIOB->MODER|=  (
			GPIO_MODER_MODER3_1|	//PB3 - AF
			GPIO_MODER_MODER4_1|	//PB4 - AF
			GPIO_MODER_MODER5_1);	//PB5 - AF
	GPIOA->MODER|=  (
			GPIO_MODER_MODER3_0|	//PA3 - OUTPUT
			GPIO_MODER_MODER4_1);	//PA4 - AF
}

void ili9341_spi_init(void)//set spi speed and settings 
{

	RCC->APB2ENR = RCC_APB2ENR_SPI1EN;
	SPI1->CR1=SPI_CR1_MSTR;
	SPI1->CR2=  SPI_CR2_DS_0|SPI_CR2_DS_1|SPI_CR2_DS_2| SPI_CR2_SSOE ;//
	SPI1->CR1|=SPI_CR1_SPE;

}

void ili9341_spi_send(uint8_t spi_data)//send spi data to display
{
	while( !( SPI1->SR & SPI_SR_TXE ));
	*( volatile uint8_t * ) ( &(SPI1->DR) ) = spi_data;
	while( SPI1->SR & SPI_SR_BSY );

}
void ili9341_spi_send16(uint16_t spi_data)//send spi data to display
{
	while( !( SPI1->SR & SPI_SR_TXE ));
	*(&(SPI1->DR) ) = spi_data;
	while( SPI1->SR & SPI_SR_BSY );

}

void ili9341_writecommand8(uint8_t com)//command write
{
	GPIOA->BSRR = (1<<3)<<16;
	ili9341_spi_send(com);
}

void ili9341_writedata8(uint8_t data)//data write
{
	GPIOA->BSRR = (1<<3);
	ili9341_spi_send(data);

}
void ili9341_writedata16(uint16_t data)//data write
{
	GPIOA->BSRR = (1<<3);
	ili9341_spi_send16(swap(data));

}

uint16_t swap(uint16_t in)
{
	  uint16_t a;
	  a = in >> 8;
	  in <<= 8;
	  in|=a;
  return in;
}
void ili9341_setaddress(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)//set coordinate for print or other function
{
	ili9341_writecommand8(LCD_COLUMN_ADDR);
	ili9341_writedata16(x1);
	ili9341_writedata16(x2);

	ili9341_writecommand8(LCD_PAGE_ADDR);
	ili9341_writedata16(y1);
	ili9341_writedata16(y2);

	ili9341_writecommand8(LCD_GRAM);//meory write
}

void ili9341_hard_reset(void)//hard reset display
{

}






void ili9341_init(void)//set up display using predefined command sequence
{
	ili9341_hard_init();
	ili9341_spi_init();
	ili9341_hard_reset();
	ili9341_writecommand8(LCD_SWRESET);//soft reset


	//power control A
	ili9341_writecommand8(LCD_POWERA);
	ili9341_writedata8(0x39);
	ili9341_writedata8(0x2C);
	ili9341_writedata8(0x00);
	ili9341_writedata8(0x34);
	ili9341_writedata8(0x02);

	//power control B
	ili9341_writecommand8(LCD_POWERB);
	ili9341_writedata8(0x00);
	ili9341_writedata8(0xC1);
	ili9341_writedata8(0x30);

	//driver timing control A
	ili9341_writecommand8(LCD_DTCA);
	ili9341_writedata8(0x85);
	ili9341_writedata8(0x00);
	ili9341_writedata8(0x78);

	//driver timing control B
	ili9341_writecommand8(LCD_DTCB);
	ili9341_writedata8(0x00);
	ili9341_writedata8(0x00);

	//power on sequence control
	ili9341_writecommand8(LCD_POWER_SEQ);
	ili9341_writedata8(0x64);
	ili9341_writedata8(0x03);
	ili9341_writedata8(0x12);
	ili9341_writedata8(0x81);

	//pump ratio control
	ili9341_writecommand8(LCD_PRC);
	ili9341_writedata8(0x20);

	//power control,VRH[5:0]
	ili9341_writecommand8(LCD_POWER1);
	ili9341_writedata8(0x23);

	//Power control,SAP[2:0];BT[3:0]
	ili9341_writecommand8(LCD_POWER2);
	ili9341_writedata8(0x10);

	//vcm control
	ili9341_writecommand8(LCD_VCOM1);
	ili9341_writedata8(0x3E);
	ili9341_writedata8(0x28);

	//vcm control 2
	ili9341_writecommand8(LCD_VCOM2);
	ili9341_writedata8(0x86);

	//memory access control
	ili9341_writecommand8(LCD_MAC);
	ili9341_writedata8(0x48);

	//pixel format
	ili9341_writecommand8(LCD_PIXEL_FORMAT);
	ili9341_writedata8(0x55);

	//frameration control,normal mode full colours
	ili9341_writecommand8(LCD_FRMCTR1);
	ili9341_writedata8(0x00);
	ili9341_writedata8(0x18);

	//display function control
	ili9341_writecommand8(LCD_DFC);
	ili9341_writedata8(0x08);
	ili9341_writedata8(0x82);
	ili9341_writedata8(0x27);

	//3gamma function disable
	ili9341_writecommand8(LCD_3GAMMA_EN);
	ili9341_writedata8(0x00);

	//gamma curve selected
	ili9341_writecommand8(LCD_GAMMA);
	ili9341_writedata8(0x01);

	//set positive gamma correction
	ili9341_writecommand8(LCD_PGAMMA);
	ili9341_writedata8(0x0F);
	ili9341_writedata8(0x31);
	ili9341_writedata8(0x2B);
	ili9341_writedata8(0x0C);
	ili9341_writedata8(0x0E);
	ili9341_writedata8(0x08);
	ili9341_writedata8(0x4E);
	ili9341_writedata8(0xF1);
	ili9341_writedata8(0x37);
	ili9341_writedata8(0x07);
	ili9341_writedata8(0x10);
	ili9341_writedata8(0x03);
	ili9341_writedata8(0x0E);
	ili9341_writedata8(0x09);
	ili9341_writedata8(0x00);

	//set negative gamma correction
	ili9341_writecommand8(LCD_NGAMMA);
	ili9341_writedata8(0x00);
	ili9341_writedata8(0x0E);
	ili9341_writedata8(0x14);
	ili9341_writedata8(0x03);
	ili9341_writedata8(0x11);
	ili9341_writedata8(0x07);
	ili9341_writedata8(0x31);
	ili9341_writedata8(0xC1);
	ili9341_writedata8(0x48);
	ili9341_writedata8(0x08);
	ili9341_writedata8(0x0F);
	ili9341_writedata8(0x0C);
	ili9341_writedata8(0x31);
	ili9341_writedata8(0x36);
	ili9341_writedata8(0x0F);

	//exit sleep
	ili9341_writecommand8(LCD_SLEEP_OUT);

	//display on
	ili9341_writecommand8(LCD_DISPLAY_ON);
}

//set colour for drawing
void ili9341_pushcolour(uint16_t colour)
{
	ili9341_writedata16(colour);
}

//clear lcd and fill with colour
void ili9341_clear(uint16_t colour)
{
	uint16_t i,j;
	ili9341_setaddress(0,0,LCD_W-1,LCD_H-1);

	for(i=0;i<LCD_W;i++)
	{
		for(j=0;j<LCD_H;j++)
		{
			ili9341_pushcolour(colour);
		}
	}
}

//draw pixel
void ili9341_drawpixel(uint16_t x3,uint16_t y3,uint16_t colour1) //pixels will always be counted from right side.x is representing LCD width which will always be less tha 240.Y is representing LCD height which will always be less than 320
{
	if ((x3 < 0) ||(x3 >= LCD_W) || (y3 < 0) || (y3 >= LCD_H))
	{
		return;
	}

	ili9341_setaddress(x3,y3,x3+1,y3+1);
	ili9341_pushcolour(colour1);
}

//draw vertical line
void ili9341_drawvline(uint16_t x,uint16_t y,uint16_t h,uint16_t colour)//basically we will see this line horizental if we see the display 320*240
{
	if ((x >= LCD_W) || (y >= LCD_H))
	{
		return;
	}

	if((y+h-1) >= LCD_H)
	{
		h=LCD_H-y;
	}

	ili9341_setaddress(x,y,x,y+h-1);

	while (h--)
	{
		ili9341_pushcolour(colour);
	}
}


//draw horizental line

void ili9341_drawhline(uint16_t x,uint16_t y,uint16_t w,uint16_t colour)
{
	if((x >=LCD_W) || (y >=LCD_H))
	{
		return;
	}

	if((x+w-1) >= LCD_W)
	{
		w=LCD_W-x;
	}

	ili9341_setaddress(x,y,x+w-1,y);

	while(w--)
	{
		ili9341_pushcolour(colour);
	}
}


//draw colour filled rectangle
void ili9341_fillrect(uint16_t x,uint16_t y,uint16_t w,uint16_t h,uint16_t colour)
{
	if ((x >= LCD_W) || (y >= LCD_H))
	{
		return;
	}

	if ((x+w-1) >= LCD_W)
	{
		w = LCD_W-x;
	}

	if ((y+h-1) >= LCD_H)
	{
		h = LCD_H-y;
	}

	ili9341_setaddress(x, y, x+w-1, y+h-1);

	for(y=h; y>0; y--) 
	{
		for(x=w; x>0; x--)
		{
			ili9341_pushcolour(colour);
		}
	}
}


//rotate screen at desired orientation
void ili9341_setRotation(uint8_t m) 
{
	uint8_t rotation;

	ili9341_writecommand8(0x36);
	rotation=m%4;

	switch (rotation) 
	{
		case 0:
			ili9341_writedata8(0x40|0x08);
			LCD_W = 240;
			LCD_H = 320;
			break;
		case 1:
			ili9341_writedata8(0x20|0x08);
			LCD_W  = 320;
			LCD_H = 240;
			break;
		case 2:
			ili9341_writedata8(0x80|0x08);
			LCD_W  = 240;
			LCD_H = 320;
			break;
		case 3:
			ili9341_writedata8(0x40|0x80|0x20|0x08);
			LCD_W  = 320;
			LCD_H = 240;
			break;
	}
}
