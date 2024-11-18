/*
 * @Author: tangbaishu 2472686303@qq.com
 * @Date: 2024-10-26 09:44:06
 * @LastEditors: tangbaishu 2472686303@qq.com
 * @LastEditTime: 2024-10-27 17:46:13
 * @FilePath: \STM32F407ZGT6_Embedded_APP_FreeRTOS_V1.1\OLED\oled_main.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : Evk123
//  生成日期   : 2014-0101
//  最近修改   : 
//  功能描述   : 0.69寸OLED 接口演示例程(STM32F103ZE系列IIC)
//              说明: 
//              ----------------------------------------------------------------
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PA5（SCL）
//              SDA   接PA7（SDA）            
//              ----------------------------------------------------------------
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

#include "oled.h"
#include "bmp.h"
#include "delay.h"
#include "oled_driver.h"

int oled_main(void)
  {	
	u8 t;
	uint8_t x_offset=2,y_offset=1;
	OLED_Init();			//初始化OLED 
	OLED_Clear(); 
	while(1)
	{
		 
		OLED_ShowCHinese(0+x_offset,y_offset,0);//
		OLED_ShowCHinese(18+x_offset,y_offset,1);//
		OLED_ShowCHinese(36+x_offset,y_offset,2);//
		OLED_ShowCHinese(54+x_offset,y_offset,3);//
		OLED_ShowCHinese(72+x_offset,y_offset,4);//
		OLED_ShowCHinese(90+x_offset,y_offset,5);//


		OLED_ShowCHinese(0+x_offset,y_offset+3,7);//中
		OLED_ShowCHinese(18+x_offset,y_offset+3,8);//景
		OLED_ShowCHinese(36+x_offset,y_offset+3,9);//园
		OLED_ShowCHinese(54+x_offset,y_offset+3,10);//电
		OLED_ShowCHinese(72+x_offset,y_offset+3,11);//子
		OLED_ShowCHinese(90+x_offset,y_offset+3,12);//科
		while(1);
	}
		OLED_Clear(); 
		t=' ';
		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
	while(1) 
	{		
		OLED_Clear();
		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
		OLED_ShowString(6,3,"0.96' OLED TEST",16);
		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	 //	OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:",16);  
		OLED_ShowString(63,6,"CODE:",16);  
		OLED_ShowChar(48,6,t,16);//显示ASCII字符	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值 	
			delay_ms(8000);
		delay_ms(8000);

					delay_ms(8000);
		delay_ms(8000);
		delay_ms(8000);
		OLED_DrawBMP(0,0,128,8,BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
		delay_ms(8000);
					delay_ms(8000);
		delay_ms(8000);
		delay_ms(8000);
		OLED_DrawBMP(0,0,128,8,BMP1);
		delay_ms(8000);
					delay_ms(8000);
		delay_ms(8000);
		delay_ms(8000);
	}	  
	
}	

