#include "mbed.h"
#include "mbed_events.h"

DigitalIn sw1(PTB8);
DigitalIn sw2(PTB11);
AnalogIn LM35(PTB0);   /* Temperature sensor connected to Analog Grove connector */
//initial serial port
Serial pc (USBTX, USBRX);

//initial event queue
EventQueue q1(32 * EVENTS_EVENT_SIZE);

//program timer
Timer program_time;

int counter = 0;
float current_temp;
float avg;

float current_time;
float avg_time;

float temp[48];
//= {20, 21, 22,23,24,25,26,27,20, 21, 22,23,24,25,26,27,20, 21, 
//22,23,24,25,26,27,20, 21, 22,23,24,25,26,27,20, 21, 22,23,24,25,26,27,20, 21, 22,23,24,25,26,27};
float avg_temp_buffer[32];

void task1();
void task2();
void task3();
void task4();
void convertTime(float time);
void  read_temperature();

void task1(void)
{
    q1.call_every(5000, task2);  
    q1.call_every(1000, task4);
}

void task2(void)
{
    current_temp = temp[counter];
    pc.printf("-------------------currnt_temp is: %f\r\n" , current_temp);
    current_time = program_time.read();
     counter = counter+1;
    if(counter>46)
        counter = 0;
    q1.call(task3);
}


void task3()
{
    int avg_num;
    int i;
    float sum =0.00;
    if(counter<33)
    {
     
        avg_temp_buffer[counter-1] = current_temp;
        avg_num = counter;
        
    }
    else
    {
        for(i=0 ; i<31; i++) 
        {
            avg_temp_buffer[i]=avg_temp_buffer[i+1];
        }
        avg_temp_buffer[31] = current_temp;
        avg_num = 32;
    }
    
    for(i =0 ; i<avg_num ;i++)
    {
        sum = sum + avg_temp_buffer[i];
    }
    
    avg = sum/avg_num;
    avg_time = program_time.read();
  
}

void task4()
{
     int switchOne;
     int switchTwo;
     switchOne = sw1;
     switchTwo = sw2;
     convertTime(program_time.read());
     if(switchOne == 0)
     {
         pc.printf("switch one is pressed ---  the current temperature is : %f\r\n" ,current_temp);
         pc.printf("time is: ");
         convertTime(current_time);
         
      }// current temp
     if(switchTwo == 0)
     {
         pc.printf("switch two is pressed --- average temperature is %f" , avg);
          pc.printf("time is: ");
         convertTime(avg_time);
     } 
}

void set_CLKOUT32k();
int main()
{
    pc.baud(9600);
    program_time.start();
    
    read_temperature();
    q1.call(task1);
    q1.dispatch();
}


void convertTime(float time)
{
    time_t sec = time;
    // setup time structure for Wed, 28 Oct 2009 11:35:37
    struct tm *t;
    t = localtime(&sec);
    // convert to timestamp and display (1256729737)
    time_t seconds = mktime(t);
    //pc.printf("Time as seconds since January 1, 1970 = %d\n", seconds);
     pc.printf("%s\r\n", ctime(&seconds));
}

void read_temperature()
{
    int i;
    for(int i=0 ; i<32 ; i++)
    {
        //float temperature = LM35.read()*330;
        temp[i] = LM35.read()*330;
        pc.printf("temp number %d is :%f \r\n" , i , temp[i]);
         wait(0.5); 
    }
     
}

