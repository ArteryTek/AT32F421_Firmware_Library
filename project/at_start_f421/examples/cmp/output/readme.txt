/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this example shows how to configure comp to get the comparison result, and 
  inernally connected to the input channel 1 of the tmr1. by input pwm(3khz - 1mhz) 
  to the cmp_in(pa1), the cmp_out(pa0) will output the result, and tmr1_ic1 will
  also capture this cmp_out and print its pwm frequency through the usart1.
  for more detailed information. please refer to the application note document AN0070.
