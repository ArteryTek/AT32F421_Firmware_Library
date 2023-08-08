/**
  **************************************************************************
  * @file     readme.txt 
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, show the receiver
  mute mode in usart1.
  - the usart1 config own match id and then enable receiver mute.
  - the usart2 transmit string data buf(include match id, error id and some data). 
  - if usart1 only receive match id and the data after match id correctly, the
    three leds will blink.
 
  set-up
  - connect usart2 tx pin (pa2) -> usart1 rx pin (pb7)

  for more detailed information. please refer to the application note document AN0099.