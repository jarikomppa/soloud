/*
SoLoud audio engine
Copyright (c) 2013-2018 Jari Komppa

vizsn speech synthesizer (c) by Ville-Matias Heikkilä,
released under WTFPL, http://www.wtfpl.net/txt/copying/
(in short, "do whatever you want to")

Integration and changes to work with SoLoud by Jari Komppa,
released under same license.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "soloud_vizsn.h"

/*

 lähtöfunktiot: voice, noise
 muut:          pitch
 volyymit:      voice, asp, fric, bypass (4kpl)
 resonaattorit: lp, nz (ar), npc, 1p, 2p, 3p, 4p, 5p, 6p, out (10kpl)


   [voice] [noise]
   <pitch>    |
      |       |
      |       |
   (re.LP)    |
      |       |
   (*voice) (*asp)
      |       |
      `--[+]--'
          |
       (ar.NZ)
          |
       (re.NPC)
          |
   .-----------.    [noise]
   |           |       |
(re.1P)     (diff)  (*fric)
   |           |       |
   |          [+]------'
   |           |
  [-]-(re.4P)--[
   |           |
  [-]-(re.3P)--[   <-- selvitäänkö kahdella tuossa vaiheessa?
   |           |
  [-]-(re.2P)--[
   |           |
   |        (*bypass)
   |           |
   `----[-]----'
         |'
      (re.OUT)
         |
        out

    resonaattori:

    x = a*input + b*y + c*z

    ja tulos kiertää x=>y=>z

    antiresonaattori:

    x = a*input + b*y + c*z

    inputti kiertää i=>y=>z
*/

#define RLP 0
#define RNZ 1
#define RNPC 2
#define ROUT 3
#define R1P 4
#define R2P 5
#define R3P 6
#define R4P 7
#define R5P 8
#define R6P 9
#define P_A 0
#define P_AE 1
#define P_E 2
#define P_OE 3
#define P_O 4
#define P_I 5
#define P_Y 6
#define P_U 7
#define P_H 8
#define P_V 9
#define P_J 10
#define P_S 11
#define P_L 12
#define P_R 13
#define P_K 14
#define P_T 15
#define P_P 16
#define P_N 17
#define P_M 18
#define P_NG 19
#define P_NEW 12
#define P_END -1
#define P_CLR -2

#define filter(i,v) i##last=i+(i##last*v);
#define plosive(a) ((((a) >= P_K) && ((a) <= P_P)) || ((a) == P_CLR))

static const float vowtab[8][4][2] =
{
	/* a */ 0.10f, 1.6f, 0, 0, 0.2f, 1.5f,  0, 0,
	/* ä */ 0.10f, 1.6f, 0, 0, 0.2f, 0,     0, 0,

	/* e */ 0.08f, 1.8f, 0, 0, 0.2f, -0.8f, 0, 0,
	/* ö */ 0.08f, 1.8f, 0, 0, 0.3f, 0.9f,  0, 0,
	/* o */ 0.08f, 1.8f, 0, 0, 0.2f, 1.6f,  0, 0,

	/* i */ 0.05f, 1.9f, 0, 0, 0.2f, -1.5f, 0, 0,
	/* y */ 0.05f, 1.9f, 0, 0, 0.2f, 0.8f,  0, 0,
	/* u */ 0.05f, 1.9f, 0, 0, 0.1f, 1.7f,  0, 0
};

