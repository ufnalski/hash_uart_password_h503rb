/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */


//#define USE_EEPROM
//#define RUN_I2C_SCANNER


/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "gpdma.h"
#include "hash.h"
#include "i2c.h"
#include "icache.h"
#include "memorymap.h"
#include "rng.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ssd1306.h"

#ifdef USE_EEPROM
#include "ee24.h"
#endif

#include "keypad4x4.h"
#include "roll_a_die.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MAX_ROLLS 8192
#define UartRxBuf_SIZE 64
#define UartTxBuf_SIZE 64

#ifdef USE_EEPROM
#define STORE_HASH_IN_EEPROM 0
#define EEPROM_I2C_PORT hi2c1
#endif

#define DEBUG_UART_POINTER (&huart3)
#define PASSWORD_UART_POINTER (&huart3)
#define PASSWORD_UART_INSTANCE USART3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

// UART
uint8_t UartRxBuf[UartRxBuf_SIZE];
uint8_t UartTxBuf[UartRxBuf_SIZE];
uint32_t UartMessageLength;
volatile uint8_t uart_data_received_flag = 0;
extern DMA_HandleTypeDef handle_GPDMA1_Channel0;

// HASH
const uint8_t StringToBeHashed[] =
		"The mediocre teacher tells. The good teacher explains. The superior teacher demonstrates. The great teacher inspires. William Arthur Ward";
// https://www.pelock.com/products/hash-calculator
const uint8_t ExpectedSHA256DigestString[65] =
		"E0B8D29E2A810BCB45F6D75FBA5AD5E1DF280045CAA115F26211D45EBA632D9F";

uint8_t ExpectedSHA256Digest[32];

const uint8_t PasswordSHA256DigestString[65] =
		"A9B4C2AD7DCB399B815F299ADF84D55215D8E3288A31D772EA349B107965CA4F"; // barteq

uint8_t PasswordSHA256Digest[32];

uint8_t SHA256Digest[32];

#ifdef USE_EEPROM
// EEPROM
EE24_HandleTypeDef ee24;
uint8_t eeprom_data_tx[64];
uint8_t eeprom_data_rx[65];
#endif

// KEYPAD
uint8_t key;

// RNG
volatile uint8_t rng_flag = 0;
uint32_t roll_count = 0;
uint32_t roll_a_die_raw;
uint32_t die_face_stats[8];
uint8_t die_face;

// OLED
char lcd_line[32];

//CRC
const uint8_t crc_input_data[] =
{ 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };
uint16_t crc_result;

const uint16_t crc_expected_maxim = 0x44C2; // MAXIM 1-Wire
const uint16_t crc_final_xor_maxim = 0xFFFF;

const uint16_t crc_expected_usb = 0xB4C8; // USB
const uint16_t crc_final_xor_usb = 0xFFFF;

const uint16_t crc_expected_genibus = 0xD64E; // Grundfos GENIBUS
const uint16_t crc_final_xor_genibus = 0xFFFF;

