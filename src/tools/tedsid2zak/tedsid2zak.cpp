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
		"-d Detect loop point and stop\n"
		"-w number Loop detection window size, 1024 by default\n"
		"-q Quantize timestamps by 1000 ticks\n\n"
		"Example:\n"
		"tedsid2dump foobar.sid 60000 -s 5 -m 0 -t 1\n\n"
		"In detection mode, msecs is the minimum length to record,\n"
		"after which the loop detection starts.\n\n");
}

extern void process(int ticks);
FILE * outfile = NULL;
int oldregs[1024];
int lasttime = 0;
int currtime = 0;
int firstwrite = 1;
int quantize = 0;
int regwrites = 0;
int headersize = 0;
int looppoint = 0;

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

unsigned short* op;
int ops = 0;
int detect = 0;
int firstkofs = 0;
int windowsize = 1024;

struct hashitem
{
	hashitem* mNext;
	int ofs;
	unsigned int hash;
};

hashitem* hashbucket[256];
hashitem* hashpile;
int pileidx = 0;

void store_hash(int ofs, unsigned int hash)
{
	if (pileidx == 0)
	{
		hashpile = new hashitem[1024 * 1024]; // million regwrites should be enough for everybody
		for (int i = 0; i < 256; i++)
			hashbucket[i] = 0;
	}
	hashpile[pileidx].hash = hash;
	hashpile[pileidx].ofs = ofs;
	hashpile[pileidx].mNext = hashbucket[hash % 256];
	hashbucket[hash % 256] = &hashpile[pileidx];
	pileidx++;
}

void finalize()
{
	fwrite(op, 2, ops, outfile);
	int songdatasize = ftell(outfile) - headersize;
	fseek(outfile, SONGSIZEOFS, SEEK_SET);
	int chunks = songdatasize / 1024;
	int lastchunk = songdatasize & 1023;
	if (lastchunk == 0) lastchunk = 1024;
	if (songdatasize & 1023) chunks++;
	write_word(outfile, chunks); // number of 1024 chunks of song data
	write_word(outfile, lastchunk); // bytes in last chunk
	write_dword(outfile, looppoint);

	fclose(outfile);
}

int mintime = 0;
extern int reset_done;
void storeregwrite(int reg, int value)
{
	if (!reset_done)
		return;
	
	if (!outfile)
		return;

	if (oldregs[reg] != value)
	{
		int timedelta = (currtime / 2)- lasttime;

		if (timedelta > 0x7fff && ops < 10)
		{
			// There may be a long pause at the start. Skip it and start over.
			ops = 0;
			regwrites = 0;
			lasttime = currtime / 2;
			timedelta = 0;
			for (int i = 0; i < 1024; i++)
				oldregs[i] = -1;
		}

		oldregs[reg] = value;
		while (timedelta > 0x7fff)
		{
			unsigned short stimedelta = 0xffff;
//			write_word(outfile, stimedelta);
			op[ops++] = stimedelta;
			timedelta -= 0x7fff;
			lasttime += 0x7fff;
		}
		if (timedelta > 1000 * quantize)
		{			
			unsigned short stimedelta = timedelta | 0x8000;
//			write_word(outfile, stimedelta);
			op[ops++] = stimedelta;
			lasttime = currtime / 2;
		}
		unsigned short opcode = ((reg & 0x7f) << 8) | (value & 0xff);
//		write_word(outfile, opcode);
		op[ops++] = opcode;
		regwrites++;
	}
	
	if (detect && ops > windowsize)
	{
		if (firstkofs == 0)
		{
			int back1k = ops;
			int count = 0;
			unsigned int hash = 0;
			while (back1k > 0 && count < windowsize)
			{
				if ((op[back1k] & 0x8000) == 0)
				{
					count++;
					hash = ((hash << 7) | (hash >> (32 - 7))) ^ op[back1k];
				}
				back1k--;
			}
			if (count == windowsize)
			{
				store_hash(back1k, hash);
			}
			int p = 0;
			
			if (mintime)
			{
				hashitem* walker = hashbucket[hash % 256];
				while (walker)
				{
					if (walker->hash == hash && walker->ofs != back1k)
					{
						p = walker->ofs;
						int c = 1;
						if (op[back1k] == op[p])
						{
							int a = 0;
							int b = 0;
							do {
								a++;
								while (((back1k + a) < ops) && (op[back1k + a] & 0x8000)) a++;
								b++;
								while (((p + b) < ops) && (op[p + b] & 0x8000)) b++;
								c++;
							} while (c < windowsize && op[back1k + a] == op[p + b]);

							if (c == windowsize)
							{
								printf("\nFound loop point %d -> %d\n", back1k, p);
								looppoint = p * 2;
								ops = back1k;
								finalize();
								exit(1);
							}
						}
					}
					walker = walker->mNext;
				}
			}
		}
	}
}

#if defined(_MSC_VER)
#include <windows.h>

long getmsec()
{
	LARGE_INTEGER ts, freq;

	QueryPerformanceCounter(&ts);
	QueryPerformanceFrequency(&freq);
	ts.QuadPart *= 1000;
	ts.QuadPart /= freq.QuadPart;

	return (long)ts.QuadPart;
}
#else
#include <time.h>

long getmsec()
{
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts->tv_sec * 1000) + (ts->tv_nsec / 1000000)
}
#endif


extern int selected_model;

int main(int argc, char *argv[])
{
	op = new unsigned short[4 * 1024 * 1024];
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
			case 'd':
			case 'D':
				detect = 1;
				break;
			case 'w':
			case 'W':				
				i++;
				windowsize = atoi(argv[i]);
				if (windowsize <= 0)
				{
					printf("Error: invalid window size.\n");
					return -1;
				}
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
		headersize = ftell(outfile);
		fseek(outfile, SONGDATAOFS, SEEK_SET);
		write_word(outfile, headersize);
		fseek(outfile, 0, SEEK_END);

		if (detect)
		{
			printf("Rendering at least %02d:%02d, then detecting loop point\n", (outputMilliseconds / 1000) / 60, (outputMilliseconds / 1000) % 60);
		}

		long start_t = getmsec();
		for (i = 0; detect || i < outputMilliseconds; i++)
		{
			mintime = i > outputMilliseconds;
			if (i % 1000 == 0 || i == outputMilliseconds - 1)
			{
				long t = (getmsec() - start_t) / 1000;
				if (detect)
				{
					printf("\r%sing %02d:%02d, realtime %02d:%02d",
						mintime ? "Detect" : "Render",
						(i + 1) / (60 * 1000),
						((i + 1) / 1000) % 60,
						t / 60,
						t % 60);
				}
				else
				{
					printf("\rRendering %02d:%02d (%3.1f%%), realtime %02d:%02d",
						(i + 1) / (60 * 1000),
						((i + 1) / 1000) % 60,
						((i + 1) * 100.0f) / outputMilliseconds,
						t / 60,
						t % 60);
				}
			}
			process(TED_SOUND_CLOCK / 1000); // 1 ms
		}

		finalize();

		printf("\n%d regwrites written to %s\n", regwrites, outfilename);
		printf("\nAll done.\n");
		tedplayClose();
	}

    return retval;
}
