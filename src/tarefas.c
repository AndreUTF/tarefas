#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

typedef struct // STRUCT
{
    uint8_t LED;   // LED
    uint16_t Time; //Blinking Time
} LEDTime; // Define o nome do novo tipo criado


osThreadId_t thread1_id, thread2_id, thread3_id, thread4_id;

void thread1(void *arg){
  uint8_t state = 0;
  
  while(1){
    state ^= LED1;
    LEDWrite(LED1, state);
    osDelay(100);
  } // while
} // thread1

void thread2(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    tick = osKernelGetTickCount();
    
    state ^= LED2;
    LEDWrite(LED2, state);
    
    osDelayUntil(tick + 100);
  } // while
} // thread2

//LED as an argument - Exercise 2 Item 5
void thread3(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    tick = osKernelGetTickCount();
    uint8_t LED = (uint8_t)arg;
    state ^= LED;
    LEDWrite(LED, state);
    osDelay(100);
  } // while
} // thread3

//LED and time as an arguments - Exercise 2 Item 6
void thread4(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  LEDTime* foo = (LEDTime*)arg;
  while(1){
    tick = osKernelGetTickCount();
    uint8_t LED = foo->LED;
    state ^= LED;
    LEDWrite(LED, state);
    
    osDelayUntil(tick + foo->Time);
  } // while4

} // thread4

//LED and time as an arguments using oSDelay() -  Exercise 2 Item 7
void thread5(void *arg){
  uint8_t state = 0;
  LEDTime* foo;
  while(1){
    foo = (LEDTime*)arg;
    uint8_t LED = foo->LED;
    state ^= LED;
    LEDWrite(LED, state);
    osDelay(foo->Time);
  } // while5

} // thread5

void main(void){
  LEDInit(LED1);
  LEDInit(LED2);
  LEDInit(LED3);
  LEDInit(LED4);
  LEDTime l1, l2, l3, l4;
  l1.LED = LED1;
  l1.Time = 200;
  
  l2.LED = LED2;
  l2.Time = 300;
  
  l3.LED = LED3;
  l3.Time = 500;

  l4.LED = LED4;
  l4.Time = 700;
  
  osKernelInitialize();
  
  //LED and time as an arguments
  thread1_id = osThreadNew(thread5,(void *)&l1, NULL);
  thread2_id = osThreadNew(thread5,(void *)&l2, NULL);
  thread3_id = osThreadNew(thread5,(void *)&l3, NULL);
  thread4_id = osThreadNew(thread5,(void *)&l4, NULL);
  
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main
