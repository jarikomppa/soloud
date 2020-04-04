#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern "C"
{
#include "zx7.h"
}
#define ZX7DECOMPRESS_IMPLEMENTATION
#include "zx7decompress.h"

struct Node
{
    Node *mNext;
    int key;
    int value;
    Node() { mNext = 0; key = 0; value = 0; }
    Node(int aKey) { key = aKey; mNext = 0; value = 0; }
};

#define BUCKETS 256
Node *nodes[BUCKETS];

void initHash()
{
    for (int i = 0; i < BUCKETS; i++)
        nodes[i] = 0;
}

Node *getHashNode(int key)
{
    int hash = key % BUCKETS;    
    if (nodes[hash] == 0)
    {
        Node *n = new Node(key);
        nodes[hash] = n;
        return n;
    }    
    
    Node *walker = nodes[hash];
    while (walker)
    {
        if (walker->key == key)
        {
            return walker;        
        }
        walker = walker->mNext;
    }
    Node *n = new Node(key);
    n->mNext = nodes[hash];
    nodes[hash] = n;
    return n;
}

char * loadFile(const char *fn, int &len)
{
    FILE * f = fopen(fn, "rb");
    if (!f)
    {
        return 0;
    }
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *b = new char[len];
    if (!b)
    {
        fclose(f);
        return 0;
    }
    fread(b, 1, len, f);
    fclose(f);
    return b;    
}

unsigned short* outbuf = 0;
int outbuf_idx = 0;

void write_short(unsigned short v)
{
    outbuf[outbuf_idx] = v;
    outbuf_idx++;
}

void write_delay(int delay)
{
    while (delay >= 0x7fff * 2)
    {
        write_short(0x8000);
        delay -= 0x7fff * 2;
    }
    while (delay)
    {
        unsigned short d = 0x7fff;
        if (delay < d)
            d = delay;
        delay -= d;
        d |= 0x8000;
        write_short(d);
    }
}

void printPascalString(char* s)
{
    int len = *(unsigned char*)s;    
    s++;
    while (len)
    {
        printf("%c", *s);
        s++;
        len--;
    }
    printf("\"");
}

