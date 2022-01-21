/**
  **************************************************************************
  * @file     main.c
  * @version  v2.0.3
  * @date     2021-12-31
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

/** @addtogroup 421_CMP_output CMP_output
  * @{
  */

gpio_init_type gpio_init_struct = {0};
__IO uint32_t capture = 0;
__IO uint32_t tmr1freq;
extern __IO uint16_t capturenumber;
extern __IO uint32_t ic1readvalue1;
extern __IO uint32_t ic1readvalue2;

/**
  * @brief  configures cmp
  * @param  none
  * @retval none
  */
void cmp_config(void)
{
  cmp_init_type cmp_init_struct;
  /* gpioa peripheral clock enable */
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  /* configure pa1: pa1 is used as cmp1 non inveting input */
  gpio_init_struct.gpio_pins = GPIO_PINS_1;
  gpio_init_struct.gpio_mode = GPIO_MODE_ANALOG;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_init_struct.gpio_pins = GPIO_PINS_0;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE0, GPIO_MUX_7);

  /* cmp peripheral clock enable */
  crm_periph_clock_enable(CRM_CMP_PERIPH_CLOCK, TRUE);

  /* cmp1 init: pa1 is used cmp1 inverting input */
  cmp_default_para_init(&cmp_init_struct);
  cmp_init_struct.cmp_inverting = CMP_INVERTING_1_4VREFINT;
  cmp_init_struct.cmp_output = CMP_OUTPUT_TMR1CH1;
  cmp_init_struct.cmp_polarity = CMP_POL_NON_INVERTING;
  cmp_init_struct.cmp_speed = CMP_SPEED_FAST;
  cmp_init_struct.cmp_hysteresis = CMP_HYSTERESIS_NONE;
  cmp_init(CMP1_SELECTION, &cmp_init_struct);

  cmp_scal_brg_config(CMP_SCAL_BRG_11);

  /* enable cmp1 */
  cmp_enable(CMP1_SELECTION, TRUE);
}

/**
  * @brief  configures tmr1_ch1 as input
  * @param  none
  * @retval none
  */
void tmr1_init(void)
{
  tmr_input_config_type tmr_ic_init_structure;

  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);

  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE8, GPIO_MUX_2);

  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_init_struct);

  nvic_irq_enable(TMR1_CH_IRQn, 0, 1);

  /* tmr1 time base configuration */
  tmr_base_init(TMR1, 0xffff, 0);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);
  tmr_clock_source_div_set(TMR1, TMR_CLOCK_DIV1);

  /* channel 4 configuration in pwm mode */
  tmr_input_default_para_init(&tmr_ic_init_structure);
  tmr_ic_init_structure.input_channel_select = TMR_SELECT_CHANNEL_1;
  tmr_ic_init_structure.input_filter_value = 0x00;
  tmr_ic_init_structure.input_mapped_select = TMR_CC_CHANNEL_MAPPED_DIRECT;
  tmr_ic_init_structure.input_polarity_select = TMR_INPUT_RISING_EDGE;
  tmr_input_channel_init(TMR1, &tmr_ic_init_structure, TMR_CHANNEL_INPUT_DIV_1);
  tmr_interrupt_enable(TMR1, TMR_C1_INT, TRUE);

  /* tmr1 counter enable */
  tmr_counter_enable(TMR1, TRUE);

  /* reset the flags */
  TMR1->ists &= 0 ;
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  system_clock_config();

  at32_board_init();

  uart_print_init(115200);

  /* cmp1 configuration */
  cmp_config();

  /* tmr1 configuration in input capture mode */
  tmr1_init();

  printf("cmp output test..\r\n");

  while (1)
  {
    if(capturenumber > 1)
    {
      /* capture computation */
      if (ic1readvalue2 > ic1readvalue1)
      {
        capture = (ic1readvalue2 - ic1readvalue1);
      }
      else
      {
        capture = ((0xffff - ic1readvalue1) + ic1readvalue2);
      }
      /* frequency computation */
      tmr1freq = (uint32_t)system_core_clock / capture;
      printf("f1=%d\r\n",tmr1freq);
      capturenumber=0;
    }
  }
}

/**
  * @}
  */

/**
  * @}
  */
