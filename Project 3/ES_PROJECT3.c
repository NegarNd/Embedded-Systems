#include <MKL25Z4.H>
void PORTD_IRQHandler(void);
int counter = 0;
//delay function
void delay(int n)
{
	int i;
	for(i = 0; i < n; i++) ;
}
//set LED
void initLED(void)
{
	SIM->SCGC5 |= 0x1400; //enable clk for port D and B
	PORTB->PCR[18] |= 0x0100; // set pinB18 to GPIO
	PORTB -> PCR[19] |= 0x0100;//set pinB19 to GPIO
	PORTD ->PCR[1] |= 0x0100;//set pinD1 to GPIO
	
	FPTB->PDOR |= (1<<18);//write 1 to PTB18
	FPTB->PDOR |= (1<<19);//write 1 to PTB19
	FPTD -> PDOR |= (1<<1);//write 1 to PTD1
	
	FPTB->PDDR |= (1<<18); //set pinB18 to output
	FPTB->PDDR |= (1<<19);//set pinB19 to output
	FPTD ->PDDR |=(1<<1);//set pinD1 to output
}
//set SW1 and interrupt
void initSW1(void)
{
	PORTD -> PCR[0] |= 0xA0102;//set pinD1 to GPIO and enable internal pull up
	PORTD ->PCR[0] |= (1<<24); 
	FPTD ->PDDR |= (0<<0); //set PTD0 to input
	FPTD -> PDOR |= (0<<0);//write 0 in PTD0
	//enable interrupt for PTD0
	PORTD_ISFR = PORT_ISFR_ISF(0x01);
	NVIC_SetPriority(PORTD_IRQn,128);
	NVIC_EnableIRQ(PORTD_IRQn);

}
//ISR
void PORTD_IRQHandler(void)
{
	if(counter < 2)
	{
		counter++;
	}
	else 
	{
		counter = 0;
	}
	delay(5000000);
	PORTD_ISFR = PORT_ISFR_ISF(0x01);
}

//change the color of the LED
void Change_Led(void)
{
 if(counter == 0)
		{
			FPTD -> PSOR =(1<<1);
			FPTB->PCOR = (1<<18);
			delay(5000);
		}
		 else if(counter == 1)
		{
			FPTB ->PSOR = (1<<18);
			FPTB -> PCOR = (1<<19);
			delay(5000);
		}
		else if(counter == 2)
		{
			FPTB ->PSOR = (1<<19);
			FPTD -> PCOR =(1<<1);
			delay(5000);
		}
}

int main()
{
	initLED();
	initSW1();
	while(1)
	{
		Change_Led();
	}

}