/* integerisoi (8bit): kerro kolmella */
static const float voo[13][5 + 3 * 10] =
{
	/* frikatiivit & puolivokaalit */

	/* h */
	0.0f, 0.0f, 0.36f, 0.0f, 0.0f, 0.32f, 1.36f, -0.67f, 22.0f, -42.0f, 21.0f, 0.04f, 1.9f, -0.92f, 1.17f, 0.8f, -0.02f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	/* v */
	0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.32f, 1.36f, -0.67f, 22.0f, -42.0f, 21.0f, 0.04f, 1.9f, -0.92f, 1.17f, 0.8f, -0.02f,
	0.005f, 1.9f, -0.95f, 0.0f, 0.0f, 0.0f, 0.04f, 1.5f, -0.93f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	/* j */
	0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 0.32f, 1.36f, -0.67f, 22.0f, -42.0f, 21.0f, 0.04f, 1.9f, -0.92f, 1.17f, 0.8f, -0.02f,
	0.005f, 1.9f, -0.95f, 0.0f, 0.0f, 0.0f, 0.04f, -1.5f, -0.93f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	/* s */
	0.0f, 0.0f, 0.36f, 0.0f, 0.0f, 0.32f, 1.36f, -0.67f, 22.0f, -42.0f, 21.0f, 0.04f, 1.9f, -0.92f, 1.7f, 0.1f, -0.02f,
	0.0f, 0.1f, -0.02f, 0.0f, 1.76f, -0.85f, 0.01f, 0.42f, -0.93f, 0.02f, -1.37f, -0.68f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	/* l */
	0.8f, 0.0f, 0.0f, 0.0f, 0.0f, 0.32f, 1.36f, -0.67f, 22.0f, -42.0f, 21.0f, 0.04f, 1.88f, -0.92f, 1.17f, 0.8f, -0.02f,
	0.1f, -0.5f, -0.93f, 0.6f, 1.0f, -0.93f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	/* r */
	0.8f, 0.4f, 0.0f, 0.0f, 0.0f, 0.32f, 1.36f, -0.67f, 22.0f, -42.0f, 21.0f, 0.04f, 1.9f, -0.92f, 1.17f, 0.8f, -0.02f,
	-0.2f, 0.0f, -0.93f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	/* klusiilit */

	/* k */
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.32f, 1.36f, -0.67f, 22.0f, -42.0f, 21.0f, 0.04f, 1.9f, -0.92f, 1.17f, 0.8f, -0.02f,
	0.0f, 1.95f, -0.94f, 0.4f, 1.0f, -0.93f, 0.6f, 1.0f, -0.89f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	/* t */
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.32f, 1.36f, -0.67f, 22.0f, -42.0f, 21.0f, 0.04f, 1.9f, -0.92f, 1.17f, 0.8f, -0.02f,
	0.0f, 1.6f, -0.94f, 0.0f, 0.3f, -0.93f, 1.5f, -0.6f, -0.89f, 1.8f, -1.5f, -0.68f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	/* p */
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.32f, 1.36f, -0.67f, 22.0f, -42.0f, 21.0f, 0.04f, 1.9f, -0.92f, 1.17f, 0.8f, -0.02f,
	0.01f, 1.9f, -0.94f, 1.5f, 1.7f, -0.93f, 1.0f, 1.0f, -0.89f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	/* nasaalit */

	/* n */
	0.81f, 0.0f, 0.0f, 0.0f, 0.0f, 0.32f, 1.36f, -0.67f, 15.0f, -20.0f, 10.0f, 0.04f, 1.88f, -0.92f, 1.17f, 0.1f, -0.02f,
	0.02f, 1.83f, -0.97f, 0.10f, 0.26f, -0.83f, 0.06f, -0.85f, -0.82f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	/* m */
	0.81f, 0.0f, 0.0f, 0.0f, 0.0f, 0.32f, 1.36f, -0.67f, 15.0f, -20.0f, 10.0f, 0.04f, 1.88f, -0.92f, 1.17f, 0.1f, -0.02f,
	0.08f, 1.5f, -0.94f, 0.06f, 1.0f, -0.9f, 0.05f, 1.5f, -0.89f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

	/* ng */
	0.81f, 0.0f, 0.0f, 0.0f, 0.0f, 0.32f, 1.36f, -0.67f, 15.0f, -20.0f, 10.0f, 0.04f, 1.88f, -0.92f, 1.17f, 0.1f, -0.02f,
	0.1f, 1.6f, -0.94f, 0.7f, -1.0f, -0.91f, 0.1f, 1.16f, -0.91f, 0.03f, -1.3f, -0.68f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
};

static const char keyz[] =
{
	P_A,  P_P,  P_S,  P_T,  P_E,  P_V,  P_NG,
	P_H,  P_I,  P_J,  P_K,  P_L,  P_M,  P_N,
	P_O,  P_P,  P_K,  P_R,  P_S,  P_T,  P_U,
	P_V,  P_U,  P_S,  P_Y,  P_S,  P_AE, P_OE
};

namespace SoLoud
{

	VizsnInstance::VizsnInstance(Vizsn *aParent)
	{
		mParent = aParent;
		ptr = 0;
		current_voice_type = 6;
		memset(echobuf, 0, 1024 * sizeof(int));
		pitch = 800;
		s = mParent->mText;
		bufwrite = 0;
		bufread = 0;
	}

