#include "mbed.h"

DigitalOut myled(LED1);
DigitalIn switchInput(PTD6);

int main() {
    int switchState;
    float val = 0.0;
    myled = 1;
    while(1) {
        switchState = switchInput;
        if(switchState == 0)
        {
            val += 0.2;
            wait(0.1);
            
        }   
        if(val > 1.0)
         {
            myled = 1;
            val = 0.0;
         }
         myled = 0;
         wait(val*0.05);
         myled = 1;
        wait((1.0-val)*0.05);   
        myled=0;      
    }
}
