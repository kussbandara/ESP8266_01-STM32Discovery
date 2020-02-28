

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "STM32vldiscovery.h"
#include "STM32f10x_rcc.h"
#include "STM32f10x_gpio.h"
#include "STM32f10x_usart.h"

#include "stdio.h"
#include "string.h"


/** @addtogroup Examples
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);
void GPIOInit(void);
void Clk_Init(void);
void USARTInit(void);
char f;

uint8_t RData[30];

void ESP8266_receive_string();
void ESP8266_send_string(char* st_pt);
char ESP8266_check_Response(int);
int i=0;

int t=0;
char resAT,resECHO,resMODE,resPWD,resMUX,resSERVER,resFSR,resSEND,resCLOSE;
char str[];


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  
 Clk_Init();
 GPIOInit();
 USARTInit();
 
 GPIOB->BRR=GPIO_Pin_9;
 Delay(50000);
  //Set Reset Pin 
 GPIOB->BSRR=GPIO_Pin_9;
 //Delay(500000);
 Delay(500000);
 Delay(500000);
 Delay(500000);
    //Test if OK is sent
    for(int i=0;i<6;i++){
      ESP8266_send_string("AT\r\n");
      ESP8266_receive_string();
      resAT=ESP8266_check_Response(1);
      if (resAT=='t'){
        break;
      }
    }
    //Disable ECHO
    if (resAT=='t'){
      for(int i=0;i<6;i++){
        ESP8266_send_string("ATE0\r\n");
        ESP8266_receive_string();
        resECHO=ESP8266_check_Response(1);
        if(resECHO=='t'){
          break;
        }
      }
    }
    //Set Mode to a client
    if (resAT=='t'){
      for(int i=0;i<6;i++){
        ESP8266_send_string("AT+CWMODE=1\r\n");
        ESP8266_receive_string();
        resMODE=ESP8266_check_Response(1);
        if(resMODE=='t'){
          break;
        }
      }
    }
    //Set internet connection
      if (resMODE=='t'){
        for(int i=0;i<6;i++){
          ESP8266_send_string("AT+CWJAP=\"YBZSJ\",\"GCP2L43SXK2ZK4L7\"\r\n");
          ESP8266_receive_string();
          ESP8266_send_string("AT+CWJAP_CUR?\r\n");
          ESP8266_receive_string();
          resPWD=ESP8266_check_Response(1);
          if(resPWD=='t'){
            break;
          }
        }
    }
    
  
 while (1)
  {
    //Set for multiple connection
    if(resAT=='t' && resMODE=='t' && resPWD=='t' ){
      for(int i=0;i<6;i++){
        ESP8266_send_string("AT+CIPMUX=1\r\n");
        ESP8266_receive_string();
        resMUX=ESP8266_check_Response(1);
        if(resMUX=='t'){
          break;
        } 
      }
      //start TCP connection
      if(resMUX=='t'){
        for(int i=0;i<6;i++){
          ESP8266_send_string("AT+CIPSTART=0,\"TCP\",\"api.thingspeak.com\",80\r\n");
          ESP8266_receive_string();
          resSERVER=ESP8266_check_Response(1);
          if(resSERVER=='t'){
            break;
          }
        }
      }
      //set the length of characters that will be sent

      if(resSERVER=='t'){
        for(int i=0;i<6;i++){
          ESP8266_send_string("AT+CIPSEND=0,56\r\n");//49
          ESP8266_receive_string();
          resSEND=ESP8266_check_Response(1);
          if(resSEND=='t'){
            break;
          }
        }
      }
      //Send 56 charactors including \r\n
      if(resSEND=='t'){
        for(int i=0;i<6;i++){
          ESP8266_send_string("GET /update?api_key=9DI016MZCPI8QEL1&field1=82field2=4\r\n");
          ESP8266_receive_string();
          resSEND=ESP8266_check_Response(1);
          if(resSEND=='t'){
            break;
          }
        }
      }
     //close tcp single connection
        for(int i=0;i<6;i++){
          ESP8266_send_string("AT+CIPCLOSE=0\r\n");
          ESP8266_receive_string();
          resCLOSE=ESP8266_check_Response(1);
          if(resCLOSE=='t'){
            break;
          }
        }
    }
  }  
  
}
 
char ESP8266_check_Response(int n){
  char* s= (char *)RData;
  int i;
  if (n==1){
    for (i=0;i<30;i++){
      if (*s=='O'){
          s++;
        if(*s=='K'){
          return 't';
        }    
      }else{
        s++;
        if (i==29){
          return 'f';
        }
      }
    }
  }
  
}

void ESP8266_send_string(char* st_pt)
{   
    while(*st_pt) {//if there is a char
      while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
      USART_SendData(USART3,*st_pt++); //process it as a byte data
    }
    
}

void ESP8266_receive_string()
{
  int i;
  for (i=0;i<30;i++ ){
      while(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
       f=USART_ReceiveData(USART3); //process it as a byte data
      RData[i]=f;
  }
}

void USARTInit(){
  USART_InitTypeDef USART_InitStructure;
  
  USART_InitStructure.USART_BaudRate=115200;
  USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode=USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStructure.USART_Parity=USART_Parity_No ;
  USART_InitStructure.USART_StopBits=USART_StopBits_1 ;
  USART_InitStructure.USART_WordLength=USART_WordLength_8b;  
  USART_Init(USART3,&USART_InitStructure);
  USART_Cmd(USART3,ENABLE);
  
}


void Clk_Init(){
  
   /* Enable the GPIO_LED Clock */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1, ENABLE);
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
}
void GPIOInit(){
  
  GPIO_InitTypeDef  GPIO_InitStructure;
   
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  //USART3 PB10(TX) and PB11(RX)
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB,&GPIO_InitStructure);
  
}


/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
