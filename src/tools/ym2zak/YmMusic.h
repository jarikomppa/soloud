/* ym2zak by Jari Komppa, (c) 2020 based on st-sound library under the following license */

/*-----------------------------------------------------------------------------
* ST-Sound, ATARI-ST Music Emulator
* Copyright (c) 1995-1999 Arnaud Carre ( http://leonard.oxg.free.fr )
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
* OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
* SUCH DAMAGE.
*
-----------------------------------------------------------------------------*/


#ifndef __YMMUSIC__
#define __YMMUSIC__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER

	typedef __int8 int8_t;
	typedef unsigned __int8 uint8_t;
	typedef __int16 int16_t;
	typedef unsigned __int16 uint16_t;
	typedef __int32 int32_t;
	typedef unsigned __int32 uint32_t;
	typedef __int64 int64_t;
	typedef unsigned __int64 uint64_t;

#else
#include <stdint.h>
#endif


#define YM_INTEGER_ONLY

	//-----------------------------------------------------------
	// Platform specific stuff
	//-----------------------------------------------------------

#ifdef YM_INTEGER_ONLY
	typedef	int64_t		yms64;
#else
	typedef	float		ymfloat;
#endif

	typedef	int8_t		yms8;			//  8 bits signed integer
	typedef	int16_t		yms16;			// 16 bits signed integer
	typedef	int32_t		yms32;			// 32 bits signed integer

	typedef	uint8_t		ymu8;			//  8 bits unsigned integer
	typedef	uint16_t	ymu16;			// 16 bits unsigned integer
	typedef	uint32_t	ymu32;			// 32 bits unsigned integer

	typedef	yms32		ymint;			// Native "int" for speed purpose. StSound suppose int is signed and at least 32bits. If not, change it to match to yms32
	typedef	char		ymchar;			// 8 bits char character (used for null terminated strings)


#ifndef NULL
#define NULL	(0L)
#endif

//-----------------------------------------------------------
// Multi-platform
//-----------------------------------------------------------
	typedef		int					ymbool;			// boolean ( theorically nothing is assumed for its size in StSound,so keep using int)
	typedef		yms16				ymsample;		// StSound emulator render mono 16bits signed PCM samples

#define		YMFALSE				(0)
#define		YMTRUE				(!YMFALSE)

#pragma pack(1)
	typedef struct
	{
		ymu8	size;
		ymu8	sum;
		char	id[5];
		ymu32	packed;
		ymu32	original;
		ymu8	reserved[5];
		ymu8	level;
		ymu8	name_lenght;
	} lzhHeader_t;
#pragma pack()



#define	MAX_DIGIDRUM	128

#define	YMTPREC		16
#define	MAX_VOICE	8

typedef enum
{
	YM_V2,
	YM_V3,
	YM_V4,
	YM_V5,
	YM_V6,
	YM_VMAX,

	YM_TRACKER1=32,
	YM_TRACKER2,
	YM_TRACKERMAX,

	YM_MIX1=64,
	YM_MIX2,
	YM_MIXMAX,
} ymFile_t;

typedef struct
{
	ymu32	sampleStart;
	ymu32	sampleLength;
	ymu16	nbRepeat;
	ymu16	replayFreq;
} mixBlock_t;

typedef struct
{
	ymu32		size;
	ymu8	*	pData;
	ymu32		repLen;
} digiDrum_t;

typedef struct
{
	ymint		nbVoice;
	ymu32		nbVbl;
	ymu8	*	pDataBufer;
	ymu32		currentVbl;
	ymu32		flags;
	ymbool		bLoop;
} ymTrackerPartoche_t;


typedef struct
{
	ymu8	*	pSample;
	ymu32		sampleSize;
	ymu32		samplePos;
	ymu32		repLen;
	yms32		sampleVolume;
	ymu32		sampleFreq;
	ymbool		bLoop;
	ymbool		bRunning;
} ymTrackerVoice_t;

typedef struct
{
	ymu8 noteOn;
	ymu8 volume;
	ymu8 freqHigh;
	ymu8 freqLow;
} ymTrackerLine_t;


enum
{
	A_STREAMINTERLEAVED = 1,
	A_DRUMSIGNED = 2,
	A_DRUM4BITS = 4,
	A_TIMECONTROL = 8,
	A_LOOPMODE = 16,
};


class	CYmMusic
{

public:
	CYmMusic(ymint _replayRate=44100);
	~CYmMusic();

	ymbool	load(const char *pName);

	void	unLoad(void);


//-------------------------------------------------------------
// WAVE Generator
//-------------------------------------------------------------

	ymbool	bMusicOver;

public:

	ymbool	checkCompilerTypes();

	ymu8 *depackFile(ymu32 size);
	ymbool	deInterleave(void);

	char	*pLastError;
	ymFile_t	songType;
	int		nbFrame;
	int		loopFrame;
	int		currentFrame;
	int		nbDrum;
	digiDrum_t *pDrumTab;
	int		musicTime;
	ymu8 *pBigMalloc;
	ymu8 *pDataStream;
	ymbool	bLoop;
	ymint	fileSize;
	ymbool	ymDecode(void);
	ymint		playerRate;
	ymint		attrib;
	volatile	ymbool	bMusicOk;
	volatile	ymbool	bPause;
	ymint		streamInc;
	ymint		innerSamplePos;
	ymint		replayRate;

	ymchar	*pSongName;
	ymchar	*pSongAuthor;
	ymchar	*pSongComment;
	ymchar	*pSongType;
	ymchar	*pSongPlayer;

//-------------------------------------------------------------
// ATARI Digi Mix Music.
//-------------------------------------------------------------

	ymint	nbRepeat;
	ymint	nbMixBlock;
	mixBlock_t *pMixBlock;
	ymint	mixPos;
	ymu8 *pBigSampleBuffer;
	ymu8	*pCurrentMixSample;
	ymu32	currentSampleLength;
	ymu32	currentPente;
	ymu32	currentPos;


	struct TimeKey
	{
		ymu32	time;
		ymu16	nRepeat;
		ymu16	nBlock;
	};

	ymint		m_nbTimeKey;
	TimeKey	*	m_pTimeInfo;
	ymu32		m_musicLenInMs;
	ymu32		m_iMusicPosAccurateSample;
	ymu32		m_iMusicPosInMs;

//-------------------------------------------------------------
// YM-Universal-Tracker
//-------------------------------------------------------------

	int			nbVoice;
	ymTrackerVoice_t	ymTrackerVoice[MAX_VOICE];
	int					ymTrackerNbSampleBefore;
	ymsample			ymTrackerVolumeTable[256*64];
	int					ymTrackerFreqShift;
};

#ifdef __cplusplus
}
#endif
#endif
