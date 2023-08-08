/**
  **************************************************************************
  * @file     readme.txt
  * @brief    readme
  **************************************************************************
  */

  this demo is based on the at-start board, in this demo, shows how to use
  dma recieve data. spi2 use jtag pin as spi pin,and config spi in hardware
  cs mode. after download the code,please disconnect the pa13 and pa14 from
  downloader to make sure communication is ok.
  the pins connection as follow:
  - spi2 slaver        spi1 master
  - pa15     <--->     pa4
  - pb3      <--->     pa5
  - pa13     <--->     pa6
  - pa14     <--->     pa7

  for more detailed information. please refer to the application note document AN0102.