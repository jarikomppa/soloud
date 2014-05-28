/*
SoLoud audio engine
Copyright (c) 2013-2014 Jari Komppa

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

#ifndef SOLOUD_FILTER_H
#define SOLOUD_FILTER_H

namespace SoLoud
{
	class Fader;

	class FilterInstance
	{
	public:
		int mNumParams;
		int mParamChanged;
		float *mParam;
		Fader *mParamFader;
		
		FilterInstance();
		virtual int initParams(int aNumParams);
		virtual void updateParams(double aTime);
		virtual void filter(float *aBuffer, int aSamples, int aChannels, float aSamplerate, double aTime);
		virtual void filterChannel(float *aBuffer, int aSamples, float aSamplerate, double aTime, int aChannel, int aChannels);
		virtual float getFilterParameter(int aAttributeId);
		virtual void setFilterParameter(int aAttributeId, float aValue);
		virtual void fadeFilterParameter(int aAttributeId, float aTo, double aTime, double aStartTime);
		virtual void oscillateFilterParameter(int aAttributeId, float aFrom, float aTo, double aTime, double aStartTime);
		virtual ~FilterInstance();
	};

	class Filter
	{
	public:
		Filter();
		virtual FilterInstance *createInstance() = 0;
		virtual ~Filter();
	};
};

#endif
