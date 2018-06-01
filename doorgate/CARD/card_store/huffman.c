#include "huffman.h"

void save( card_t *cards, int cardsRegistered )
{
	byte *data = calloc( cardsRegistered, sizeof( card_t ) );
	uint dataSize = cardListToByteArray( cards, cardsRegistered, data );
	
	int i;
	for( i = 0; i < dataSize; ++i )
	{
		printf("%d ", data[i]);
	}

	uint outputSize = 0;

	/*byte *compressedData = compress( data, dataSize, &outputSize );*/

	generateKey();

	/*encode( data, dataSize );*/

	writeToFile( data, dataSize, "database" );

}

int load( card_t *cards )
{
	printf("LLLL - %c\n", cards[0].first_name[0]);	
	uint inputSize = getSizeOfFile( "database" );

	byte *inputData;
	inputData = calloc( inputSize, sizeof( byte ) );
	
	readFromFile( inputData, &inputSize, "database" );

	/*decode( inputData, inputSize );*/

	/*
	uint decompressedSize = 0;

	byte* decompressedData = decompress( inputData, inputSize, 
															&decompressedSize );
	*/
	return byteArrayToCardList( inputData, cards, inputSize );
}

uint cardListToByteArray( card_t *cards, int cardsRegistered, byte *data )
{
	uint currentByte = 0;
	int i;
	for( i = 0; i < cardsRegistered; ++i )
	{
		int j;

		/* First Name */
		for( j = 0; j < MAX_NAME_SIZE; ++j )
		{
			printf("%d ", cards[i].first_name[j]);
			data[currentByte++] = cards[i].first_name[j];
			printf("%d ", data[currentByte-1]);
		}
		printf("\n");

		/* Initial */
		data[currentByte++] = cards[i].initial;

		/* Last Name */
		for( j = 0; j < MAX_NAME_SIZE; ++j )
		{
			data[currentByte++] = cards[i].last_name[j];
		}

		/* Card Number */
		byte* cardNumber = longLongIntToByteArray( cards[i].cardnumber );
		for( j = 0; j < sizeof( long long int ); ++j )
		{
			data[currentByte++] = cardNumber[j];
		}

		/* Month */
		byte* month = intToByteArray( cards[i].month );
		for( j = 0; j < sizeof( int ); ++j )
		{
			data[currentByte++] = month[j];
		}
		
		/* Year */
		byte* year = intToByteArray( cards[i].year );
		for( j = 0; j < sizeof( int ); ++j )
		{
			data[currentByte++] = year[j];
		}
		
		/* CVC */
		byte* cvc = intToByteArray( cards[i].cvc );
		for( j = 0; j < sizeof( int ); ++j )
		{
			data[currentByte++] = cvc[j];
		}
	}
	return currentByte - 1;
}
	
int byteArrayToCardList( byte* data, card_t *cards, uint dataSize )
{
	uint currentByte = 0;
	int i = 0;
	while(currentByte < dataSize )
	{
		int j;
		/* First Name */
		for( j = 0; j < MAX_NAME_SIZE; ++j )
		{
			cards[i].first_name[j] = data[currentByte++];
		}

		/* Initial */
		cards[i].initial = data[currentByte++];

		/* Last Name */
		for( j = 0; j < MAX_NAME_SIZE; ++j )
		{
			cards[i].last_name[j] = data[currentByte++];
		}

		/* Card Number */
		byte *buf = calloc( sizeof( long long int ) , sizeof( byte ) );
		for( j = 0; j < sizeof( long long int ); ++j )
		{
			printf("%d ", data[currentByte]);
			buf[j] = data[currentByte++];
		} 
		printf("\n");
		long long int f = byteArrayToLongLongInt( buf );
		printf("toLong %lld \n",f );
		cards[i].cardnumber =f;

		/* Month */
		buf = calloc( sizeof( int ), sizeof( byte )  ); 
		for( j = 0; j < sizeof( int ); ++j )
		{
			buf[j] = data[currentByte++];
		}
		cards[i].month = byteArrayToInt( buf );
		
		/* Year */
		buf = calloc( sizeof( int ), sizeof( byte )  ); 
		byte* year = intToByteArray( cards[i].year );
		for( j = 0; j < sizeof( int ); ++j )
		{
			buf[j] = data[currentByte++];
		}
		cards[i].year = byteArrayToInt( buf );
		
		/* CVC */
		buf = calloc( sizeof( int ), sizeof( byte )  ); 
		byte* cvc = intToByteArray( cards[i].cvc );
		for( j = 0; j < sizeof( int ); ++j )
		{
			buf[j] = data[currentByte++];
		}
		cards[i].cvc = byteArrayToInt( buf );
		
		cards[i].ID = get_ID();
		i++;
	}
	return i;
}
	
