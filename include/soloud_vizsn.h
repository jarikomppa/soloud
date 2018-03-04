/*
SoLoud audio engine
Copyright (c) 2013-2018 Jari Komppa

vizsn speech synthesizer (c) by Ville-Matias Heikkilä,
released under WTFPL, http://www.wtfpl.net/txt/copying/
(in short, "do whatever you want to")

Integration and changes to work with SoLoud by Jari Komppa,
released under same license.
*/

#ifndef SOLOUD_VIZSN_H
#define SOLOUD_VIZSN_H

#include "soloud.h"

namespace SoLoud
{
	class Vizsn;

	class VizsnInstance : public AudioSourceInstance
	{
		class resonator
		{
		public:
			float a, b, c, p1, p2;

			float resonate(float i)
			{
				float x = (a * i) + (b * p1) + (c * p2);
				p2 = p1;
				p1 = x;
				return x;
			}

			float antiresonate(float i)
			{
				float x = a * i + b * p1 + c * p2;
				p2 = p1;
				p1 = i;
				return x;
			}
		};

		struct bank
		{
			resonator r[10];
			float pitch;
			float frica, voice, aspir, bypas, breth;
		};
	public:
		VizsnInstance(Vizsn *aParent);
		~VizsnInstance();

		virtual unsigned int getAudio(float *aBuffer, unsigned int aSamplesToRead, unsigned int aBufferSize);
		virtual bool hasEnded();

    public:
        Vizsn *mParent;
		bank bank0, bank1, bank0to1;
		int nper, nmod, nopen;
		int echobuf[1024], ptr;
		int current_voice_type;
		float pitch;
		char *s;
		float buf[2048];
		unsigned int bufwrite;
		unsigned int bufread;
		float vcsrc(int pitch, int voicetype);
		float noisrc();
		float genwave();
		void setphone(bank *b, char p, float pitch);
		void slide_prepare(int numtix);
		void slide_tick();
	};

	class Vizsn : public AudioSource
	{
	public:
		char *mText;
		Vizsn();
		virtual ~Vizsn();
		void setText(char *aText);
		
		virtual AudioSourceInstance *createInstance();
	};
};

#endif
