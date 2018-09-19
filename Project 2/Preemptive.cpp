#include "mbed.h"

DigitalOut redLed(PTD0);
DigitalOut greenLed(PTD3);
InterruptIn button1(PTD1);
InterruptIn button2(PTD2);

int currentPriority = 3;
int nextPriority = 5;
bool task3Ready = false;
bool task4Ready = false;
void process4();
void process3();

void interrupt_routine3()
{
        task3Ready = true;
}
void interrupt_routine4()
{
       task4Ready = true;
}

void scheduler()
{
    if(task4Ready)
    {
       currentPriority = 1;
       task4Ready = false;
       process4();
     
    }
    if(task3Ready)
    {
        currentPriority = 2;
        task3Ready = false;
        process3();
        
    }
    
}

void process3()
{
    redLed = 1;
    greenLed = 0;
     for(int i =0 ; i<10 ; i++)
        {
        redLed = !redLed;
        greenLed = !greenLed;    
        wait(0.5); 
        }
    scheduler();
}

void process4()
{
    redLed = 1;
    greenLed = 1;
     for(int i =0 ; i<10 ; i++)
        {
        redLed = !redLed;
        greenLed = !greenLed;    
        wait(0.5); 
        }
    scheduler();
}




int main() 
{
    button1.rise(&interrupt_routine3);
    button2.rise(&interrupt_routine4);
    
        for(int i =0 ; i<10 ; i++)
        {
        redLed = !redLed;
        greenLed = !greenLed;    
        wait(0.25); 
        }
        while(1)
        {
         scheduler();
        }
    
}
