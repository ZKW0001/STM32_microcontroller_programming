#include "mbed.h"
#include "C12832.h"

#define kH 24   //  Minute Multiplier for Potentiometer
#define kM 60   //  Hour Multiplier for Potentiometer
#define kS 60   //  Second Multiplier for Potentiometer

#define N 3     //  Width of Rolling Average Filtering

//  Classes
class LED {

    protected:
        DigitalOut red, green, blue;
        int ledState, pledState;

    public:
        LED(PinName led_r, PinName led_g, PinName led_b): red(led_r), green(led_g), blue(led_b) {off();}

        void off(void) {
            red = 1;
            green = 1;
            blue = 1;
            pledState = ledState;
            ledState = 0;
        }

        void dispRed(void) {
            red = 0;
            green = 1;
            blue = 1;
            pledState = ledState;
            ledState = 1;
        }

        void dispGreen(void) {
            red = 1;
            green = 0;
            blue = 1;
            pledState = ledState;
            ledState = 2;
        }

        void dispBlue(void) {
            red = 1;
            green = 1;
            blue = 0;
            pledState = ledState;
            ledState = 3;
        }

        void dispYellow(void) {
            red = 0;
            green = 0;
            blue = 1;
            pledState = ledState;
            ledState = 4;
        }

        void dispCyan(void) {
            red = 1;
            green = 0;
            blue = 0;
            pledState = ledState;
            ledState = 5;
        }

        void dispMagenta(void) {
            red = 0;
            green = 1;
            blue = 0;
            pledState = ledState;
            ledState = 6;
        }

        void dispWhite(void) {
            red = 0;
            green = 0;
            blue = 0;
            pledState = ledState;
            ledState = 7;
        }

        void dispUndo(void) {
            switch (pledState) {

                case (0) :
                    off();

                    break;

                case (1) :
                    dispRed();

                    break;

                case (2) :
                    dispGreen();

                    break;

                case (3) :
                    dispBlue();

                    break;

                case (4) :
                    dispYellow();

                    break;

                case (5) :
                    dispCyan();

                    break;

                case (6) :
                    dispMagenta();

                    break;

                case (7) :
                    dispWhite();

                    break;

                default :
                    off();
            }
        }
        
};

class Potentiometer {

    private:
        AnalogIn inputSignal;
        float VDD;

    public:
        Potentiometer(PinName pin, float v): inputSignal(pin), VDD(v) {}

        float sampleNorm;
    
        void sample(void) {
            sampleNorm = inputSignal.read();
        }
    
};

class SamplingPotentiometer: public Potentiometer {

    private:
        float samplingFrequency, samplingPeriod, sampleAvg, rollingAverage[N];
        Ticker sampler;

        void filter(void) {
            sampleAvg = 0;
            for (int i = (N - 1); i > 0; i--) {
                rollingAverage[i] = rollingAverage[i - 1];
            }
            rollingAverage[0] = sampleNorm;
            for (int i = 0; i < N; i++) {
                sampleAvg += (rollingAverage[i] / N);
            }
        }

    public:
        SamplingPotentiometer(PinName pin, float v, float fs): Potentiometer(pin, v), samplingFrequency(fs), samplingPeriod(1 / fs) {
            sampler.attach(callback(this, &Potentiometer::sample), samplingPeriod);
        }

        int getSample(int range) {
            return (sampleNorm * range);
        }

        int getFilteredSample(int range) {
            filter();
            return (sampleAvg * range);
        }

};

class Speaker{

private:
    DigitalOut outputSignal;
    char state;

public:
    Speaker(PinName pin): outputSignal(pin){off();}

    void on(void) {
      outputSignal = 1;
      state = 1;
    }

    void off(void) {
      outputSignal = 0;
      state = 0;
    }

    void toggle(void) {
      if (state)
        off();
      else
        on();
    }

    void playnote(float frequency, float time) {
      while(time > 0) {
        toggle();
        wait(0.5 / frequency);
        time -= 0.5 / frequency;
      }

    }
};

class Clock {

    private:
        int time_hour, time_min, time_sec, time_msec;

    public:
        Clock() {}

        void reset(void) {
            time_hour = 0;
            time_min = 0;
            time_sec = 0;
            time_msec = 0;
        }

