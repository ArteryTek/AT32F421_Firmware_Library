/**
  **************************************************************************
  * @file     main.c
  * @version  v2.0.7
  * @date     2022-06-28
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

/** @addtogroup 421_IRTMR_output IRTMR_output
  * @{
  */

tmr_output_config_type tmr_oc_init_structure;
crm_clocks_freq_type crm_clocks_freq_struct = {0};

uint16_t ccr1_val = 249;
uint16_t ccr2_val = 24;
uint16_t prescalervalue = 0;

void crm_configuration(void);
void gpio_configuration(void);

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  /* system clocks configuration */
  crm_configuration();

  /* get system clock */
  crm_clocks_freq_get(&crm_clocks_freq_struct);

  /* gpio configuration */
  gpio_configuration();

  /* irtmr configuration */
  scfg_infrared_config(SCFG_IR_SOURCE_TMR16, SCFG_IR_POLARITY_NO_AFFECTE);

  /* compute the prescaler value */
  prescalervalue = (uint16_t) ((crm_clocks_freq_struct.apb1_freq * 2) / 1000000) - 1;

  /* TMR16 time base configuration */
  tmr_base_init(TMR16, 665, prescalervalue);
  tmr_cnt_dir_set(TMR16, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR16, TMR_CLOCK_DIV1);

  tmr_output_default_para_init(&tmr_oc_init_structure);
  tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_oc_init_structure.oc_idle_state = FALSE;
  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_oc_init_structure.oc_output_state = TRUE;
  tmr_output_channel_config(TMR16, TMR_SELECT_CHANNEL_1, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR16, TMR_SELECT_CHANNEL_1, ccr1_val);
  tmr_output_channel_buffer_enable(TMR16, TMR_SELECT_CHANNEL_1, TRUE);

  tmr_period_buffer_enable(TMR16, TRUE);

  /* TMR17 time base configuration */
  tmr_base_init(TMR17, 65, prescalervalue);
  tmr_cnt_dir_set(TMR17, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR17, TMR_CLOCK_DIV1);

  tmr_output_default_para_init(&tmr_oc_init_structure);
  tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_oc_init_structure.oc_idle_state = FALSE;
  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_oc_init_structure.oc_output_state = TRUE;
  tmr_output_channel_config(TMR17, TMR_SELECT_CHANNEL_1, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR17, TMR_SELECT_CHANNEL_1, ccr2_val);
  tmr_output_channel_buffer_enable(TMR17, TMR_SELECT_CHANNEL_1, TRUE);

  tmr_period_buffer_enable(TMR17, TRUE);

  tmr_output_enable(TMR16, TRUE);
  tmr_output_enable(TMR17, TRUE);

  /* tmr enable counter */
  tmr_counter_enable(TMR16, TRUE);
  tmr_counter_enable(TMR17, TRUE);

  while(1)
  {
  }
}

/**
  * @brief  configure the TMR16 pin and irtmr pin.
  * @param  none
  * @retval none
  */
void gpio_configuration(void)
{
  gpio_init_type gpio_init_struct;

  gpio_default_para_init(&gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_6 | GPIO_PINS_7;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE6, GPIO_MUX_5);
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE7, GPIO_MUX_5);

  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init(GPIOB, &gpio_init_struct);

  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE9, GPIO_MUX_0);
}

/**
  * @brief  configures the different system clocks.
  * @param  none
  * @retval none
  */
void crm_configuration(void)
{
  /* scfg clock enable */
  crm_periph_clock_enable(CRM_SCFG_PERIPH_CLOCK, TRUE);

  /* TMR16 clock enable */
  crm_periph_clock_enable(CRM_TMR16_PERIPH_CLOCK, TRUE);

  /* TMR17 clock enable */
  crm_periph_clock_enable(CRM_TMR17_PERIPH_CLOCK, TRUE);

  /* gpioa gpiob clock enable */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* gpiob gpiob clock enable */
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
}


/**
  * @}
  */

/**
  * @}
  */

