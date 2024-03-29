#include "stm32f10x_it.h" 

/* 50ms执行一次 */
void Timer_50ms(void)
{
}

/* 100ms执行一次 */
void Timer_100ms(void)
{
	static u8 time = 0;
	if(Motor_Start)
	{
		time ++;
		if(time>=5)
			Motor_PID_Speed(-320, 0);
		if(time>=35)
		{
			Motor_Pause();
			Motor_Start = DISABLE;
			Valve.Valve_EN = ENABLE;
			Valve.Valve_Status = CLOSE;
			time &= 0x00;
		}
	}
}

/* 200ms执行一次 */
void Timer_200ms(void)
{
}


void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}

/* 2.5ms执行一次 */
void SysTick_Handler(void)
{
	static u8 n = 0;
	static u8 EN = DISABLE ;
	static uint16_t timer1 = 0;
	static uint16_t timer2 = 0;
	
	timer1 ++;
	
	if(timer1%20 == 0)
		Timer_50ms();
	if(timer1%40 == 0)
		Timer_100ms();
	if(timer1%80 == 0)
		Timer_200ms();
	if(timer1 >= 65530)
		timer1 = 0;

//延时 1000ms	
	if(Valve.Valve_EN == ENABLE)
	{
		Valve_Control();
		timer2 ++;
		if(timer2 >= 1500)
		{
			Valve.Valve_EN = DISABLE;
		  Send_to_Arduino(SIGNAL1);
			timer2 &= 0x00;
		}
	}

	if(L2==WHITE && M0==BLACK && R2==WHITE)
		Road.Action_Mode = Action_Mode_Straight;
	else if(L1==WHITE && L2==WHITE && R2==BLACK && R1==WHITE)
		Road.Action_Mode = Action_Mode_Left;
	else if(L1==WHITE && R1==BLACK)
		Road.Action_Mode = Action_Mode_Left_Badly;
	else if(L1==WHITE && L2==BLACK && R2==WHITE && R1==WHITE)
		Road.Action_Mode = Action_Mode_Right;
	else if(L1==BLACK && R1==WHITE)
		Road.Action_Mode = Action_Mode_Right_Badly;
	else 
		Road.Action_Mode = Action_Mode_Inline;

	if(Road.Road_Status == Road_Status_ENABLE)
	{
		if(L2==BLACK && M0==BLACK && R2==BLACK )
			EN = ENABLE;
		if(EN)
		{
			if(n++ >=10)
			{
				if((R2==WHITE || L2==WHITE) && M0==BLACK)
				{
					Road.times++;
					EN = DISABLE;
					n = 0;
					if(Road.times == 3)
						Road.Action_Mode = Action_Mode_End;
				}
			}
		}
		if(Road.times < 3)
			Road_Mode();
		else
		{
			Motor_Pause();
			Road.Road_Status = Road_Status_DISABLE;
		}
	}
}


