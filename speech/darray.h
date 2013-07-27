#if !defined(DARRAY_H)
#define DARRAY_H

class darray
{
protected:
	char *mData;
	int mUsed;
	int mAllocated;
	int mAllocChunk;
public:
	darray();
	~darray();
	char *getDataInPos(int aPosition);
	void put(int aData);
	int getSize() { return mUsed; }
	char *getData() { return mData; }
};

#endif

