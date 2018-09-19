#include "mbed.h"

PwmOut myled(LED1);
DigitalIn switchInput(PTD6);

int main() {
    int switchState;
    float val = 1.0;
    myled = 1;
    
    while(1) {
        switchState = switchInput;
        if(switchState == 0)
        {
         if(val>0.0)
         {
            val -= 0.2;
         }  
         myled = val;
        }
        if(val <0.2)
         {
            val = 1.0;
            myled=val;
         } 
        wait(0.5);    
    }
}
