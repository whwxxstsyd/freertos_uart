#include "xorEncode.h"

void encode( byte* data, uint size )
{
	byte* key = loadKey();
	if (key == NULL)
	{
		printf( "Could not load key file.\n" );
		return;
	}

	uint i;
	for( i = 0; i < size; ++i)
	{
		data[i] = data[i] ^ *key;
	}

	free( key );
}

void decode( byte* data, uint size )
{
	encode( data, size );
}

byte* loadKey()
{
	FILE *fp = fopen( "xor.key", "r" );
	if (fp == NULL)
	{
		return NULL;	
	}

	byte* key = malloc( sizeof( byte ) );
	*key = (byte)fgetc( fp );
	return key;
}

