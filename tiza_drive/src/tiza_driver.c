#define TIZA_DRIVER_GLOBAL

#include "tiza_include.h"


/******************************************************
GPIO初始化函数
******************************************************/
void Tiza_GpioInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
/***	 
	GPIO_DeInit(GPIOA);
	GPIO_DeInit(GPIOB);
	GPIO_DeInit(GPIOC);
	GPIO_DeInit(GPIOD);
	GPIO_DeInit(GPIOE);
	GPIO_DeInit(GPIOF);
	GPIO_DeInit(GPIOG);
***/	
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE 
											  | RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG		, ENABLE); //使能GPIO的时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;									//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//输入
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//50MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;									//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All & 0x1FFF;					//PA13~15 JTAG
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_All & 0xFFE7;					//PB03~04 JTAG
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOE , DISABLE); 			//关闭GPIOE的时钟	(未使用)

///输出引脚		
		
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_14 | GPIO_Pin_15;		//红灯和绿灯 对应IO口
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;								//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽输出
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;									//上拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//50MHz
  GPIO_Init(GPIOB, &GPIO_InitStructure);												//初始化GPIO
	
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;										//C_LED 黄灯对应IO口
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;								//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽输出
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;									//上拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//50MHz
  GPIO_Init(GPIOF, &GPIO_InitStructure);												//初始化GPIO
	
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_8;										//CT_LOCK 继电器 对应IO口
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;								//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽输出
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_UP;									//上拉
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;							//50MHz
  GPIO_Init(GPIOG, &GPIO_InitStructure);												//初始化GPIO
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_1;										//CT_SD电源 控制引脚
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽输出
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//无上下拉
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//高速GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_2;										//CT_485232电源 控制引脚
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽输出
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//无上下拉
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//高速GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10;									//CT_5V 5V电源 控制引脚
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽输出
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//无上下拉
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//高速GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;										//POWERKEY_CONNECT，模块关机控制引脚
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽输出
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//无上下拉
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//高速GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_8;										//CT_4V 4V电源 控制引脚
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽输出
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//无上下拉
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//高速GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_9;										//CTS_L218 控制引脚
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽输出
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//无上下拉
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//高速GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_8;										//RSEET_L218 控制引脚
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽输出
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//无上下拉
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//高速GPIO
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_12;										//DTR_L218 控制引脚
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽输出
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//无上下拉
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//高速GPIO
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_14;									//WDI 喂外狗 控制引脚
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;//GPIO_Mode_OUT;								//输出
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽输出
  GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//无上下拉
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//高速GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
///输入引脚

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_7;										// RING_L218 检测
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;									//输入
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽 对输入无效
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//高速GPIO
	GPIO_Init(GPIOF,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;										// ACC 检测
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;									//输入
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽 对输入无效
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//高速GPIO
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;										// check power 外电检测
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;									//输入
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽 对输入无效
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//高速GPIO
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	//=========================================================================//
	OFF_LOCK();						//关继电器
	ON_485232_PWR();			//开232 485 3.3V电源
	ON_CT5V_PWR();				//开5V总电源
	
}


/******************************************************
看门狗初始化函数
输入：
			flag(1启用): bit0内部看门狗 bit1外部看门狗
			ReloadVal	:	 内部看门狗重载时间(设置内部时用)
******************************************************/
void Tiza_WdtInit(uint8 flag, uint16 ReloadVal)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	if((flag&OPERATE_IWDG) == OPERATE_IWDG){
	//内部看门狗	
		IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
		IWDG_SetPrescaler(IWDG_Prescaler_32);
		IWDG_SetReload(ReloadVal);
		IWDG_ReloadCounter();
		IWDG_Enable();
	}
		
	if((flag&OPERATE_EWDG) == OPERATE_EWDG){
	//外部看门狗	
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_14;									//WDI 喂外狗 控制引脚
		GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;								//输出
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;								//推挽输出
		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;							//无上下拉
		GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz; 						//高速GPIO
		GPIO_Init(GPIOD,&GPIO_InitStructure);

		REST_WDT();
	}
}

/******************************************************
看门狗喂狗函数
输入：
			flag(1启用): bit0内部看门狗 bit1外部看门狗
******************************************************/
void Tiza_FeedWdg(uint8 flag)
{
	
	if((flag&OPERATE_IWDG) == OPERATE_IWDG){
	//内部看门狗	
		IWDG_ReloadCounter();
	}
		
	if((flag&OPERATE_EWDG) == OPERATE_EWDG){
	//外部看门狗	
		FEED_WDT();
	}
}





void SysReset(void)
{
//	while(1)
//	{
//		NVIC_DISABLE();
//	}
}




