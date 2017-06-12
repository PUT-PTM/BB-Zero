#ifndef _Handlers_
#define _Handlers_
#include "inits.h"

float kp;
float ki;
float kd;
float target_position;
int k;
char p[5];
int w,g,o;

void interpreter();
void USART3_IRQHandler(void);
#endif
