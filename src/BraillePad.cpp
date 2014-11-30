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
void read_button_states();
int change_mode();
unsigned int check_states_empty(unsigned int * six_states);
void empty_max();
void OR_state_to_max();
int max_as_int();
#ifdef __cplusplus
} // extern "C"
#endif

/* Variable declarations/definitions */

/* indicator led pin */
int led = 13u;

/* Button number to pin number correspondance */
const int button_1 = 2;
const int button_2 = 3;
const int button_3 = 4;
const int button_4 = 5;
const int button_5 = 6;
const int button_6 = 7;

//the global variables for chord retriaval
unsigned int button_state[6] = { 0, 0, 0, 0, 0, 0 };
unsigned int button_max[6] = { 0, 0, 0, 0, 0, 0 };



//The setup function is called once at startup of the sketch
void setup()
{
  /*
  pinMode(led, OUTPUT);
  */
  pinMode(button_1, INPUT);
  pinMode(button_4, INPUT);
  pinMode(button_2, INPUT);
  pinMode(button_5, INPUT);
  pinMode(button_3, INPUT);
  pinMode(button_6, INPUT);
  // initialize control over the keyboard:
  Keyboard.begin();
  //also serial for debugging
  //Serial.begin(9600);
  /* TODO: assert return value */
  if (GenerateAlphabetMap() || GenerateNumericMap() || GenerateComputerMap())
  {
    digitalWrite(led, HIGH);
  }
}

MODE chord_mode = ALPHABET;
static char * BrailleMap = BrailleAlphabetMap;
unsigned char cur_braille_code;

void loop()
{
	// read the pushbuttons:
	read_button_states();
	OR_state_to_max();

	//if all buttons are released print last maximum
	if (check_states_empty(button_state) == TRUE)
	{/* if the last maximum is zero, it has already been printed */
		cur_braille_code = max_as_int();
		if(cur_braille_code != 0)
		{ /* Process the combination ... */
			if(change_mode() == 0)
			{
				if(chord_mode == CAPITAL
					&& BrailleMap[cur_braille_code] >='a'
					&& BrailleMap[cur_braille_code] <='z'){
					Keyboard.press(BrailleMap[cur_braille_code]-32);
					chord_mode = ALPHABET;
				}else{
					Keyboard.press(BrailleMap[cur_braille_code]);
				}
				Keyboard.releaseAll();
    	    	//Serial.print(BrailleMap[cur_braille_code]);
    	  	}
			empty_max();
		}
	}
  //we dont need this:
  delay(10);
}

//returns 1 if the chord should not be futher processed
int change_mode(){
	//if we write a spacebar/newline, then change back to alphabet
	if(cur_braille_code == GET_BRAILLE_CODE(0,0,1, 0,0,0)
	|| cur_braille_code == GET_BRAILLE_CODE(0,0,0, 0,1,0))
	{
		BrailleMap = BrailleAlphabetMap;
		chord_mode = ALPHABET;
		Keyboard.press(BrailleMap[cur_braille_code]);
		Keyboard.releaseAll();
		return 1;
	}
	//if the number code is written, then change to or from numbers
	if(cur_braille_code == NUMBER_SIGN)
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
	if(cur_braille_code == CAPITAL_SIGN)
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

void read_button_states()
{
  button_state[0] = 1^digitalRead(button_1);
  button_state[3] = 1^digitalRead(button_4);
  button_state[1] = 1^digitalRead(button_2);
  button_state[4] = 1^digitalRead(button_5);
  button_state[2] = 1^digitalRead(button_3);
  button_state[5] = 1^digitalRead(button_6);
}

void OR_state_to_max()
{
  int i;
  for (i = 0; i < 6; i++)
  {
    button_max[i] |= button_state[i];
  }
}

unsigned int check_states_empty(unsigned int * six_states)
{
  unsigned int ret_val = TRUE;
  int i;
  for (i = 0; i < 6; i++)
  {
    if(six_states[i] == HIGH)
    {
      ret_val = FALSE;
    }
  }
  return ret_val;
}

void empty_max()
{
  int i;
  for (i = 0; i < 6; i++)
  {
    button_max[i] = 0;
  }
}

int max_as_int()
{
  int i;
  unsigned int sum = 0u;
  for (i = 5; i >= 0; i--)
  {
    sum = sum << 1;
    if (button_max[i] == HIGH)
    {
      sum++;
    }
  }
  return sum;
}
