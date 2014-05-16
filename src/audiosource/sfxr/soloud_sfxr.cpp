/*
SFXR module for SoLoud audio engine
Copyright (c) 2014 Jari Komppa
Based on code (c) by Tomas Pettersson, re-licensed under zlib by permission

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
#include <math.h>
#include "soloud_sfxr.h"

namespace SoLoud
{

	SfxrInstance::SfxrInstance(Sfxr *aParent)
	{
		mParent = aParent;
		resetSample(false);
		playing_sample = 1;
	}

#define frnd(x) ((float)(mParent->rand()%10001)/10000*(x))

	void SfxrInstance::getAudio(float *aBuffer, int aSamples)
	{
		float *buffer = aBuffer;
		for(int i=0;i<aSamples;i++)
		{
			if(!playing_sample)
			{
				*aBuffer = 0;
				aBuffer++;
				continue;
			}

			rep_time++;
			if(rep_limit!=0 && rep_time>=rep_limit)
			{
				rep_time=0;
				resetSample(true);
			}

			// frequency envelopes/arpeggios
			arp_time++;
			if(arp_limit!=0 && arp_time>=arp_limit)
			{
				arp_limit=0;
				fperiod*=arp_mod;
			}
			fslide+=fdslide;
			fperiod*=fslide;
			if(fperiod>fmaxperiod)
			{
				fperiod=fmaxperiod;
				if(mParent->p_freq_limit>0.0f)
					playing_sample=false;
			}
			float rfperiod=(float)fperiod;
			if(vib_amp>0.0f)
			{
				vib_phase+=vib_speed;
				rfperiod=(float)(fperiod*(1.0+sin(vib_phase)*vib_amp));
			}
			period=(int)rfperiod;
			if(period<8) period=8;
			square_duty+=square_slide;
			if(square_duty<0.0f) square_duty=0.0f;
			if(square_duty>0.5f) square_duty=0.5f;		
			// volume envelope
			env_time++;
			if(env_time>env_length[env_stage])
			{
				env_time=0;
				env_stage++;
				if(env_stage==3)
					playing_sample=false;
			}
			if(env_stage==0)
				env_vol=(float)env_time/env_length[0];
			if(env_stage==1)
				env_vol=1.0f+pow(1.0f-(float)env_time/env_length[1], 1.0f)*2.0f*mParent->p_env_punch;
			if(env_stage==2)
				env_vol=1.0f-(float)env_time/env_length[2];

			// phaser step
			fphase+=fdphase;
			iphase=abs((int)fphase);
			if(iphase>1023) iphase=1023;

			if(flthp_d!=0.0f)
			{
				flthp*=flthp_d;
				if(flthp<0.00001f) flthp=0.00001f;
				if(flthp>0.1f) flthp=0.1f;
			}

			float ssample=0.0f;
			for(int si=0;si<8;si++) // 8x supersampling
			{
				float sample=0.0f;
				phase++;
				if(phase>=period)
				{
					//				phase=0;
					phase%=period;
					if(mParent->wave_type==3)
						for(int i=0;i<32;i++)
							noise_buffer[i]=frnd(2.0f)-1.0f;
				}
				// base waveform
				float fp=(float)phase/period;
				switch(mParent->wave_type)
				{
				case 0: // square
					if(fp<square_duty)
						sample=0.5f;
					else
						sample=-0.5f;
					break;
				case 1: // sawtooth
					sample=1.0f-fp*2;
					break;
				case 2: // sine
					sample=(float)sin(fp*2*M_PI);
					break;
				case 3: // noise
					sample=noise_buffer[phase*32/period];
					break;
				}
				// lp filter
				float pp=fltp;
				fltw*=fltw_d;
				if(fltw<0.0f) fltw=0.0f;
				if(fltw>0.1f) fltw=0.1f;
				if(mParent->p_lpf_freq!=1.0f)
				{
					fltdp+=(sample-fltp)*fltw;
					fltdp-=fltdp*fltdmp;
				}
				else
				{
					fltp=sample;
					fltdp=0.0f;
				}
				fltp+=fltdp;
				// hp filter
				fltphp+=fltp-pp;
				fltphp-=fltphp*flthp;
				sample=fltphp;
				// phaser
				phaser_buffer[ipp&1023]=sample;
				sample+=phaser_buffer[(ipp-iphase+1024)&1023];
				ipp=(ipp+1)&1023;
				// final accumulation and envelope application
				ssample+=sample*env_vol;
			}
			ssample=ssample/8*mParent->master_vol;

			ssample*=2.0f*mParent->sound_vol;

			if(buffer!=NULL)
			{
				if(ssample>1.0f) ssample=1.0f;
				if(ssample<-1.0f) ssample=-1.0f;
				*buffer++=ssample;
			}
		}

	}

	int SfxrInstance::hasEnded()
	{
		return !playing_sample;
	}

	void SfxrInstance::resetSample(bool aRestart)
	{
		if(!aRestart)
			phase=0;
		fperiod=100.0/(mParent->p_base_freq*mParent->p_base_freq+0.001);
		period=(int)fperiod;
		fmaxperiod=100.0/(mParent->p_freq_limit*mParent->p_freq_limit+0.001);
		fslide=1.0-pow((double)mParent->p_freq_ramp, 3.0)*0.01;
		fdslide=-pow((double)mParent->p_freq_dramp, 3.0)*0.000001;
		square_duty=0.5f-mParent->p_duty*0.5f;
		square_slide=-mParent->p_duty_ramp*0.00005f;
		if(mParent->p_arp_mod>=0.0f)
			arp_mod=1.0-pow((double)mParent->p_arp_mod, 2.0)*0.9;
		else
			arp_mod=1.0+pow((double)mParent->p_arp_mod, 2.0)*10.0;
		arp_time=0;
		arp_limit=(int)(pow(1.0f-mParent->p_arp_speed, 2.0f)*20000+32);
		if(mParent->p_arp_speed==1.0f)
			arp_limit=0;
		if(!aRestart)
		{
			// reset filter
			fltp=0.0f;
			fltdp=0.0f;
			fltw=pow(mParent->p_lpf_freq, 3.0f)*0.1f;
			fltw_d=1.0f+mParent->p_lpf_ramp*0.0001f;
			fltdmp=5.0f/(1.0f+pow(mParent->p_lpf_resonance, 2.0f)*20.0f)*(0.01f+fltw);
			if(fltdmp>0.8f) fltdmp=0.8f;
			fltphp=0.0f;
			flthp=pow(mParent->p_hpf_freq, 2.0f)*0.1f;
			flthp_d=(float)(1.0+mParent->p_hpf_ramp*0.0003f);
			// reset vibrato
			vib_phase=0.0f;
			vib_speed=pow(mParent->p_vib_speed, 2.0f)*0.01f;
			vib_amp=mParent->p_vib_strength*0.5f;
			// reset envelope
			env_vol=0.0f;
			env_stage=0;
			env_time=0;
			env_length[0]=(int)(mParent->p_env_attack*mParent->p_env_attack*100000.0f);
			env_length[1]=(int)(mParent->p_env_sustain*mParent->p_env_sustain*100000.0f);
			env_length[2]=(int)(mParent->p_env_decay*mParent->p_env_decay*100000.0f);

			fphase=pow(mParent->p_pha_offset, 2.0f)*1020.0f;
			if(mParent->p_pha_offset<0.0f) fphase=-fphase;
			fdphase=pow(mParent->p_pha_ramp, 2.0f)*1.0f;
			if(mParent->p_pha_ramp<0.0f) fdphase=-fdphase;
			iphase=abs((int)fphase);
			ipp=0;
			for(int i=0;i<1024;i++)
				phaser_buffer[i]=0.0f;

			for(int i=0;i<32;i++)
				noise_buffer[i]=frnd(2.0f)-1.0f;

			rep_time=0;
			rep_limit=(int)(pow(1.0f-mParent->p_repeat_speed, 2.0f)*20000+32);
			if(mParent->p_repeat_speed==0.0f)
				rep_limit=0;
		}
	}

	void Sfxr::srand(int aSeed)
	{
		index = 0;
		int i;
		for (i = 0; i < 16; i++)
			state[i] = aSeed + i * aSeed + i;
	}

	// WELL512 implementation, public domain by Chris Lomont
	unsigned int Sfxr::rand()
	{
		unsigned int a, b, c, d;
		a = state[index];
		c = state[(index+13)&15];
		b = a^c^(a<<16)^(c<<15);
		c = state[(index+9)&15];
		c ^= (c>>11);
		a = state[index] = b^c;
		d = a^((a<<5)&0xDA442D24UL);
		index = (index + 15)&15;
		a = state[index];
		state[index] = a^b^d^(a<<2)^(b<<18)^(c<<28);
		return state[index];
	}

#define rnd(n) (rand()%(n+1))
#undef frnd
#define frnd(x) ((float)(rand()%10001)/10000*(x))


	void Sfxr::loadPreset(int aPresetNo, int aRandSeed)
	{
		resetParams();
		srand(aRandSeed);
		switch(aPresetNo)
		{
		case 0: // pickup/coin
			p_base_freq=0.4f+frnd(0.5f);
			p_env_attack=0.0f;
			p_env_sustain=frnd(0.1f);
			p_env_decay=0.1f+frnd(0.4f);
			p_env_punch=0.3f+frnd(0.3f);
			if(rnd(1))
			{
				p_arp_speed=0.5f+frnd(0.2f);
				p_arp_mod=0.2f+frnd(0.4f);
			}
			break;
		case 1: // laser/shoot
			wave_type=rnd(2);
			if(wave_type==2 && rnd(1))
				wave_type=rnd(1);
			p_base_freq=0.5f+frnd(0.5f);
			p_freq_limit=p_base_freq-0.2f-frnd(0.6f);
			if(p_freq_limit<0.2f) p_freq_limit=0.2f;
			p_freq_ramp=-0.15f-frnd(0.2f);
			if(rnd(2)==0)
			{
				p_base_freq=0.3f+frnd(0.6f);
				p_freq_limit=frnd(0.1f);
				p_freq_ramp=-0.35f-frnd(0.3f);
			}
			if(rnd(1))
			{
				p_duty=frnd(0.5f);
				p_duty_ramp=frnd(0.2f);
			}
			else
			{
				p_duty=0.4f+frnd(0.5f);
				p_duty_ramp=-frnd(0.7f);
			}
			p_env_attack=0.0f;
			p_env_sustain=0.1f+frnd(0.2f);
			p_env_decay=frnd(0.4f);
			if(rnd(1))
				p_env_punch=frnd(0.3f);
			if(rnd(2)==0)
			{
				p_pha_offset=frnd(0.2f);
				p_pha_ramp=-frnd(0.2f);
			}
			if(rnd(1))
				p_hpf_freq=frnd(0.3f);
			break;
		case 2: // explosion
			wave_type=3;
			if(rnd(1))
			{
				p_base_freq=0.1f+frnd(0.4f);
				p_freq_ramp=-0.1f+frnd(0.4f);
			}
			else
			{
				p_base_freq=0.2f+frnd(0.7f);
				p_freq_ramp=-0.2f-frnd(0.2f);
			}
			p_base_freq*=p_base_freq;
			if(rnd(4)==0)
				p_freq_ramp=0.0f;
			if(rnd(2)==0)
				p_repeat_speed=0.3f+frnd(0.5f);
			p_env_attack=0.0f;
			p_env_sustain=0.1f+frnd(0.3f);
			p_env_decay=frnd(0.5f);
			if(rnd(1)==0)
			{
				p_pha_offset=-0.3f+frnd(0.9f);
				p_pha_ramp=-frnd(0.3f);
			}
			p_env_punch=0.2f+frnd(0.6f);
			if(rnd(1))
			{
				p_vib_strength=frnd(0.7f);
				p_vib_speed=frnd(0.6f);
			}
			if(rnd(2)==0)
			{
				p_arp_speed=0.6f+frnd(0.3f);
				p_arp_mod=0.8f-frnd(1.6f);
			}
			break;
		case 3: // powerup
			if(rnd(1))
				wave_type=1;
			else
				p_duty=frnd(0.6f);
			if(rnd(1))
			{
				p_base_freq=0.2f+frnd(0.3f);
				p_freq_ramp=0.1f+frnd(0.4f);
				p_repeat_speed=0.4f+frnd(0.4f);
			}
			else
			{
				p_base_freq=0.2f+frnd(0.3f);
				p_freq_ramp=0.05f+frnd(0.2f);
				if(rnd(1))
				{
					p_vib_strength=frnd(0.7f);
					p_vib_speed=frnd(0.6f);
				}
			}
			p_env_attack=0.0f;
			p_env_sustain=frnd(0.4f);
			p_env_decay=0.1f+frnd(0.4f);
			break;
		case 4: // hit/hurt
			wave_type=rnd(2);
			if(wave_type==2)
				wave_type=3;
			if(wave_type==0)
				p_duty=frnd(0.6f);
			p_base_freq=0.2f+frnd(0.6f);
			p_freq_ramp=-0.3f-frnd(0.4f);
			p_env_attack=0.0f;
			p_env_sustain=frnd(0.1f);
			p_env_decay=0.1f+frnd(0.2f);
			if(rnd(1))
				p_hpf_freq=frnd(0.3f);
			break;
		case 5: // jump
			wave_type=0;
			p_duty=frnd(0.6f);
			p_base_freq=0.3f+frnd(0.3f);
			p_freq_ramp=0.1f+frnd(0.2f);
			p_env_attack=0.0f;
			p_env_sustain=0.1f+frnd(0.3f);
			p_env_decay=0.1f+frnd(0.2f);
			if(rnd(1))
				p_hpf_freq=frnd(0.3f);
			if(rnd(1))
				p_lpf_freq=1.0f-frnd(0.6f);
			break;
		case 6: // blip/select
			wave_type=rnd(1);
			if(wave_type==0)
				p_duty=frnd(0.6f);
			p_base_freq=0.2f+frnd(0.4f);
			p_env_attack=0.0f;
			p_env_sustain=0.1f+frnd(0.1f);
			p_env_decay=frnd(0.2f);
			p_hpf_freq=0.1f;
			break;
		}
	}
	
	void Sfxr::resetParams()
	{
		wave_type=0;

		p_base_freq=0.3f;
		p_freq_limit=0.0f;
		p_freq_ramp=0.0f;
		p_freq_dramp=0.0f;
		p_duty=0.0f;
		p_duty_ramp=0.0f;

		p_vib_strength=0.0f;
		p_vib_speed=0.0f;
		p_vib_delay=0.0f;

		p_env_attack=0.0f;
		p_env_sustain=0.3f;
		p_env_decay=0.4f;
		p_env_punch=0.0f;

		filter_on=false;
		p_lpf_resonance=0.0f;
		p_lpf_freq=1.0f;
		p_lpf_ramp=0.0f;
		p_hpf_freq=0.0f;
		p_hpf_ramp=0.0f;
	
		p_pha_offset=0.0f;
		p_pha_ramp=0.0f;

		p_repeat_speed=0.0f;

		p_arp_speed=0.0f;
		p_arp_mod=0.0f;

		master_vol=0.05f;
		sound_vol=0.5f;
	}

	int Sfxr::loadParams(const char* aFilename)
	{
		FILE* file=fopen(aFilename, "rb");
		if(!file)
			return 1;

		int version=0;
		fread(&version, 1, sizeof(int), file);
		if(version!=100 && version!=101 && version!=102)
		{
			fclose(file);
			return 1;
		}

		fread(&wave_type, 1, sizeof(int), file);


		sound_vol=0.5f;
		if(version==102)
			fread(&sound_vol, 1, sizeof(float), file);

		fread(&p_base_freq, 1, sizeof(float), file);
		fread(&p_freq_limit, 1, sizeof(float), file);
		fread(&p_freq_ramp, 1, sizeof(float), file);
		if(version>=101)
			fread(&p_freq_dramp, 1, sizeof(float), file);
		fread(&p_duty, 1, sizeof(float), file);
		fread(&p_duty_ramp, 1, sizeof(float), file);

		fread(&p_vib_strength, 1, sizeof(float), file);
		fread(&p_vib_speed, 1, sizeof(float), file);
		fread(&p_vib_delay, 1, sizeof(float), file);

		fread(&p_env_attack, 1, sizeof(float), file);
		fread(&p_env_sustain, 1, sizeof(float), file);
		fread(&p_env_decay, 1, sizeof(float), file);
		fread(&p_env_punch, 1, sizeof(float), file);

		fread(&filter_on, 1, sizeof(bool), file);
		fread(&p_lpf_resonance, 1, sizeof(float), file);
		fread(&p_lpf_freq, 1, sizeof(float), file);
		fread(&p_lpf_ramp, 1, sizeof(float), file);
		fread(&p_hpf_freq, 1, sizeof(float), file);
		fread(&p_hpf_ramp, 1, sizeof(float), file);
	
		fread(&p_pha_offset, 1, sizeof(float), file);
		fread(&p_pha_ramp, 1, sizeof(float), file);

		fread(&p_repeat_speed, 1, sizeof(float), file);

		if(version>=101)
		{
			fread(&p_arp_speed, 1, sizeof(float), file);
			fread(&p_arp_mod, 1, sizeof(float), file);
		}

		fclose(file);
		return 0;
	}

	Sfxr::Sfxr()
	{
		resetParams();
		mBaseSamplerate = 44100;
	}


	AudioSourceInstance * Sfxr::createInstance() 
	{
		return new SfxrInstance(this);
	}

};