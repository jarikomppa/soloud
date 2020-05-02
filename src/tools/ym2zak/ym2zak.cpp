/*
* ym to zak format converter by Jari Komppa (c) 2020, based on
* st-sound, by taking the loader and removing most of the other code.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "YmMusic.h"


void write_dword(FILE* f, unsigned int dword)
{
	fwrite(&dword, 1, 4, f);
}

void write_word(FILE* f, unsigned short word)
{
	fwrite(&word, 1, 2, f);
}

void write_byte(FILE* f, unsigned char byte)
{
	fwrite(&byte, 1, 1, f);
}

void write_buffer(FILE* f, const char* buf, int bytes)
{
	fwrite(buf, 1, bytes, f);
}

void write_string(FILE* f, const char* s)
{
	int len = strlen(s);
	unsigned char clen = (unsigned char)len;
	if (len != clen) clen = 255; // crop it
	write_byte(f, clen);
	write_buffer(f, s, len);
}

#define SONGDATAOFS 8
#define SONGSIZEOFS 12

// Skip garbage strings
int checkstring(char* p)
{
	if (p == 0) return 0;
	if (*p == 0) return 0;
	int validchars = 0;
	int chars = 0;
	while (*p)
	{
		if (*p >= 31 && *p < 127)
			validchars++;
		chars++;
		p++;
	}
	if (validchars < 2) return 0;
	if (validchars < chars / 2) return 0;
	return 1;
}

int main(int argc, char **args)
{
	printf(	"ym2zak ym to zak converter.\n");

	if (argc < 2)
	{
		printf("Usage: ym2zak <ym music file>\n\n");
		return -1;
	}

	CYmMusic *m = new CYmMusic;

	if (m->load(args[1]))
	{
		if (m->nbDrum > 0)
		{
			printf("Song contains digidrums, which is not supported (yet?)\n");
			return -1;
		}

/*
		pInfo->pSongName = pSongName;
		pInfo->pSongAuthor = pSongAuthor;
		pInfo->pSongComment = pSongComment;
		pInfo->pSongType = pSongType;
		pInfo->pSongPlayer = pSongPlayer;

		pInfo->musicTimeInMs = getMusicTime();
		pInfo->musicTimeInSec = pInfo->musicTimeInMs / 1000;
*/

		char temp[512];
		sprintf(temp, "%s_ym.zak", args[1]);
		printf("Writing %s\n", temp);
		FILE* f = fopen(temp, "wb");

		write_dword(f, 'PIHC'); // chip
		write_dword(f, 'ENUT'); // tune
		write_word(f, 0); // song data ofs
		write_byte(f, 1); // 1 ym chip
		write_byte(f, 1 | 64); // uncompressed data, 64 = YM2149F
		write_word(f, 0); // number of 1024 chunks of song data
		write_word(f, 0); // bytes in last chunk
		write_dword(f, 0); // loop position in bytes
		write_dword(f, 50); // .ym files are all 50hz
		write_dword(f, 2000000); // atari-st ym rate

		if (checkstring(m->pSongName))
		{
			write_string(f, m->pSongName);
		}
		else
		{
			write_byte(f, 0);
		}
		if (checkstring(m->pSongAuthor))
		{
			write_string(f, m->pSongAuthor);
		}
		else
		{
			write_byte(f, 0);
		}
		if (checkstring(m->pSongComment))
		{
			write_string(f, m->pSongComment);
		}
		else
		{
			write_byte(f, 0);
		}

		int ofs = ftell(f);
		fseek(f, SONGDATAOFS, SEEK_SET);
		write_word(f, ofs);
		fseek(f, 0, SEEK_END);

		uint64_t lasttime = 0;
		int delaycount = 0;
		int regwritecount = 0;
		int duplicatecount = 0;

		int oldvalue[256];
		for (int i = 0; i < 256; i++)
			oldvalue[i] = -1;

		int looppos = 0;
		int eventofs = ftell(f);
		m->pDataStream;
		m->streamInc;
		m->loopFrame;
		m->nbFrame;

		for (unsigned int i = 0; i < (unsigned int)m->nbFrame; i++)
		{
			unsigned char* p = m->pDataStream + m->streamInc * i;			
			if (i == m->loopFrame)
				looppos = ftell(f) - eventofs;
			// each frame contains all register writes, we'll ignore dupes..
			for (unsigned int r = 0; r < 14; r++)
			{
				// Don't ignore envelope register rewrites
				if ((r & 0xf) != 13 && oldvalue[r] == p[r])
				{
					duplicatecount++;
				}
				else
				{
					oldvalue[r] = p[r];
					// Write register write; highest bit always 0 (less than 255 registers)
					unsigned short cmd = (r << 8) | (p[r]);
					fwrite(&cmd, 1, 2, f);
					regwritecount++;
				}
			}
			// Write time delay info (15 bits; highest bit on)
			// this may lead to repeated delays, zakopt will combine them
			unsigned short to = 1; // always wait one frame
			to |= 0x8000;
			fwrite(&to, 1, 2, f);
			delaycount++;
		}
		int songdatasize = (delaycount + regwritecount) * 2;
		fseek(f, SONGSIZEOFS, SEEK_SET);
		int chunks = songdatasize / 1024;
		int lastchunk = songdatasize & 1023;
		if (lastchunk == 0) lastchunk = 1024;
		if (songdatasize & 1023) chunks++;
		write_word(f, chunks); // number of 1024 chunks of song data
		write_word(f, lastchunk); // bytes in last chunk
		write_word(f, looppos / 1024); // loop chunk
		write_word(f, looppos % 1024); // loop byte

		fclose(f);
		printf(
			"%d writes ignored (register already set to value)\n"
			"%d delay commands\n"
			"%d regwrite commands\n"
			"%d bytes\n",
			duplicatecount, delaycount, regwritecount, songdatasize);
	}
	else
	{	// Error in loading music.
		printf("Error in loading file %s:\n%s\n", args[1], m->pLastError);
		return -1;
	}

	delete m;

	return 0;
}

