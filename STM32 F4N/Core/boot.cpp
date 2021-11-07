/* ----------------------------------------------------------------------- */
/*                                                                         */
/*   Quartz Arc                                                            */
/*                                                                         */
/*   STM32 F411RE Nucleo 64                                                */
/*                                                                         */
/*   System: Core                                                          */
/*   Role: System Initialization                                           */
/*   Filename: boot.cpp                                                    */
/*   Date: 22th October 2021                                               */
/*   Created By: Benjamin Rosser                                           */
/*                                                                         */
/*   This code is covered by Creative Commons CC-BY-NC-SA license          */
/*   (C) Copyright 2021 Benjamin Rosser                                    */
/*                                                                         */
/* ----------------------------------------------------------------------- */

//Includes
#include "boot.hpp"


	//------------------------------------------
	//------------------------------------------
	//------------------------------------------

  //------------------------------
  //System Initialization Function


//SystemInitialize
//System Initialization Function
//
//Used to initialize CPU caches, NVIC and SysTick, as well as oscillators, PLLs, system clocks, bus clocks and some peripheral clocks.
//Also enables all GPIOs
//
//Returns QA_OK if successful, or QA_Fail if initialized failed
QA_Result SystemInitialize(void) {

	//-----------------------
	//Enable Cache & Prefetch
	__HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
	__HAL_FLASH_DATA_CACHE_ENABLE();
	__HAL_FLASH_PREFETCH_BUFFER_ENABLE();


  //--------------------------
  //Set NVIC Priority Grouping
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);


  //------------
	//Init SysTick
	if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK)
		return QA_Fail;


	//-------------------------
	//Setup Power Configuration
	__HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);


  //---------------------
  //Configure Oscillators
  //
  //Configure High Speed External oscillator to be used, and setup primary PLL to provide a 100MHz clock
  RCC_OscInitTypeDef RCC_OscInit = {0};
  RCC_OscInit.OscillatorType = RCC_OSCILLATORTYPE_HSE; //Define which oscillator is to be configured
  RCC_OscInit.HSEState       = RCC_HSE_BYPASS;         //Set High Speed External oscillator as system clock
  RCC_OscInit.PLL.PLLState   = RCC_PLL_ON;             //Enable primary PLL
  RCC_OscInit.PLL.PLLSource  = RCC_PLLSOURCE_HSE;      //Set High Speed External oscillator as PLL input clock

    //Set PLL values required for 100MHz system clock
  RCC_OscInit.PLL.PLLM       = 2;
  RCC_OscInit.PLL.PLLN       = 50;
  RCC_OscInit.PLL.PLLP       = RCC_PLLP_DIV2;
  RCC_OscInit.PLL.PLLQ       = 4;

  if (HAL_RCC_OscConfig(&RCC_OscInit) != HAL_OK) {     //Initialize Oscillators using values in init structure
    return QA_Fail;
  }


  //-------------------
  //Enable SysCfg Clock
  __HAL_RCC_SYSCFG_CLK_ENABLE();


  //--------------------------
  //Configure CPU & Bus Clocks
  RCC_ClkInitTypeDef RCC_ClkInit = {0};
  RCC_ClkInit.ClockType      = RCC_CLOCKTYPE_HCLK |       //Define which clocks are to be configured (HCLK, SYSCLK, PCLK1, PCLK2)
  		                         RCC_CLOCKTYPE_SYSCLK |
															 RCC_CLOCKTYPE_PCLK1 |
															 RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInit.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;   //Set primary PLL as system clock source
  RCC_ClkInit.AHBCLKDivider  = RCC_SYSCLK_DIV1;           //Set clock divider for host bus (AHB), DIV1 provides an AHB frequency of 100MHz
  RCC_ClkInit.APB1CLKDivider = RCC_HCLK_DIV2;             //Set clock divider for peripheral bus 1 (APB1), DIV2 provides an APB1 frequency of 50MHz
  RCC_ClkInit.APB2CLKDivider = RCC_HCLK_DIV1;             //Set clock divider for peripheral bus 2 (APB2), DIV1 provides an APB2 frequency of 100MHz

  if (HAL_RCC_ClockConfig(&RCC_ClkInit, FLASH_LATENCY_4) != HAL_OK) { //Initialize system clocks using required values, and setting Flash Latency to 4 cycles
  	return QA_Fail;
  }


  //------------------
  //Enable GPIO Clocks
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();


  //Return
  return QA_OK;
}



