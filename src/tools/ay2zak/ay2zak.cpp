#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chipsong.h"

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

int main(int argc, char **args)
{
	printf("ZX Spectrum AY song to zax converter\n");
	if (argc < 2)
	{
		printf("Usage: %s inputfile\n", args[0]);
		return 0;
	}
	ChipSong *song = ChipSong::CreateFromFile(args[1], OPEN_READONLY);
	if (!song) 
	{ 
		printf("can't open <%s> or file format not supported\n", args[1]); 
		return -1; 
	}

	ChipPlayer *pl = song->CreatePlayer(44100);

	if (!pl) 
	{ 
		printf("can't play this file format"); 
		return -1; 
	}

	printf("%d events\n", pl->dumpsize);

	unsigned max_time_ms = pl->get_len(), time_s = max_time_ms / 1000;
	printf("playtime %d:%02d%s\n", time_s / 60, time_s % 60, pl->ts_mode ? " turboSound" : "");

	char temp[512];
	sprintf(temp, "%s_%s.zak", args[1], pl->ts_mode?"2ay":"ay");
	printf("Writing %s\n", temp);
	FILE * f = fopen(temp, "wb");
	
	write_dword(f, 'PIHC'); // chip
	write_dword(f, 'ENUT'); // tune
	write_word(f, 0); // song data ofs
	write_byte(f, pl->ts_mode ? 2 : 1); // 1 or 2 ay:s
	write_byte(f, 1); // uncompressed data
	write_word(f, 0); // number of 1024 chunks of song data
	write_word(f, 0); // bytes in last chunk
	write_dword(f, 0); // loop position in bytes
	write_dword(f, 3500000); // zx spectrum clock rate
	write_dword(f, 1774400); // zx spectrum ay rate
	
	if (song->get_property(SP_TITLE, temp, 512) == SSKERR_OK)
	{
		write_string(f, temp);
	}
	else
	{
		write_byte(f, 0);
	}
	if (song->get_property(SP_AUTHOR, temp, 512) == SSKERR_OK)
	{
		write_string(f, temp);
	}
	else
	{
		write_byte(f, 0);
	}
	if (song->get_property(SP_COMMENT, temp, 512) == SSKERR_OK)
	{
		write_string(f, temp);
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

	for (unsigned int i = 0; i < pl->dumpsize; i++)
	{
		if (i == pl->loop)
			looppos = ftell(f) - eventofs;
		// Don't ignore envelope register rewrites
		if ((pl->dump[i].num & 0xf) != 13 && oldvalue[pl->dump[i].num] == pl->dump[i].val)
		{
			duplicatecount++;
		}
		else
		{
			oldvalue[pl->dump[i].num] = pl->dump[i].val;
			if (pl->dump[i].time < lasttime)
			{
				printf("Error: Time moved backwards\n");
				lasttime = pl->dump[i].time;
			}
			if (pl->dump[i].time != lasttime)
			{
				uint64_t td = pl->dump[i].time - lasttime;
				if (td > 1774400 * 2)
				{
					printf("over 2 sec delay (%3.3f), skipping\n", td / 1774400.0f);
				}
				else
				{
					while (td > 0)
					{
						// Write time delay info (15 bits; highest bit on)
						unsigned short to = 0x7fff;
						if (td < to) to = (unsigned short)td;
						td -= to;
						//printf("%d\n", to);
						to |= 0x8000;
						fwrite(&to, 1, 2, f);
						delaycount++;
					}
				}
			}
			lasttime = pl->dump[i].time;
			// Write register write; highest bit always 0 (less than 255 registers)
			unsigned short cmd = (pl->dump[i].num << 8) | (pl->dump[i].val);
			fwrite(&cmd, 1, 2, f);
			regwritecount++;
		}
	}

	int songdatasize = (delaycount + regwritecount) * 2;
	fseek(f, SONGSIZEOFS, SEEK_SET);
	int chunks = songdatasize / 1024;
	int lastchunk = songdatasize & 1023;
	if (lastchunk == 0) lastchunk = 1024;
	if (songdatasize & 1023) chunks++;
	write_word(f, chunks); // number of 1024 chunks of song data
	write_word(f, lastchunk); // bytes in last chunk
	if (!pl->loop_enabled)
		looppos = 0;
	write_word(f, looppos / 1024); // loop chunk
	write_word(f, looppos % 1024); // loop byte

	fclose(f);
	printf(
		"%d writes ignored (register already set to value)\n"
		"%d delay commands\n"
		"%d regwrite commands\n"
		"%d bytes\n", 
		duplicatecount, delaycount, regwritecount, songdatasize);

	delete song;
	delete pl;
	return 0;
}
