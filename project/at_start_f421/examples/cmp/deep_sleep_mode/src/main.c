/**
  **************************************************************************
  * @file     main.c
  * @version  v2.0.8
  * @date     2022-08-16
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

/** @addtogroup 421_CMP_deep_sleep_mode CMP_deep_sleep_mode
  * @{
  */

gpio_init_type gpio_init_struct = {0};
__IO uint32_t capture = 0;
__IO uint32_t tmr1freq;
extern __IO uint16_t capturenumber;
extern __IO uint32_t ic1readvalue1;
extern __IO uint32_t ic1readvalue2;

void cmp_exint_init(void)
{
  exint_init_type exint_init_struct;

  exint_default_para_init(&exint_init_struct);
  exint_init_struct.line_enable = TRUE;
  exint_init_struct.line_mode = EXINT_LINE_INTERRUPUT;
  exint_init_struct.line_select = EXINT_LINE_21;
  exint_init_struct.line_polarity = EXINT_TRIGGER_FALLING_EDGE;
  exint_init(&exint_init_struct);

  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(ADC1_CMP_IRQn, 1, 0);
}

/**
  * @brief  configures cmp: pa1 to cmp inverting input
  *         and cmp output to tmr1 ch1.
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

  gpio_init_struct.gpio_pins = GPIO_PINS_6;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE6, GPIO_MUX_7);

  /* cmp peripheral clock enable */
  crm_periph_clock_enable(CRM_CMP_PERIPH_CLOCK, TRUE);

  /* cmp1 init: pa1 is used cmp1 inverting input */
  cmp_default_para_init(&cmp_init_struct);
  cmp_init_struct.cmp_inverting = CMP_INVERTING_VREFINT;
  cmp_init_struct.cmp_output = CMP_OUTPUT_NONE;
  cmp_init_struct.cmp_polarity = CMP_POL_NON_INVERTING;
  cmp_init_struct.cmp_speed = CMP_SPEED_FAST;
  cmp_init_struct.cmp_hysteresis = CMP_HYSTERESIS_NONE;
  cmp_init(CMP1_SELECTION, &cmp_init_struct);

  cmp_scal_brg_config(CMP_SCAL_BRG_11);

  /* enable cmp1 */
  cmp_enable(CMP1_SELECTION, TRUE);

  cmp_exint_init();
}

void sysclk_config_deep_sleep(void)
{
  /* enable external high-speed crystal oscillator - hext */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_HEXT, TRUE);

  /* wait till hext is ready */
  while(crm_hext_stable_wait() == ERROR);

  /* enable pll */
  crm_clock_source_enable(CRM_CLOCK_SOURCE_PLL, TRUE);

  /* wait till pll is ready */
  while(crm_flag_get(CRM_PLL_STABLE_FLAG) == RESET);

  /* enable auto step mode */
  crm_auto_step_mode_enable(TRUE);

  /* select pll as system clock source */
  crm_sysclk_switch(CRM_SCLK_PLL);

  /* wait till pll is used as system clock source */
  while(crm_sysclk_switch_status_get() != CRM_SCLK_PLL);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
  __IO uint32_t index = 0;

  /* enable pwc and bpr clock */
  crm_periph_clock_enable(CRM_PWC_PERIPH_CLOCK, TRUE);

  /* congfig the voltage regulator mode.only used with deep sleep mode */
  pwc_voltage_regulate_set(PWC_REGULATOR_EXTRA_LOW_POWER);

  system_clock_config();

  at32_board_init();

  uart_print_init(115200);

  /* cmp1 configuration */
  cmp_config();

  while (1)
  {
    printf("\r\nenter deep sleep mode...\r\n");
    /* enter deep sleep mode */
    pwc_deep_sleep_mode_enter(PWC_DEEP_SLEEP_ENTER_WFI);

    /* wait clock stable */
    for(index = 0; index < 600; index++)
    {
      __NOP();
    }

    sysclk_config_deep_sleep();
    printf("\r\nwakeup from deep_sleep mode by cmp, mcu is running...\r\n");
    delay_sec(1);
  }
}

/**
  * @}
  */

/**
  * @}
  */
