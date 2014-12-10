/*
This code is meant to be run on arduino Leonardo or similar platform
History: 24-26.10.2014 - Juha Kivekäs: created during TehnoHack weekend
20.11.2014 - Siim Sepman: Created multiple input maps; added numbers
*/

#include "BraillePad.h"

/* Function declarations */
#ifdef __cplusplus
extern "C" {
#endif
void loop();
void setup();

void button_setup();
void timer1_setup(int cycles);
void timer3_setup(int cycles);	
void timer1_stop();
void timer3_stop();
void timer1_start();
void timer3_start();

void pin_change();
void send_chord(char chord);
void release_chord();
int change_mode(char chord);
char read_buttons();
#ifdef __cplusplus
} // extern "C"
#endif

/* Variable declarations/definitions */

/* Button number to pin correspondance */
//the buttons are numbered according to the braille cells
//ie. button_1 is cell one.
#define button_1 3
#define button_2 2
#define button_3 0
#define button_4 1
#define button_5 8
#define button_6 9
#define button_7 10
#define button_8 11

//----GLOBAL VARIABLES
//for chord retrival
volatile char curr_chord = 0;
//for keymap handling
MODE chord_mode = ALPHABET;
static char * BrailleMap = BrailleAlphabetMap;

//The setup function is called once at startup of the sketch
void setup(){
	pinMode(13, OUTPUT);
	// initialize control over the keyboard:
	Keyboard.begin();
	GenerateAlphabetMap();
	GenerateNumericMap();
	GenerateComputerMap();

	//TIMER1 for debouncing buttons
	timer1_setup(0x100);//15 milliseconds
	//TIMER3 for keypress repetition
	timer3_setup(0x4000);//one second
	//XXX THE VALUES OF INITIALIZATION ARE NOT WORKING RIGHT
	//i know those are the right values, but i cant get them to work the way i
	// want. it seems that they dont matter at all?
	button_setup();
}

void loop(){
	//IDLE
}

//any pin change will start the debounce timer
ISR(INT0_vect)  {pin_change();}//{timer1_start();}
ISR(INT1_vect)  {pin_change();}//{timer1_start();}
ISR(INT2_vect)  {pin_change();}//{timer1_start();}
ISR(INT3_vect)  {pin_change();}//{timer1_start();}
ISR(PCINT0_vect){pin_change();}//{timer1_start();}

//the debouncing timer will call the pin change function.
ISR(TIMER1_COMPA_vect){
	timer1_stop();
	timer3_stop();
	pin_change();
}

//the pin change function will handle making button input into chords
void pin_change(){
	char buttons = read_buttons();
	if(buttons == 0){
		//if no buttons are pressed send a chord
		send_chord(curr_chord);
		release_chord();
		curr_chord = 0;
	}else{
		//if some buttons are pressed add them to the chord
		curr_chord |= buttons;
		//also start the repetition timer.
		timer3_start();
	}
}

//the repetition timer will send a chord when captured
//the sent chord will not be "turned off"
volatile int asdf = 0;
//asdf is a workaround variable since i can't get the timers to work correctly
ISR(TIMER3_COMPA_vect){
	asdf++;
	if(asdf > 30){
		asdf = 0;
		send_chord(curr_chord);
		timer3_stop();
	}
}



void send_chord(char chord){
	char mode_changed = change_mode(chord);
	if(mode_changed == 0){
		if(chord_mode == CAPITAL
		&& BrailleMap[chord] >='a'
		&& BrailleMap[chord] <='z'){
			Keyboard.press(BrailleMap[chord]-32);
			chord_mode = ALPHABET;
		}else{
			Keyboard.press(BrailleMap[chord]);
		}
	}
}

void release_chord(){
	Keyboard.releaseAll();
}