void showOffEncoding( byte *data, uint size )
{
	
	uint i;
	
	for( i = 0; i < size; ++i )
	{
		printf( "%c", data[i] );
	} 
	printf( "\n" );
	
	encode( data, size );

	for( i = 0; i < size; ++i )
	{
		printf( "%c", data[i] );
	} 
	printf( "\n" );

	decode( data, size );

	for( i = 0; i < size; ++i )
	{
		printf( "%c", data[i] );
	} 
	printf( "\n" );
}


byte* compress( byte *data, uint size, uint *dataSize ) 
{
	ByteList *byteList = createByteList( data, size );

	Node* node = createHuffmanTree( byteList );

	Code* code = createHuffmanCode( node );

	byte* buf = calloc( MAX_HEADER_SIZE + size , sizeof( byte ) );

	int currentByte = 0, currentBit = 0;
	
	addHuffmanHeaderToByteArray( buf, code, data, &currentByte );
	
	addHuffmanDataToByteArray( buf, code, data, 
								&currentByte, &currentBit, size );

	if( !( currentBit % 8 ) )
	{
		currentByte++;
	}
	
	byte* compressedData = calloc( currentByte, sizeof( byte ) );

	uint i;

	for ( i = 0; i < currentByte; i++ )
	{
		compressedData[i] = buf[i];
	}

	free( buf ); 

	*dataSize = currentByte;
	
	return compressedData;
}

byte* decompress( byte *data, uint size, uint *dataSize ) 
{
	uint currentByte = 0;
	byte numberOfElements = data[currentByte++];
	CodeElement *elements = calloc( numberOfElements, sizeof( CodeElement ) );
	/* Load Headers Into ByteListElements */
	uint i;

	
	for( i = 0; i < numberOfElements; ++i )
	{
		elements[i].data = data[currentByte++];
		elements[i].pathLength = data[currentByte++];
		elements[i].path = calloc( elements[i].pathLength, sizeof( byte* ) );
		uint pathLength =  ( elements[i].pathLength / 8 ) + 1;
		uint j;
		for ( j = 0; j < pathLength; ++j )
		{
			elements[i].path[j] = data[currentByte++];
		}
	}

	/* Load Data and Decompress */
	byte dataCountBytes[UINT_BYTES];

	for ( i = 0; i < UINT_BYTES; ++i )
	{
		dataCountBytes[i] = data[currentByte++];
	}
	
	uint dataCount = byteArrayToUint( dataCountBytes );

	byte* uncompressedData = calloc( dataCount, sizeof( byte ) );

	uint maxPathLength = 0;
	for ( i = 0; i < numberOfElements; ++i )
	{
		if( maxPathLength < elements[i].pathLength )
		{
			maxPathLength = elements[i].pathLength;
		}
	}
	
	maxPathLength = (maxPathLength / 8) + 1;


	int parsedDataCount = 0, depth = 0, bitOffSet = 0;
	byte* tempPath = calloc( maxPathLength, sizeof( byte ) ); 
	while ( parsedDataCount < dataCount )
	{
		int bit = getBit( data[currentByte], bitOffSet );
		bitOffSet++;
		if( bitOffSet == 8 )
		{
			currentByte++;
			bitOffSet = 0;
		}
		setBit( &(tempPath[ depth / 8 ]), depth, bit );
		int index = pathInCodeElements( elements, numberOfElements, 
													tempPath, depth);
		if( index != -1 )
		{
			uncompressedData[parsedDataCount++] = elements[index].data;	
			uint i;
			for( i = 0; i < maxPathLength; ++i )
			{
				tempPath[i] = 0;
			}
			depth = 0; 
		}
		else
		{
			depth++;
		}
	}
	*dataSize = dataCount;
	return uncompressedData;
}

int pathInCodeElements( CodeElement *elements, uint elementsCount,
							 byte *path, uint pathSize)
{
	int i, j;
	for( i = 0; i < elementsCount; ++i )
	{
		uint matches = 1;
		if(elements[i].pathLength != pathSize + 1)
		{
			matches=0;
		}

		int bytePathSize = elements[i].pathLength / 8;
		if( elements[i].pathLength % 8 )
		{
			++bytePathSize;
		}
		for( j = 0; j < bytePathSize; ++j )
		{
			if( path[j] != elements[i].path[j])
			{
				matches = 0;
			}
		}
		if( matches )
		{
			return i;
		}
	}
	return -1;
}

