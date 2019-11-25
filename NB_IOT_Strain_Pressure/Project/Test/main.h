/**
  ******************************************************************************
  * @file    Project/STM32L1xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.2.0
  * @date    16-May-2014
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
**/
  
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32l1xx.h"

typedef enum
{
	PRESS_HANDLE_STA=0,
	STRAIN_HANDLE_STA,
	UPLOAD_HANDLE_STA,
	BAT_HANDLE_STA
}main_data_type;

extern main_data_type g_sta;


//every loop around 10ms,send in 1th loop,receive in 101th loop
#define MAIN_LOOP_UPLOAD_NUM 1 
#define MAIN_LOOP_RECEIVE_NUM 101

void main_sta_judge(void);
void main_handle(void);
void main_init(void);

#endif 


