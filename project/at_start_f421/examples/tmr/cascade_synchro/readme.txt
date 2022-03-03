/**
  **************************************************************************
  * @file     readme.txt
  * @version  v2.0.4
  * @date     2022-02-11
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to
  synchronize tmr peripherals in cascade mode. three timers are used:

  timers synchronisation in cascade mode:
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

  set-up
  connect the following pins to an oscilloscope to monitor the different waveforms:
  - tmr1 ch1  (pa.08)
  - tmr3 ch1  (pa.06)
  - tmr15 ch1 (pa.02) 