/*
Huffman Code Format
1 Byte - Number of Code Elements
FOR(Number of Code Elements)
{
	1 Byte - Data
	1 Byte - Path Length
	n Byte - Path
}
4 Byte - Number of Elements
x byte - Paths
*/

/*
	When it says n bytes then use the number stored above.
	e.g. 
		n Byte - Path
		n = Path Length

	x Bytes is untracked length, you keep reading for each number of elements
*/

void addHuffmanHeaderToByteArray(byte* compressedData, Code *code, byte* data, 
									int *currentByte )
{
	int byteOffSet = *currentByte;
	compressedData[byteOffSet++] = code->elementsCount;
	uint i;
	for( i = 0; i < code->elementsCount; ++i )
	{
		compressedData[byteOffSet++] = code->elements[i].data; 
		compressedData[byteOffSet++] = code->elements[i].pathLength;
		uint j;
		uint pathLength = (code->elements[i].pathLength / 8 );
		if ( (code->elements[i].pathLength % 8) )
		{
			++pathLength;
		}
		for( j = 0; j < pathLength; ++j)
		{
			compressedData[byteOffSet++] = code->elements[i].path[j]; 
		}
	}
	*currentByte = byteOffSet;
}


void addHuffmanDataToByteArray( byte *compressedData, Code *code, byte* data, 
								int *currentByte, int *currentBit, 
								uint dataSize )
{
	byte* num = uintToByteArray( dataSize + 1 );
	putBitsInByteArray( num, UINT_BYTES * BYTE_SIZE, compressedData, currentByte, 
							currentBit );

	
	uint i; 
	for( i = 0; i < dataSize ; ++i )
	{
		CodeElement* codeElement = getCodeForByte( code, data[i] );
		uint j;
		
		putBitsInByteArray( codeElement->path, codeElement->pathLength, 
								compressedData, currentByte, currentBit );
	}

}

CodeElement* getCodeForByte( Code* code, byte data )
{
	uint i;
	for( i = 0; i < code->elementsCount; ++i )
	{
		if(code->elements[i].data == data )
		{
			return &(code->elements[i]);
		}
	}
}

byte* longLongIntToByteArray( long long int data )
{
	byte *bytes = calloc( sizeof( long long int ), sizeof( byte ) );

	uint i;
	for( i = 0; i < sizeof( long long int ); ++i )
	{
		uint temp = data;
		bytes[sizeof( long long int ) - i] = ( data >> ( (sizeof( long long int ) - i) * 8 ) ) 
						& 0xFF;
	}
	return bytes;
}


byte* intToByteArray( int data )
{
	byte *bytes = calloc( sizeof( int ), sizeof( byte ) );

	uint i;
	for( i = 0; i < sizeof( int ); ++i )
	{
		uint temp = data;
		bytes[sizeof( long long int ) - i] = ( data >> ( (sizeof( int ) - i) * 8 ) ) & 0xFF;
	}
	return bytes;
}

long long int byteArrayToLongLongInt( byte *bytes )
{
	long long int num;
	uint i;
	for( i = 0; i < sizeof( long long int ); ++i )
	{
		num |= ( long long int )bytes[i] << 8 * i;
	}

	return num;
}

int byteArrayToInt( byte *bytes )
{
	int num;
	uint i;
	for( i = 0; i < sizeof( int ); ++i )
	{
		num |= ( int )bytes[i] << 8 * i;
	}

	return num;
}

byte* uintToByteArray( uint data )
{
	byte *bytes = calloc( UINT_BYTES, sizeof( byte ) );

	uint i;
	for( i = 0; i < UINT_BYTES; ++i )
	{
		uint temp = data;
		bytes[i] = ( data >> ( (UINT_BYTES - i) * 8 ) ) & 0xFF;
	}
	return bytes;
}

uint byteArrayToUint( byte* bytes )
{
	uint num;

    num = bytes[0] | (uint)bytes[1] << 8 | (uint)bytes[2] << 16 | 
					 (uint)bytes[3] << 24;

	return num;
}

void showOffCode( Code *code )
{
	/*printf( "BytesForPath = %u\n", code->bytesForPath );*/
	uint i, j;
	for( i = 0; i < code->elementsCount; ++i )
	{
		printf( "Node = %u\n", i );
		printf( "Data = %c\n", code->elements[i].data );
		printf( "PathLength = %u\n", code->elements[i].pathLength );
		printf( "Path = " );
		for( j = 0; j < code->bytesForPath; ++j )
		{
			printDecToBin( code->elements[i].path[j]);
			printf( " ");
		}
		printf( "\n" );
	}
}

void printDecToBin( byte  dec )
{
	int i;
	for( i = 0; i < 8; ++i )
	{
		printf("%d", getBit( dec, i ));
	}
}
