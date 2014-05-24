/*
SoLoud audio engine - tool to develop resamplers with
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
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include "stb_image_write.h"

#ifndef TAU
#define TAU 6.283185307179586476925286766559f
#endif

#define SAMPLE_GRANULARITY 512

void resample_pointsample(float *aSrc,
                          float *aSrc1,
                          float *aDst,
                          int aSrcOffset,
                          int aDstSampleCount,
                          float aSrcSamplerate,
                          float aDstSamplerate,
                          int aStepFixed)
{
  int i;
  int pos = aSrcOffset;

  for (i = 0; i < aDstSampleCount; i++, pos += aStepFixed)
  {
    int p = pos >> 16;
    aDst[i] = aSrc[p];
  }
}

void resample_linear(float *aSrc,
                     float *aSrc1,
                     float *aDst,
                     int aSrcOffset,
                     int aDstSampleCount,
                     float aSrcSamplerate,
                     float aDstSamplerate,
                     int aStepFixed)
{
  int i;
  int pos = aSrcOffset;

  for (i = 0; i < aDstSampleCount; i++, pos += aStepFixed)
  {
    int p = pos >> 16;
    int f = pos & 0xffff;
#ifdef _DEBUG

    if (p >= SAMPLE_GRANULARITY || p < 0)
    {
      // This should never actually happen
      p = SAMPLE_GRANULARITY - 1;
    }

#endif
    float s1 = aSrc1[SAMPLE_GRANULARITY - 1];

    float s2 = aSrc[p];

    if (p != 0)
    {
      s1 = aSrc[p-1];
    }

    aDst[i] = s1 + (s2 - s1) * f * (1 / 65536.0f);
  }
}

void plot_diff(const char *aFilename, int aSampleCount, int aHeight, float *aSrc1, float *aSrc2, int aColor1, int aColor2, int aBgColor, int aGridColor)
{
	int *bitmap = new int[aSampleCount * aHeight];
	int i;
	for (i = 0; i < aSampleCount * aHeight; i++)
	{
		bitmap[i] = aBgColor;
	}

	for (i = 0; i < aSampleCount; i++)
	{
		bitmap[(aHeight / 2) * aSampleCount + i] = aGridColor;

		if (i & 1)
		{
			bitmap[((aHeight * 1) / 4) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 3) / 4) * aSampleCount + i] = aGridColor;
		}

		if ((i & 3) == 0)
		{
			bitmap[((aHeight * 1) / 8) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 3) / 8) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 5) / 8) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 7) / 8) * aSampleCount + i] = aGridColor;
		}

		if ((i & 7) == 0)
		{
			bitmap[((aHeight * 1) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 3) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 5) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 7) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 9) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 11) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 13) / 16) * aSampleCount + i] = aGridColor;
			bitmap[((aHeight * 15) / 16) * aSampleCount + i] = aGridColor;
		}
	}

	for (i = 0; i < aHeight; i++)
	{
		bitmap[i * aSampleCount + aSampleCount / 2] = aGridColor;

		if (i & 1)
		{
			bitmap[i * aSampleCount + (aSampleCount * 1) / 4] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 3) / 4] = aGridColor;
		}

		if ((i & 3) == 0)
		{
			bitmap[i * aSampleCount + (aSampleCount * 1) / 8] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 3) / 8] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 5) / 8] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 7) / 8] = aGridColor;
		}

		if ((i & 7) == 0)
		{
			bitmap[i * aSampleCount + (aSampleCount * 1) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 3) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 5) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 7) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 9) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 11) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 13) / 16] = aGridColor;
			bitmap[i * aSampleCount + (aSampleCount * 15) / 16] = aGridColor;
		}
	}

	for (i = 0; i < aSampleCount-1; i++)
	{
		if (aSrc1[i] > -2 && aSrc1[i] < 2 && aSrc1[i+1] > -2 && aSrc1[i+1] < 2)
		{
			float v1 = 0.5 - (aSrc1[i] + 1) / 4 + 0.25;
			float v2 = 0.5 - (aSrc1[i+1] + 1) / 4 + 0.25;
			v1 *= aHeight;
			v2 *= aHeight;
			if (v1 > v2)
			{
				float t = v1;
				v1 = v2;
				v2 = t;
			}
			float j;
			for (j = v1; j <= v2; j++)
			{
				bitmap[(int)floor(j) * aSampleCount + i] = aColor1;
			}
		}

		if (aSrc2[i] > -2 && aSrc2[i] < 2 && aSrc2[i+1] > -2 && aSrc2[i+1] < 2)
		{
			float v1 = 0.5 - (aSrc2[i] + 1) / 4 + 0.25;
			float v2 = 0.5 - (aSrc2[i+1] + 1) / 4 + 0.25;
			v1 *= aHeight;
			v2 *= aHeight;
			if (v1 > v2)
			{
				float t = v1;
				v1 = v2;
				v2 = t;
			}
			float j;
			for (j = v1; j <= v2; j++)
			{
				bitmap[(int)floor(j) * aSampleCount + i] = aColor2;
			}
		}
	}
	stbi_write_png(aFilename, aSampleCount, aHeight, 4, bitmap, aSampleCount * 4);
	delete[] bitmap;
}

float saw(float v)
{
	float t = v / TAU;
	t = t - floor(t);
	return (float)(t-0.5)*2;
}

float square(float v)
{
	float t = v / TAU;
	t = t - floor(t);
	if (t > 0.5) return 1;
	return -1;
}

void upsampletest()
{
	float *a, *b, *temp;
	a = new float[512];
	b = new float[512];
	temp = new float[512];
	
	float multiplier = 5;

	int i;
	for (i = 0; i < 512; i++)
	{
		a[i] = square(i/256.0f * TAU);
		temp[i] = square(i/256.0f * TAU * multiplier);
	}
	//resample_pointsample(temp,temp,b,0,512,11025,44100,floor(0.25*65536));
	resample_linear(temp,temp,b,0,512,441200/multiplier,44100,floor(65536/multiplier));
	plot_diff("test.png",512,256,a,b,0xff0000ff, 0xffff0000,0xffffffff,0xffcccccc);
}

int main(int parc, char ** pars)
{
	upsampletest();
}