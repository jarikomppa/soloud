#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chipsong.h"

int main(int argc, char **args)
{
	printf("ZX Spectrum AY song to register dump converter for SoLoud\n");
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
	delete song;

	if (!pl) 
	{ 
		printf("can't play this file format"); 
		return -1; 
	}

	printf("%d events\n", pl->dumpsize);

	unsigned max_time_ms = pl->get_len(), time_s = max_time_ms / 1000;
	printf("playtime %d:%02d%s\n", time_s / 60, time_s % 60, pl->ts_mode ? " turboSound" : "");

	char temp[512];
	sprintf(temp, "%s.aydump", args[1]);
	FILE * f = fopen(temp, "wb");
	unsigned int tag = 'pmUD';
	fwrite(&tag, 1, 4, f);

	unsigned int lasttime = 0;
	int delaycount = 0;
	int regwritecount = 0;
	int duplicatecount = 0;

	int oldvalue[256];
	for (int i = 0; i < 256; i++)
		oldvalue[i] = -1;

	for (unsigned int i = 0; i < pl->dumpsize; i++)
	{
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
				int td = pl->dump[i].time - lasttime;
				while (td > 0)
				{
					// Write time delay info (15 bits; highest bit on)
					unsigned short to = 0x7fff;
					if (td < to) to = td;
					td -= to;
					//printf("%d\n", to);
					to |= 0x8000;
					fwrite(&to, 1, 2, f);
					delaycount++;
				}
			}
			lasttime = pl->dump[i].time;
			// Write register write; highest bit always 0 (less than 255 registers)
			unsigned short cmd = (pl->dump[i].num << 8) | (pl->dump[i].val);
			fwrite(&cmd, 1, 2, f);
			regwritecount++;
		}
	}
	fclose(f);
	printf("%s written\n", temp);
	printf(
		"%d writes ignored (register already set to value)\n"
		"%d delay commands\n"
		"%d regwrite commands\n"
		"%d bytes\n", 
		duplicatecount, delaycount, regwritecount, (delaycount + regwritecount) * 2);
}
