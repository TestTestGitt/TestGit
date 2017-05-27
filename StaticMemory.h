#pragma once

#include "MemoryChunk.h"

struct HeapHeader
{
	size_t len;
};

struct MemoryHeap
{
	HeapHeader header;
	char pBuffer;
};

class StaticMemory
{
public:
	typedef enum{MAX_SIZE = 1024,MIN_SIZE = sizeof(MemoryChunk*) };
	StaticMemory()
	{
		chunkCounts = 0;
		for (size_t size = MAX_SIZE;size <= MAX_SIZE;size *= 2)
		{
			++chunkCounts;
		}
		pMemChunkList = new MemoryChunk *[chunkCounts];
		int index = 0;
		for (size_t size = MAX_SIZE; size <= MAX_SIZE; size *= 2)
		{
			pMemChunkList[index] = new MemoryChunk(size, 1000);
			++index;
		}
		
	}
	~StaticMemory()
	{

	
		for (int index = 0; index <= chunkCounts;index++)
		{
			delete pMemChunkList[index];
		}
		delete[] pMemChunkList;
	}
	void* Malloc(size_t size)
	{

		if (size > MAX_SIZE)
		{
			return malloc(size);
		}
		int index = 0;
		for (size_t s = MAX_SIZE; s <= MAX_SIZE; s *= 2)
		{
			if(s > size)
				break;
			++index;
		}
		

		return pMemChunkList[index]->MemoryBlockMalloc();
	}

	void Free(void* pMem)
	{
		if (!free(pMem))
			MemoryChunk::free(pMem);
	}
protected:
	void* malloc(size_t size)
	{
		MemoryHeap *pHeap = (MemoryHeap*)::malloc( sizeof(HeapHeader) + size );
		if (pHeap)
		{
			pHeap->header.len= size;
			return &pHeap->pBuffer;
		}
		return NULL;
	}

	bool free(void *pMem)
	{
		MemoryHeap *pHeap = (MemoryHeap*)((char*)pMem - sizeof(HeapHeader));
		if (pHeap->header.len > MAX_SIZE)
		{
			::free(pHeap);
			pHeap = NULL;
			return true;
		}
		return false;
	}
private:
	MemoryChunk **pMemChunkList;
	int chunkCounts;
};

