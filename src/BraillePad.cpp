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
  Serial.begin(9600);
  /* TODO: assert return value */
  if (GenerateAlphabetMap() || GenerateNumericMap() || GenerateComputerMap())
  {
    digitalWrite(led, HIGH);
  }
}

// The loop function is called in an endless loop
void loop()
{
  static unsigned char numeric_mode = FALSE;
  static char * BrailleMap = BrailleAlphabetMap;
  unsigned char cur_braille_code;
  // read the pushbuttons:
  read_button_states();
  OR_state_to_max();

  //if all buttons are released print last maximum
  if (check_states_empty(button_state) == TRUE)
  {
    if (cur_braille_code = max_as_int()) /* if the last maximum is zero, it has already been printed */
    { /* Process the combination ... */
      if (cur_braille_code == NUMBER_SIGN)
      {
        if (numeric_mode) /* if previously we had numeric mode */
        {
          BrailleMap = BrailleAlphabetMap;
        }
        else
        {
          BrailleMap = BrailleNumericMap;
        }
        numeric_mode = !numeric_mode;
      }
      else
      {
        Keyboard.print(BrailleMap[cur_braille_code]);
      }
      empty_max();
    }
  }
  //we dont need this:
  delay(10);
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
