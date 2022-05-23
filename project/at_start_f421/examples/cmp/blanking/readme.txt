/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.6
  * @date     2022-05-20
  * @brief    readme
  **************************************************************************
  */

  this example shows how to configure comparator's blanking source, the tmr1_ch4 is
  anded with the comparator output to provide the wanted comparator output. 
  tmr1_ch4(pa11) output 3khz and duty 25% pwm, input an external signal(100khz)
  to comparator non-inveting channel(pa1). when tmr1_ch4 pwm level is high, 
  comparator output won't be change.for more detailed information. please refer 
  to the application note document AN0070.
