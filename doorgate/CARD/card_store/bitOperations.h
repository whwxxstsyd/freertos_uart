#ifndef BIT_OPERATIONS 
#define BIT_OPERATIONS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dataTypes.h"

void setBit( byte*, int, int );
int getBit( byte, int );
void putBitsInByteArray( byte* bytes, uint numberOfBits, byte* byteArray, 
							int *byteOffSet, int *bitOffSet );

#endif /* BIT_OPERATIONS */
