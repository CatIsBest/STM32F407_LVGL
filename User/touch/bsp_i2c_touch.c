/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @author  STMicroelectronics
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   ���ݴ�������ר��iic����
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  STM32 F407 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./touch/bsp_i2c_touch.h"
#include "./touch/gt5xx.h"


/* STM32 I2C ����ģʽ */
#define I2C_Speed              400000

/* �����ַֻҪ��STM32��ҵ�I2C������ַ��һ������ */
#define I2C_OWN_ADDRESS7      0x0A



static void Delay(__IO uint32_t nCount)	 //�򵥵���ʱ����
{
	for(; nCount != 0; nCount--);
}


/**
  * @brief  ʹ�ܴ������ж�
  * @param  ��
  * @retval ��
  */
void I2C_GTP_IRQEnable(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  /*ѡ��Ҫ���Ƶ�GPIO����*/															   
  GPIO_InitStructure.Pin = GTP_INT_GPIO_PIN;	
  /*�������ŵ��������Ϊ����*/
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;	
  /*��������Ϊ����ģʽ*/
  GPIO_InitStructure.Pull  =  GPIO_NOPULL;
  /*������������Ϊ���� */   
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH; 
  /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
  HAL_GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);	
  /* ���� EXTI �ж�Դ ��key1 ���š������ж����ȼ�*/
  HAL_NVIC_SetPriority(GTP_INT_EXTI_IRQ , 1, 1);
  /* ʹ���ж� */
  HAL_NVIC_EnableIRQ(GTP_INT_EXTI_IRQ );
}

/**
  * @brief  �رմ������ж�
  * @param  ��
  * @retval ��
  */
void I2C_GTP_IRQDisable(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /*ѡ��Ҫ���Ƶ�GPIO����*/															   
  GPIO_InitStructure.Pin = GTP_INT_GPIO_PIN;	
  /*�������ŵ��������Ϊ����*/
  GPIO_InitStructure.Mode = GPIO_MODE_IT_RISING;	  
  /*��������Ϊ����ģʽ*/
  GPIO_InitStructure.Pull  =  GPIO_NOPULL;
  /*������������Ϊ���� */   
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH; 
  /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
  HAL_GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);	
  /* ���� EXTI �ж�Դ �������ж����ȼ�*/
  HAL_NVIC_SetPriority(GTP_INT_EXTI_IRQ , 1, 1);
    /* ʹ���ж� */
  HAL_NVIC_DisableIRQ(GTP_INT_EXTI_IRQ );
}

/**
  * @brief  ������ I/O����
  * @param  ��
  * @retval ��
  */
static void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;  
  /*ʹ�ܴ�����ʹ�õ����ŵ�ʱ��*/
  GTP_I2C_SCL_CLK();
  GTP_I2C_SDA_CLK();
  GTP_I2C_RST_CLK(); 
  GTP_I2C_GTP_CLK();
   /*ѡ��Ҫ���Ƶ�GPIO����*/															   
  GPIO_InitStructure.Pin = GTP_I2C_SCL_PIN;	
  /*�������ŵ��������Ϊ����*/
  GPIO_InitStructure.Mode  =  GPIO_MODE_OUTPUT_OD;  
  /*��������Ϊ����ģʽ*/
  GPIO_InitStructure.Pull  =  GPIO_NOPULL;
  /*������������Ϊ���� */   
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH; 
  /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
  HAL_GPIO_Init(GTP_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);	
   /*ѡ��Ҫ���Ƶ�GPIO����*/															   
  GPIO_InitStructure.Pin =GTP_I2C_SDA_PIN;	
   /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
  HAL_GPIO_Init(GTP_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);
  /*ѡ��Ҫ���Ƶ�GPIO����*/															   
  GPIO_InitStructure.Pin = GTP_RST_GPIO_PIN;	
  /*�������ŵ��������Ϊ����*/
  GPIO_InitStructure.Mode  =  GPIO_MODE_OUTPUT_PP;  
  /*��������Ϊ����ģʽ*/
  GPIO_InitStructure.Pull  =  GPIO_PULLDOWN;
  /*������������Ϊ���� */   
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH; 
  /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
  HAL_GPIO_Init(GTP_RST_GPIO_PORT, &GPIO_InitStructure);	
  /*ѡ��Ҫ���Ƶ�GPIO����*/															   
  GPIO_InitStructure.Pin = GTP_RST_GPIO_PIN;	
   /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
  HAL_GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);	
}


/**
  * @brief  ��GT91xxоƬ���и�λ
  * @param  ��
  * @retval ��
  */
void I2C_ResetChip(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /*ѡ��Ҫ���Ƶ�GPIO����*/															   
  GPIO_InitStructure.Pin =GTP_INT_GPIO_PIN;	
  /*�������ŵ��������Ϊ����*/
  GPIO_InitStructure.Mode  =  GPIO_MODE_OUTPUT_PP;  
  /*��������Ϊ����ģʽ*/
  GPIO_InitStructure.Pull  =  GPIO_PULLDOWN;
  /*������������Ϊ���� */   
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH; 
  /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
  HAL_GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);	
  /*��λΪ�͵�ƽ��Ϊ��ʼ����׼��*/
  digitalLow (GTP_RST_GPIO_PORT,GTP_RST_GPIO_PIN);
  Delay(0x0FFFFF);
  /*����һ��ʱ�䣬���г�ʼ��*/
  digitalHigh (GTP_RST_GPIO_PORT,GTP_RST_GPIO_PIN);
  Delay(0x0FFFFF);
  /*ѡ��Ҫ���Ƶ�GPIO����*/															   
  GPIO_InitStructure.Pin =GTP_INT_GPIO_PIN;	
  /*�������ŵ��������Ϊ����*/
  GPIO_InitStructure.Mode  =  GPIO_MODE_INPUT;  
  /*��������Ϊ����ģʽ*/
  GPIO_InitStructure.Pull  =  GPIO_NOPULL;
  /*������������Ϊ���� */   
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH; 
  /*���ÿ⺯����ʹ���������õ�GPIO_InitStructure��ʼ��GPIO*/
  HAL_GPIO_Init(GTP_INT_GPIO_PORT, &GPIO_InitStructure);	
}

