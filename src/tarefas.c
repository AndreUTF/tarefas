#include "system_tm4c1294.h" // CMSIS-Core
#include "driverleds.h" // device drivers
#include "cmsis_os2.h" // CMSIS-RTOS

typedef struct // STRUCT
{
    uint8_t LED;   // LED
    uint8_t time;    // time
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

//LED as an argument
void thread3(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  
  while(1){
    tick = osKernelGetTickCount();
    uint8_t LED = (uint8_t)arg;
    state ^= LED;
    LEDWrite(LED, state);
    
    osDelayUntil(tick + 100);
  } // while
} // thread3

//LED and time as an arguments
void thread4(void *arg){
  uint8_t state = 0;
  uint32_t tick;
  LEDTime* foo = (LEDTime*)arg;
  while(1){
    tick = osKernelGetTickCount();
    uint8_t LED = foo->LED;
    state ^= LED;
    LEDWrite(LED, state);
    
    osDelayUntil(tick + foo->time);
  } // while3

} // thread
void main(void){
  LEDInit(LED4 | LED3 | LED2 | LED1);
  
  LEDTime led1t1;
  LEDTime led2t2;
  LEDTime led3t3;
  LEDTime led4t4;
  led1t1.LED = LED1;
  led1t1.time = 200;
  
  led2t2.LED = LED2;
  led2t2.time = 300;
  
  led3t3.LED = LED3;
  led3t3.time = 500;
  
  led4t4.LED = LED4;
  led4t4.time = 700;
  
  osKernelInitialize();
  
  //LED as an argument
  //thread1_id = osThreadNew(thread3, (void *)LED1, NULL);
  //thread2_id = osThreadNew(thread3, (void *)LED2, NULL);
  
  //LED and time as an arguments
  thread1_id = osThreadNew(thread4, (void *)&led1t1, NULL);
  thread2_id = osThreadNew(thread4, (void *)&led2t2, NULL);
  thread3_id = osThreadNew(thread4, (void *)&led3t3, NULL);
  thread4_id = osThreadNew(thread4, (void *)&led4t4, NULL);
  
  if(osKernelGetState() == osKernelReady)
    osKernelStart();

  while(1);
} // main
