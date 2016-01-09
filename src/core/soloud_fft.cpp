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

// FFT based on fftreal by Laurent de Soras, under WTFPL

#include "soloud.h"
#include "soloud_fft.h"
#include <string.h>

extern int Soloud_fft_bitrev_10[1024];
extern int Soloud_fft_bitrev_8[256];
extern float Soloud_fft_trig_10[508];
extern float Soloud_fft_trig_8[124];

static void do_fft_8(float *f, const float *x)
{
    float tempbuf[256];
    float *sf = tempbuf;
    float *df = f;

    /* Do the transformation in several pass */
    {
        int pass;
        int nbr_coef;
        int h_nbr_coef;
        int d_nbr_coef;
        int coef_index;

        /* First and second pass at once */
        {
            int *bit_rev_lut_ptr = Soloud_fft_bitrev_8;
            coef_index = 0;
            do
            {
                int rev_index_0 = bit_rev_lut_ptr[coef_index];
                int rev_index_1 = bit_rev_lut_ptr[coef_index + 1];
                int rev_index_2 = bit_rev_lut_ptr[coef_index + 2];
                int rev_index_3 = bit_rev_lut_ptr[coef_index + 3];

                float *df2 = df + coef_index;
                df2[1] = x[rev_index_0] - x[rev_index_1];
                df2[3] = x[rev_index_2] - x[rev_index_3];

                float sf_0 = x[rev_index_0] + x[rev_index_1];
                float sf_2 = x[rev_index_2] + x[rev_index_3];

                df2[0] = sf_0 + sf_2;
                df2[2] = sf_0 - sf_2;

                coef_index += 4;
            } 
            while (coef_index < 256);
        }

        /* Third pass */
        {
            coef_index = 0;
            float sqrt2_2 = (float)sqrt(2.0f) * 0.5f;
            do
            {
                float v;

                sf[coef_index] = df[coef_index] + df[coef_index + 4];
                sf[coef_index + 4] = df[coef_index] - df[coef_index + 4];
                sf[coef_index + 2] = df[coef_index + 2];
                sf[coef_index + 6] = df[coef_index + 6];

                v = (df[coef_index + 5] - df[coef_index + 7]) * sqrt2_2;
                sf[coef_index + 1] = df[coef_index + 1] + v;
                sf[coef_index + 3] = df[coef_index + 1] - v;

                v = (df[coef_index + 5] + df[coef_index + 7]) * sqrt2_2;
                sf[coef_index + 5] = v + df[coef_index + 3];
                sf[coef_index + 7] = v - df[coef_index + 3];

                coef_index += 8;
            } 
            while (coef_index < 256);
        }

        /* Next pass */
        for (pass = 3; pass < 8; ++pass)
        {
            coef_index = 0;
            nbr_coef = 1 << pass;
            h_nbr_coef = nbr_coef >> 1;
            d_nbr_coef = nbr_coef << 1;
			float *cos_ptr = Soloud_fft_trig_8 + (int)(1 << (pass - 1)) - 4;

            do
            {
                int i;
                float *sf1r = sf + coef_index;
                float *sf2r = sf1r + nbr_coef;
                float *dfr = df + coef_index;
                float *dfi = dfr + nbr_coef;

                /* Extreme coefficients are always real */
                dfr[0] = sf1r[0] + sf2r[0];
                dfi[0] = sf1r[0] - sf2r[0];    // dfr [nbr_coef] =
                dfr[h_nbr_coef] = sf1r[h_nbr_coef];
                dfi[h_nbr_coef] = sf2r[h_nbr_coef];

                /* Others are conjugate complex numbers */
               float *sf1i = sf1r + h_nbr_coef;
               float *sf2i = sf1i + nbr_coef;
                for (i = 1; i < h_nbr_coef; ++i)
                {
                    float c = cos_ptr[i];                    // cos (i*PI/nbr_coef);
                    float s = cos_ptr[h_nbr_coef - i];    // sin (i*PI/nbr_coef);
                    float v;

                    v = sf2r[i] * c - sf2i[i] * s;
                    dfr[i] = sf1r[i] + v;
                    dfi[-i] = sf1r[i] - v;    // dfr [nbr_coef - i] =

                    v = sf2r[i] * s + sf2i[i] * c;
                    dfi[i] = v + sf1i[i];
                    dfi[nbr_coef - i] = v - sf1i[i];
                }

                coef_index += d_nbr_coef;
            } 
            while (coef_index < 256);

            /* Prepare to the next pass */
            {
                float *temp_ptr = df;
                df = sf;
                sf = temp_ptr;
            }
        }
    }
}