int main(int parc, char ** pars)
{
    int compressed = 1;
    printf("Zak optimizer by Jari Komppa\n");

    char* infilename = 0;
    char* outfilename = 0;

    int badparams = 0;

    for (int i = 1; i < parc; i++)
    {
        if (pars[i][0] == '-')
        {
            if (_stricmp(pars[i], "-u") == 0)
            {
                compressed = 0;
            }
            else
            {
                badparams = 1;
            }
        }
        else
        {
            if (infilename == 0)
            {
                infilename = pars[i];
            } else
                if (outfilename == 0)
                {
                    outfilename = pars[i];
                }
                else
                {
                    printf("Unexpected parameter %s\n", pars[i]);
                    return 1;
                }
        }
    }

    if (outfilename == 0)
    {
        badparams = 1;
    }

    if (badparams)
    {
        printf(
            "Usage:\n"
            "%s [options] infilename outfilename\n"
            "Options:\n"
            "-u output uncompressed song data\n\n", pars[0]);
        return 1;
    }

    printf("\n");

    int len;
    char *b = loadFile(infilename, len);
    if (b == 0)
    {
        printf("Unable to load %s\n", infilename);
        return 1;
    }
    
    if (((unsigned int*)b)[0] != 'PIHC' && ((unsigned int*)b)[1] != 'ENUT') // 'CHIP' 'TUNE'
    {
        printf("Unknown header in %s\n", infilename);
        return -1;
    }

    int hdrsize = *(unsigned short*)(b + 8);
    int chiptype = *(unsigned char*)(b + 10);
    int flags = *(unsigned char*)(b + 11);
    int kchunks = *(unsigned short*)(b + 12);
    int lastchunk = *(unsigned short*)(b + 14);
    int looppos = *(unsigned int*)(b + 16);
    int emuspeed = *(unsigned int*)(b + 20);
    int chipspeed = *(unsigned int*)(b + 24);
    int totalsize = (kchunks - 1) * 1024 + lastchunk;
    int ay = 0;
    
    printf("Filename     : %s\n", infilename);
    printf("Chip type    : ");
    switch (chiptype)
    {
    case 0: printf("SID"); break;
    case 1: printf("AY/YM"); ay = 1;  break;
    case 2: printf("Turbosound"); ay = 1; break;
    case 3: printf("Turbosound Next"); ay = 1; break;
    case 4: printf("TED"); break;
    case 5: printf("2xSID"); break;
    case 6: printf("TED+2xSID"); break;
    case 7: printf("AdLib"); break;
    default:
        printf("Unknown");
    }
    printf("\nFlags        : ");
    if (flags & 1) printf("Uncompressed "); else printf("Compressed ");
    if (flags & 2) printf("Digidrums ");
    if (flags & 4) printf("Signed-digidrum ");
    if (flags & 8) printf("ST4-digidrum ");
    if ((flags & 48) == 0) printf("6581-SID ");
    if ((flags & 48) == 16) printf("8580-SID ");
    if ((flags & 48) == 32) printf("8580DB-SID ");
    if ((flags & 48) == 48) printf("6581R1-SID ");
    if (flags & 64) printf("YM2149F "); else printf("AY-3-8910 ");
    printf("\nHeader size  :%8d bytes\n", hdrsize);
    printf("1k chunks    :%8d\n", kchunks);
    printf("Last chunk   :%8d bytes\n", lastchunk);
    printf("Total size   :%8d bytes\n", totalsize);
    printf("Loop position:%8d bytes\n", looppos);
    printf("Emu speed    :%8d Hz\n", emuspeed);
    printf("Chip speed   :%8d Hz\n", chipspeed);
    char* stringptr = b + 28;
    printf("Song name    : ");
    printPascalString(stringptr);
    stringptr += *(unsigned char*)stringptr + 1;
    printf("\nAuthor       : ");
    printPascalString(stringptr);
    stringptr += *(unsigned char*)stringptr + 1;
    printf("\nComment      : ");
    printPascalString(stringptr);

    printf("\n\n");
    unsigned short* d = (unsigned short*)(b + hdrsize);

    if ((flags & 1) == 0)
    {
        printf("- Decompressing\n");
        unsigned char* input = (unsigned char*)(b + hdrsize);
        unsigned char* u = new unsigned char[kchunks * 1024];
        
        int inputofs = 0;
        for (int i = 0; i < kchunks; i++)
        {
            inputofs += zx7_decompress(input + inputofs, u + 1024 * i);            
        }
        d = (unsigned short*)u;
        flags |= 1;
    }
    
    // Figure out the smallest delays through hashmap
    initHash();
    
    int prev_command = 1;
    int min_delay = 0xfffff;
    int delay = 0;
    for (int i = 0; i < totalsize / 2; i++)
    {        
        if (d[i] & 0x8000)
        {
            if (d[i] == 0x8000)
            {
                //printf("Empty delay?\n");
                delay += 0x7fff * 2;
            }
            else
            {
                prev_command = 0;
                delay += d[i] ^ 0x8000;
            }
        }
        else
        {
            getHashNode(delay)->value++;
            if (prev_command == 0)
            {
                if (delay < min_delay)
                    min_delay = delay;
            }
            delay = 0;
            prev_command = 1;
        }
    }
    int mindelays = getHashNode(min_delay)->value;
    int longdelay = 0x7fff*2;
    printf("- Minimum delay between commands %d, appars %d times\n", min_delay, mindelays);

    // TODO: figure out if delays have common divisor with emuspeed and minimize emuspeed

    outbuf = new unsigned short[totalsize]; // should(tm) be enough


    int convert_delay_to_50hz = 0;
    int combine_small_delays = 1;
    if ((emuspeed / min_delay) == 50)
    {
        printf("- 50Hz update detected\n");
        convert_delay_to_50hz = 1;
        combine_small_delays = 0;
        emuspeed = 50; // change emu speed to 50Hz
        *(unsigned int*)(b + 20) = emuspeed;
    }

    if (combine_small_delays)
    {
        if (min_delay < 10) min_delay = 10;
        if (min_delay > 100) min_delay = 100;
        printf("- Combining small delays (up to %d clocks)\n", min_delay * 2);
    }

    int* regval = new int[256];
    for (int i = 0; i < 256; i++)
        regval[i] = -1;
    
    int skipped_regwrites = 0;
    
    for (int i = 0; i < totalsize / 2; i++)
    {        
        if (d[i] & 0x8000)
        {
            // delay opcode
            unsigned short dt = d[i] ^ 0x8000;
            delay += dt;
            if (combine_small_delays)
            {
                if (dt > min_delay * 2 || delay > 1000)
                {
                    write_delay(delay);
                    delay = 0;
                }
            }
            else
            if (convert_delay_to_50hz)
            {
                // nop - we'll combine multiple frame delays if able
            }
            else
            {
                write_delay(delay);
                delay = 0;
            }
        }
        else
        {
            // reg write opcode
            int reg = (d[i] & 0x7f00) >> 8;
            int val = (d[i] & 0x00ff) >> 0;
            if (regval[reg] != val || (ay && ((reg & 0xf) == 13)))
            {
                regval[reg] = val;
                if (combine_small_delays)
                {
                    if (delay >= emuspeed / 50) // flush delays at least 50Hz
                    {
                        write_delay(delay);
                        delay = 0;
                    }
                }
                if (convert_delay_to_50hz)
                {
                    if (delay)
                    {
                        int frames = 0;
                        while (delay >= min_delay)
                        {
                            delay -= min_delay;
                            frames++;
                        }
                        write_delay(frames);
                    }
                }
                write_short(d[i]);
            }
            else
            {
                skipped_regwrites++;
            }
        }
    }        
    printf("- Skipped %d regwrites\n", skipped_regwrites);
    printf("- Reg writes/delay opcodes after processing %d -> %d (%+d) bytes\n", totalsize, outbuf_idx * 2, outbuf_idx * 2 - totalsize);
    // Remember the actual size of last chunk
    lastchunk = (outbuf_idx * 2) & 1023;
    // Fill last chunk with zeros
    while ((outbuf_idx * 2) & 1023) write_short(0);
    // Calculate number of k-chunks
    kchunks = (outbuf_idx * 2) / 1024;
    *(unsigned short*)(b + 12) = kchunks;
    *(unsigned short*)(b + 14) = lastchunk;
    printf("- New 1k chunks:%d, last chunk:%d bytes\n", kchunks, lastchunk);

    FILE* f = fopen(outfilename, "wb");
    if (!f)
    {
        printf("Unable to write %s\n", outfilename);
        return 1;
    }
    if (compressed)
    {
        printf("- Compressing\n");
        flags = flags & ~(1); // remove uncompressed flag
    }
    *(unsigned char*)(b + 11) = flags;

    fwrite(b, 1, hdrsize, f);

    if (!compressed)
    {
        fwrite(outbuf, 1, (kchunks - 1) * 1024 + lastchunk, f); // for uncompressed output
    }

    if (compressed)
    {
        Optimal* o;
        unsigned char* cd = 0;
        unsigned char* ob = (unsigned char*)outbuf;
        size_t sz;
        long dt;
        int i;
        for (i = 0; i < kchunks; i++)
        {
            printf("\r- Chunk%04d/%d", i, kchunks);
            if (i == 0)
            {
                o = optimize(ob, 1024, 0);
                cd = compress(o, ob, 1024, 0, &sz, &dt);
            }
            else
            {
                o = optimize(ob + ((i - 1) * 1024), 1024 * 2, 1024);
                cd = compress(o, ob + ((i - 1) * 1024), 1024 * 2, 1024, &sz, &dt);
            }
            fwrite(cd, 1, sz, f);
            free(o);
            free(cd);
        }
    }

    int newlen = ftell(f);
    fclose(f);
    printf("\r- %s written, %d->%d bytes (%d%%)\n", outfilename, len, newlen, (newlen*100)/len);
    return 0;
}