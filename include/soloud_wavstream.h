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

#ifndef SOLOUD_WAVSTREAM_H
#define SOLOUD_WAVSTREAM_H

#include <stdio.h>
#include "soloud.h"


struct stb_vorbis;

namespace SoLoud
{
	class WavStream;
	class File;

	class WavStreamInstance : public AudioSourceInstance
	{
		WavStream *mParent;
		unsigned int mOffset;
		File *mFile;
		stb_vorbis *mOgg;
		unsigned int mOggFrameSize;
		unsigned int mOggFrameOffset;
		float **mOggOutputs;
	public:
		WavStreamInstance(WavStream *aParent);
		virtual void getAudio(float *aBuffer, unsigned int aSamples);
		virtual result rewind();
		virtual bool hasEnded();
		virtual ~WavStreamInstance();
	};

	class WavStream : public AudioSource
	{
		result loadwav(File * fp);
		result loadogg(File * fp);
	public:
		int mOgg;
		char *mFilename;
		File *mMemFile;
		File *mStreamFile;
		unsigned int mDataOffset;
		unsigned int mBits;
		unsigned int mSampleCount;

		WavStream();
		virtual ~WavStream();
		result load(const char *aFilename);
		result loadMem(unsigned char *aData, unsigned int aDataLen, bool aCopy = false, bool aTakeOwnership = true);
		result loadToMem(const char *aFilename);
		result loadFile(File *aFile);
		result loadFileToMem(File *aFile);		
		virtual AudioSourceInstance *createInstance();
		time getLength();
	public:
		result parse(File *aFile);
	};
};

#endif