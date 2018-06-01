#ifndef HUFFMAN_CODE
#define HUFFMAN_CODE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dataTypes.h"
#include "bitOperations.h"

Code* createHuffmanCode( Node* ); 
void populateHuffmanCode( Code*, Node* );
uint findTreeDepth( Node* );
uint countLeafs( Node* );
void nodeToCode( Code*, Node*, uint, byte*, uint* );
void getLeafNodes( Node*, Node**, uint );
void setCode( Code*, Node*, byte*, uint, uint );

#endif /* HUFFMAN_CODE*/
