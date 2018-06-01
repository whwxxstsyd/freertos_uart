#include "bitOperations.h"

void setBit( byte *data, int bit, int value )
{
	/* Swap the endianess of the bit so it writes from left to right */
	if( bit < 0 || bit > 7 || ( value != 0 && value != 1 ) )
	{
		/*printf("ERROR");*/
		return;
	}
	*data ^= (-value ^ *data ) & ( 1 << bit );
}

int getBit( byte data, int bit )
{
	return (data >> bit ) & 1;
}

void putBitsInByteArray( byte* bytes, uint numberOfBits, byte* byteArray, 
							int *byteOffSet, int *bitOffSet )
{
	/* We need offset trackers for both the input and the output */
	uint i, inputByteOffSet = 0, inputBitOffSet = 0;
	for( i = 0; i < numberOfBits; ++i )
	{
		/* Get the next bit from the input */
		int bitValue = getBit( bytes[inputByteOffSet], inputBitOffSet );

		/* Set the next bit in the output */
		setBit( &(byteArray[*byteOffSet]), *bitOffSet, bitValue );
		
		/* Input OffSet Tracking */
		inputBitOffSet++; 
		if( !(inputBitOffSet % 8) )
		{
			inputByteOffSet++;
			inputBitOffSet = 0;
		}

		/* Output OffSet Tracking */
		(*bitOffSet)++;
		if( !(*bitOffSet % 8) )
		{
			(*byteOffSet)++;
			(*bitOffSet) = 0;
		}
	}
}

