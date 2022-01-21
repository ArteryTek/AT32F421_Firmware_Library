/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.3
  * @date     2021-12-31
  * @brief    readme
  **************************************************************************
  */

  this example shows how to configure comparator's glitch filter, using tmr1 output(pa8)
  pwm(freq = 468.7khz, period = 2.1333us, duty = 50%), connect pa8 to cmp_input(pa1).
  and then observe the cmp_output(pa0) through oscilloscope, it should have a 
  similar frequency to pwm, but the duty cycle is only 25%. because 64 pclk(~0.5333us)
  have been filtered out.
