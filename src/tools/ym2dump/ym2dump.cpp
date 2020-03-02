#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int parc, char ** pars)
{
    printf("Atari ST .ym to aydump converter for SoLoud by Jari Komppa\n");
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

    char *o = b + 34;

    printf("Song name:%s\n", o);
    o += strlen(o) + 1;
    printf("Author   :%s\n", o);
    o += strlen(o) + 1;
    printf("Comment  :%s\n", o);    
    o += strlen(o) + 1;
    if (*(unsigned int*)(o + frames * 16) != '!dnE')
        printf("Warning: End signature not found\n");

    char temp[512];
    sprintf(temp, "%s.aydump", pars[1]);
    FILE * f = fopen(temp, "wb");
    unsigned int tag = 'pmUD';
    fwrite(&tag, 1, 4, f);
    int oldvalue[16] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
    for (int i = 0; i < frames; i++)
    {
        // instead of file's clock rate, use the simulated ay chip clock
        int delay = /*clockrate*/ 3500000 / refresh;
        while (delay)
        {
            unsigned short out = 0x7fff;
            if (delay < out)
                out = delay;
            delay -= out;
            out |= 0x8000;
            fwrite(&out, 1, 2, f);
        }
        for (int j = 0; j < 14; j++)
        {
            if (j == 13 || oldvalue[j] != o[j*frames+i])
            {
                unsigned char c;
                c = o[j*frames+i];
                fwrite(&c, 1, 1, f);
                c = j;
                fwrite(&c, 1, 1, f);
                oldvalue[j] = o[j*frames+i];
            }            
        }
    }
    fclose(f);
    printf("%s written.\n", temp);
    return 0;
}
