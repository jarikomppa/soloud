#include <iostream>
#include "psid.h"
#include "tedmem.h"
#include "tedplay.h"

using namespace std;


static void printHeader()
{
	printf("TedSid2Zak - TED / SID register write dumper to zak format\nCopyright 2015-2020 Jari Komppa\n"
		"Based on tedplay - a (mostly) Commodore 264 family media player\nCopyright 2012 Attila Grosz\n"
		"\n");
}

static void printUsage()
{
	printf("Usage:\n"
		"tedsid2zak filename msecs [options]\n\n"
		"Where:\n"
		"-s 1-6, song speed. 3 = single, 5 = double. Default 3\n"
		"-m model 0:6581 1:8580 2:8580DB 3:6581R1. Default 1\n"
		"-t the number of sub-tune to play. Default 1\n"
		"-i Show information and quit\n"
		"-q Quantize timestamps by 1000 ticks\n\n"
		"Example:\n"
		"tedsid2dump foobar.sid 60000 -s 5 -m 0 -t 1\n\n");		
// s = setplaybackSpeed(3);// 5);// 3);
// m = setModel
// t = ~psidChangeTrack
}

extern void process(int ticks);
FILE * outfile = NULL;
int oldregs[1024];
int lasttime = 0;
int currtime = 0;
int firstwrite = 1;
int quantize = 0;
int regwrites = 0;


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

void storeregwrite(int reg, int value)
{
	if (!outfile)
		return;
	if (oldregs[reg] != value)
	{
		oldregs[reg] = value;
		int timedelta = (currtime / 2)- lasttime;
		// skip start silence
		if (firstwrite)
		{
			timedelta = 0;
			firstwrite = 0;
		}
		while (timedelta > 0x7fff)
		{
			unsigned short stimedelta = 0xffff;
			write_word(outfile, stimedelta);
			timedelta -= 0x7fff;
			lasttime += 0x7fff;
		}
		if (timedelta > 1000 * quantize)
		{			
			unsigned short stimedelta = timedelta | 0x8000;
			write_word(outfile, stimedelta);
			lasttime = currtime / 2;
		}
		unsigned short op = ((reg & 0x7f) << 8) | (value & 0xff);
		write_word(outfile, op);
		regwrites++;
	}
}

extern int selected_model;

int main(int argc, char *argv[])
{
	printHeader();
    if (argc < 3) {
        printUsage();
        return -1;
    }
	for (int i = 0; i < 1024; i++)
		oldregs[i] = -1;
	
	char * fn = NULL; // first non- '-' param
	int msec = 0; // second non- '-' param
	int speed = 3;
	int model = -1;
	int tune = 1;
	int info = 0;

	int i;
	for (i = 1; i < argc; i++)
	{
		char *s = argv[i];
		if (s[0] == '-')
		{
			switch (s[1])
			{
			case 's':
			case 'S':
				i++;
				speed = atoi(argv[i]);
				if (speed <= 0)
				{
					printf("Error: invalid speed number.\n");
					return -1;
				}
				break;
			case 'm':
			case 'M':
				i++;
				model = atoi(argv[i]);
				if (model <= 0)
				{
					printf("Error: invalid model number.\n");
					return -1;
				}
				break;
			case 't':
			case 'T':
				i++;
				tune = atoi(argv[i]);
				if (tune <= 0)
				{
					printf("Error: invalid tune number.\n");
					return -1;
				}
				break;
			case 'i':
			case 'I':
				info = 1;			
				break;
			case 'q':
			case 'Q':
				quantize = 1;
				break;
			}
		}
		else
		{
			if (fn == NULL)
			{
				fn = s;
				FILE * f = fopen(s, "rb");
				if (!f)
				{
					printf("Error: %s not found. Run without parameters for help.\n", s);
					return -1;
				}
				fclose(f);
			}
			else
			{
				msec = atoi(s);
				if (msec <= 0)
				{
					printf("Error: Milliseconds <= 0. Run without parameters for help.\n");
					return -1;
				}
			}
		}
	}

	if (fn == NULL)
	{
		printf("Error: No filename given. Run without parameters for help.\n");
		return -1;
	}

	int outputMilliseconds = msec;

	machineInit();
	int retval = tedplayMain(fn, model);
	if (0 == retval) 
	{
		printPsidInfo(getPsidHeader());
		if (info)
		{
			return 0;
		}
		tedPlaySetSpeed(speed);
		int i;
		for (i = 1; i < tune; i++)
			psidChangeTrack(1);
		
		char outfilename[2048];		
		int tedfile = 0;
		if (strstr(getPsidHeader().model, "SID") == NULL)
		{
			tedfile = 1;
			sprintf(outfilename, "%s_ted.zak", fn);
		}
		else
		{
			sprintf(outfilename, "%s_sid.zak", fn);
		}
		outfile = fopen(outfilename, "wb");
		write_dword(outfile, 'PIHC'); // chip
		write_dword(outfile, 'ENUT'); // tune
		write_word(outfile, 0); // song data ofs
		write_byte(outfile, tedfile ? 4 : 0);
		write_byte(outfile, 1 | (selected_model << 4)); // uncompressed data, sid model
		write_word(outfile, 0); // number of 1024 chunks of song data
		write_word(outfile, 0); // bytes in last chunk
		write_dword(outfile, 0); // loop position in bytes
		write_dword(outfile, 985248); // c64 clock
		write_dword(outfile, tedfile ? 886720 : 985248); // ted / sid rate
		write_string(outfile, getPsidHeader().title);
		write_string(outfile, getPsidHeader().author);
		write_string(outfile, getPsidHeader().copyright);
		int headersize = ftell(outfile);
		fseek(outfile, SONGDATAOFS, SEEK_SET);
		write_word(outfile, headersize);
		fseek(outfile, 0, SEEK_END);

		for (i = 0; i < outputMilliseconds; i++)
		{
			if (i % 1000 == 0 || i == outputMilliseconds-1)
			printf("\rRendering %02d:%02d (%3.1f%%)", (i+1) / (60 * 1000), ((i+1) / 1000) % 60, ((i+1)*100.0f)/outputMilliseconds);
			process(TED_SOUND_CLOCK / 1000); // 1 ms
		}

		int songdatasize = ftell(outfile) - headersize;
		fseek(outfile, SONGSIZEOFS, SEEK_SET);
		int chunks = songdatasize / 1024;
		int lastchunk = songdatasize & 1023;
		if (lastchunk == 0) lastchunk = 1024;
		if (songdatasize & 1023) chunks++;
		write_word(outfile, chunks); // number of 1024 chunks of song data
		write_word(outfile, lastchunk); // bytes in last chunk

		fclose(outfile);
		printf("\n%d regwrites written to %s\n", regwrites, outfilename);
		printf("\nAll done.\n");
		tedplayClose();
	}

    return retval;
}
