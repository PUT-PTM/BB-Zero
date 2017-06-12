# BB-Zero Project

## Overview
  BB-Zero Project is about balancing robot. It uses widely known techniques to calculate angle and output to make it balance
  
## Description

  Robot uses complementary filter for reading values and PID regulator. The robot is able to interpret commands via bluetooth to change PID values the target of balance to make the robot move.

  ### Robot's parts:
  * STM32F407 DISCOVERY
  * MPU6050 for reading angle of robot 
  * XM15B bluetooth module to communicate
  * 2 wheels 65x26mm
  * 2 motors - 5V, 80rpm

## Tools

  * [CooCox CoIDE](http://www.coocox.org/software/coide.php)
  * [ST-LINK](http://www.st.com/en/development-tools/st-link-v2.html)
  * [STM firmware upgrade](http://www.st.com/en/evaluation-tools/stm32f4discovery.html)

## How to run

  To get this program on your STM you need CooCox CoFlash or CooCox CoIDE environment where flash programming is included

## How to compile

  Everything you need for compiling is CoIDE (available from link above) to compile code and send it to flash of STM

## Future improvements

  - Improve commands interpreter
  - Android app to easily control the robot
  - Better cover parts

## Attributions

  * [MPU6050 library](https://github.com/MaJerle/stm32f429/tree/master/00-STM32F429_LIBRARIES)
  * [Angle calculations library (ahrs_imu)](https://github.com/MaJerle/stm32fxxx_hal_libraries/tree/master/00-STM32_LIBRARIES)

## License

  MIT

## Credits

  Oskar Rutkowski, Patryk Mroczyński, Jakub Wiśniewski
  
## Notice

  The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology.
  Supervisor: Tomasz Mańkowski
