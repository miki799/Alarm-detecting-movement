# Alarm-detecting-movement

Alarm with light sensor working as motion detector. Appliction created for FRDM-KL05Z with ARM Cortex-M0+ processor.

Alarm has two modes:
- user - from this mode alarm is armed,
- admin - used for changing ALARM pin and deactivating blockade,

Application is controlled with 4x3 keypad. 4-digits PINs are used to control alarm. Messages are displayed on 2x16 LCD.

The alarm goes off (RED LED on) when value acquired from light sensor is lower than some fixed value. 
Turning alarm off is only possible by entering proper PIN. Device is blocked after 3 failed attempts.
Admin PIN takes down the blockade.

Used modules:
- ALS-PT19 - light sensor 
- 2x16 LCD1602 + i2c Expander PCF8574
- 4x3 keypad

Project created for Microprocessor Technology course.

