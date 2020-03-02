/*
AY module for SoLoud audio engine
Copyright (c) 2020 Jari Komppa

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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "sndbuffer.h"
#include "sndrender.h"
#include "sndchip.h"
#include "chipplayer.h"
#include "soloud_ay.h"
#include "soloud_file.h"

namespace SoLoud
{

	AyInstance::AyInstance(Ay *aParent)
	{
		mParent = aParent;
		mChip = new ChipPlayer(aParent->mFile);
	}

	unsigned int AyInstance::getAudio(float *aBuffer, unsigned int aSamplesToRead, unsigned int /*aBufferSize*/)
	{
		int samples = mChip->play(aBuffer, aSamplesToRead);
		return samples;
	}
	
	bool AyInstance::hasEnded()
	{
		return mParent->mFile->eof();
	}

	result AyInstance::rewind()
	{
		mParent->mFile->seek(4);
		// Reset the chip.
		delete mChip;
		mChip = new ChipPlayer(mParent->mFile);
		return SO_NO_ERROR;
	}

	float AyInstance::getInfo(unsigned int aInfoKey)
	{
		if ((aInfoKey & 0xf) <= 13)
		{
			if (aInfoKey < 0x10)
				return mChip->chip.get_reg(aInfoKey);
			if (aInfoKey & 0x10)
				return mChip->chip2.get_reg(aInfoKey & 0xf);
		}
		return 0;
	}


	AyInstance::~AyInstance()
	{
		delete mChip;
	}

	Ay::Ay()
	{
		mBaseSamplerate = 44100;
		mChannels = 2;
		mFile = 0;
		mFileOwned = false;
	}

	Ay::~Ay()
	{
		stop();
		if (mFileOwned)
			delete mFile;
	}

	result Ay::loadMem(const unsigned char *aMem, unsigned int aLength, bool aCopy, bool aTakeOwnership)
	{
		if (!aMem || aLength == 0)
			return INVALID_PARAMETER;
		MemoryFile *mf = new MemoryFile;
		if (!mf)
			return OUT_OF_MEMORY;
		int res = mf->openMem(aMem, aLength, aCopy, aTakeOwnership);
		if (res != SO_NO_ERROR)
		{
			delete mf;
			return res;
		}
		res = loadFile(mf);
		if (res != SO_NO_ERROR)
		{
			delete mf;
			return res;
		}
		mFileOwned = aCopy || aTakeOwnership;

		return SO_NO_ERROR;
	}

	result Ay::load(const char *aFilename)
	{
		if (!aFilename)
			return INVALID_PARAMETER;
		DiskFile *df = new DiskFile;
		if (!df) return OUT_OF_MEMORY;
		int res = df->open(aFilename);
		if (res != SO_NO_ERROR)
		{
			delete df;
			return res;
		}
		res = loadFile(df);
		if (res != SO_NO_ERROR)
		{
			delete df;
			return res;
		}
		mFileOwned = true;				
		return SO_NO_ERROR;
	}

	result Ay::loadToMem(const char *aFilename)
	{
		if (!aFilename)
			return INVALID_PARAMETER;
		MemoryFile *mf = new MemoryFile;
		if (!mf) return OUT_OF_MEMORY;
		int res = mf->openToMem(aFilename);
		if (res != SO_NO_ERROR)
		{
			delete mf;
			return res;
		}
		res = loadFile(mf);
		if (res != SO_NO_ERROR)
		{
			delete mf;
			return res;
		}
		mFileOwned = true;
		return SO_NO_ERROR;
	}

	result Ay::loadFileToMem(File *aFile)
	{
		if (!aFile)
			return INVALID_PARAMETER;
		MemoryFile *mf = new MemoryFile;
		if (!mf) return OUT_OF_MEMORY;
		int res = mf->openFileToMem(aFile);
		if (res != SO_NO_ERROR)
		{
			delete mf;
			return res;
		}
		res = loadFile(mf);
		if (res != SO_NO_ERROR)
		{
			delete mf;
			return res;
		}
		mFileOwned = true;
		return SO_NO_ERROR;
	}

	result Ay::loadFile(File *aFile)
	{
		if (aFile == NULL)
			return INVALID_PARAMETER;
		if (mFileOwned)
			delete mFile;
		// Expect a file wih header and at least one reg write
		if (aFile->length() < 4+4+2) return FILE_LOAD_FAILED;

		aFile->seek(0);
		if (aFile->read8() != 'D') return FILE_LOAD_FAILED;
		if (aFile->read8() != 'U') return FILE_LOAD_FAILED;
		if (aFile->read8() != 'm') return FILE_LOAD_FAILED;
		if (aFile->read8() != 'p') return FILE_LOAD_FAILED;

		mFile = aFile;
		mFileOwned = false;


		return SO_NO_ERROR;
	}

	AudioSourceInstance * Ay::createInstance() 
	{
		return new AyInstance(this);
	}

};