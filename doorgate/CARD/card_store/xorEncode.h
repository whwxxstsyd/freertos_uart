#ifndef XOR_ENCODE
#define XOR_ENCODE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dataTypes.h"

void encode( byte*, uint );
void decode( byte*, uint );
byte* loadKey();

#endif /* XOR_ENCODE */