const uint16_t crc_expected_modbus = 0x4B37; // MODBUS
const uint16_t crc_final_xor_modbus = 0x0000;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Hash256String2ArrayOfBytes(const uint8_t *hash_string, uint8_t *hash_bytes);
void MX_CRC16_USB_Init(void);
void MX_CRC16_MAXIM_Init(void);
void MX_CRC16_MODBUS_Init(void);
void MX_CRC16_GENIBUS_Init(void);
void TestCrcConfigurations(void);
void I2CScanner(void);
void DisplayDieStats(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{

	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_GPDMA1_Init();
	MX_ICACHE_Init();
	MX_USB_PCD_Init();
	MX_HASH_Init();
	MX_USART3_UART_Init();
	MX_USART1_UART_Init();
	MX_I2C1_Init();
	MX_RNG_Init();
	MX_CRC_Init();
	MX_I2C2_Init();
	/* USER CODE BEGIN 2 */

	// OLED
	ssd1306_Init();
	ssd1306_Fill(Black);
	ssd1306_SetCursor(20, 0);
	ssd1306_WriteString("ufnalski.edu.pl", Font_6x8, White);
	ssd1306_SetCursor(15, 12);
	ssd1306_WriteString("HASH, CRC and RNG", Font_6x8, White);
	ssd1306_UpdateScreen();

//	// Clear UART terminal
//	UartMessageLength = sprintf((char*) UartRxBuf, "%c[2J", 0x1B);
//	HAL_UART_Transmit(DEBUG_UART_POINTER, UartRxBuf, UartMessageLength, 100);
//	UartMessageLength = sprintf((char*) UartRxBuf, "%c[1;1H", 0x1B); // some terminals count from 0: "%c[0;0H"
//	HAL_UART_Transmit(DEBUG_UART_POINTER, UartRxBuf, UartMessageLength, 100);

	TestCrcConfigurations();

#ifdef RUN_I2C_SCANNER
	I2CScanner();
#endif

#ifdef USE_EEPROM
	// EEPROM and HASH
	if (!EE24_Init(&ee24, &EEPROM_I2C_PORT, EE24_ADDRESS_DEFAULT))
	{
		HAL_UART_Transmit(DEBUG_UART_POINTER, (uint8_t*) "EEPROM not OK!!!\r\n",
				strlen("EEPROM not OK!!!\r\n"), 100);
		while (1)
		{
			Error_Handler();
		}
	}
	else
	{
		HAL_UART_Transmit(DEBUG_UART_POINTER, (uint8_t*) "\r\n\r\r",
				strlen("\r\n\r\r"), 100);
	}

#if (STORE_HASH_IN_EEPROM)

	if (EE24_Write(&ee24, 0, (uint8_t*) ExpectedSHA256DigestString, 64, 1000))
	{
		HAL_UART_Transmit(DEBUG_UART_POINTER,
				(uint8_t*) "Hash successfully stored in EEPROM\r\n",
				strlen("Hash successfully stored in EEPROM\r\n"), 100);
	}
	else
	{
		HAL_UART_Transmit(DEBUG_UART_POINTER,
				(uint8_t*) "Problem with storing hash in EEPROM!!!\r\n",
				strlen("Problem with storing hash in EEPROM!!!\r\n"), 100);
	}
#endif
#endif

	HAL_UARTEx_ReceiveToIdle_DMA(PASSWORD_UART_POINTER, UartRxBuf,
	UartRxBuf_SIZE);
	__HAL_DMA_DISABLE_IT(&handle_GPDMA1_Channel0, DMA_IT_HT);

#ifdef USE_EEPROM
	if (EE24_Read(&ee24, 0, eeprom_data_rx, 64, 1000))
	{
		HAL_UART_Transmit(DEBUG_UART_POINTER,
				(uint8_t*) "Hash successfully retrieved from EEPROM.\r\n\r\n",
				strlen("Hash successfully retrieved from EEPROM.\r\n\r\n"),
				100);
		eeprom_data_rx[64] = '\0';
	}
	else
	{
		HAL_UART_Transmit(DEBUG_UART_POINTER,
				(uint8_t*) "Problem with retrieving hash from EEPROM!!!\r\n",
				strlen("Problem with retrieving hash from EEPROM!!!"), 100);
	}

	Hash256String2ArrayOfBytes(eeprom_data_rx, ExpectedSHA256Digest);
#else
	Hash256String2ArrayOfBytes(ExpectedSHA256DigestString,
			ExpectedSHA256Digest);
#endif

	HAL_UART_Transmit(DEBUG_UART_POINTER, StringToBeHashed,
			strlen((char const*) StringToBeHashed), 100);
	HAL_UART_Transmit(DEBUG_UART_POINTER, (uint8_t*) "\r\n", strlen("\r\n"),
			100);

#ifdef USE_EEPROM
	HAL_UART_Transmit(DEBUG_UART_POINTER, eeprom_data_rx,
			strlen((char const*) eeprom_data_rx), 100);
	HAL_UART_Transmit(DEBUG_UART_POINTER, (uint8_t*) "\r\n", strlen("\r\n"),
			100);
#endif

	HAL_HASH_Start(&hhash, StringToBeHashed,
			strlen((char const*) StringToBeHashed), SHA256Digest, 100);

	if (memcmp(SHA256Digest, ExpectedSHA256Digest, 32) == 0)
	{
		HAL_UART_Transmit(DEBUG_UART_POINTER, (uint8_t*) "Hash OK.\r\n",
				strlen("Hash OK.\r\n"), 100);
	}
	else
	{
		HAL_UART_Transmit(DEBUG_UART_POINTER, (uint8_t*) "Hash not OK.\r\n",
				strlen("Hash not OK.\r\n"), 100);
	}

	HAL_UART_Transmit(DEBUG_UART_POINTER, (uint8_t*) "\r\n", strlen("\r\n"),
			100);

	// RNG
	HAL_RNG_GenerateRandomNumber_IT(&hrng);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */

	Hash256String2ArrayOfBytes(PasswordSHA256DigestString,
			PasswordSHA256Digest);

	while (1)
	{

		key = read_keypad();
		if (key != '\0')
		{
			HAL_Delay(1000); // Just for testing in Live Expressions
		}

		if ((1 == rng_flag) && (roll_count < MAX_ROLLS))
		{
			rng_flag = 0;
			roll_count++;

			roll_a_die_raw = HAL_RNG_ReadLastRandomNumber(&hrng);
			die_face = RawRandomTo6FaceDie(roll_a_die_raw);
			AddToDieStats(&die_face_stats[0], die_face);
			HAL_RNG_GenerateRandomNumber_IT(&hrng);

			DisplayDieStats();
		}

		if (uart_data_received_flag == 1)
		{
			uart_data_received_flag = 0;

			HAL_HASH_Start(&hhash, UartRxBuf, strlen((char const*) UartRxBuf),
					SHA256Digest, 100);

			if (memcmp(SHA256Digest, PasswordSHA256Digest, 32) == 0)
			{
				HAL_UART_Transmit(PASSWORD_UART_POINTER,
						(uint8_t*) "Password OK.\r\n",
						strlen("Password OK.\r\n"), 100);
			}
			else
			{
				HAL_UART_Transmit(PASSWORD_UART_POINTER,
						(uint8_t*) "Password not OK.\r\n",
						strlen("Password not OK.\r\n"), 100);
			}
			HAL_UART_Transmit(DEBUG_UART_POINTER, (uint8_t*) "\r\n",
					strlen("\r\n"), 100);

			memset(UartRxBuf, 0, UartRxBuf_SIZE);
			HAL_UARTEx_ReceiveToIdle_DMA(PASSWORD_UART_POINTER, UartRxBuf,
			UartRxBuf_SIZE);
			__HAL_DMA_DISABLE_IT(&handle_GPDMA1_Channel0, DMA_IT_HT);
		}

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct =
	{ 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct =
	{ 0 };

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

	while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY))
	{
	}

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48
			| RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 12;
	RCC_OscInitStruct.PLL.PLLN = 250;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_1;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 | RCC_CLOCKTYPE_PCLK3;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure the programming delay
	 */
	__HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_2);
}

