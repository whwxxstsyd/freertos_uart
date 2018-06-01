#ifndef HUFFMAN_TREE
#define HUFFMAN_TREE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dataTypes.h"

Node* createHuffmanTree( ByteList* );
void printHuffmanTree( Node* );
uint findSmallestRoot( Node**, uint );
uint findSecondSmallestRoot( Node**, uint );
void addRoot( Node**, uint, Node*, Node* );
void removeRoot( Node**, uint, uint );
void combineRoots( Node**, uint, uint, uint );
Node* makeLeaf( ByteListElement* );
uint countRoots( Node**, uint );


#endif /* HUFFMAN_TREE */
