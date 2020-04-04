#ifndef _VTXSONG_H_INCLUDED
#define _VTXSONG_H_INCLUDED

#include "chipsong.h"

#ifdef __GNUC__
#pragma pack(push,1)
#else
#pragma pack(push)
#pragma pack(1)
#endif
struct VTXHDR
{
   unsigned short sig;
   unsigned char stereomode;
   unsigned short loop;
   unsigned chipfq;
   unsigned char intfq;
   unsigned short year;
   unsigned vtxsize;
};
#pragma pack(pop)
const unsigned short VTXv2_SIG_AY = 0x7961; // "ay"
const unsigned short VTXv2_SIG_YM = 0x6D79; // "ym"


class VtxSong : public ChipSong
{
 public:

   virtual ChipPlayer *CreatePlayer(unsigned sample_fq);

   virtual unsigned get_readable_props() { return SP_TITLE_MASK | SP_AUTHOR_MASK | SP_ALBUM_MASK | SP_COMMENT_MASK | SP_PLAYTIME_MASK; }
   // virtual unsigned get_writeable_props() { return 0; }
   // virtual unsigned get_volatile_props() { return 0; }

   virtual SSK_ERRCODE get_property(SONG_PROP code, char *buffer, size_t bufsize);
   // virtual SSK_ERRCODE set_property(SONG_PROP code, char *buffer, bool update_file) { return SSKERR_NOT_IMPLEMENTED; }

   VtxSong(FILE *file);

 private:

   VTXHDR hdr;

   void get_string_fields();
   char *title, *author, *program, *tracker, *comment;
   unsigned char *pdata;

   void set_chip(ChipPlayer *pl);
};


#endif // _VTXSONG_H_INCLUDED