        void tick(void) {
            if (time_msec >= 999) {
                if (time_sec >= 59) {
                    if (time_min >= 59) {
                        if (time_hour >= 23) {
                            time_hour = 0;
                        }
                        else {
                            time_hour++;
                        }
                        time_min = 0;
                    }
                    else {
                        time_min++;
                    }
                    time_sec = 0;
                }
                else {
                    time_sec++;
                }
                time_msec = 0;
            }
            else {
                time_msec++;
            }
        }

        void tickdown(void) {
            if (time_msec == 0) {
                if (time_sec == 0) {
                    if (time_min == 0) {
                        if (time_hour == 0) {
                        //    return 1;
                        }
                        else {
                            time_hour--;
                        }
                        time_min = 59;
                    }
                    else {
                        time_min--;
                    }
                    time_sec = 59;
                }
                else {
                    time_sec--;
                }
                time_msec = 999;
            }
            else {
                time_msec--;
            }
        }

        void setClock(int hour, int min, int sec) {
            time_hour = hour;
            time_min = min;
            time_sec = sec;
        }

        void warpClock(int min) {
            time_min += min;
            if (time_min < 0) {
                time_min += 1440;
            }
            while (time_min >= 60) {
                time_hour++;
                time_min -= 60;
            }
            if (time_hour >= 24) {
                time_hour -= 24;
            }
        }

        int getHour(void) {
            return time_hour;
        }

        int getMin(void) {
            return time_min;
        }

        int getSec(void) {
            return time_sec;
        }

        int getMSec(void) {
            return time_msec;
        }

        float getTime(void) {
            return (time_hour * 3600 + time_min * 60 + time_sec + time_msec * 0.001);
        }

        int getIntTime(void) {
            return (time_hour * 3600 + time_min * 60 + time_sec);
        }
        
};

//  Declarations
LED led(D5, D9, D8);

C12832 lcd(D11, D13, D12, D7, D10);

SamplingPotentiometer pot_1(A0, 3.3, 1000);
SamplingPotentiometer pot_2(A1, 3.3, 1000);

InterruptIn fire(D4), up(A2), down(A3);

Speaker speaker(D6);

Clock clockGlobal, clockWorldTime, clockStopWatch, clockCountDown;

Ticker masterTime, flashLED;
volatile int timer = 0, cdFlash = 0;

Timeout countDownTime;

typedef enum {setTime, currentTime, worldTime, stopWatch, countDown, countDownEnd} ProgramState;
ProgramState state;

float countDownOrigin = 0;
bool setTimeActive = 0, stopWachActive = 0, countDownActive = 0;

//  Function Prototypes
void dispClr(void);
void updatingTimer(void);
void updatingFlash(void);
void countDownISR(void);
void fireISR(void);
void upISR(void);
void downISR(void);

