#include "stm32f10x.h"                  // Device header
#include "bsp_spi.h"
#include "W25Q64_Ins.h"

/**
  * �?    数：W25Q64初始�?
  * �?    数：�?
  * �? �? 值：�?
  */
void W25Q64_Init(void)
{
	MySPI_Init();					//先初始化底层的SPI
}

/**
  * �?    数：W25Q64读取ID�?
  * �?    数：MID 工厂ID，使用输出参数的形式返回
  * �?    数：DID 设备ID，使用输出参数的形式返回
  * �? �? 值：�?
  */
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID)
{
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q64_JEDEC_ID);			//交换发送读取ID的指�?
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//交换接收MID，通过输出参数返回
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//交换接收DID�?8�?
	*DID <<= 8;									//�?8位移到高�?
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//或上交换接收DID的低8位，通过输出参数返回
	MySPI_Stop();								//SPI终止
}

/**
  * �?    数：W25Q64写使�?
  * �?    数：�?
  * �? �? 值：�?
  */
void W25Q64_WriteEnable(void)
{
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);		//交换发送写使能的指�?
	MySPI_Stop();								//SPI终止
}

/**
  * �?    数：W25Q64等待�?
  * �?    数：�?
  * �? �? 值：�?
  */
void W25Q64_WaitBusy(void)
{
	uint32_t Timeout;
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);				//交换发送读状态寄存器1的指�?
	Timeout = 100000;							//给定超时计数时间
	while ((MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01) == 0x01)	//循环等待忙标志位
	{
		Timeout --;								//等待时，计数值自�?
		if (Timeout == 0)						//自减�?0后，等待超时
		{
			/*超时的错误处理代码，可以添加到此�?*/
			break;								//跳出等待，不等了
		}
	}
	MySPI_Stop();								//SPI终止
}

/**
  * �?    数：W25Q64页编�?
  * �?    数：Address 页编程的起始地址，范围：0x000000~0x7FFFFF
  * �?    数：DataArray	用于写入数据的数�?
  * �?    数：Count 要写入数据的数量，范围：0~256
  * �? �? 值：�?
  * 注意事项：写入的地址范围不能跨页
  */
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count)
{
	uint16_t i;
	
	W25Q64_WriteEnable();						//写使�?
	
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);		//交换发送页编程的指�?
	MySPI_SwapByte(Address >> 16);				//交换发送地址23~16�?
	MySPI_SwapByte(Address >> 8);				//交换发送地址15~8�?
	MySPI_SwapByte(Address);					//交换发送地址7~0�?
	for (i = 0; i < Count; i ++)				//循环Count�?
	{
		MySPI_SwapByte(DataArray[i]);			//依次在起始地址后写入数�?
	}
	MySPI_Stop();								//SPI终止
	
	W25Q64_WaitBusy();							//等待�?
}

/**
  * �?    数：W25Q64扇区擦除�?4KB�?
  * �?    数：Address 指定扇区的地址，范围：0x000000~0x7FFFFF
  * �? �? 值：�?
  */
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();						//写使�?
	
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);	//交换发送扇区擦除的指令
	MySPI_SwapByte(Address >> 16);				//交换发送地址23~16�?
	MySPI_SwapByte(Address >> 8);				//交换发送地址15~8�?
	MySPI_SwapByte(Address);					//交换发送地址7~0�?
	MySPI_Stop();								//SPI终止
	
	W25Q64_WaitBusy();							//等待�?
}

/**
  * �?    数：W25Q64读取数据
  * �?    数：Address 读取数据的起始地址，范围：0x000000~0x7FFFFF
  * �?    数：DataArray 用于接收读取数据的数组，通过输出参数返回
  * �?    数：Count 要读取数据的数量，范围：0~0x800000
  * �? �? 值：�?
  */
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count)
{
	uint32_t i;
	MySPI_Start();								//SPI起始
	MySPI_SwapByte(W25Q64_READ_DATA);			//交换发送读取数据的指令
	MySPI_SwapByte(Address >> 16);				//交换发送地址23~16�?
	MySPI_SwapByte(Address >> 8);				//交换发送地址15~8�?
	MySPI_SwapByte(Address);					//交换发送地址7~0�?
	for (i = 0; i < Count; i ++)				//循环Count�?
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//依次在起始地址后读取数�?
	}
	MySPI_Stop();								//SPI终止
}
