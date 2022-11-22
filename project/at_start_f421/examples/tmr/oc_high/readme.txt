/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to configur
  the tmr peripheral in output high mode with the corresponding interrupt requests
  for each channel.

  tmr3 configuration:
  tmr3clk = system_core_clock / 4,
  the objective is to get tmr3 counter clock at 1 khz:
  - prescaler = (tim3clk / tmr3 counter clock) - 1
  and generate 4 signals with 4 different delays:
  tmr3_ch1 delay = c1dt_val/tmr3 counter clock = 1000 ms
  tmr3_ch2 delay = c2dt_val/tmr3 counter clock = 500 ms
  tmr3_ch3 delay = c3dt_val/tmr3 counter clock = 250 ms
  tmr3_ch4 delay = c4dt_val/tmr3 counter clock = 125 ms

  connect the following pins to an oscilloscope to monitor the different waveforms:
  - pa0
  - pa1
  - pa2
  - pa3

  for more detailed information. please refer to the application note document AN0085.