//  Main Function
int main(void) {

    char cities[30][20] = {
        "Midway",
        "Honolulu",
        "Anchorage",
        "San Francisco",
        "Gilbert",
        "Chicago",
        "Toronto",
        "Santiago",
        "Sao Paulo",
        "Greenland",
        "London",
        "Berlin",
        "Athens",
        "Istanbul",
        "Tehran",
        "Dubai",
        "Kabul",
        "Yekaterinburg",
        "Kathmandu",
        "Bangkok",
        "Shanghai",
        "Tokyo",
        "Vladivostok",
        "Wellington"
    };

    char timeZone[30][20] = {
        "-11", 
        "-10",
        "-9",
        "-8",
        "-7",
        "-6",
        "-5",
        "-4",
        "-3",
        "-1",
        "+0",
        "+1",
        "+2",
        "+3",
        "+3:30",
        "+4",
        "+4:30",
        "+5",
        "+5:45",
        "+7",
        "+8",
        "+9",
        "+10",
        "+12"
    };

    float timeZoneValue[] = {
        -11,
        -10,
        -9,
        -8,
        -7,
        -6,
        -5,
        -4,
        -3,
        -1,
        0,
        1,
        2,
        3,
        3.5,
        4,
        4.5,
        5,
        5.75,
        7,
        8,
        9,
        10,
        12
    };

    int kC = sizeof(timeZoneValue) / sizeof(timeZoneValue[0]);

    masterTime.attach(&updatingTimer, 0.0005);
    flashLED.attach(&updatingFlash, 1);
    up.rise(&upISR);
    down.rise(&downISR);
    fire.rise(&fireISR);


    while(1) {

        switch (state) {
            
            case (setTime) :

                if (setTimeActive) {

                    lcd.locate(0, 0);
                    lcd.printf("Set new time (HH:MM)\n%.2d:%.2d     \n", pot_1.getFilteredSample(kH), pot_2.getFilteredSample(kM));
                    lcd.printf("                   ");

                }
                else {

                    lcd.locate(0, 0);
                    lcd.printf("Press FIRE to set time\n%.2d:%.2d:%.2d\n", clockGlobal.getHour(), clockGlobal.getMin(), clockGlobal.getSec());
                    lcd.printf("                   ");

                }

                break;

            case (currentTime) :

                lcd.locate(0, 0);
                lcd.printf("Current time:          \n%.2d:%.2d:%.2d", clockGlobal.getHour(), clockGlobal.getMin(), clockGlobal.getSec());
                lcd.printf("                  \n");
                lcd.printf("                  ");

                break;

            case (worldTime) :

                clockWorldTime.setClock(clockGlobal.getHour(), clockGlobal.getMin(), clockGlobal.getSec());
                clockWorldTime.warpClock(timeZoneValue[pot_1.getFilteredSample(kC)] * 60);

                lcd.locate(0, 0);
                lcd.printf("%s (UTC%s)     \n", cities[pot_1.getFilteredSample(kC)], timeZone[pot_1.getFilteredSample(kC)]);
                lcd.printf("%.2d:%.2d:%.2d                \n", clockWorldTime.getHour(), clockWorldTime.getMin(), clockWorldTime.getSec());
                lcd.printf("%.2d:%.2d:%.2d (Manchester)", clockGlobal.getHour(), clockGlobal.getMin(), clockGlobal.getSec());

                break;

            case (stopWatch) :

                if (stopWachActive) {

                    lcd.locate(0, 0);
                    lcd.printf("Stopwatch Running\n");
                    lcd.printf("Time: %.2d:%.2d.%.3d s         \n", clockStopWatch.getMin(), clockStopWatch.getSec(), clockStopWatch.getMSec());
                    lcd.printf("                   ");

                }
                else {

                    lcd.locate(0, 0);
                    lcd.printf("Stopwatch Inactive\n");
                    lcd.printf("Last Time: %.2d:%.2d.%.3d s\n", clockStopWatch.getMin(), clockStopWatch.getSec(), clockStopWatch.getMSec());
                    lcd.printf("                   ");

                }
                
                break;

            case (countDown) :

                if (countDownActive) {

                    lcd.locate(0, 0);
                    lcd.printf("Countdown Running   \n");
                    lcd.printf("%.0f / %.0f s       \n", clockCountDown.getTime(), countDownOrigin);
                    lcd.printf("                   ");

                }
                else {

                    lcd.locate(0, 0);
                    lcd.printf("Set countdown period\n");
                    lcd.printf("%.2d:%.2d                \n", pot_1.getFilteredSample(kM), pot_2.getFilteredSample(kS));
                    lcd.printf("                   ");

                }
                
                break;

            case (countDownEnd) :

                lcd.locate(0, 0);
                lcd.printf("Time Period Elasped!!\n");
                lcd.printf("               ");
                lcd.printf("               ");
                
                speaker.playnote(1174.66, 0.25);
                speaker.playnote(932.33, 0.25);
                speaker.playnote(698.46, 0.25);

                /* speaker.playnote(659.26, 0.5);
                speaker.playnote(1108.73, 0.25);
                speaker.playnote(1174.66, 0.25);
                speaker.playnote(1318.51, 0.5);
                speaker.playnote(659.26, 0.5);
                speaker.playnote(987.77, 0.5);
                speaker.playnote(1, 0.5);

                speaker.playnote(659.26, 0.5);
                speaker.playnote(1174.66, 0.25);
                speaker.playnote(1108.73, 0.25);
                speaker.playnote(987.77, 0.25);
                speaker.playnote(880.00, 0.25);
                speaker.playnote(987.77, 0.25);
                speaker.playnote(1108.73, 0.25);
                speaker.playnote(987.77, 0.5);
                speaker.playnote(1, 0.5);

                speaker.playnote(587.33, 0.25);
                speaker.playnote(880.00, 0.5);
                speaker.playnote(739.99, 0.25);
                speaker.playnote(880.00, 0.25);
                speaker.playnote(739.99, 0.25);
                speaker.playnote(659.26, 0.5);
                speaker.playnote(659.26, 0.5);
                speaker.playnote(659.26, 0.25);
                speaker.playnote(739.99, 0.25);
                speaker.playnote(554.37, 0.25);
                speaker.playnote(493.88, 0.25);
                speaker.playnote(440.00, 0.5);
                speaker.playnote(1, 0.5); */

                break;

            default :

                state = setTime;
        }
    }

    return 0;

}

