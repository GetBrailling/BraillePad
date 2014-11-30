#ifndef _BraillePad_H_
#define _BraillePad_H_
#include "Arduino.h"
/* Custom includes for the BraillePad project */

/* Macros */
#define GET_BRAILLE_CODE(b1, b4,\
                         b2, b5,\
                         b3, b6)\
  ((b1) | ((b2)<<1) | ((b3)<<2) | ((b4)<<3) | ((b5)<<4) | ((b6)<<5))

typedef enum MODE {
	ALPHABET,//default
	CAPITAL,
	NUMBER
}MODE;


/* Constants */
#define TRUE 1u
#define FALSE 0u
#define X 1u
#define O 0u
#define SUCCESS 0u
#define MULTIPLE_DEF 1u
#define NUMBER_SIGN (GET_BRAILLE_CODE(O, X,\
                                      O, X,\
                                      X, X))
#define CAPITAL_SIGN (GET_BRAILLE_CODE(0, 0,\
									   0, 0,\
									   0, 1))

/* Variable declarations */
extern char BrailleAlphabetMap[64];
extern char BrailleNumericMap[64];
extern char BrailleComputerMap[64];

/* Function declarations */
/* ... defined in CharMap.cpp */
unsigned int GenerateAlphabetMap();
unsigned int GenerateNumericMap();
unsigned int GenerateComputerMap();

//Do not add code below this line
#endif /* _BraillePad_H_ */
