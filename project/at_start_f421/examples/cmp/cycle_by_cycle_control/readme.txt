/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this example shows how to use the comparator's cycle-by-cycle current control
  function, and inernally connected to the tmr1 channel clear. connect the current
  sensor to the cmp_in(pa1), the cmp_out(pa0) will output the result. when the
  cmp_out is low level, tmr1_ch1(pa8) will output PWM; when the cmp_out is high level,
  the PWM of tmr1_ch1 will be cleared. for more detailed information. please refer
  to the application note document AN0070.