	VizsnInstance::~VizsnInstance()
	{
	}

	unsigned int VizsnInstance::getAudio(float *aBuffer, unsigned int aSamplesToRead, unsigned int aBufferSize)
	{	
		unsigned int idx = 0;
		int i, j;
		if (bufwrite > bufread)
		{
			for (; bufwrite > bufread && idx < aSamplesToRead; bufread++)
			{
				aBuffer[idx] = buf[bufread];
				idx++;
			}
		}
		if (idx == aSamplesToRead) return aSamplesToRead;
		bufwrite = bufread = 0;
		while (idx + bufwrite < aSamplesToRead)
		{
			setphone(&bank0, *s, pitch);

			if (*s == P_END)
			{
				buf[bufwrite] = 0;
				bufwrite++;
				SOLOUD_ASSERT(bufwrite < 2048);
				break;
			}

			setphone(&bank1, s[1], pitch);//pitch+=50);  -- RAISE SOUND

			slide_prepare(50);

			nper = 0;

			for (i = plosive(*s) ? 100 : 300; i; i--)
			{
				buf[bufwrite] = genwave();
				bufwrite++;
				SOLOUD_ASSERT(bufwrite < 2048);
			}

			if (!plosive(s[1]))
			{
				for (i = 50; i; i--)
				{
					for (j = 10; j; j--)
					{
						buf[bufwrite] = genwave();
						bufwrite++;
						SOLOUD_ASSERT(bufwrite < 2048);
					}
					slide_tick();
				}
			}
			else
			{
				for (i = 50; i; i--)
				{
					for (j = 3; j; j--)
					{
						buf[bufwrite] = genwave();
						bufwrite++;
						SOLOUD_ASSERT(bufwrite < 2048);
					}
				}
			}

			s++;

			memcpy(&bank0, &bank1, sizeof(bank));
		}
		for (; idx < aSamplesToRead; idx++)
		{
			aBuffer[idx] = buf[bufread];
			bufread++;
		}
		return aSamplesToRead;
	}

	float VizsnInstance::vcsrc(int pitch, int voicetype)
	{
		static int a = 0, b = 100, orgv = -1;
		a += pitch;

		if (orgv != voicetype)
		{
			orgv = voicetype;
			a = 0;
			b = 100;
		}

		switch (voicetype)
		{
		case 0:	return (a & (256 + 128 + 32)) * 5 * 0.0002f;
		case 1:	return (float)(sin(a * 0.0002) * cos(a * 0.0003) * 0.2f + ((rand() % 200 - 100) / 300.0f)); // ilmava
		case 2: return (float)tan(a*0.00002)*0.01f; // burpy
		case 3: return ((a & 65535) > 32768 ? 65535 : 0) * 0.00001f; // square wave
		case 4: return a * a * 0.0000000002f; // kuisku
		case 5:	a += 3;	b++; return ((a & 255) > ((b >> 2) & 255)) ? 0.3f : 0.0f;
		case 7:	return ((a >> 8) & (256 + 128)) * 0.001f; // robottipulssi
		case 8:	return (float)(rand() % (1 + ((a & 65535) >> 8))) / 256; // -- hiukka ihmisempi tsaatana
		case 9:	return ((float)(rand() & 32767)) / 32767; // -- noise: tsaatana 
		case 6: // fallthrough			
		default: return (a & 65535) * (0.001f / 256); /*-- sawtooth: near natural */
		}
	}

	float VizsnInstance::noisrc()
	{
		return ((float)(rand() & 32767)) / 32768;
	}

	float VizsnInstance::genwave()
	{
		static float glotlast = 0;
		float s, o, noise, voice, glot, parglot;
		int ob;

		noise = noisrc();

		if (nper > nmod)
		{
			noise *= 0.5f;
		}

		s = bank0.frica * noise;

		voice = vcsrc((int)floor(bank0.pitch), current_voice_type);
		voice = bank0.r[RLP].resonate(voice);

		if (nper < nopen)
		{
			voice += bank0.breth * noise;
		}

		parglot = glot = (bank0.voice * voice) + (bank0.aspir * noise);

		parglot = bank0.r[RNZ].antiresonate(parglot);
		parglot = bank0.r[RNPC].resonate(parglot);
		s += (parglot - glotlast);
		glotlast = parglot;
		o = bank0.r[R1P].resonate(parglot);

		int i;
		for (i = R4P; i > R2P; i--)
		{
			o = bank0.r[i].resonate(s) - o;
		}

		o = bank0.r[ROUT].resonate(bank0.bypas * s - o);

		/*********/

		ob = (int)floor(o * 400 * 256 + (echobuf[ptr] / 4));
		echobuf[ptr] = ob;
		ptr = (ptr + 1) & 1023;

		ob = (ob >> 8) + 128;

		if (ob < 0)	ob = 0;
		if (ob > 255) ob = 255;		

		nper++;

		return ob * (1.0f / 255.0f);
	}

