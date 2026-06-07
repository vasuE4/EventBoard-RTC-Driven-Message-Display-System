# EventBoard-RTC-Driven-Message-Display-System
The EventBoard - RTC-Driven Message Display System is an LPC2148-based system that uses an on-chip RTC to trigger scheduled messages and scroll on a 16x2 LCD. It includes an Admin Mode by using External interrupt for time and message management via a 4x4 keypad and switch. The system uses an LM35 sensor for temperature monitoring.   
## Features   
1. Real-Time Message Scheduling.
2. Interactive Admin Mode.
3. Sensor Integration.
4. Input Handling.
## Required Components    
1. LPC2148 Board   
2. LCD(16x2)   
3. Keypad Matrix     
4. LM35  
5. On chip ADC  
6. On chip RTC  
7. Switch
## System Architecture and Component roles   
1. The Central Controller: LPC2148   
The LPC2148 manages the entire execution flow. It handles:
   * Peripheral Management: Managing the GPIO peripherals required to interface like the LCD, RTC, ADC, Keypad and LM35 sensor.
   * Logic Processing: Running the main control loop for device state management, including message scheduling and environment monitoring.
2. Data Management:
   * Keypad Matrix: Acts as the primary input interface. It provides a tactile interface for the user to navigate the system, select active messages, and edit the current time in Admin Mode.
3. User Interface:
   * LCD Module: Acts as the Visual Dashboard, providing immediate feedback to the user. It displays scheduled messages using a scrolling mechanism, the current time,date,day and room temperature.
4. Environmental Monitoring:
   * LM35 Sensor: Communicates with the controller via the on-chip ADC. This allows the system to measure and display ambient room temperature when no message is scheduled.
## Set-Up Instructions   
1. Before using the Peripherals you must initialize the peripherals by calling InitLCD(),InitKPM(),RTC_Init(),Init_ADC().   
2. Notes that you must include all the required headers like "LPC21xx.h","string.h" and user defined hearders like "lcd_defines.h","lcd.h","delay.h","types.h","defines.h","KPM.h","KPM_defines.h","ADC.h","minimain_rtc.h".
   * "lcd_defines" contains the command values and the pin connections of LCD.   
   * "lcd.h" contains the function declarations of LCD.   
   * "delay.h" contains the function declarations of delay functions according to the time of delay required.   
   * "types.h" contains the type casted details of the existing data types.
   * "defines.h" contains the macro expansions of Bit/Byte manipulation.   
   * "KPM.h" contains the function declarations of the Keypad related operations.   
   * "KPM_defines.h" contains the Pin connections of the Keypad.   
   * "ADC.h" contains the function declarations of the ADC related functions.
   * "minimain_rtc.h" contains the function declaations of the RTC related functions.
## Code Flow Execution   
Main block:   
FUNCTION main():   
    INITIALIZE peripherals (LCD, RTC, ADC, KPM)   
    ENABLE EINT0 interrupt  
    SET RTC to predefined time and date  
      
    LOOP FOREVER:  
        SET flag = 0  
        FOR i FROM (totalmsgs - 1) DOWN TO 0:  
            IF (RTC.hour == msglist[i].hour) AND   
               (RTC.min >= msglist[i].min) AND   
               (RTC.min <= msglist[i].min + 14):  
                  
                IF msglist[i].enabled == 1:  
                    COPY msglist[i].text TO buffer   
                ELSE:  
                    COPY "                   " TO buffer  
                  
                SET flag = 1  
                BREAK loop  
          
        IF flag == 1:   
            CALL scrolllcd(buffer, i)  
        ELSE:  
            GET current RTC time, date, day  
            READ ADC value  
            DISPLAY RTC info, Date, Day, and ADC on LCD  
ISR BLock:  
FUNCTION eint0_isr():  
    DISPLAY menu: "1.RTC, 2.msg, 3.ext"  
    READ option from Keypad (op)  
      
    SWITCH op:  
        CASE '1':   
            INPUT new Time (H, M, S) -> SET RTC Time  
            INPUT new Date (D, M, Y) -> SET RTC Date  
            INPUT Day -> SET RTC Day  
              
        CASE '2':   
            INPUT message Index  
            INPUT enable status (1 or 0)  
            UPDATE msglist[index].enabled  
              
        CASE '3':   
            EXIT  
              
    CLEAR interrupt flag (EXTINT)  
    RESET vector address (VICVectAddr)   
