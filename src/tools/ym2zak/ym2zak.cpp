#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Work in progress. Only handles specific versions of ym files,
doesn't decompress from lha, doesn't save digidrums, etc.

*/

char *loadfile(const char *fn, int &len)
{
    FILE * f = fopen(fn, "rb");
    if (!f)
    {
        return 0;
    }
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    char *b = new char[len];
    if (!b)
    { 
        fclose(f); 
        return 0; 
    }
    fseek(f, 0, SEEK_SET);
    fread(b, 1, len, f);
    fclose(f);
    return b;
}

int swap16(unsigned short a) 
{
    return (((a&0xff) << 8) | ((a >> 8) & 0xff));
}

int swap32(unsigned int a) 
{
    return (
        ((a << 24) & 0xff000000) | 
        ((a << 8)  & 0xff0000) |
        ((a >> 8)  & 0xff00) |
        ((a >> 24) & 0xff)
        );
}

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

int main(int parc, char ** pars)
{
    printf("Atari ST .ym to zak converter by Jari Komppa\n");
    if (parc < 2)
    {
        printf("Usage: give me a .ym file as a parameter\n");
        return 0;
    }
    int len;
    char *b = loadfile(pars[1], len);
    if (!b)
    {
        printf("Unable to load %s\n", pars[1]);
        return 0;
    }
    if (*(unsigned int*)(b+0) != '!6MY' ||
        *(unsigned int*)(b+4) != 'nOeL' ||
        *(unsigned int*)(b+8) != '!DrA')
    {
        printf("Header doesn't start with YM6!LeOnArD!.\n"
               "This may be a compressed YM file. Rename to .lha and decompress with 7zip.\n");
        return 0;
    }
    int frames = swap32(*(unsigned int*)(b+12));
    int attrib = swap32(*(unsigned int*)(b+16));
    int digidrum = swap16(*(unsigned short*)(b+20));
    int clockrate = swap32(*(unsigned int*)(b+22));
    int refresh = swap16(*(unsigned short*)(b+26));
    int loop = swap32(*(unsigned int*)(b+28));
    int extra = swap32(*(unsigned short*)(b+32));
        
    printf(
        "Frames   :%d\n"
        "Attrib   :%d\n"
        "Digidrums:%d\n"
        "Clockrate:%d\n"
        "Refresh  :%d\n"
        "Loopframe:%d\n"
        "Extra    :%d\n",
        frames,
        attrib,
        digidrum,
        clockrate,
        refresh,
        loop,
        extra);
    
    if (digidrum > 0)
    {
        printf("Formats with digidrum not supported\n");
        return 0;
    }
    if (extra > 0)
    {
        printf("Formats with extra not supported\n");
        return 0;
    }

    unsigned char *o = (unsigned char*)b + 34;

    printf("Song name:%s\n", o);
    char* songname = (char*)o;
    o += strlen((char*)o) + 1;
    printf("Author   :%s\n", o);
    char* author = (char*)o;
    o += strlen((char*)o) + 1;
    printf("Comment  :%s\n", o);    
    char* comment = (char*)o;
    o += strlen((char*)o) + 1;
    if (*(unsigned int*)(o + frames * 16) != '!dnE')
        printf("Warning: End signature not found\n");

    char temp[512];
    sprintf(temp, "%s_ym.zak", pars[1]);
    FILE * f = fopen(temp, "wb");
    write_dword(f, 'PIHC'); // chip
    write_dword(f, 'ENUT'); // tune
    write_word(f, 0); // song data ofs
    write_byte(f, 1); // ym
    write_byte(f, 1 | 64); // uncompressed data, YM2149F
    write_word(f, 0); // number of 1024 chunks of song data
    write_word(f, 0); // bytes in last chunk
    write_dword(f, 0); // loop position in bytes
    write_dword(f, 50); // all ym files are frame synced
    write_dword(f, 2000000); // Atari ST ym rate
    write_string(f, songname);
    write_string(f, author);
    write_string(f, comment);

    int ofs = ftell(f);
    fseek(f, SONGDATAOFS, SEEK_SET);
    write_word(f, ofs);
    fseek(f, 0, SEEK_END);


    int songdatasize = 0;
    int oldvalue[16];
    for (int i = 0; i < 16; i++)
        oldvalue[i] = -1;

    int looppos = 0;
    int dataofs = ftell(f);

    for (int frame = 0; frame < frames; frame++)
    {
        if (frame == loop)
        {
            looppos = ftell(f) - dataofs;
        }
        // delay one frame
        write_word(f, 0x8001);
        songdatasize += 2;
        for (int reg = 0; reg < 14; reg++)
        {
            int val = o[reg * frames + frame];
            if (reg == 13 || oldvalue[reg] != val)
            {
                unsigned short v = (reg << 8) | val;
                write_word(f, v);
                oldvalue[reg] = val;
                songdatasize += 2;
            }
        }
    }
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
    printf("%s written.\n", temp);
    return 0;
}
