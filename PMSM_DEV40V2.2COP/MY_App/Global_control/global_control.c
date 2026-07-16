
/* ����ͷ�ļ� ----------------------------------------------------------------*/

#include "global_control.h"
#include "wk_tmr.h"
#include "wk_adc.h"
// #include "mt6816.h"
#include "ADC-front-end.h"
#include "foc_drv.h"
#include "motor_sensoruse.h"
#include "PID.h"
#include "wk_system.h"
#include "arm_math.h"
#include "thermistor.h"
#include "UART_CAN.h"
#include "luenberger_observer.h"
#include "ADRC.h"
#include "step_counter.h"

extern volatile uint16_t LedTaskTim;
extern volatile uint16_t LcdTaskTim;
extern volatile uint16_t KeyTaskTim;
extern volatile uint16_t UsartTaskTim;
uint16_t TaskTim100 = 0;
uint16_t TaskTim1 = 0;
uint16_t TaskTim1Khz = 0;
uint16_t FANTim1 = 0;

uint8_t flag2  = 1, obs_flag=0,TEMP_DELAY=0;
int FANTim1pwm=0;

/* 计步器 */
StepCounter g_stepCounter;

float obs_speed;


ADRC_HandleTypeDef ADRC;

float PID_Speed(PID_Controller *pid, float target, float feedback, float dt) ;
/**
 * ��������: ȫ�ֳ�ʼ��
 * �������:
 * ���ز���:
 * ˵    ��:
 */
void Global_Init(void)
{
  //	HAL_Delay(100);                                            //��ʱ�ȴ���Դ�ȶ�
  wk_delay_ms(5);



  adc_resolution_set(ADC1, ADC_RESOLUTION_12B);
  adc_enable(ADC1, TRUE);
  while (adc_flag_get(ADC1, ADC_RDY_FLAG) == RESET)
    ;
  /**adc calibration**/
  adc_calibration_init(ADC1);
  while (adc_calibration_init_status_get(ADC1))
    ;
  adc_calibration_start(ADC1);
  while (adc_calibration_status_get(ADC1))
    ;

  CurrentLoop_Init();



  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, 0);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_2, 0);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_3, 0);

  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_4, 4550);

  FOC_PhaseData_Calibrate(&g_foc_phase_data);

  // ESO��ʼ����dt��beta1��beta2��beta3����������b

  ADRC_Init(&ADRC,1,0.00005,18761.0f);  

  StepCounter_Init(&g_stepCounter);   /* 计步器初始化, 默认 IqLPF 模式 */

  obs_flag = 1;
}

void TMR1_OVF_TMR10_IRQHandler(void)
{
  /* add user code begin TMR1_OVF_TMR10_IRQ 0 */

  /* add user code end TMR1_OVF_TMR10_IRQ 0 */

  /* overflow interrupt management */
  if (tmr_interrupt_flag_get(TMR1, TMR_OVF_FLAG) != RESET)
  {
    /* add user code begin TMR1_TMR_OVF_FLAG */

		gpio_bits_write(GPIOF, GPIO_PINS_8, 1);


    adc_preempt_software_trigger_enable(ADC1, TRUE);
    while (adc_flag_get(ADC1, ADC_PCCE_FLAG) == RESET)
      ;
    adc_flag_clear(ADC1, ADC_PCCE_FLAG);

    g_foc_phase_data.adc_Injected[0] = adc_odt[0]; // ��ȡ�����
    g_foc_phase_data.adc_Injected[1] = adc_odt[1]; // ��ȡ�����

    FOC_phaseData_current_folat(&g_foc_phase_data);

    Sensoruse_Control();
    /* ---- 计步检测：喂入 IqLPF，10kHz 周期 ---- */
    {
        uint16_t iq_val = (uint16_t)(FocStruct.IqLPF * 100.0f);
        StepCounter_Update(&g_stepCounter, iq_val);
    }

		
		
		
    tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, FocStruct.DutyCycleA);
    tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_2, FocStruct.DutyCycleB);
    tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_3, FocStruct.DutyCycleC);

		
		
		
    TaskTim100++;
    if (TaskTim100 >= 1800)
    {

      TaskTim100 = 0;
      TaskTim1++;
			
			TEMP_DELAY=!TEMP_DELAY;
     if(TEMP_DELAY==1) g_TempProtect.current_temp = Thermistor_GetFilteredTemperature(adc_ordinary_channel(ADC2, ADC_CHANNEL_4));
			
			else GET_IPM_TEMP();
			
      Temp_Protect_Process(g_TempProtect.current_temp);

      // /* 上报计步数据到寄存器供主机读取 */
      // g_device_regs[18] = (g_stepCounter.step_count >> 8) & 0xFF;
      // g_device_regs[19] =  g_stepCounter.step_count & 0xFF;
      // g_device_regs[20] = (g_stepCounter.cadence >> 8) & 0xFF;
      // g_device_regs[21] =  g_stepCounter.cadence & 0xFF;
			
    }

    FANTim1++;
		FANTim1pwm=(g_Sensoruse.IPM_TEMP-50)*5;
		if(FANTim1pwm>=100)FANTim1pwm=100;
		if(FANTim1pwm<=0)FANTim1pwm=0;
			/* 兜底策略：电机温度超过95°C 强制风扇全速 */
			if(g_TempProtect.current_temp >= 95.0f)
			    FANTim1pwm = 100;
		
    if (FANTim1 >= FANTim1pwm)
    {

      gpio_bits_write(GPIOC, GPIO_PINS_13, 0);
    }
    else
      gpio_bits_write(GPIOC, GPIO_PINS_13, 1);


    if (FANTim1 >=100)
      FANTim1 = 0;

		gpio_bits_write(GPIOF, GPIO_PINS_8, 0);

    /* clear flag */
    tmr_flag_clear(TMR1, TMR_OVF_FLAG);
    /* add user code end TMR1_TMR_OVF_FLAG */
  }

  /* add user code begin TMR1_OVF_TMR10_IRQ 1 */

  /* add user code end TMR1_OVF_TMR10_IRQ 1 */
}

