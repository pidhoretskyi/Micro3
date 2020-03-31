
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "lwip.h"


#include "lwip/apps/httpd.h"
#include <string.h>

/* Private variables ---------------------------------------------------------*/
char const* TAGCHAR[]={"p","r","s","t"};
char const** TAGS=TAGCHAR;
uint32_t n=0;
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[]);
const tCGI LEDS_CGI={"/leds.cgi", LEDS_CGI_Handler};
tCGI CGI_TAB[1];
uint8_t ledstate=0;
char htmstr1[] = "<input value=\"";
char htmstr2[] = "\" name=\"led\" type=\"checkbox\"";


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void GPIO_Init(void);

//--------------------------------------------------------
uint16_t SSI_Handler(int iIndex, char *pcInsert, int iInsertLen)
{
	/*
	if (iIndex ==0)
	  {
		n++;
		sprintf(pcInsert,"%lu",n);
		return strlen(pcInsert);
	  }
	  else if (iIndex ==1)
	  {
		sprintf(pcInsert,"%lu",n+5);
		return strlen(pcInsert);
	  }
	  else if (iIndex ==2)
	  {
		sprintf(pcInsert,"%lu",n+10);
		return strlen(pcInsert);
	  }
	  else if (iIndex ==3)
	  {
		sprintf(pcInsert,"%lu",n+15);
		return strlen(pcInsert);
	  }
	  */
	strcpy(pcInsert,htmstr1);
	if (iIndex ==0)
	{
	  strcat(pcInsert,"1");
	  strcat(pcInsert,htmstr2);
	  if(ledstate&0x01)
	  {
	    strcat(pcInsert," checked> ");
	  }
	  else
	  {
	    strcat(pcInsert,"> ");
	  }
	  return strlen(pcInsert);
	}
	else if (iIndex ==1)
	{
	  strcat(pcInsert,"2");
	  strcat(pcInsert,htmstr2);
	  if(ledstate&0x02)
	  {
	    strcat(pcInsert," checked> ");
	  }
	  else
	  {
	    strcat(pcInsert,"> ");
	  }
	  return strlen(pcInsert);
	}
	else if (iIndex ==2)
	{
	  strcat(pcInsert,"3");
	  strcat(pcInsert,htmstr2);
	  if(ledstate&0x04)
	  {
	    strcat(pcInsert," checked> ");
	  }
	  else
	  {
	    strcat(pcInsert,"> ");
	  }
	  return strlen(pcInsert);
	}
	else if (iIndex ==3)
	{
	  strcat(pcInsert,"4");
	  strcat(pcInsert,htmstr2);
	  if(ledstate&0x08)
	  {
	    strcat(pcInsert," checked> ");
	  }
	  else
	  {
	    strcat(pcInsert,"> ");
	  }
	  return strlen(pcInsert);
	}
  return 0;
}
//--------------------------------------------------------
const char * LEDS_CGI_Handler(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])

{
  uint32_t i=0;
  if (iIndex==0)
  {
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
    ledstate = 0;
    for (i=0; i<iNumParams; i++)
    {
      if (strcmp(pcParam[i] , "led")==0)
      {
        if(strcmp(pcValue[i], "1") ==0)
        {
          ledstate |= 1;
          HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);
        }
        else if(strcmp(pcValue[i], "2") ==0)
        {
          HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
          ledstate |= 2;
        }
        else if(strcmp(pcValue[i], "3") ==0)
        {
          HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
          ledstate |= 4;
        }
        else if(strcmp(pcValue[i], "4") ==0)
        {
          HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
          ledstate |= 8;
        }
      }
    }
  }
  return "/led.shtml";
}

//---------------------------------------------------------------


int main(void)
{

  
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  GPIO_Init();
  LWIP_Init();

 
  httpd_init();
  http_set_ssi_handler(SSI_Handler, (char const **)TAGS, 4);
  CGI_TAB[0] = LEDS_CGI;
  http_set_cgi_handlers(CGI_TAB, 1);
  
  while (1)
  {
  
	  LWIP_Process();
  }


}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

static void GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD12 PD13 PD14 PD15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}


void _Error_Handler(char * file, int line)
{
  while(1) 
  {
  }
 
}
