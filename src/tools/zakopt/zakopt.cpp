#include <stdlib.h>
#include <stdio.h>
#include <string.h>
extern "C"
{
#include "zx7.h"
}
#define ZX7DECOMPRESS_IMPLEMENTATION
#include "zx7decompress.h"

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
int debug = 0;

void write_short(unsigned short v)
{
    outbuf[outbuf_idx] = v;
    outbuf_idx++;
}

int write_delay(int delay)
{
    int writes = 0;
    while (delay >= 0x7fff * 2)
    {
        write_short(0x8000);
        writes++;
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
        writes++;
    }
    return writes;
}

void printPascalString(char* s)
{
    int len = *(unsigned char*)s;    
    s++;
    printf("\"");
    while (len)
    {
        printf("%c", *s);
        s++;
        len--;
    }
    printf("\"");
}

class Zak
{
public:
    int hdrsize;
    int chiptype;
    int flags;
    int kchunks;
    int lastchunk;
    int looppos;
    int emuspeed;
    int chipspeed;
    int totalsize;
    int ay;
    int min_delay;

    unsigned char* header;    
    unsigned char* data;
    int datasize;
    int origlen;

    void print_info()
    {
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
        char* stringptr = (char*)header + 28;
        printf("Song name    : ");
        printPascalString(stringptr);
        stringptr += *(unsigned char*)stringptr + 1;
        printf("\nAuthor       : ");
        printPascalString(stringptr);
        stringptr += *(unsigned char*)stringptr + 1;
        printf("\nComment      : ");
        printPascalString(stringptr);
        printf("\n\n");
    }

    int load(const char* fname)
    {
        int len;
        char* b = loadFile(fname, len);
        if (b == 0)
            return 0;

        if (((unsigned int*)b)[0] != 'PIHC' && ((unsigned int*)b)[1] != 'ENUT') // 'CHIP' 'TUNE'
        {
            printf("Unknown header in %s\n", fname);
            delete[] b;
            return -1;
        }


        hdrsize = *(unsigned short*)(b + 8);
        chiptype = *(unsigned char*)(b + 10);
        flags = *(unsigned char*)(b + 11);
        kchunks = *(unsigned short*)(b + 12);
        lastchunk = *(unsigned short*)(b + 14);
        looppos = *(unsigned int*)(b + 16);
        emuspeed = *(unsigned int*)(b + 20);
        chipspeed = *(unsigned int*)(b + 24);
        totalsize = (kchunks - 1) * 1024 + lastchunk;
        ay = 0;

        header = new unsigned char[hdrsize];
        memcpy(header, b, hdrsize);
        data = new unsigned char[len - hdrsize];
        memcpy(data, b + hdrsize, len - hdrsize);
        delete[] b;
        datasize = len - hdrsize;
        origlen = datasize;

        if (totalsize != datasize)
            printf("borked\n");

        return 1;
    }

    void decompress()
    {
        if (flags & 1)
        {
            printf("- Uncompressed data\n");
            return;
        }
        printf("- Decompressing\n");
        unsigned char* u = new unsigned char[kchunks * 1024];

        int inputofs = 0;
        for (int i = 0; i < kchunks; i++)
        {
            inputofs += zx7_decompress(data + inputofs, u + 1024 * i);
        }
        delete[] data;
        data = u;
        datasize = (kchunks - 1) * 1024 + lastchunk;
        flags |= 1;
    }

    void compress()
    {
        if ((flags & 1) == 0)
        {
            printf("- Already compressed\n");
            return;
        }

        printf("- Compressing\n");
        flags = flags & ~(1); // remove uncompressed flag
        *(unsigned char*)(header + 11) = flags;

        Optimal* o;
        unsigned char* cd = 0;
        unsigned char* ob = new unsigned char[kchunks * 2048]; // should be enough in all cases
        datasize = 0;
        size_t sz;
        long dt;
        int i;
        for (i = 0; i < kchunks; i++)
        {
            if (debug)
            {
                printf("- Chunk%4d/%d", i, kchunks);
            }
            else
            {
                printf("\r- Chunk%4d/%d", i, kchunks);
            }
            if (i == 0)
            {
                o = ::optimize(data, 1024, 0);
                cd = ::compress(o, data, 1024, 0, &sz, &dt);
            }
            else
            {
                o = ::optimize(data + ((i - 1) * 1024), 1024 * 2, 1024);
                cd = ::compress(o, data + ((i - 1) * 1024), 1024 * 2, 1024, &sz, &dt);
            }
            if (debug) printf(" %d bytes\n", (int)sz);
            memcpy(ob + datasize, cd, sz);
            datasize += sz;
            free(o);
            free(cd);
        }
        delete[] data;
        data = ob;
    }

