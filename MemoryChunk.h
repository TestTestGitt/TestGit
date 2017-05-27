#pragma once
#include <iostream>
#include <assert.h>
using namespace std;



class MemoryChunk;
struct BlockHeader
{
	MemoryChunk *pMChunck;
	size_t size;
};
struct MemoryBlock;
struct BlockData
{
	union {
		MemoryBlock* pNext;
		char pBuffer;
	};
};
struct MemoryBlock
{
	BlockHeader header;
	BlockData data;
};

class MemoryChunk
{
public:
	MemoryChunk(size_t size, int counts)
	{
		pFreeList = NULL;
		this->size = size;
		this->count = counts;
		MemoryBlock *pBlock = NULL;
		while (counts--)
		{
			pBlock = createBlock();
			if (!pBlock)
			{
				break;
			}
			pBlock->data.pNext = pFreeList;
			pFreeList = pBlock;
		}
	}
	~MemoryChunk()
	{
		int tempCounts = 0;
		MemoryBlock *pBlock = NULL;
		while (pFreeList)
		{
			pBlock = pFreeList;
			pFreeList = pBlock->data.pNext;
			DeleteBlock(pBlock);
			++tempCounts;
		}
		assert(tempCounts == count);
	}

	void* MemoryBlockMalloc()
	{
		MemoryBlock *pBlock = NULL;
		if (pFreeList)
		{
			pBlock = pFreeList;
			pFreeList = pBlock->data.pNext;
		}
		else
		{
			pBlock = createBlock();
			if (!pBlock)
				return NULL;
		}

		return &pBlock->data.pBuffer;
	}
	static void free(void *pMem)
	{
		MemoryBlock *pBlock = (MemoryBlock*)((char*)pMem - sizeof(BlockHeader) - sizeof(BlockData));
		if (pBlock)
		{
			pBlock->header.pMChunck->Free(pBlock);
		}
	}
	void Free(MemoryBlock *pBlock)
	{
		pBlock->data.pNext = pFreeList;
		pFreeList = pBlock;
	}

protected:
	MemoryBlock* createBlock()
	{
		MemoryBlock *p = (MemoryBlock*)malloc(sizeof(BlockHeader) + sizeof(BlockData) + size);
		if (p)
		{
			p->header.pMChunck = this;
			p->header.size = size;

			++count;
		}
		return p;
	}
	void DeleteBlock(MemoryBlock *pMem)
	{
		delete pMem;
		pMem = NULL;
	}
private:
	size_t size;
	int count;
	MemoryBlock *pFreeList;
	//MemoryChunk *pNext;
};

