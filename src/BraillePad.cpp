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

void pin_change();
void send_chord(char chord);
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
	// initialize control over the keyboard:
	Keyboard.begin();
	GenerateAlphabetMap();
	GenerateNumericMap();
	GenerateComputerMap();

	
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

void loop(){
	//IDLE
}

//redirect all the different button interrupts to one function
ISR(INT0_vect)  {pin_change();}
ISR(INT1_vect)  {pin_change();}
ISR(INT2_vect)  {pin_change();}
ISR(INT3_vect)  {pin_change();}
ISR(PCINT0_vect){pin_change();}

void pin_change(){
	char buttons = read_buttons();
	if(buttons == 0){
		//if no buttons are pressed send a chord
		send_chord(curr_chord);
		curr_chord = 0;
	}else{
		//if some buttons are pressed add them to the chord
		curr_chord |= buttons;
	}
}

char read_buttons(){
	//fiddle the raw data so that the bit at position 'n'
	//will correspond to the braille cell 'n+1' being pressed
	char pretty = (~PINB & 0b00110000) | (~PIND & 0b00001111);
	//XXX change first mask to 0b11110000 to enable cells 7 and 8
	return pretty;
}

void send_chord(char chord){
	if(change_mode(chord) == 0){
		if(chord_mode == CAPITAL
			&& BrailleMap[chord] >='a'
			&& BrailleMap[chord] <='z'){
			Keyboard.press(BrailleMap[chord]-32);
			chord_mode = ALPHABET;
		}else{
			Keyboard.press(BrailleMap[chord]);
		}
		Keyboard.releaseAll();
	}
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
