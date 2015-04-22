/*
SoLoud audio engine
Copyright (c) 2013-2015 Jari Komppa

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.

2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include <stdio.h>
#include <string.h>
#include "soloud.h"
#include "soloud_file.h"

namespace SoLoud
{
	unsigned int File::read_uint8()
	{
		unsigned char d = 0;
		read((unsigned char*)&d, 1);
		return d;
	}

	unsigned int File::read_uint16()
	{
		unsigned short d = 0;
		read((unsigned char*)&d, 2);
		return d;
	}

	unsigned int File::read_uint32()
	{
		unsigned int d = 0;
		read((unsigned char*)&d, 4);
		return d;
	}


	unsigned int DiskFile::read(unsigned char *aDst, unsigned int aBytes)
	{
		return fread(aDst, 1, aBytes, mFileHandle);
	}

	unsigned int DiskFile::length()
	{
		int pos = ftell(mFileHandle);
		fseek(mFileHandle, SEEK_END, 0);
		int len = ftell(mFileHandle);
		fseek(mFileHandle, SEEK_SET, pos);
		return len;
	}

	void DiskFile::seek(int aOffset)
	{
		fseek(mFileHandle, SEEK_SET, aOffset);
	}

	unsigned int DiskFile::pos()
	{
		return ftell(mFileHandle);
	}

	DiskFile::~DiskFile()
	{
		if (mFileHandle)
			fclose(mFileHandle);
	}

	DiskFile::DiskFile()
	{
		mFileHandle = 0;
	}

	result DiskFile::open(char *aFilename)
	{
		mFileHandle = fopen(aFilename, "rb");
		if (!mFileHandle)
			return FILE_NOT_FOUND;
		return SO_NO_ERROR;
	}



	unsigned int MemoryFile::read(unsigned char *aDst, unsigned int aBytes)
	{
		if (mOffset + aBytes >= mDataLength)
			aBytes = mDataLength - mOffset;

		memcpy(aDst, mDataPtr + mOffset, aBytes);
		mOffset += aBytes;

		return aBytes;
	}

	unsigned int MemoryFile::length()
	{
		return mDataLength;
	}

	void MemoryFile::seek(int aOffset)
	{
		if (aOffset >= 0)
			mOffset = aOffset;
		else
			mOffset = mDataLength + aOffset;
		if (mOffset > mDataLength-1)
			mOffset = mDataLength-1;
	}

	unsigned int MemoryFile::pos()
	{
		return mOffset;
	}

	MemoryFile::~MemoryFile()
	{
		if (mDataOwned)
			delete[] mDataPtr;
	}

	MemoryFile::MemoryFile()
	{
		mDataPtr = 0;
		mDataLength = 0;
		mOffset = 0;
		mDataOwned = false;
	}

	result MemoryFile::open(unsigned char *aData, unsigned int aDataLength, bool aCopy, bool aTakeOwnership)
	{
		if (aData == NULL || aDataLength == 0)
			return INVALID_PARAMETER;

		if (mDataOwned)
			delete[] mDataPtr;
		mDataPtr = 0;

		mDataLength = aDataLength;

		if (aCopy)
		{
			mDataOwned = true;
			mDataPtr = new unsigned char[aDataLength];
			if (mDataPtr == NULL)
				return OUT_OF_MEMORY;
			memcpy(mDataPtr, aData, aDataLength);
			return SO_NO_ERROR;			
		}

		mDataPtr = aData;
		mDataOwned = aTakeOwnership;
		return SO_NO_ERROR;
	}

	result MemoryFile::open(File *aFile)
	{
		if (aFile == NULL)
			return INVALID_PARAMETER;

		if (mDataOwned)
			delete[] mDataPtr;
		mDataPtr = 0;

		mDataLength = aFile->length();
		mDataPtr = new unsigned char[mDataLength];
		if (mDataPtr == NULL)
			return OUT_OF_MEMORY;
		int pos = aFile->pos();
		aFile->seek(0);
		aFile->read(mDataPtr, mDataLength);
		aFile->seek(pos);
		mDataOwned = true;
		return SO_NO_ERROR;
	}
}

