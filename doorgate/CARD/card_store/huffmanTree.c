#include "huffmanTree.h"

Node* createHuffmanTree( ByteList* byteList )
{
	if (byteList == NULL || byteList->elements == NULL)
	{
		return;
	}

	Node **roots = malloc( sizeof( Node* ) *  (byteList->size)  );

	uint i;
	for( i = 0; i < byteList->size; ++i )
	{
		
		roots[i] = makeLeaf( &byteList->elements[i] );
	}

	uint numberOfRoots = countRoots( roots, byteList->size );
	while (numberOfRoots > 1)
	{
		uint smallest = findSmallestRoot( roots, numberOfRoots );
		uint secondSmallest = findSecondSmallestRoot( roots, numberOfRoots );
		combineRoots( roots, byteList->size, smallest, secondSmallest );
		numberOfRoots--;
	}
	
	return roots[0];
}

uint countRoots( Node** roots, uint size )
{
	uint count = 0;
	while( count < size && roots[count++] != NULL );
	return count;
}

Node* makeLeaf( ByteListElement* element )
{
	Node *leaf = malloc( sizeof( Node ) );
	leaf->count = element->count;
	leaf->data = element->data;
	return leaf;
}

void addRoot( Node** roots, uint index, Node* left, Node* right )
{
	Node *newRoot = malloc( sizeof( Node ) );
	newRoot->right = right;
	newRoot->left = left;
	newRoot->count = left->count + right->count;
	roots[index] = newRoot;
}

void removeRoot( Node** roots, uint size, uint index)
{
	roots[index] = NULL;
	uint i;
	for (i = index; i < size-1; ++i)
	{
		roots[i] = roots[i+1];
	}
}

void combineRoots( Node** roots, uint size, uint firstIndex, uint secondIndex )
{
	if (firstIndex < secondIndex)
	{
		addRoot( roots, firstIndex, roots[firstIndex], roots[secondIndex] );
		removeRoot( roots, size, secondIndex );
	}
	else
	{
		addRoot( roots, secondIndex, roots[secondIndex], roots[firstIndex] );
		removeRoot( roots, size, firstIndex );
	}
}

uint findSmallestRoot( Node** roots, uint numberOfRoots )
{
	uint i, smallestIndex = 0;
	for (i = 1; i < numberOfRoots; ++i)
	{
		if (roots[i]->count < roots[smallestIndex]->count)
		{
			smallestIndex = i;
		}
	}
	return smallestIndex;
}

uint findSecondSmallestRoot( Node** roots, uint numberOfRoots )
{
	if(numberOfRoots < 2)
		return;

	uint i, smallestIndex = 0, secondSmallestIndex = 1;
	if( roots[smallestIndex] > roots[secondSmallestIndex])
	{
		smallestIndex = 1;
		secondSmallestIndex = 0;
	}

	for (i = 2; i < numberOfRoots; ++i)
	{
		if (roots[i]->count < roots[smallestIndex]->count)
		{
			secondSmallestIndex = smallestIndex;
			smallestIndex = i;
		}
		else if (roots[i]->count < roots[secondSmallestIndex]->count)
		{
			secondSmallestIndex = i;
		}
	}
	return secondSmallestIndex;
}


void printHuffmanTree( Node *node )
{
	printf( "Node Count - %u\n", node->count );
	printf( "Node Data - %c\n", node->data );
	if( node->left != NULL)
	{
		printf("Left\n");
		printHuffmanTree( node->left );
	}
	if( node->right != NULL)
	{
		printf("Right\n");
		printHuffmanTree( node->right );
	}
}