static void do_fft_10(float *f, const float *x)
{
    float tempbuf[1024];
    float *sf = tempbuf;
    float *df = f;

    /* Do the transformation in several pass */
    {
        int pass;
        int nbr_coef;
        int h_nbr_coef;
        int d_nbr_coef;
        int coef_index;

        /* First and second pass at once */
        {
            int *bit_rev_lut_ptr = Soloud_fft_bitrev_10;
            coef_index = 0;

            do
            {
                int rev_index_0 = bit_rev_lut_ptr[coef_index];
                int rev_index_1 = bit_rev_lut_ptr[coef_index + 1];
                int rev_index_2 = bit_rev_lut_ptr[coef_index + 2];
                int rev_index_3 = bit_rev_lut_ptr[coef_index + 3];

                float *df2 = df + coef_index;
                df2[1] = x[rev_index_0] - x[rev_index_1];
                df2[3] = x[rev_index_2] - x[rev_index_3];

                float sf_0 = x[rev_index_0] + x[rev_index_1];
                float sf_2 = x[rev_index_2] + x[rev_index_3];

                df2[0] = sf_0 + sf_2;
                df2[2] = sf_0 - sf_2;

                coef_index += 4;
            } 
			while (coef_index < 1024);
        }

        /* Third pass */
        {
            coef_index = 0;
            float sqrt2_2 = (float)sqrt(2.0f) * 0.5f;
            do
            {
                float v;

                sf[coef_index] = df[coef_index] + df[coef_index + 4];
                sf[coef_index + 4] = df[coef_index] - df[coef_index + 4];
                sf[coef_index + 2] = df[coef_index + 2];
                sf[coef_index + 6] = df[coef_index + 6];

                v = (df[coef_index + 5] - df[coef_index + 7]) * sqrt2_2;
                sf[coef_index + 1] = df[coef_index + 1] + v;
                sf[coef_index + 3] = df[coef_index + 1] - v;

                v = (df[coef_index + 5] + df[coef_index + 7]) * sqrt2_2;
                sf[coef_index + 5] = v + df[coef_index + 3];
                sf[coef_index + 7] = v - df[coef_index + 3];

                coef_index += 8;
            } 
			while (coef_index < 1024);
        }

        /* Next pass */
        for (pass = 3; pass < 10; ++pass)
        {
            coef_index = 0;
            nbr_coef = 1 << pass;
            h_nbr_coef = nbr_coef >> 1;
            d_nbr_coef = nbr_coef << 1;
			float *cos_ptr = Soloud_fft_trig_10 + (int)(1 << (pass - 1)) - 4;
            do
            {
                int i;
                float *sf1r = sf + coef_index;
                float *sf2r = sf1r + nbr_coef;
                float *dfr = df + coef_index;
                float *dfi = dfr + nbr_coef;

                /* Extreme coefficients are always real */
                dfr[0] = sf1r[0] + sf2r[0];
                dfi[0] = sf1r[0] - sf2r[0];    // dfr [nbr_coef] =
                dfr[h_nbr_coef] = sf1r[h_nbr_coef];
                dfi[h_nbr_coef] = sf2r[h_nbr_coef];

                /* Others are conjugate complex numbers */
                float *sf1i = sf1r + h_nbr_coef;
                float *sf2i = sf1i + nbr_coef;
                for (i = 1; i < h_nbr_coef; ++i)
                {
                    float c = cos_ptr[i];                    // cos (i*PI/nbr_coef);
                    float s = cos_ptr[h_nbr_coef - i];    // sin (i*PI/nbr_coef);
                    float v;

                    v = sf2r[i] * c - sf2i[i] * s;
                    dfr[i] = sf1r[i] + v;
                    dfi[-i] = sf1r[i] - v;    // dfr [nbr_coef - i] =

                    v = sf2r[i] * s + sf2i[i] * c;
                    dfi[i] = v + sf1i[i];
                    dfi[nbr_coef - i] = v - sf1i[i];
                }

                coef_index += d_nbr_coef;
            } 
            while (coef_index < 1024);

            /* Prepare to the next pass */
            {
                float *temp_ptr = df;
                df = sf;
                sf = temp_ptr;
            }
        }
    }
}