//  Clear LCD Display
void dispClr(void) {

    lcd.cls();
    lcd.rect(0, 0, 127, 31, 0);
    //wait(0.05);

}

//  Master timer
void updatingTimer(void) {

    if (timer == 1) {
        timer = 0;
        clockGlobal.tick();

        if (stopWachActive) {
            clockStopWatch.tick();
        }

        if (countDownActive) {
            clockCountDown.tickdown();
        }
    }
    else{
        timer = 1;

        if (countDownActive) {
        }
    }

}

//  LED Logic
void updatingFlash(void) {

    if (cdFlash) {
        cdFlash = 0;

        if (countDownActive || state == countDownEnd) {
            led.dispYellow();
        }
        else if (stopWachActive) {
            led.dispBlue();
        }
        else {
            led.off();
        }
    }
    else{
        cdFlash = 1;

        if (state == countDownEnd) {
            led.dispYellow();
        }
        else if (stopWachActive) {
            led.dispBlue();
        }
        else {
            led.off();
        }
    }

}

//  Countdown End ISR
void countDownISR(void) {

    dispClr();
    countDownActive = 0;
    state = countDownEnd;

}

//  Fire Button ISR
void fireISR(void) {

    dispClr();
    
    switch (state) {
        
        case (setTime) :

            if (setTimeActive) {
                setTimeActive = 0;
                clockGlobal.setClock(pot_1.getFilteredSample(kH), pot_2.getFilteredSample(kM), 0);
            }
            else {
                setTimeActive = 1;
            }

            break;

        case (currentTime) :

            
            break;

        case (worldTime) :

            
            break;

        case (stopWatch) :
        
            if (stopWachActive) {
                stopWachActive = 0;
            }
            else {
                stopWachActive = 1;
                clockStopWatch.reset();
                led.dispBlue();
            }
            break;

        case (countDown) :

            if (countDownActive) {
                countDownActive = 0;
            }
            else {
                countDownActive = 1;
                clockCountDown.reset();
                clockCountDown.setClock(0, pot_1.getFilteredSample(kM), pot_2.getFilteredSample(kS));
                countDownOrigin = clockCountDown.getIntTime();
                countDownTime.attach(&countDownISR, clockCountDown.getIntTime());
            }
            break;

        case (countDownEnd) :

            state = countDown;

            break;

        default :
            
            state = setTime;
    }

}

//  Up Button ISR
void upISR(void) {

    dispClr();
    
    switch (state) {
        
        case (setTime) :

            state = countDown;
            setTimeActive = 0;

            break;

        case (currentTime) :

            state = setTime;

            break;

        case (worldTime) :

            state = currentTime;

            break;

        case (stopWatch) :

            state = worldTime;

            break;

        case (countDown) :

            state = stopWatch;

            break;

        case (countDownEnd) :

            state = countDown;

            break;

        default :
            
            state = setTime;
    }

}

//  Down Button ISR
void downISR(void) {

    dispClr();

    switch (state) {

        case (countDown) :

            state = setTime;

            break;

        case (setTime) :

            state = currentTime;
            setTimeActive = 0;

            break;

        case (currentTime) :

            state = worldTime;

            break;

        case (worldTime) :

            state = stopWatch;

            break;

        case (stopWatch) :

            state = countDown;

            break;

        case (countDownEnd) :

            state = countDown;

            break;

        default :
            
            state = setTime;
    }

}