/* USER CODE BEGIN 4 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
	if (huart->Instance == PASSWORD_UART_INSTANCE)
	{
		uart_data_received_flag = 1;
	}
}

void Hash256String2ArrayOfBytes(const uint8_t *hash_string, uint8_t *hash_bytes)
{
	uint8_t bb[3];
	for (uint8_t i = 0; i < 32; i++)
	{
		strncpy((char*) bb, (char*) hash_string + (2 * i), 2);
		hash_bytes[i] = (uint8_t) strtol((char*) bb, NULL, 16);
	}
}

void HAL_RNG_ReadyDataCallback(RNG_HandleTypeDef *hrng, uint32_t random32bit)
{
	rng_flag = 1;
}

void MX_CRC16_USB_Init(void)
{
	hcrc.Instance = CRC;
	hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
	hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
	hcrc.Init.GeneratingPolynomial = 0x8005;
	hcrc.Init.CRCLength = CRC_POLYLENGTH_16B;
	hcrc.Init.InitValue = 0xFFFF;
	hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_BYTE;
	hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;
	hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
	if (HAL_CRC_Init(&hcrc) != HAL_OK)
	{
		Error_Handler();
	}
}

void MX_CRC16_MAXIM_Init(void)
{
	hcrc.Instance = CRC;
	hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
	hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
	hcrc.Init.GeneratingPolynomial = 0x8005;
	hcrc.Init.CRCLength = CRC_POLYLENGTH_16B;
	hcrc.Init.InitValue = 0x0000;
	hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_BYTE;
	hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;
	hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
	if (HAL_CRC_Init(&hcrc) != HAL_OK)
	{
		Error_Handler();
	}
}

void MX_CRC16_MODBUS_Init(void)
{
	hcrc.Instance = CRC;
	hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
	hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
	hcrc.Init.GeneratingPolynomial = 0x8005;
	hcrc.Init.CRCLength = CRC_POLYLENGTH_16B;
	hcrc.Init.InitValue = 0xFFFF;
	hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_BYTE;
	hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_ENABLE;
	hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
	if (HAL_CRC_Init(&hcrc) != HAL_OK)
	{
		Error_Handler();
	}
}

void MX_CRC16_GENIBUS_Init(void)
{
	hcrc.Instance = CRC;
	hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
	hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
	hcrc.Init.GeneratingPolynomial = 0x1021;
	hcrc.Init.CRCLength = CRC_POLYLENGTH_16B;
	hcrc.Init.InitValue = 0xFFFF;
	hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
	hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
	hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
	if (HAL_CRC_Init(&hcrc) != HAL_OK)
	{
		Error_Handler();
	}
}

void TestCrcConfigurations(void)
{
	// CRC MAXIM
	MX_CRC16_MAXIM_Init();
	UartMessageLength = sprintf((char*) UartRxBuf,
			"Expected CRC16_MAXIM:   0x%04X\r\n", crc_expected_maxim);
	HAL_UART_Transmit(DEBUG_UART_POINTER, UartRxBuf, UartMessageLength, 100);
	UartMessageLength = sprintf((char*) UartRxBuf,
			"Calculated CRC16_MAXIM: 0x%04X\r\n\r\n",
			((uint16_t) HAL_CRC_Calculate(&hcrc, (uint32_t*) crc_input_data,
					sizeof(crc_input_data))) ^ crc_final_xor_maxim);
	HAL_UART_Transmit(DEBUG_UART_POINTER, UartRxBuf, UartMessageLength, 100);

	// CRC USB
	MX_CRC16_USB_Init();
	UartMessageLength = sprintf((char*) UartRxBuf,
			"Expected CRC16_USB:   0x%04X\r\n", crc_expected_usb);
	HAL_UART_Transmit(DEBUG_UART_POINTER, UartRxBuf, UartMessageLength, 100);
	UartMessageLength = sprintf((char*) UartRxBuf,
			"Calculated CRC16_USB: 0x%04X\r\n\r\n",
			((uint16_t) HAL_CRC_Calculate(&hcrc, (uint32_t*) crc_input_data,
					sizeof(crc_input_data))) ^ crc_final_xor_usb);
	HAL_UART_Transmit(DEBUG_UART_POINTER, UartRxBuf, UartMessageLength, 100);

	// CRC MODBUS
	MX_CRC16_MODBUS_Init();
	UartMessageLength = sprintf((char*) UartRxBuf,
			"Expected CRC16_MODBUS:   0x%04X\r\n", crc_expected_modbus);
	HAL_UART_Transmit(DEBUG_UART_POINTER, UartRxBuf, UartMessageLength, 100);
	UartMessageLength = sprintf((char*) UartRxBuf,
			"Calculated CRC16_MODBUS: 0x%04X\r\n\r\n",
			((uint16_t) HAL_CRC_Calculate(&hcrc, (uint32_t*) crc_input_data,
					sizeof(crc_input_data))) ^ crc_final_xor_modbus);
	HAL_UART_Transmit(DEBUG_UART_POINTER, UartRxBuf, UartMessageLength, 100);

	// CRC GENIBUS
	MX_CRC16_GENIBUS_Init();
	UartMessageLength = sprintf((char*) UartRxBuf,
			"Expected CRC16_GENIBUS:   0x%04X\r\n", crc_expected_genibus);
	HAL_UART_Transmit(DEBUG_UART_POINTER, UartRxBuf, UartMessageLength, 100);
	UartMessageLength = sprintf((char*) UartRxBuf,
			"Calculated CRC16_GENIBUS: 0x%04X\r\n\r\n",
			((uint16_t) HAL_CRC_Calculate(&hcrc, (uint32_t*) crc_input_data,
					sizeof(crc_input_data))) ^ crc_final_xor_genibus);
	HAL_UART_Transmit(DEBUG_UART_POINTER, UartRxBuf, UartMessageLength, 100);
}

void I2CScanner(void)
{
	for (uint8_t device_id = 1; device_id < 128; device_id++)
	{
		uint8_t ret = HAL_I2C_IsDeviceReady(&SSD1306_I2C_PORT,
				(uint16_t) (device_id << 1), 3, 5);
		if (ret != HAL_OK) /* No ACK received at that address */
		{
			HAL_UART_Transmit(DEBUG_UART_POINTER, (uint8_t*) ".", strlen("."),
					100);
		}
		else if (ret == HAL_OK)
		{
			uint8_t uart_msg[8];
			sprintf((char*) uart_msg, "0x%X", device_id);
			HAL_UART_Transmit(DEBUG_UART_POINTER, uart_msg,
					strlen((char*) uart_msg), 100);
		}
		HAL_Delay(25);
	}
	HAL_UART_Transmit(DEBUG_UART_POINTER, (uint8_t*) "\r\n\r\r",
			strlen("\r\n\r\r"), 100);
}

void DisplayDieStats(void)
{
	ssd1306_SetCursor(2, 25);
	sprintf(lcd_line, "[0]: %4li  [1]: %4li", die_face_stats[0],
			die_face_stats[1]);
	ssd1306_WriteString(lcd_line, Font_6x8, White);
	ssd1306_SetCursor(2, 35);
	sprintf(lcd_line, "[2]: %4li  [3]: %4li", die_face_stats[2],
			die_face_stats[3]);
	ssd1306_WriteString(lcd_line, Font_6x8, White);
	ssd1306_SetCursor(2, 45);
	sprintf(lcd_line, "[4]: %4li  [5]: %4li", die_face_stats[4],
			die_face_stats[5]);
	ssd1306_WriteString(lcd_line, Font_6x8, White);
	ssd1306_SetCursor(2, 55);
	sprintf(lcd_line, "[6]: %4li  [7]: %4li", die_face_stats[6],
			die_face_stats[7]);
	ssd1306_WriteString(lcd_line, Font_6x8, White);
	ssd1306_UpdateScreen();
}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
