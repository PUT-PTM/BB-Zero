# BB-Zero Project

## Overview
  This is code repository for our balancing robot. It uses complementary filter for reading values and PID regulator calculating output.  
 
## Description

  ### Robot's parts:
  * STM32F407 DISCOVERY with this code
  * MPU6050 for reading angle of robot 
  * XM15B bluetooth module to communicate
  * 2 wheels 65x26mm
  * 2 motors - 5V, 80obr/min, 48:1

## Tools

  * [CooCox CoIDE](http://www.coocox.org/software/coide.php)
  * [MPU6050 library]()
  * [PID regulator library]()

## How to run

  To get this program on your STM you need CooCox CoFlash or CooCox CoIDE environment where flash programming is included

## How to compile

  Everything you need for compiling is CoIDE (available from link above), import this project, compile and send to flash of STM

## Future improvements

  There's nothing we want to change in this project except code readibility

## Attributions

  See [Tools](#tools)

## License

  MIT

## Credits

  Oskar Rutkowski, Patryk Mroczyński, Jakub Wiśniewski
  
## Notice

  The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology.
  Supervisor: Tomasz Mańkowski