void TMR4_GLOBAL_IRQHandler(void)
{
  /* add user code begin TMR4_GLOBAL_IRQ 0 */

  // FocStruct.MCangle=(update_electrical_angle()/4095.0f)*2*PI;

	
	
//	    gpio_bits_write(GPIOF, GPIO_PINS_8, 1);
//	gpio_bits_toggle(GPIOF, GPIO_PINS_8);
//	
//	
//	
//		    gpio_bits_write(GPIOF, GPIO_PINS_8, 0);
  /* add user code end TMR4_GLOBAL_IRQ 0 */

  /* overflow interrupt management */
  if (tmr_interrupt_flag_get(TMR4, TMR_OVF_FLAG) != RESET)
  {
    /* add user code begin TMR4_TMR_OVF_FLAG */
    /* clear flag */

		
	FocStruct.speed = 1.0f * Calculate_velocity_raw(FocStruct.anglefloat, 0.0002f);

  FocStruct.speedLPF = FocStruct.speed * 0.5 + FocStruct.speedLPF * (1 - 0.5);
		
		if(obs_flag==1)update_ADRC(&ADRC, NULL, FocStruct.speed );	
	if(g_Sensoruse.RunMode==VIBRATINO_1)  FocStruct.Taspeed = -1.0f * PID_Speed(&pid_pos, update_multi_turn_position(),FocStruct.Taposition, 0.005f);

	UART_CommandWatchdog_Tick();
  g_Master();
		
		
		TaskTim1Khz++;
		if(TaskTim1Khz>=10){
		
		
		  FocStruct.Iq_ref = PID_Speed(&pid_speed, FocStruct.Taspeed, obs_speed, 0.001f);
		
		
		TaskTim1Khz=0;
		
		}

		

    tmr_flag_clear(TMR4, TMR_OVF_FLAG);
    /* add user code end TMR4_TMR_OVF_FLAG */
  }

  /* add user code begin TMR4_GLOBAL_IRQ 1 */

  /* add user code end TMR4_GLOBAL_IRQ 1 */
}

void TMR1_CH_IRQHandler(void)
{
  /* add user code begin TMR1_CH_IRQ 0 */

  /* add user code end TMR1_CH_IRQ 0 */

  /* channel4 interrupt management */
  if (tmr_interrupt_flag_get(TMR1, TMR_C4_FLAG) != RESET)
  {
    /* add user code begin TMR1_TMR_C4_FLAG */
    /* clear flag */

    tmr_flag_clear(TMR1, TMR_C4_FLAG);
    /* add user code end TMR1_TMR_C4_FLAG */
  }

  /* add user code begin TMR1_CH_IRQ 1 */

  /* add user code end TMR1_CH_IRQ 1 */
}


// ����PID������������Ե��ã�������PWM�ж��У�
float PID_Speed(PID_Controller *pid, float target, float feedback, float dt) {
    // �������
    float error = target - feedback;
    // ��������㣨�������ͣ�
    pid->integral += pid->Ki *error * dt;
    // �����޷�
    if (pid->integral >= pid->integral_limit) {
        pid->integral = pid->integral_limit;
    } else if (pid->integral <= -pid->integral_limit) {
        pid->integral = -pid->integral_limit;
    }

    // ΢������㣨��ɢ��֣�
    float derivative = (error - pid->prev_error) / dt;
    pid->prev_error = error;

    // PID��� = P + I + D
    float output = (pid->Kp * error) + ( pid->integral)+ (pid->Kd * derivative);

    // ����޷�
    if (output > pid->output_limit) {
        output = pid->output_limit;
    } else if (output < -pid->output_limit) {
        output = -pid->output_limit;
    }

    return output;
}