static void do_ifft_8(float *f, float *x)
{
    float tempbuf[256];
    float *sf = f;
    float *df;
    float *df_temp;

    df = x;
    df_temp = tempbuf;

    /* Do the transformation in several pass */
    {
        int pass;
        int nbr_coef;
        int h_nbr_coef;
        int d_nbr_coef;
        int coef_index;

        /* First pass */
        for (pass = 8 - 1; pass >= 3; --pass)
        {
            coef_index = 0;
            nbr_coef = 1 << pass;
            h_nbr_coef = nbr_coef >> 1;
            d_nbr_coef = nbr_coef << 1;
            float *cos_ptr = Soloud_fft_trig_8 + (int)(1 << (pass - 1)) - 4;
            do
            {
                int i;
                float *sfr = sf + coef_index;
                float *sfi = sfr + nbr_coef;
                float *df1r = df + coef_index;
                float *df2r = df1r + nbr_coef;

                /* Extreme coefficients are always real */
                df1r[0] = sfr[0] + sfi[0];        // + sfr [nbr_coef]
                df2r[0] = sfr[0] - sfi[0];        // - sfr [nbr_coef]
                df1r[h_nbr_coef] = sfr[h_nbr_coef] * 2;
                df2r[h_nbr_coef] = sfi[h_nbr_coef] * 2;

                /* Others are conjugate complex numbers */
                float *df1i = df1r + h_nbr_coef;
                float *df2i = df1i + nbr_coef;
                for (i = 1; i < h_nbr_coef; ++i)
                {
                    df1r[i] = sfr[i] + sfi[-i];        // + sfr [nbr_coef - i]
                    df1i[i] = sfi[i] - sfi[nbr_coef - i];

                    float c = cos_ptr[i];                    // cos (i*PI/nbr_coef);
                    float s = cos_ptr[h_nbr_coef - i];    // sin (i*PI/nbr_coef);
                    float vr = sfr[i] - sfi[-i];        // - sfr [nbr_coef - i]
                    float vi = sfi[i] + sfi[nbr_coef - i];

                    df2r[i] = vr * c + vi * s;
                    df2i[i] = vi * c - vr * s;
                }

                coef_index += d_nbr_coef;
            } 
			while (coef_index < 256);

            /* Prepare to the next pass */
            if (pass < 8 - 1)
            {
                float *temp_ptr = df;
                df = sf;
                sf = temp_ptr;
            }
            else
            {
                sf = df;
                df = df_temp;
            }
        }

        /* Antepenultimate pass */
        {
            float sqrt2_2 = (float)sqrt(2.0f) * 0.5f;
            coef_index = 0;
            do
            {
                df[coef_index] = sf[coef_index] + sf[coef_index + 4];
                df[coef_index + 4] = sf[coef_index] - sf[coef_index + 4];
                df[coef_index + 2] = sf[coef_index + 2] * 2;
                df[coef_index + 6] = sf[coef_index + 6] * 2;

                df[coef_index + 1] = sf[coef_index + 1] + sf[coef_index + 3];
                df[coef_index + 3] = sf[coef_index + 5] - sf[coef_index + 7];

                float vr = sf[coef_index + 1] - sf[coef_index + 3];
                float vi = sf[coef_index + 5] + sf[coef_index + 7];

                df[coef_index + 5] = (vr + vi) * sqrt2_2;
                df[coef_index + 7] = (vi - vr) * sqrt2_2;

                coef_index += 8;
            } 
			while (coef_index < 256);
        }

        /* Penultimate and last pass at once */
        {
            coef_index = 0;
            int *bit_rev_lut_ptr = Soloud_fft_bitrev_8;
            float *sf2 = df;
            do
            {
                {
                    float b_0 = sf2[0] + sf2[2];
                    float b_2 = sf2[0] - sf2[2];
                    float b_1 = sf2[1] * 2;
                    float b_3 = sf2[3] * 2;

                    x[bit_rev_lut_ptr[0]] = b_0 + b_1;
                    x[bit_rev_lut_ptr[1]] = b_0 - b_1;
                    x[bit_rev_lut_ptr[2]] = b_2 + b_3;
                    x[bit_rev_lut_ptr[3]] = b_2 - b_3;
                }
                {
                    float b_0 = sf2[4] + sf2[6];
                    float b_2 = sf2[4] - sf2[6];
                    float b_1 = sf2[5] * 2;
                    float b_3 = sf2[7] * 2;

                    x[bit_rev_lut_ptr[4]] = b_0 + b_1;
                    x[bit_rev_lut_ptr[5]] = b_0 - b_1;
                    x[bit_rev_lut_ptr[6]] = b_2 + b_3;
                    x[bit_rev_lut_ptr[7]] = b_2 - b_3;
                }

                sf2 += 8;
                coef_index += 8;
                bit_rev_lut_ptr += 8;
            } 
			while (coef_index < 256);
        }
    }

    // rescale

    float mul = (1.0f / 256);
    int i = 256 - 1;

    do
    {
        x[i] *= mul;
        --i;
    } 
    while (i >= 0);
}


namespace SoLoud
{
    namespace FFT
    {
        void fft1024(float *aBuffer)
        {
            float temp[1024];
            memcpy(temp, aBuffer, sizeof(float) * 1024);
            do_fft_10(aBuffer, temp);
        }    

        void fft256(float *aBuffer)
        {
            float temp[256];
            memcpy(temp, aBuffer, sizeof(float) * 256);
            do_fft_8(aBuffer, temp);
        }
        
        void ifft256(float *aBuffer)
        {
            float temp[256];
            memcpy(temp, aBuffer, sizeof(float) * 256);
			do_ifft_8(temp, aBuffer);
        }
    };
};