    void find_shortest_delay()
    {
        int prev_command = 1;
        min_delay = 0xfffff;
        int delay = 0;
        unsigned short* d = (unsigned short*)data;
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
                if (prev_command == 0)
                {
                    if (delay < min_delay)
                        min_delay = delay;
                }
                delay = 0;
                prev_command = 1;
            }
        }
        printf("- Minimum delay between commands %d\n", min_delay);
    }

    void optimize()
    {
        delete[] outbuf;
        outbuf_idx = 0;
        outbuf = new unsigned short[totalsize * 2]; // twice the size it should need

        int convert_delay_to_50hz = 0;
        int combine_small_delays = 1;
        if ((emuspeed / min_delay) == 50)
        {
            printf("- 50Hz update detected\n");
            convert_delay_to_50hz = 1;
            combine_small_delays = 0;
            emuspeed = 50; // change emu speed to 50Hz
            *(unsigned int*)(header + 20) = emuspeed;
        }

        if (combine_small_delays)
        {
            if (min_delay < emuspeed / 2000) min_delay = emuspeed / 2000;
            if (min_delay > emuspeed / 200) min_delay = emuspeed / 200;
            printf("- Combining small delays (up to %d clocks)\n", min_delay * 2);
        }

        int* regval = new int[256];
        for (int i = 0; i < 256; i++)
            regval[i] = -1;

        int skipped_regwrites = 0;
        int looppos_adjust = 0;
        int written_regwrites = 0;
        int written_delays = 0;
        int found_delays = 0;

        unsigned short* d = (unsigned short*)data;
        int delay = 0;
        for (int i = 0; i < totalsize / 2; i++)
        {
            if (i < (looppos / 2)) looppos_adjust -= 2;
            if (d[i] & 0x8000)
            {                
                // delay opcode
                found_delays++;
                unsigned short dt = d[i] ^ 0x8000;
                delay += dt;
                if (combine_small_delays)
                {
                    if (dt > min_delay * 2 || delay > 1000)
                    {
                        int wc = write_delay(delay);
                        written_delays++;
                        if (i < (looppos / 2)) looppos_adjust += 2 * wc;
                        delay = 0;
                    }
                }
                else
                {
                    if (convert_delay_to_50hz)
                    {
                        // nop - we'll combine multiple frame delays if able
                    }
                    else
                    {
                        int wc = write_delay(delay);
                        written_delays++;
                        if (i < (looppos / 2)) looppos_adjust += 2 * wc;
                        delay = 0;
                    }
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
                            int wc = write_delay(delay);
                            written_delays++;
                            if (i < (looppos / 2)) looppos_adjust += 2 * wc;
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
                            int wc = write_delay(frames);
                            written_delays++;
                            if (i < (looppos / 2)) looppos_adjust += 2 * wc;
                        }
                    }
                    write_short(d[i]);
                    written_regwrites++;
                    if (i < (looppos / 2)) looppos_adjust += 2;
                }
                else
                {
                    skipped_regwrites++;
                }
            }
        }
        delete[] regval;
        printf("- Skipped %d regwrites and reduced %d delays\n", skipped_regwrites, found_delays - written_delays);
        printf("- Written delays:%d, regwrites:%d\n", written_delays, written_regwrites);
        printf("- Reg writes/delay opcodes after processing %d -> %d (%+d) bytes\n", totalsize, outbuf_idx * 2, outbuf_idx * 2 - totalsize);

        totalsize = outbuf_idx * 2;
        // Remember the actual size of last chunk
        lastchunk = (outbuf_idx * 2) & 1023;
        // Fill last chunk with zeros
        while ((outbuf_idx * 2) & 1023) write_short(0);
        // Calculate number of k-chunks
        kchunks = (outbuf_idx * 2) / 1024;
        *(unsigned short*)(header + 12) = kchunks;
        *(unsigned short*)(header + 14) = lastchunk;
        printf("- New 1k chunks:%d, last chunk:%d bytes\n", kchunks, lastchunk);
        delete[] data;
        data = (unsigned char*)outbuf;
        outbuf = 0;
        looppos += looppos_adjust;
        *(unsigned int*)(header + 16) = looppos;
        printf("- Loop position moved %+d bytes\n", looppos_adjust);
    }

    void save(const char* filename)
    {
        FILE* f = fopen(filename, "wb");
        if (!f)
        {
            printf("Unable to save %s\n", filename);
            exit(-1);
        }
        fwrite(header, 1, hdrsize, f);
        fwrite(data, 1, datasize, f);
        fclose(f);
    }

    void print_final_stats()
    {
        printf("\r- %d->%d bytes (%d%%)\n", origlen, datasize, (datasize * 100) / origlen);
    }
};

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
            if (_stricmp(pars[i], "-d") == 0)
            {
                debug = 1;
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
            "-u output uncompressed song data\n"
            "-d print diagnostic info\n\n", pars[0]);
        return 1;
    }

    printf("\n");

    Zak zak;

    if (!zak.load(infilename))
    {
        printf("Unable to load %s\n", infilename);
        return 1;
    }
    
    zak.print_info();

    zak.decompress();

    zak.find_shortest_delay();
    
    zak.optimize();

    if (compressed)
        zak.compress();

    zak.save(outfilename);

    zak.print_final_stats();
 
    return 0;
}