/**
  * @brief  I2C ����(GT5xx)��ʼ��
  * @param  ��
  * @retval ��
  */
void I2C_Touch_Init(void)
{
  I2C_GPIO_Config(); 
 
  I2C_ResetChip();

  I2C_GTP_IRQEnable();
}
/*
*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ180MHz ��MDK���뻷����1���Ż�
      
		ѭ������Ϊ50ʱ��SCLƵ�� = 333KHz 
		ѭ������Ϊ30ʱ��SCLƵ�� = 533KHz��  
	 	ѭ������Ϊ20ʱ��SCLƵ�� = 727KHz�� 
  */
	for (i = 0; i < 10*5; i++)
	__NOP();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C���������ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Start(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Start
*	����˵��: CPU����I2C����ֹͣ�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Stop(void)
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_SendByte
*	����˵��: CPU��I2C�����豸����8bit����
*	��    �Σ�_ucByte �� �ȴ����͵��ֽ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;

	/* �ȷ����ֽڵĸ�λbit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();	
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); // �ͷ�����
		}
		_ucByte <<= 1;	/* ����һ��bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_ReadByte
*	����˵��: CPU��I2C�����豸��ȡ8bit����
*	��    �Σ���
*	�� �� ֵ: ����������
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;

	/* ������1��bitΪ���ݵ�bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_WaitAck
*	����˵��: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
*	��    �Σ���
*	�� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(void)
{
	uint8_t re;

	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
	if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_0();
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Ack
*	����˵��: CPU����һ��ACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU����SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_NAck
*	����˵��: CPU����1��NACK�ź�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void i2c_NAck(void)
{
	I2C_SDA_1();	/* CPU����SDA = 1 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();	
}



#define I2C_DIR_WR	0		/* д����bit */
#define I2C_DIR_RD	1		/* ������bit */
/**
  * @brief   ʹ��IIC��ȡ����
  * @param   
  * 	@arg ClientAddr:���豸��ַ
  *		@arg pBuffer:����ɴӻ���ȡ�����ݵĻ�����ָ��
  *		@arg NumByteToRead:��ȡ�����ݳ���
  * @retval  ��
  */
uint32_t I2C_ReadBytes(uint8_t ClientAddr,uint8_t* pBuffer, uint16_t NumByteToRead)
{
	/* ��1��������I2C���������ź� */
	i2c_Start();
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	i2c_SendByte(ClientAddr | I2C_DIR_RD);	/* �˴��Ƕ�ָ�� */
	/* ��3�����ȴ�ACK */
	if (i2c_WaitAck() != 0)
	{
		goto cmd_fail;	/* ������Ӧ�� */
	}
	while(NumByteToRead) 
  {
   if(NumByteToRead == 1)
    {
			i2c_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
      /* ����I2C����ֹͣ�ź� */
      i2c_Stop();
    }
   *pBuffer = i2c_ReadByte();
    /* ��ָ������ */
    pBuffer++; 
    /*�������Լ� */
    NumByteToRead--;
    i2c_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */  
  }
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;	/* ִ�гɹ� */
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 1;
}
/**
  * @brief   ʹ��IICд������
  * @param   
  * 	@arg ClientAddr:���豸��ַ
  *		@arg pBuffer:������ָ��
  *     @arg NumByteToWrite:д���ֽ���
  * @retval  ��
  */
uint32_t I2C_WriteBytes(uint8_t ClientAddr,uint8_t* pBuffer,  uint8_t NumByteToWrite)
{
	uint16_t m;	
  /*����0������ֹͣ�źţ������ڲ�д������*/
  i2c_Stop();
  /* ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ����, һ��С�� 10ms 			
    CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
  */
  for (m = 0; m < 1000; m++)
  {				
    /* ��1��������I2C���������ź� */
    i2c_Start();
    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
    i2c_SendByte(ClientAddr | I2C_DIR_WR);	/* �˴���дָ�� */
    /* ��3��������һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
    if (i2c_WaitAck() == 0)
    {
      break;
    }
  }
  if (m  == 1000)
  {
    goto cmd_fail;	/* EEPROM����д��ʱ */
  }	
  while(NumByteToWrite--)
  {
  /* ��4������ʼд������ */
  i2c_SendByte(*pBuffer);
  /* ��5�������ACK */
  if (i2c_WaitAck() != 0)
  {
    goto cmd_fail;	/* ������Ӧ�� */
  }
      pBuffer++;	/* ��ַ��1 */		
  }
	/* ����ִ�гɹ�������I2C����ֹͣ�ź� */
	i2c_Stop();
	return 0;
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	i2c_Stop();
	return 1;
}


/*********************************************END OF FILE**********************/
