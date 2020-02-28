#include "stm32f10x.h"
#include "LCD.h"

void LCD_PulseEnablePin(int);
void delayMS(int);
void LCD_sendNibble(uint8_t,int);
void LCD_sendCommand(uint8_t,int);
//void LCD_sendByte(uint8_t, int,int);


int CMD  = 0;
int DATA = 1;


void LCD_Init(int chip)
{
  delayMS(40);
  GPIOC->BSRR = GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0;
  GPIOC->BSRR = GPIO_Pin_13;
  GPIOC->BSRR = GPIO_Pin_6;
  GPIOC->BSRR = GPIO_Pin_7;
  GPIOC->BSRR = GPIO_Pin_5;
  
  GPIOC->BRR =  GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0;
  GPIOC->BRR = GPIO_Pin_13;
  GPIOC->BRR = GPIO_Pin_6;
  GPIOC->BRR = GPIO_Pin_7;
  GPIOC->BRR = GPIO_Pin_5;

  if (chip == 1){
    
    LCD_sendNibble(0x03,1);
    delayMS(5);
    LCD_sendNibble(0x03,1);
    delayMS(5);
    LCD_sendNibble(0x03,1);
    delayMS(5);
    //set 4bit interface
    LCD_sendNibble(0x02,1);
    //delayMS(2);
    //sending function cmd and two line display
    LCD_sendCommand(0x28,1);
    delayMS(2);
    //sending display cmd,display on and cursor on
    LCD_sendCommand(0x0F,1);
    delayMS(2);
    //sending entry mode cmd and cursor increment
    LCD_sendCommand(0x06,1);
    //delayMS(1);
    //delayMS(1000);
    LCD_clearScreen(1);
  
  }
  if (chip == 2){
    
    LCD_sendNibble(0x03,2);
    delayMS(5);
    LCD_sendNibble(0x03,2);
    delayMS(5);
    LCD_sendNibble(0x03,2);
    delayMS(5);
    //set 4bit interface
    LCD_sendNibble(0x02,2);
    //delayMS(2);
    //sending function cmd and two line display
    LCD_sendCommand(0x28,2);
    delayMS(2);
    //sending display cmd,display on and cursor on
    LCD_sendCommand(0x0F,2);
    delayMS(2);
    //sending entry mode cmd and cursor increment
    LCD_sendCommand(0x06,2);
    //delayMS(1);
    //delayMS(1000);
    LCD_clearScreen(2);
  
  }
  
 
}

void LCD_printString(uint8_t *Str,int chip){
  uint8_t *c;
  c=Str;
  while((c!=0)&& (*c!=0)){
    LCD_sendByte(*c,1,chip);
    c++;
  }
  
}

void LCD_setCursorPosition(uint8_t row, uint8_t col,int chip){
  uint8_t address=0x00;
  if (row==0){
    address=0;
  }else if(row==1){
    address=0x40;
  }
  address|=col;
  LCD_sendCommand(0x80|address,chip);
}


void LCD_CtrlLinesConfig(void){
  
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

  /* Configure NCS in Output Push-Pull mode -Enable Pin(E1)*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
   /* Configure NCS in Output Push-Pull mode -Enable Pin(E2)*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  
   /* Configure NWR(RNW),in Output Push-Pull mode -R/W*///read or write
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* Configure RS in Output Push-Pull mode -Reset Pin*///Data /Instruction
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  
  
  //PC0 for LCD pin4(PD4)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //PC1 for LCD pin3(PD5)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //PC2 for LCD pin2(PD6)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  //PC3 for LCD pin1(PD7)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
}

void LCD_clearScreen(int chip){
  LCD_sendCommand (0x01, chip);
  LCD_sendCommand (0x02,chip);
 // LCD_sendCommand (0x03,chip);
}

void LCD_sendCommand (uint8_t cmdValue,int chip){
  uint8_t cmdValue1=cmdValue;
  LCD_sendByte(cmdValue, CMD,chip);
}

void LCD_sendByte(uint8_t cmdValue,int CMD_DATA,int chip){
  
  if ( CMD_DATA == 0){
    GPIOC->BRR = GPIO_Pin_7;
  }else{
    GPIOC->BSRR = GPIO_Pin_7;
  }
  //sending high nibble
  LCD_sendNibble(((cmdValue & 0xF0)>> 4),chip);
  //sending low nibble
  LCD_sendNibble((cmdValue & 0x0F),chip);
}

void LCD_sendNibble(uint8_t nibble,int chip){
  
  GPIOC->ODR &=~(GPIO_Pin_3|GPIO_Pin_2|GPIO_Pin_1|GPIO_Pin_0);
  GPIOC->ODR |=nibble;
    
  LCD_PulseEnablePin(chip);
}



void LCD_PulseEnablePin(int chip){
 
  if (chip==1){
    GPIOC ->BRR = GPIO_Pin_13;
    delayMS(1);
    GPIOC ->BSRR = GPIO_Pin_13;
    delayMS(1);
    GPIOC ->BRR = GPIO_Pin_13;
    delayMS(1);
  }else if(chip==2) {
    GPIOC ->BRR = GPIO_Pin_5;
    delayMS(1);
    GPIOC ->BSRR = GPIO_Pin_5;
    delayMS(1);
    GPIOC ->BRR = GPIO_Pin_5;
    delayMS(1); 
  }
}

void delayMS(int ms)
  {
    TIM_SetCounter(TIM2,0);
    TIM_Cmd(TIM2,ENABLE);
    while(TIM_GetCounter(TIM2)<ms){
    }
    TIM_Cmd(TIM2,DISABLE);
  }

void delayMicroSec(){
  
}

