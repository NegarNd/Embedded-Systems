#include "mbed.h"
class Speaker
{
public:
    Speaker(PinName pin) : _pin(pin) {
    }
    void PlayNote(float frequency[], float duration[], float volume=1.0) {
        for(int i=0 ; i<29 ; i++)
        {
         _pin.period(1.0/frequency[i]);
        _pin = volume/2.0;
        wait(duration[i]);
        _pin = 0.0;   
        }
    }
 
private:
    PwmOut _pin;
};
 
DigitalOut led1(LED1);
float note[30]= {329.63, 311.13, 329.63, 311.13, 329.63, 246.94, 293.66,261.63, 220, 146.83, 174.61, 220, 246.94, 174.61,233.08,
                    246.94,261.63, 329.63,311.13, 329.63, 311.13,329.63, 246.94, 293.66, 261.63,220, 146.83,174.61, 220, 0.0 };
float duration[30]= {0.35, 0.35, 0.35, 0.35, 0.35, 0.4, 0.4 ,0.4, 1, 0.35, 0.4, 0.4 , 1, 0.4 , 0.4 ,0.4,   0.3 ,0.4 ,0.4 ,0.4, 0.4,0.4,
                    0.4,0.4,0.4,1,0.4,0.4,0.4,0.0
                    };
                    
int main()
{
     Speaker mySpeaker(PTD4);
     mySpeaker.PlayNote(note, duration);
    while(1) {
        led1 = !led1;
        wait(.1);
    }
}