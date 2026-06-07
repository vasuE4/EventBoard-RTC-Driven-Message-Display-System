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
