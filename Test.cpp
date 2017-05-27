#pragma once

#include "MemoryChunk.h"
#include "StaticMemory.h"
//#include <iostream>
//using namespace std;
////
//struct BlockData
//{
//	union {
//		int* pNext;
//		char pBuffer;
//	};
//};
int main()
{
	int m = 10;
	BlockData b;
	MemoryBlock *pNe = new MemoryBlock();
	b.pNext = pNe;
	b.pBuffer = 'a';

	cout << sizeof(b) << endl;


	return 0;
}