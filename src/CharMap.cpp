#include "BraillePad.h"

/* Global variables */
char BrailleAlphabetMap[64] = {0, 0, 0, 0,   0, 0, 0, 0,
                               0, 0, 0, 0,   0, 0, 0, 0,
                               0, 0, 0, 0,   0, 0, 0, 0,
                               0, 0, 0, 0,   0, 0, 0, 0,

                               0, 0, 0, 0,   0, 0, 0, 0,
                               0, 0, 0, 0,   0, 0, 0, 0,
                               0, 0, 0, 0,   0, 0, 0, 0,
                               0, 0, 0, 0,   0, 0, 0, 0};
char BrailleNumericMap[64] = {0, 0, 0, 0,   0, 0, 0, 0,
                              0, 0, 0, 0,   0, 0, 0, 0,
                              0, 0, 0, 0,   0, 0, 0, 0,
                              0, 0, 0, 0,   0, 0, 0, 0,

                              0, 0, 0, 0,   0, 0, 0, 0,
                              0, 0, 0, 0,   0, 0, 0, 0,
                              0, 0, 0, 0,   0, 0, 0, 0,
                              0, 0, 0, 0,   0, 0, 0, 0};
char BrailleComputerMap[64] = {0, 0, 0, 0,   0, 0, 0, 0,
                               0, 0, 0, 0,   0, 0, 0, 0,
                               0, 0, 0, 0,   0, 0, 0, 0,
                               0, 0, 0, 0,   0, 0, 0, 0,

                               0, 0, 0, 0,   0, 0, 0, 0,
                               0, 0, 0, 0,   0, 0, 0, 0,
                               0, 0, 0, 0,   0, 0, 0, 0,
                               0, 0, 0, 0,   0, 0, 0, 0};


/* Function declarations */
int insert_to_map(const unsigned char braille_code, const unsigned char corresponds_to, char * const map);

/* Defines the alphabet */
unsigned int GenerateAlphabetMap()
{
  unsigned int ret_val = 0;

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            O, O,
                                            O, O), 'a', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            X, O,
                                            O, O), 'b', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            O, O,
                                            O, O), 'c', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            O, X,
                                            O, O), 'd', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            O, X,
                                            O, O), 'e', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            X, O,
                                            O, O), 'f', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            X, X,
                                            O, O), 'g', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            X, X,
                                            O, O), 'h', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(O, X,
                                            X, O,
                                            O, O), 'i', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(O, X,
                                            X, X,
                                            O, O), 'j', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            O, O,
                                            X, O), 'k', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            X, O,
                                            X, O), 'l', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            O, O,
                                            X, O), 'm', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            O, X,
                                            X, O), 'n', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            O, X,
                                            X, O), 'o', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            X, O,
                                            X, O), 'p', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            X, X,
                                            X, O), 'q', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            X, X,
                                            X, O), 'r', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(O, X,
                                            X, O,
                                            X, O), 's', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            O, X,
                                            X, X), 'z', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(O, X,
                                            X, X,
                                            X, O), 't', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            O, O,
                                            X, X), 'u', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            X, O,
                                            X, X), 'v', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(O, X,
                                            X, X,
                                            O, X), 'w', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            O, O,
                                            X, X), 'x', BrailleAlphabetMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            O, X,
                                            X, X), 'y', BrailleAlphabetMap);
//punctuation
	ret_val += insert_to_map(GET_BRAILLE_CODE(O, O,
											  X, X,
											  O, X), '.', BrailleAlphabetMap);
	ret_val += insert_to_map(GET_BRAILLE_CODE(O, O,
											  X, O,
											  O, O), ',', BrailleAlphabetMap);
	ret_val += insert_to_map(GET_BRAILLE_CODE(O, O,
											  X, X,
											  X, O), '!', BrailleAlphabetMap);
//XXX: NON-STANDARD CHORDS!
	ret_val += insert_to_map(GET_BRAILLE_CODE(O, O,
											  O, O,
											  X, O), ' ', BrailleAlphabetMap);
	ret_val += insert_to_map(GET_BRAILLE_CODE(O, O,
											  X, X,
											  X, X), '#', BrailleAlphabetMap);
	ret_val += insert_to_map(GET_BRAILLE_CODE(O, O,
											  O, X,
											  O, O), KEY_RETURN, BrailleAlphabetMap);
	ret_val += insert_to_map(GET_BRAILLE_CODE(O, X,
											  O, O,
											  O, O), KEY_BACKSPACE, BrailleAlphabetMap);
  return ret_val;
}

/* Defines the numbers */
unsigned int GenerateNumericMap()
{
  unsigned int ret_val = 0;

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            O, O,
                                            O, O), '1', BrailleNumericMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            X, O,
                                            O, O), '2', BrailleNumericMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            O, O,
                                            O, O), '3', BrailleNumericMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            O, X,
                                            O, O), '4', BrailleNumericMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            O, X,
                                            O, O), '5', BrailleNumericMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            X, O,
                                            O, O), '6', BrailleNumericMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, X,
                                            X, X,
                                            O, O), '7', BrailleNumericMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(X, O,
                                            X, X,
                                            O, O), '8', BrailleNumericMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(O, X,
                                            X, O,
                                            O, O), '9', BrailleNumericMap);

  ret_val += insert_to_map(GET_BRAILLE_CODE(O, X,
                                            X, X,
                                            O, O), '0', BrailleNumericMap);

  return ret_val;
}

unsigned int GenerateComputerMap()
{
  return 0;
}

int insert_to_map(const unsigned char braille_code, const unsigned char corresponds_to, char * const map)
{
  int ret_val = SUCCESS;

  if (map[braille_code] == 0)
  {
    map[braille_code] = corresponds_to;
  }
  else /* previous definition prevails */
  {
    ret_val = MULTIPLE_DEF;
  }

  return ret_val;
}
