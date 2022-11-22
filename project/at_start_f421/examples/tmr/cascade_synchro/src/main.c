/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */

#include "at32f421_board.h"
#include "at32f421_clock.h"

/** @addtogroup AT32F421_periph_examples
  * @{
  */

/** @addtogroup 421_TMR_cascade_synchro TMR_cascade_synchro
  * @{
  */

gpio_init_type gpio_init_struct = {0};
tmr_output_config_type tmr_output_struct;

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  at32_board_init();

  /* turn led2/led3/led4 on */
  at32_led_on(LED2);
  at32_led_on(LED3);
  at32_led_on(LED4);

  /* enable tmr1/tmr3/tmr15/gpioa/gpiob clock */
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_TMR3_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_TMR15_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  /* timer1/timer3/timer15 output pin Configuration */
  gpio_init_struct.gpio_pins = GPIO_PINS_8 | GPIO_PINS_6;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_2;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE8, GPIO_MUX_2);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE6, GPIO_MUX_1);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE2, GPIO_MUX_0);

  /* timers synchronisation in cascade mode
   1/tmr1 is configured as primary timer:
   - pwm mode is used
   - the tmr1 update event is used as trigger output

   2/tmr3 is sub for tmr1 and primary for tmr15,
   - pwm mode is used
   - the itr0(tmr1) is used as input trigger
   - gated mode is used, so start and stop of sub counter
    are controlled by the master trigger output signal(tmr3 update event).
    - the tmr1 update event is used as trigger output.

    3/tmr15 is slave for tmr3,
   - pwm mode is used
   - the itr1(tmr15) is used as input trigger
   - gated mode is used, so start and stop of sub counter
    are controlled by the master trigger output signal(tmr15 overflow event).

   *  the timxclk is fixed to 240 mhz, the tmr2 counter clock is 120 mhz.

    the primary timer tmr1 is running at tmr1 frequency :
    tim1 frequency = (tmr1 counter clock)/ (tmr1 period + 1) = 1 mhz
    and the duty cycle = tmr1_c1dt/(tmr1_pr + 1) = 25%.

    the tmr3 is running at:
    (tmr3 frequency)/ (tmr3 period + 1) = 83.33 khz and a duty cycle equal
    to tmr3_c1dt/(tmr3_pr + 1) = 25%

    the tmr15 is running at:
    (tmr15 frequency)/ (tmr15 period + 1) = 6.94 khz and a duty cycle equal
    to tmr15_c1dt/(tmr15_pr + 1) = 25%
  */

  tmr_base_init(TMR1, 119, 0);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);

  tmr_base_init(TMR3, 11, 0);
  tmr_cnt_dir_set(TMR3, TMR_COUNT_UP);

  tmr_base_init(TMR15, 11, 0);
  tmr_cnt_dir_set(TMR15, TMR_COUNT_UP);

  /* channelx Configuration in output mode */
  tmr_output_default_para_init(&tmr_output_struct);
  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_output_struct.oc_output_state = TRUE;
  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_LOW;
  tmr_output_struct.oc_idle_state = TRUE;
  tmr_output_struct.occ_output_state = FALSE;
  tmr_output_struct.occ_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_output_struct.occ_idle_state = FALSE;

  /* timer1 channel 1 */
  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, 29);

  /* timer1 select enable signal to sub timer */
  tmr_primary_mode_select(TMR1, TMR_PRIMARY_SEL_OVERFLOW);
  tmr_sub_sync_mode_set(TMR1, TRUE);

  /* timer1 channel 1 */
  tmr_output_channel_config(TMR3, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
  tmr_channel_value_set(TMR3, TMR_SELECT_CHANNEL_1, 3);

  /* sub mode selection: tmr3  */
  tmr_sub_mode_select(TMR3, TMR_SUB_HANG_MODE);
  tmr_trigger_input_select(TMR3, TMR_SUB_INPUT_SEL_IS0);

  /* timer3 select enable signal to sub timer */
  tmr_primary_mode_select(TMR3, TMR_PRIMARY_SEL_OVERFLOW);
  tmr_sub_sync_mode_set(TMR3, TRUE);

  /* timer15 channel 1 */
  tmr_output_channel_config(TMR15, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
  tmr_channel_value_set(TMR15, TMR_SELECT_CHANNEL_1, 3);

  /* sub mode selection: tmr15 */
  tmr_sub_mode_select(TMR15, TMR_SUB_HANG_MODE);
  tmr_trigger_input_select(TMR15, TMR_SUB_INPUT_SEL_IS1);

  /* tmr1/tmr15 output enable */
  tmr_output_enable(TMR1, TRUE);
  tmr_output_enable(TMR15, TRUE);

  /* enable tmr1/tmr3/tmr15 */
  tmr_counter_enable(TMR1, TRUE);
  tmr_counter_enable(TMR3, TRUE);
  tmr_counter_enable(TMR15, TRUE);

  while(1)
  {
  }
}

/**
  * @}
  */

/**
  * @}
  */
