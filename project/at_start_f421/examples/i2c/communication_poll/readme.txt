/**
  **************************************************************************
  * @file     communication_poll/readme.txt 
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, use hardware i2c1 
  with two board communication based on polling mode. if the communication is
  successful, led3 will turn on, if th communication fails, led2 will keep flashing.
  change the macro definition MASTER_BOARD can switch among master-transmit/
  slave-receive master-receive/slave-transmit operation.

  attention:
    1. i2c bus must pull-up
    2. press the slave button first, then press the master button to start communication.

  pin used:
    1. scl --- pb6
    2. sda --- pb7