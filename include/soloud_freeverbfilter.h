/*
SoLoud audio engine
Copyright (c) 2013-2020 Jari Komppa

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

#ifndef SOLOUD_FREEVERBFILTER_H
#define SOLOUD_FREEVERBFILTER_H

#include "soloud.h"

namespace SoLoud
{
	class FreeverbFilter;
	namespace FreeverbImpl
	{
		class Revmodel;
	}

	class FreeverbFilterInstance : public FilterInstance
	{
		enum {
			WET = 0,
			MODE,
			ROOMSIZE,
			DAMP,
			WIDTH
		};		

		FreeverbFilter *mParent;
		FreeverbImpl::Revmodel *mModel;
	public:
		virtual void filter(float* aBuffer, unsigned int aSamples, unsigned int aChannels, float aSamplerate, time aTime);		
		virtual ~FreeverbFilterInstance();
		FreeverbFilterInstance(FreeverbFilter *aParent);
	};

	class FreeverbFilter : public Filter
	{
	public:
		enum {
			WET = 0,
			MODE,
			ROOMSIZE,
			DAMP,
			WIDTH
		};

		float mMode;
		float mRoomSize;
		float mDamp;
		float mWidth;
		virtual FreeverbFilterInstance *createInstance();
		FreeverbFilter();
		result setParams(float aMode, float aRoomSize, float aDamp, float aWidth);
		virtual ~FreeverbFilter();
	};
}

#endif