//returns 1 if the chord should not be futher processed
int change_mode(char chord){
	//if we write a spacebar/newline, then change back to alphabet
	if(chord == GET_BRAILLE_CODE(0,0,1, 0,0,0)
	|| chord == GET_BRAILLE_CODE(0,0,0, 0,1,0))
	{
		BrailleMap = BrailleAlphabetMap;
		chord_mode = ALPHABET;
		Keyboard.press(BrailleMap[chord]);
		Keyboard.releaseAll();
		return 1;
	}
	//if the number code is written, then change to or from numbers
	if(chord == NUMBER_SIGN)
	{
		if (chord_mode == NUMBER) /* if previously we had numeric mode */
		{
			BrailleMap = BrailleAlphabetMap;
			chord_mode = ALPHABET;
		}
		else
		{
			BrailleMap = BrailleNumericMap;
			chord_mode = NUMBER;
		}
		return 1;
	}
	//if shift/capital code is written, then change to or from capitals
	if(chord == CAPITAL_SIGN)
	{
		if (chord_mode == CAPITAL) /* if previously we had numeric mode */
		{
			BrailleMap = BrailleAlphabetMap;
			chord_mode = ALPHABET;
		}
		else
		{
			BrailleMap = BrailleAlphabetMap;
			chord_mode = CAPITAL;
		}
		return 1;
	}
	return 0;
}


//--------------BUTTON functions-------------
void button_setup(){
	//initialize the buttons
	pinMode(button_1, INPUT);
	pinMode(button_2, INPUT);
	pinMode(button_3, INPUT);
	pinMode(button_4, INPUT);
	pinMode(button_5, INPUT);
	pinMode(button_6, INPUT);
	//XXX uncomment to enable cells 7 and 8
	//pinMode(button_7, INPUT);
	//pinMode(button_8, INPUT);
	
	//----initialize all button interrupts
	//enable and unmask external interrupts INT0, INT1, INT2, INT3
	//these will be on pins PD0 (#3) PD1 (2) PD2 (0) PD3 (1)
	EICRA = (1<<ISC00)|(1<<ISC10)|(1<<ISC20)|(1<<ISC30);
	EIMSK = (1<<INT0) |(1<<INT1) |(1<<INT2) |(1<<INT3);
	//enable and unmask the pin change interrupt
	//this will fire on pins PB4 (8) PB5 (#9) PB6 (10) PB7(11)
	PCICR = (1<<PCIE0);
	PCMSK0= (1<<PCINT4)|(1<<PCINT5);//|(1<<PCINT6)|(1<<PCINT7);
	//XXX uncomment end of the line to enable cells 7 and 8
	sei();
}

char read_buttons(){
	//fiddle the raw data so that the bit at position 'n'
	//will correspond to the braille cell 'n+1' being pressed
	char pretty = (~PINB & 0b00110000) | (~PIND & 0b00001111);
	//XXX change first mask to 0b11110000 to enable cells 7 and 8
	return pretty;
}

//---------------TIMER functions--------------
/*The timers 1 and 3 are identical to the extent they are used in this program.
We are using a simple CTC mode for the timers, CTC is for Clear Timer on Compare
match. This means that the timer will count upwards form zero until it reaches
a certain value. When it reaches this value it will fire an interrupt and zero
itself. The timer now starts counting from zero again and repeats the process.

The top limit is saved in the 16-bit register OCR1A (Output Compare Register A
 for timer 1). The current value of the timer is saved in TCNT1. When OCR1A
reaches TCNT1 the TIMER1_COMPA interrupt will fire. This is when we will do
 whatever we have to do in a synchronized manner.
*/



void timer1_setup(int cycles){	
	//Clear Timer on Compare match (CTC) mode, 1/1024 prescaling = 15625Hz
	TCCR1B = (1<<WGM12)|(1<<CS10)|(1<<CS12);
	//set the compare value (time between interrupts in clock cycles)
	OCR1A = cycles;
}

void timer3_setup(int cycles){	
	//CTC mode, 1/1024 prescaling
	TCCR3B = (1<<WGM32)|(1<<CS30)|(1<<CS32);
	//set the compare value
	OCR3A = cycles;
}

void timer1_stop(){
	//"stop" timer, actually we just mask the interrupt, the timer will keep on running.
	TIMSK1 &= ~(1<<OCIE1A);
}

void timer3_stop(){
	//"stop" timer, actually we just mask the interrupt, the timer will keep on running.
	TIMSK3 &= ~(1<<OCIE3A);
}

void timer1_start(){
	//reset the counter to make sure we start counting from zero
	TCNT1 = 0;
	//unmask the interrupt
	TIMSK1 |= (1<<OCIE1A);
}

void timer3_start(){
	//reset the counter
	TCNT3 = 0;
	//unmask the interrupt
	TIMSK3 |= (1<<OCIE3A);	
}
