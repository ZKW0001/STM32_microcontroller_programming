## STM32_Microcontroller_Programming

# Project overview
This project involves developing an embedded system application on an STM32 microcontroller, integrating peripherals such as a joystick, RGB LEDs, an LCD, potentiometers, and a speaker. The system is implemented as a state machine to handle various functionalities, including **clock management with the ability to set and display the current time**, **a world clock supporting multiple time zones**, **a stopwatch**, and **a countdown timer**.

User interaction is facilitated through joystick buttons with interrupts and potentiometers for setting values and navigating options, while visual and audio feedback is provided via LEDs and the speaker.

# Example output
- **Clock management with the ability to set and display the current time**  

https://github.com/user-attachments/assets/9e239847-46ab-4b06-9a5f-64263dbc824c  

The time management logic is as follows: when at the "Time" page, pressing the fire button on the joystick causes the microcontroller to read values from the two potentiometers in real time. The left potentiometer sets the hours (from 0 to 23), while the right potentiometer sets the minutes (from 0 to 59). Pressing the fire button again sets the time. Moving the joystick downward transitions the microcontroller to the next page, where the current time is displayed.  
<br>
- **World clock supporting multiple time zones**  


https://github.com/user-attachments/assets/6cd723c4-6a41-438c-ae33-6ce1cc93b758  

After the "Current Time" page, moving the joystick downward transitions the microcontroller to the next page, the "World Clock" page. In this page, the microcontroller allows the user to select a city and its corresponding time zone using the left potentiometer. Each position of the potentiometer corresponds to a predefined city with a specific offset relative to GMT, including fractional offsets like +5:30. Once the city is selected, the microcontroller calculates the local time for that city by applying the time zone offset to the current system time. The updated time is displayed on the LCD along with the name of the selected city and its GMT offset. Simultaneously, the home city (e.g., Manchester) and its local time are displayed as a reference.  
<br>
- **Stopwatch**  


https://github.com/user-attachments/assets/5d48a604-6101-4947-b032-dfb1303d95c4  

After the "World Clock" page, moving the joystick downward transitions the microcontroller to the next page, the "Stopwatch" page, pressing the fire button on the joystick starts the stopwatch, and the elapsed time is displayed in real time on the LCD in the format minutes:seconds:milliseconds. During stopwatch operation, the RGB LED glows blue to indicate that the stopwatch is running. Pressing the fire button again stops the stopwatch, freezing the displayed time. The elapsed time remains visible until the stopwatch is restarted or reset.  
<br>
- **Countdown timer**  


https://github.com/user-attachments/assets/3b38722d-067f-4970-94db-5afce1e2d357  

After the "Stopwatch" page, moving the joystick downward transitions the microcontroller to the next page, the "Countdown Timer" page. The user can set the countdown duration using the potentiometers. The left potentiometer sets the minutes (0 to 59), while the right potentiometer sets the seconds (0 to 59). Pressing the fire button on the joystick starts the countdown, which is displayed in real time on the LCD in the format minutes:seconds. While the countdown is running, the RGB LED flashes green to indicate active countdown operation. When the countdown reaches zero, the RGB LED turns yellow, and the speaker plays a sequence of tones to signal the end of the countdown. Pressing the fire button again stops the countdown.
<br>
- **Multitasks**  

https://github.com/user-attachments/assets/f986f498-c00b-4083-b754-91df781f054c  

While the "Countdown Timer" is running, the user can still press the joystick upward or downward to switch between pages. The RGB LED continues flashing green, indicating that the countdown is operating normally. If the user navigates to the "Stopwatch" page and presses the fire button, both the countdown and stopwatch will run simultaneously. When the countdown reaches zero, a pop-up window will appear on the LCD regardless of the current page, accompanied by the speaker playing a sequence of tones to signal the end of the countdown. Pressing the joystick upward or the fire button will return the user to the "Countdown Timer" page. The stopwatch will remain running throughout, and the user can navigate to the "Stopwatch" page and press the fire button to freeze the stopwatch timer.
