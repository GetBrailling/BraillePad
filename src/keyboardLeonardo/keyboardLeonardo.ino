/*
TehnoHack
24-26.10.2014

Braille Pad code by Juha Kivekäs
This code is meant to be run on arduino Leonardo or similar platform
*/


//index this by the bit pattern of the keystroke
/*
Bit values for the different positions:
[0] [1]
[2] [3]
[4] [5]

This means that the button press:
[*] [*]
[ ] [*]
[*] [ ]
Translates to:
011011 = 27 -> 'n'
*/

const char lookup[64] = {
	0,
	'a',//1
	' ',//XXXXXXXXXXXXXXXXXXXXXXXXXXX not treal braille
	'c',//3
	',',//comma
	'b',//2
	'i',//9
	'f',//6
	0 ,
	'e',//5
	0,
	'd',//4
	':',
	'h',//8
	'j',//0
	'g',//7
//-------------------------------16
	0,//apastrophe
	'k',
	'/',
	'm',
	';',//semicolon
	'l',
	's',
	'p',
	0,
	'o',
	0,
	'n',
	'!',
	'r',
	't',
	'q',
//-------------------------------32
	'-',
	0,
	0,
	0,//3
	'?',
	0,//2
	0,//9
	0,//6
	0,
	0,//5
	0,
	0,//4
	'(',
	0,//8
	0,//0
	0//7
//-------------------------------48
	0,//SHIFT!
	'u',
	',',
	'x',
	0,
	'v',
	0,
	0,
	0,
	'z',
	0,
	'y',
	'.',
	'r',
	'w',
	0,
}

//button number to pin number correspondance:
const int button_0 = 6;          // input pins for pushbuttons
const int button_1 = 7;
const int button_2 = 4;
const int button_3 = 5;
const int button_4 = 3;
const int button_5 = 2;


void setup() {
  // make the pushbutton pins inputs:
  pinMode(button_0, INPUT);
  pinMode(button_1, INPUT);
  pinMode(button_2, INPUT);
  pinMode(button_3, INPUT);
  pinMode(button_4, INPUT);
  pinMode(button_5, INPUT);
  // initialize control over the keyboard:
  Keyboard.begin();
  //also serial for debugging
  Serial.begin(9600);
}

//the global variables for chord retriaval
int button_state[6] = {0, 0 ,0 ,0 ,0 ,0};
int button_max[6]= {0, 0, 0, 0, 0, 0};

void read_button_states(){
  button_state[0] = digitalRead(button_0);
  button_state[1] = digitalRead(button_1);
  button_state[2] = digitalRead(button_2);
  button_state[3] = digitalRead(button_3);
  button_state[4] = digitalRead(button_4);
  button_state[5] = digitalRead(button_5);
}

//I'm very sorry for the unclean code and the global variables
//This is kindof a mashup
int button_state_empty(){
  int i;
  for(i=0; i<6; i++){
    if(button_state[i] == HIGH)
      return 0;
  }
  return 1;
}

int max_state_empty(){
  int i;
  for(i=0; i<6; i++){
    if(button_max[i] == HIGH)
      return 0;
  }
  return 1;
}

void empty_max(){
  int i;
  for(i=0; i<6; i++){
    button_max[i] = 0;
  }
}

void OR_state_to_max(){
  int i;
  for(i=0; i<6; i++){
    button_max[i] |= button_state[i];
  }
}

int max_as_int(){
  int i;
  int sum = 0;
  for(i=5; i>=0; i--){
    sum = sum << 1;
    if(button_max[i] == HIGH){
      sum++;
    }
  }
  return sum;
}

void loop() {
  // read the pushbuttons:
  read_button_states();
  OR_state_to_max();
  
  //if all buttons are released print last maximum
  if(button_state_empty() == 1){
    //Keyboard.print("debug!");
    //if the last maximum is zero, it has already been printed
    if(max_state_empty() != 1){
	  //lots of debugging!
      //Serial.print(button_max[0]);
      //Serial.print(button_max[1]);
      //Serial.print(button_max[2]);
      //Serial.print(button_max[3]);
      //Serial.print(button_max[4]);
      //Serial.print(button_max[5]);
      //Serial.print("\n");
      //Serial.print(max_as_int());
      //Serial.print("\n");
      //Serial.print(lookup[max_as_int()]);
      //Serial.print("\n");
      Keyboard.print(lookup[max_as_int()]);
      empty_max();
    }
  }
  //we dont need this:
  delay(10);
}

