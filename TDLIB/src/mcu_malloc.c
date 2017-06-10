/**
  ******************************************************************************
  * @file    mcu_malloc.c
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    09-June-2017
  * @brief   This file provides �ڴ���� functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
#define MCU_MALLOC_GLOBAL

/* Includes ------------------------------------------------------------------*/
#include "mcu_malloc.h"

/** @addtogroup STM32F4xx_StdPeriph_Driver
  * @{
  */
#define mem_allot 0

/** @defgroup CRC 
  * @brief CRC driver modules
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//�ڴ��(32�ֽڶ���),ʵ�ʴ洢���ݵĵط�
__align(32) static u8 mem1base[MEM1_MAX_SIZE];																					//�ڲ�SRAM�ڴ��
#if mem_allot
__align(32) static u8 mem2base[MEM2_MAX_SIZE] __attribute__((at(0X68000000)));					//�ⲿSRAM�ڴ��
__align(32) static u8 mem3base[MEM3_MAX_SIZE] __attribute__((at(0X10000000)));					//�ڲ�CCM�ڴ��
#else
__align(32) static u8 mem2base[MEM2_MAX_SIZE];
__align(32) static u8 mem3base[MEM3_MAX_SIZE];
#endif
//�ڴ�����ÿ���ڴ��־
static u16 mem1mapbase[MEM1_ALLOC_TABLE_SIZE];
#if mem_allot													//�ڲ�SRAM�ڴ��MAP
static u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000+MEM2_MAX_SIZE)));         //�ⲿSRAM�ڴ��MAP
static u16 mem3mapbase[MEM3_ALLOC_TABLE_SIZE] __attribute__((at(0X10000000+MEM3_MAX_SIZE)));         //�ڲ�CCM�ڴ��MAP
#else
static u16 mem2mapbase[MEM2_ALLOC_TABLE_SIZE];
static u16 mem3mapbase[MEM3_ALLOC_TABLE_SIZE];
#endif
//�ڴ����������ڴ���Ϣ��	   
static const u32 memtblsize[SRAMBANK]={MEM1_ALLOC_TABLE_SIZE,MEM2_ALLOC_TABLE_SIZE,MEM3_ALLOC_TABLE_SIZE};	//�ڴ���С
static const u32 memblksize[SRAMBANK]={MEM1_BLOCK_SIZE,MEM2_BLOCK_SIZE,MEM3_BLOCK_SIZE};					//�ڴ�ֿ��С
static const u32 memsize[SRAMBANK]={MEM1_MAX_SIZE,MEM2_MAX_SIZE,MEM3_MAX_SIZE};                             //�ڴ��ܴ�С

//�ڴ���������
static struct _m_mallco_dev mallco_dev=
{
//-����ĳ�ʼ����ҪC99 mode�������֧��
//	.init = my_mem_init,						//�ڴ��ʼ��
//	.perused = my_mem_perused,						//�ڴ�ʹ����
//	.membase = {mem1base},			//�ڴ��
//	.memmap = {mem1mapbase},//�ڴ����״̬��
//	.memrdy[0] = 0,  		 					//�ڴ����δ����
	my_mem_init,						    //�ڴ��ʼ��
	my_mem_perused,						    //�ڴ�ʹ����
	{mem1base,mem2base,mem3base},			//�ڴ��
	{mem1mapbase,mem2mapbase,mem3mapbase},  //�ڴ����״̬��
	0,0,0,  		 					    //�ڴ����δ����
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
//�����ڴ�
//*des:Ŀ�ĵ�ַ
//*src:Դ��ַ
//n:��Ҫ���Ƶ��ڴ泤��(�ֽ�Ϊ��λ)
static void mymemcpy(void *des,void *src,u32 n)  
{
    u8 *xdes=des;
    u8 *xsrc=src; 
    while(n--)*xdes++=*xsrc++;  
}
//�����ڴ�
//*s:�ڴ��׵�ַ
//c :Ҫ���õ�ֵ
//count:��Ҫ���õ��ڴ��С(�ֽ�Ϊ��λ)
static void mymemset(void *s,u8 c,u32 count)  
{
    u8 *xs = s;  
    while(count--)*xs++=c;  
}
//�ڴ�����ʼ��  
//memx:�����ڴ��
static void my_mem_init(u8 memx)
{
    mymemset(mallco_dev.memmap[memx], 0,memtblsize[memx]*2);//�ڴ�״̬����������  
    mymemset(mallco_dev.membase[memx], 0,memsize[memx]);	//�ڴ��������������  
    mallco_dev.memrdy[memx]=1;								//�ڴ�����ʼ��OK  
}
//��ȡ�ڴ�ʹ����
//memx:�����ڴ��
//����ֵ:ʹ����(0~100)
static u8 my_mem_perused(u8 memx)  
{
    u32 used=0;  
    u32 i;  
    for(i=0;i<memtblsize[memx];i++)  
    {
        if(mallco_dev.memmap[memx][i])used++; 
    } 
    return (used*100)/(memtblsize[memx]);  
}
//�ڴ����(�ڲ�����)
//memx:�����ڴ��
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ 
static u32 my_mem_malloc(u8 memx,u32 size)  
{
    signed long offset=0;  
    u32 nmemb;	//��Ҫ���ڴ����  
	u32 cmemb=0;//�������ڴ����
    u32 i;  
    if(!mallco_dev.memrdy[memx])mallco_dev.init(memx);//δ��ʼ��,��ִ�г�ʼ�� 
    if(size==0)return 0XFFFFFFFF;   //����Ҫ����
    nmemb=size/memblksize[memx];  	//��ȡ��Ҫ����������ڴ����
    if(size%memblksize[memx])nmemb++;
    for(offset=memtblsize[memx]-1;offset>=0;offset--)//���������ڴ������  
    {
		if(!mallco_dev.memmap[memx][offset])cmemb++;//�������ڴ��������
		else cmemb=0;								//�����ڴ������
		if(cmemb==nmemb)							//�ҵ�������nmemb�����ڴ��
		{
            for(i=0;i<nmemb;i++)  					//��ע�ڴ��ǿ� 
            {
                mallco_dev.memmap[memx][offset+i]=nmemb;  //��ǿռ�ʹ�����
            }
            return (offset*memblksize[memx]);       //����ƫ�Ƶ�ַ  
		}
    }  
    return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��  
}
//�ͷ��ڴ�(�ڲ�����),������������ڴ��־,��û������ڴ������
//memx:�����ڴ��
//offset:�ڴ��ַƫ��
//����ֵ:0,�ͷųɹ�;1,�ͷ�ʧ��;2,ƫ�Ƴ���
static u8 my_mem_free(u8 memx,u32 offset)  
{
    int i;  
    if(!mallco_dev.memrdy[memx])//δ��ʼ��,��ִ�г�ʼ��
	{
		mallco_dev.init(memx);    
        return 1;//δ��ʼ��  
    }
    if(offset<memsize[memx])//ƫ�����ڴ����. 
    {
        int index=offset/memblksize[memx];			//ƫ�������ڴ�����  
        int nmemb=mallco_dev.memmap[memx][index];	//�ڴ������
        for(i=0;i<nmemb;i++)  						//�ڴ������
        {
            mallco_dev.memmap[memx][index+i]=0;  
        }
        return 0;  
    }else return 2;//ƫ�Ƴ�����.  
}

/* Exported functions ---------------------------------------------------------*/
//�ͷ��ڴ�(�ⲿ����) 
//memx:�����ڴ��
//ptr:�ڴ��׵�ַ 
void myfree(u8 memx,void *ptr)
{
	u32 offset;   
	if(ptr==NULL)return;//��ַΪ0.  
 	offset=(u32)ptr-(u32)mallco_dev.membase[memx];     
    my_mem_free(memx,offset);	//�ͷ��ڴ�      
}
//�����ڴ�(�ⲿ����)
//memx:�����ڴ��
//size:�ڴ��С(�ֽ�)
//����ֵ:���䵽���ڴ��׵�ַ.
void *mymalloc(u8 memx,u32 size)
{
    u32 offset;   
	offset=my_mem_malloc(memx,size);  	   	 	   
    if(offset==0XFFFFFFFF)return NULL;  
    else return (void*)((u32)mallco_dev.membase[memx]+offset);  
}
//���·����ڴ�(�ⲿ����)
//memx:�����ڴ��
//*ptr:���ڴ��׵�ַ
//size:Ҫ������ڴ��С(�ֽ�)
//����ֵ:�·��䵽���ڴ��׵�ַ.
void *myrealloc(u8 memx,void *ptr,u32 size)
{
    u32 offset;
    offset=my_mem_malloc(memx,size);
    if(offset==0XFFFFFFFF)return NULL;
    else
    {
	    mymemcpy((void*)((u32)mallco_dev.membase[memx]+offset),ptr,size);	//�������ڴ����ݵ����ڴ�   
        myfree(memx,ptr);  											  		//�ͷž��ڴ�
        return (void*)((u32)mallco_dev.membase[memx]+offset);  				//�������ڴ��׵�ַ
    }
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