	void VizsnInstance::setphone(bank *b, char p, float pitch)
	{
		int i;
		b->frica = b->aspir = b->bypas = b->breth = b->voice = 0;
		b->pitch = pitch;

		if (p < 0)
		{
			for (i = 0; i < 10; i++)
			{
				b->r[i].p1 = b->r[i].p2 = b->r[i].a = b->r[i].b = b->r[i].c = 0;
			}
		}
		else
		{
			if (p < 8)
			{
				/* vokaali */
				resonator *r = b->r;
				const float *s = vowtab[p][0];

				r[R1P].c = -0.95f; r[R2P].c = -0.93f; r[R3P].c = -0.88f; r[R4P].c = -0.67f;
				r[RLP].a = 0.31f;  r[RLP].b = 1.35f;  r[RLP].c = -0.67f;
				r[RNZ].a = 22.0f;  r[RNZ].b = -42.0f; r[RNZ].c = 21.0f;
				r[RNPC].a = 0.04f; r[RNPC].b = 1.87f; r[RNPC].c = -0.92f;
				r[ROUT].a = 1.16f; r[ROUT].b = 0.08f; r[ROUT].c = -0.02f;

				r += R1P;

				for (i = 4; i; i--)
				{
					r->a = *s++;
					r->b = *s++;
					r++;
				}

				b->voice = 0.8f;
			}
			else
			{
				/* v */
				int i;
				const float *v = voo[p - 8];

				b->voice = *v++;
				b->aspir = *v++;
				b->frica = *v++;
				b->bypas = *v++;
				b->breth = *v++;

				for (i = 0; i < 10; i++)
				{
					b->r[i].a = *v++;
					b->r[i].b = *v++;
					b->r[i].c = *v++;
				}

				b->voice = 0.8f;

				b->breth = 0.18f;
			}
		}
	}

	void VizsnInstance::slide_prepare(int numtix)
	{
		int i;

		for (i = 0; i < 10; i++)
		{
			bank0to1.r[i].a = (bank1.r[i].a - bank0.r[i].a) / numtix;
			bank0to1.r[i].b = (bank1.r[i].b - bank0.r[i].b) / numtix;
			bank0to1.r[i].c = (bank1.r[i].c - bank0.r[i].c) / numtix;
		}

		bank0to1.pitch = (bank1.pitch - bank0.pitch) / numtix;
	}

	void VizsnInstance::slide_tick()
	{
		int i;

		for (i = 0; i < 10; i++)
		{
			bank0.r[i].a += bank0to1.r[i].a;
			bank0.r[i].b += bank0to1.r[i].b;
			bank0.r[i].c += bank0to1.r[i].c;
		}

		bank0.pitch += bank0to1.pitch;
	}

	bool VizsnInstance::hasEnded()
	{
		return *s == P_END;
	}

	Vizsn::Vizsn()
	{
		mBaseSamplerate = 8000;
		mText = 0;
	}

	Vizsn::~Vizsn()
	{
		stop();
	}

	AudioSourceInstance * Vizsn::createInstance() 
	{
		return new VizsnInstance(this);
	}

	void Vizsn::setText(char *aText)
	{
		if (!aText)
			return;
		stop();
		delete[] mText;
		int len = strlen(aText);
		mText = new char[len + 3];		
		memcpy(mText+1, aText, len);
		mText[0] = P_CLR;
		int i;
		for (i = 0; i < len; i++)
		{
			int c = mText[i + 1];
			if (c == 'ä' || c == -124) c = '{';
			if (c == 'ö' || c == -108) c = '|';
			if (c >= 'a' && c <= '|')
			{
				mText[i + 1] = keyz[c - 'a'];
			}
			else
			{
				mText[i + 1] = P_CLR;
			}
		}
		mText[len + 1] = P_END;
		mText[len + 2] = 0;
	}

};