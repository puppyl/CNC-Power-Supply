#ifndef __PWR_MANAGE_H
#define __PWR_MANAGE_H

#include "stm32g4xx.h"


uint8_t read_button1_GPIO();
uint8_t read_button2_GPIO();
uint8_t read_button3_GPIO();


void button1_callback(void *button);
void button2_callback(void *button);
void button3_callback(void *button);
void button4_callback(void *button);
void allbutton_init(void);
void show_set(void);
void set_parameter(void);

	



#endif
