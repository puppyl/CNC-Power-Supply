#include "stm32g4xx.h"
#include "dac.h"
#include "adc.h"
#include "lcd.h"
#include "multi_button.h"
#include "pwr_manage.h"
#include "PID.h"


int i = 0;
uint8_t state = 0;
unsigned char set[8] = {0,9,0,2,1,3,1,4};
unsigned char set1[8] = {0};
unsigned int pos[8] = {63,71,87,95,167,175,191,199};
extern float actvol;

struct Button button2;
struct Button button4;



uint8_t read_button2_GPIO() 
{
 return HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6);
}

uint8_t read_button4_GPIO() 
{
 return HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1);
}


void allbutton_init()
{
	button_init(&button2, read_button2_GPIO,0,1);
	button_attach(&button2,SINGLE_CLICK,button2_callback);
	button_attach(&button2,LONG_PRESS_START,button2_callback);
	button_start(&button2);
	
	button_init(&button4,read_button4_GPIO,0,3);
	button_attach(&button4,SINGLE_CLICK,button4_callback);
	button_start(&button4);
}


void button2_callback(void *button)
{
    uint32_t btn_event_val; 
    btn_event_val = get_button_event((struct Button *)button);
    switch(btn_event_val)
    {
			case SINGLE_CLICK: 
				if(state == 1)
				{				
					if(i < 7)
					{
						LCD_ShowChar(pos[i],207,set[i]+48,0,GREEN);
						i++;
						LCD_ShowChar(pos[i],207,set[i]+48,0,MAGENTA);
					}
					else 
					{
						LCD_ShowChar(199,207,set[7]+48,0,GREEN);
						i =0;
						LCD_ShowChar(63,207,set[0]+48,0,MAGENTA);
					}
				}
			break;

      case LONG_PRESS_START:
				state = !state;
				i = 0;
				if(state == 1)
				{
					LCD_ShowChar(63,207,set[0]+48,0,MAGENTA);
				}
				if(state == 0)
				{
					unsigned int z = 0;
					for(z = 0;z < 8;z++)
					{
						LCD_ShowChar(pos[z],207,set[z]+48,0,GREEN);
						int temp = 0;
						for(temp = 0;temp < 8;temp++)
						{
							set1[temp] = set[temp];
						}
					}
				}
      break; 
 }
}

void button4_callback(void *button)
{
    uint32_t btn_event_val; 
    btn_event_val = get_button_event((struct Button *)button); 
    switch(btn_event_val)
    {
      case SINGLE_CLICK: 
				HAL_GPIO_TogglePin(GPIOF,GPIO_PIN_0);
      break; 
    }
}

void show_set()
{
	LCD_ShowString6432(183,7,"V",GREEN);
	LCD_ShowString6432(183,71,"A",YELLOW);
	LCD_ShowString6432(183,135,"W",LIGHTGREEN);
	LCD_ShowString(23,207,"VSET",YELLOW);
	LCD_ShowString(103,207,"V",YELLOW);
	LCD_ShowString(127,207,"ISET",YELLOW);
	LCD_ShowString(207,207,"A",YELLOW);
	LCD_ShowChar(79,207,'.',0,GREEN);
	LCD_ShowChar(183,207,'.',0,GREEN);
	unsigned int z = 0;
	for(z = 0;z < 8;z++)
	{
			LCD_ShowChar(pos[z],207,set[z]+48,0,GREEN);
	}
}

void set_parameter()
{
	float vol = 0;
	uint32_t count = 0;
	vol = set1[0]*10 + set1[1]*1 + set1[2]*0.1 + set1[3]*0.01;
	count = (uint32_t)PID_realize(vol,actvol);
//	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, ((vol/8-0.1718)*4096)/3.3);
	if(count > 4095)
		count = 4095;
	HAL_DAC_SetValue(&hdac1, DAC_CHANNEL_1, DAC_ALIGN_12B_R, count);
}




		
		
		
		
		