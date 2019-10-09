/*
 * dma_isr.h
 *
 *  Created on: 01.10.2019
 *      Author: adudzinski
 */

#ifndef DMA_ISR_H_
#define DMA_ISR_H_

#include "stm32f0xx.h"


void *dma1(void);
void *dma2(void);
void *dma3(void);
void *dma4(void);
void *dma5(void);
void *dma6(void);
void *dma7(void);

void DMA1_Ch1_IRQHandler(void);
void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler(void);
void DMA1_Channel4_5_IRQHandler(void);
void DMA1_Ch4_7_DMA2_Ch3_5_IRQHandler(void);



#endif /* DMA_ISR_H_ */
