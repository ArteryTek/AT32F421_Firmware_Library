/**
  **************************************************************************
  * @file     main.c
  * @version  v2.0.1
  * @date     2021-11-26
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
  
/** @addtogroup 421_USART_tx_rx_swap USART_tx_rx_swap
  * @{
  */

#define COUNTOF(a)                       (sizeof(a) / sizeof(*(a)))
#define USART2_TX_BUFFER_SIZE            (COUNTOF(usart2_tx_buffer) - 1)
#define USART1_TX_BUFFER_SIZE            (COUNTOF(usart1_tx_buffer) - 1)

uint8_t usart2_tx_buffer[] = "usart transfer by interrupt: usart2 -> usart1 using interrupt";
uint8_t usart1_tx_buffer[] = "usart transfer by interrupt: usart1 -> usart2 using interrupt";
uint8_t usart2_rx_buffer[USART1_TX_BUFFER_SIZE];
uint8_t usart1_rx_buffer[USART2_TX_BUFFER_SIZE];
uint8_t usart2_tx_counter = 0x00;
uint8_t usart1_tx_counter = 0x00;
uint8_t usart2_rx_counter = 0x00; 
uint8_t usart1_rx_counter = 0x00;
uint8_t usart2_tx_buffer_size = USART2_TX_BUFFER_SIZE;
uint8_t usart1_tx_buffer_size = USART1_TX_BUFFER_SIZE;

/**
  * @brief  config usart2 and usart1   
  * @param  none
  * @retval none
  */
void usart_configuration(void)
{
  gpio_init_type gpio_init_struct;
  
  /* enable the usart2 and gpio clock */
  crm_periph_clock_enable(CRM_USART2_PERIPH_CLOCK, TRUE);  
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  
  /* enable the usart1 and gpio clock */  
  crm_periph_clock_enable(CRM_USART1_PERIPH_CLOCK, TRUE);  
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);    

  gpio_default_para_init(&gpio_init_struct);
  
  /* configure the usart2 tx/rx pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_2 | GPIO_PINS_3;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* config usart2 iomux */
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE2, GPIO_MUX_1); 
  gpio_pin_mux_config(GPIOA, GPIO_PINS_SOURCE3, GPIO_MUX_1);
  
  /* configure the usart1 tx/rx pin */
  gpio_init_struct.gpio_pins = GPIO_PINS_6 | GPIO_PINS_7;
  gpio_init(GPIOB, &gpio_init_struct);

  /* config usart1 iomux */
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE6, GPIO_MUX_0); 
  gpio_pin_mux_config(GPIOB, GPIO_PINS_SOURCE7, GPIO_MUX_0);
    
  /* configure usart2 param */
  usart_init(USART2, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_parity_selection_config(USART2, USART_PARITY_NONE);
  usart_transmitter_enable(USART2, TRUE);
  usart_receiver_enable(USART2, TRUE);
  usart_transmit_receive_pin_swap(USART2, TRUE);
  usart_enable(USART2, TRUE);
  
  /* configure usart1 param */
  usart_init(USART1, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_parity_selection_config(USART1, USART_PARITY_NONE);
  usart_transmitter_enable(USART1, TRUE);
  usart_receiver_enable(USART1, TRUE);
  usart_enable(USART1, TRUE);  
} 

/**
  * @brief  compares two buffers.
  * @param  pbuffer1, pbuffer2: buffers to be compared.
  * @param  buffer_length: buffer's length
  * @retval 1: pbuffer1 identical to pbuffer2
  *         0: pbuffer1 differs from pbuffer2
  */
uint8_t buffer_compare(uint8_t* pbuffer1, uint8_t* pbuffer2, uint16_t buffer_length)
{
  while(buffer_length--)
  {
    if(*pbuffer1 != *pbuffer2)
    {
      return 0;
    }
    pbuffer1++;
    pbuffer2++;
  }
  return 1;
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
  usart_configuration();
  
  /* wait until end of transmission from usart2 to usart1 */
  while(usart1_rx_counter < usart2_tx_buffer_size)
  {
    while(usart_flag_get(USART2, USART_TDBE_FLAG) == RESET);
    usart_data_transmit(USART2, usart2_tx_buffer[usart2_tx_counter++]);
    
    while(usart_flag_get(USART1, USART_RDBF_FLAG) == RESET);
    usart1_rx_buffer[usart1_rx_counter++] = usart_data_receive(USART1);
  }
  
  /* wait until end of transmission from usart1 to usart2 */
  while(usart2_rx_counter < usart1_tx_buffer_size)
  {
    while(usart_flag_get(USART1, USART_TDBE_FLAG) == RESET);
    usart_data_transmit(USART1, usart1_tx_buffer[usart1_tx_counter++]);
    
    while(usart_flag_get(USART2, USART_RDBF_FLAG) == RESET);
    usart2_rx_buffer[usart2_rx_counter++] = usart_data_receive(USART2);
  }
  
  while(1)
  { 
    /* compare data buffer */ 
    if(buffer_compare(usart2_tx_buffer, usart1_rx_buffer, USART2_TX_BUFFER_SIZE) && \
       buffer_compare(usart1_tx_buffer, usart2_rx_buffer, USART1_TX_BUFFER_SIZE))
    {
      at32_led_toggle(LED2);
      at32_led_toggle(LED3);
      at32_led_toggle(LED4);      
      delay_sec(1);
    }
  }
}

/**
  * @}
  */ 

/**
  * @}
  */ 
