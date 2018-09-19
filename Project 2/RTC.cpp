#include "mbed.h"


DigitalOut redLed(LED_GREEN);
DigitalOut greenLed(LED_RED);
InterruptIn button1(PTA1);
InterruptIn button2(PTD2);

int currentPriority = 3;
int nextPriority = 4;
bool task3Ready = false;
bool task4Ready = false;

void process4();
void process3();
void scheduler();

void interrupt_routine3()
{
        task3Ready = true;
        scheduler();
}

void interrupt_routine4()
{
        //myLed = !myLed;
        task4Ready = true;
        scheduler();
}

void scheduler()
{
    if((task4Ready)) // dare y task joz 4 ro ejra moikone
    {
        currentPriority = 1;
        task4Ready = false;
        process4();
    }
    if(task3Ready & currentPriority >=2)
    {
        currentPriority = 2;
        task3Ready = false;
        process3();
    }
    else if((task3Ready ==false)&&(task4Ready==false))
    currentPriority = 3;
}

void process3()
{
    redLed = 1;
    greenLed = 0;
    wait(5);
     for(int i =0 ; i<5 ; i++)
        {
        redLed = !redLed;
        greenLed = !greenLed;    
        wait(3); 
        }
    scheduler();
}

void process4()
{
    redLed = 1;
    greenLed = 1;
    wait(1);
     for(int i =0 ; i<5 ; i++)
        {
        redLed = !redLed;
        greenLed = !greenLed;    
        wait(3); 
        }
    currentPriority = 5;
    scheduler();
}




int main() 
{
    NVIC_SetPriority(PORTD_IRQn,0);
    NVIC_SetPriority(PORTA_IRQn,1);
    button1.rise(&interrupt_routine3);
    button2.rise(&interrupt_routine4);
    while(1){
    while(currentPriority == 3)
    {
        redLed = greenLed;
        redLed = !redLed;
        greenLed = !greenLed;    
        wait(1); 
    }}
}

