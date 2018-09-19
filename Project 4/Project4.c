#include <MKL25Z4.H>
//define states
struct State {
  uint32_t Outb;
  uint32_t Outc;
  uint32_t Oute;
  uint32_t Time;  // 1 ms units
  uint32_t NextState; // list of next states
}; 
typedef const struct State STyp;

//define state machine
#define goN_goBus 	0
#define goN_waitBus 1
#define	goN_goW		2
#define waitN_waitW 3
#define goE			4
#define waitE		5

STyp FSM[6] = {
 {0x101,30000,goN_waitBus , 
 {0x201, 5000,goN_goW},
 {0x401,30000,waitN_waitW},
 {0x401, 5000,goE},
 {0x40C, 5000,waitE},
 {0x40A, 5000,goN_goBus}
};

#define red1 (0)
#define yellow1 (1)
#define green1 (2)

#define red2 (3)
#define yellow2 (2)
#define green2 (1)

#define red3 (10)
#define yellow3 (9)
#define green3 (8)

#define red4 (2)
#define yellow4 (3)
#define green4 (4)


#define emergW (2)
#define emergN (1)

void delay(unsigned int length_ms)
{
   SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;  // Make sure clock is enabled
   LPTMR0_CSR = 0;                     // Reset LPTMR settings
   LPTMR0_CMR = length_ms;             // Set compare value (in ms)
   // Use 1kHz LPO with no prescaler
   LPTMR0_PSR = LPTMR_PSR_PCS(1) | LPTMR_PSR_PBYP_MASK;
    // Start the timer and wait for it to reach the compare value
   LPTMR0_CSR = LPTMR_CSR_TEN_MASK;
   while (!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK));
   LPTMR0_CSR = 0;                     
 }
 
void initPorts(void)
{
	SIM->SCGC5 |= 0x1400; //enable clk for port D and B
	//set all pins to GPIO
	PORTB -> PCR[red1] |= 0x0100;
	PORTB -> PCR[yellow1] |=0x0100;
	PORTB -> PCR[green1] |= 0x0100;
	
	PORTB -> PCR[red2] |= 0x0100;
	PORTC -> PCR[yellow2] |= 0x0100;
	PORTC -> PCR[green2] |= 0x0100;
	
	PORTB -> PCR[red3] |= 0x0100;
	PORTB -> PCR[yellow3] |= 0x0100;
	PORTB -> PCR[green3] |= 0x0100;
	
	PORTE -> PCR[red4] |= 0x0100;
	PORTE -> PCR[yellow4] |= 0x0100;
	PORTE -> PCR[green4] |= 0x0100;
	
	PORTD -> PCR[emergN] |= 0x0100;
	PORTA -> PCR[emergW] |= 0x0100;
	
	//set pins to output - others are input by default
	FPTB -> PDDR |= (1<<red1)|(1<<yellow1)|(1<<green1)|(1<<red2)|(1<<red3)|(1<<yellow3)|(1<<green3); // set pin B to output
	FPTC -> PDDR |= (1<<yellow2)|(1<<green2);
	FPTE -> PDDR |= (1<<red4)| (1<<yellow4)|(1<<green4)
	
	FPTB -> PDOR |= (0<<red1)|(0<<yellow1)|(0<<green1)|(0<<red2)|(0<<red3)|(0<<yellow3)|(0<<green3);;
	FPTC -> PDOR |= (0<<yellow2)|(0<<green2);
	FPTE -> PDOR |= (0<<red4)| (0<<yellow4)|(0<<green4)
}

void initSensors(void)
{
	//sensors should act like interrupt
	PORTD -> PCR[emergN] |= 0xA0102;//set pinD1 to GPIO and enable internal pull up
	PORTA -> PCR[emergW] |= 0xA0102;
	
	PORTD ->PCR[emergN] |= (1<<24); 
	PORTA ->PCR[emergW] |= (1<<24);
	
	FPTD ->PDDR |= (0<<emergN); //set PTD0 to input
	FPTA ->PDDR |= (0<<emergW);
	
	FPTD -> PDOR |= (0<<emergN);//write 0 in PTD0
	FPTA -> PDOR |= (0<<emergW);
	
	//enable interrupt for PTD0
	PORTD_ISFR = PORT_ISFR_ISF(0x01);
	PORTA_ISFR = PORT_ISFR_ISF(0x01);
	
	NVIC_SetPriority(PORTD_IRQn,128);
	NVIC_EnableIRQ(PORTD_IRQn);
	
	NVIC_SetPriority(PORTA_IRQn,128);
	NVIC_EnableIRQ(PORTD_IRQn);
	
}
void PORTD_IRQHandler(void)
{
	//masire b samte shomal sabz baghie joz otobus ghermez
}

void PORTA_IRQHandler(void)
{
	//masir b samte shargh sabz baghie ghermez
}


int main()
{
	uint32_t CS;  // index of current state
	CS = goN_goBus; //start state
	initPorts();
	initSensors();
	while(1) 
	{
		PTB -> PDOR |= FSM[CS].Outb;  // set lights
		PTC -> PDOR |= FSM[CS].Outc;
		PTE -> PDOR |= FSM[CS].Oute;
		delay(FSM[CS].Time);
		CS = FSM[CS].NextState;  
  }
}