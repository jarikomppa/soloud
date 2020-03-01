#ifndef _PT3SONG_H_INCLUDED
#define _PT3SONG_H_INCLUDED

#include "chipsong.h"

class PT3Player;

class PT3Song : public ChipSong
{
public:

        virtual ChipPlayer *CreatePlayer(unsigned sample_fq);
        PT3Song(FILE *file);
        ~PT3Song();

        virtual unsigned get_readable_props() { return SP_TITLE_MASK | SP_AUTHOR_MASK | SP_TS_MASK | SP_PLAYTIME_MASK; }
        virtual SSK_ERRCODE get_property(SONG_PROP code, char *buffer, size_t bufsize);

private:
        PT3Player* player;
};


#endif // _PT3SONG_H_INCLUDED
