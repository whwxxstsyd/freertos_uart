#ifndef HUFFMAN
#define HUFFMAN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "creditCard.h"
#include "dataTypes.h"
#include "bytelist.h"
#include "huffmanTree.h"
#include "huffmanCode.h"
#include "keyGen.h"

void save( card_t *cards, int cardsRegistered );
int load( card_t *cards );
uint cardListToByteArray( card_t *cards, int cardsRegistered, byte *data );
int byteArrayToCardList( byte* decompressedData, card_t *cards, uint dataSize );
byte* longLongIntToByteArray( long long int data );
long long int byteArrayToLongLongInt( byte *bytes );
byte* intToByteArray( int data );
int byteArrayToInt( byte *bytes );
byte* compress( byte*, uint, uint* );
byte* decompress( byte*, uint, uint* );
CodeElement* getCodeForByte( Code*, byte ); 
void showOffEncoding( byte*, uint );
void showOffCode( Code* );
void printDecToBin( byte );
byte* uintToByteArray( uint data );
uint byteArrayToUint( byte* bytes );
int pathInCodeElements( CodeElement *elements, uint elementsCount,
							 byte *path, uint pathSize);
void addHuffmanHeaderToByteArray(byte* compressedData, Code *code, byte* data, 
									int *currentByte );
void addHuffmanDataToByteArray( byte* compressedData, Code *code, byte* data, 
									int *currentByte, int *currentBit,
									uint dataSize );
#endif /* HUFFMAN */
