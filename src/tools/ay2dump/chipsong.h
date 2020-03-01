#ifndef _CHIPSONG_H_INCLUDED
#define _CHIPSONG_H_INCLUDED

#include "sysdefs.h"
#include "errcode.h"
#include "chipplayer.h"

enum OPEN_MODE { OPEN_READONLY, OPEN_READWRITE };

enum SONG_PROP // song property code
{
   SP_FORMAT,  // pre-defined string like VTX, PT3, STP, STC, ...
   SP_PLAYTIME,// usually read-only, (ms)
   SP_LOOPPOS, // usually read-only, (ms)
   SP_LOOPCOUNT, // how many times repeat song

   SP_TITLE,
   SP_AUTHOR,
   SP_ALBUM,
   SP_TRACKER,
   SP_SOFTWARE,
   SP_COMMENT,
   SP_RELEASED, // party/software where first released
   SP_YEAR,

   SP_CHIP,     // AY-3-8910 or YM2149
   SP_STEREO,   // mono, ABC, ACB, etc...
   SP_INTFQ,
   SP_AYFQ,
   SP_CPUFQ,
   SP_TS,       // TurboSound flag

   SP_MAX       // total number of possible properties, up to 32
};

const unsigned SP_FORMAT_MASK   = 1 << SP_FORMAT;
const unsigned SP_PLAYTIME_MASK = 1 << SP_PLAYTIME;
const unsigned SP_LOOPPOS_MASK  = 1 << SP_LOOPPOS;
const unsigned SP_LOOPCOUNT_MASK= 1 << SP_LOOPCOUNT;
const unsigned SP_TITLE_MASK    = 1 << SP_TITLE;
const unsigned SP_AUTHOR_MASK   = 1 << SP_AUTHOR;
const unsigned SP_ALBUM_MASK    = 1 << SP_ALBUM;
const unsigned SP_TRACKER_MASK  = 1 << SP_TRACKER;
const unsigned SP_SOFTWARE_MASK = 1 << SP_SOFTWARE;
const unsigned SP_COMMENT_MASK  = 1 << SP_COMMENT;
const unsigned SP_RELEASED_MASK = 1 << SP_RELEASED;
const unsigned SP_YEAR_MASK     = 1 << SP_YEAR;
const unsigned SP_CHIP_MASK     = 1 << SP_CHIP;
const unsigned SP_STEREO_MASK   = 1 << SP_STEREO;
const unsigned SP_INTFQ_MASK    = 1 << SP_INTFQ;
const unsigned SP_AYFQ_MASK     = 1 << SP_AYFQ;
const unsigned SP_CPUFQ_MASK    = 1 << SP_CPUFQ;
const unsigned SP_TS_MASK       = 1 << SP_TS;

class ChipSong
{
 public:

   // return NULL if file format is unknown or failed to access file
   #ifndef _WIN32_WCE
   static ChipSong *CreateFromFile(const char *filename, OPEN_MODE mode);
   #endif

   #ifdef _WIN32
   static ChipSong *CreateFromFile(const wchar_t *filename, OPEN_MODE mode);
   #endif

   // return NULL if file format is not playable
   virtual ChipPlayer *CreatePlayer(unsigned sample_fq) { return 0; }

   // next 3 functions return properties bitmap (set of SP_XXX_MASK)
   virtual unsigned get_readable_props() { return 0; }
   virtual unsigned get_writeable_props() { return 0; }
   // volatile properties can't be saved into file
   // (for ex. SP_LOOPCOUNT, SP_CHIP, SP_AYFQ or SP_STEREO into PT3/STC file),
   // but can be set before creating player and affects playing
   virtual unsigned get_volatile_props() { return 0; }

   virtual SSK_ERRCODE get_property(SONG_PROP code, char *buffer, size_t bufsize) { return SSKERR_NOT_IMPLEMENTED; }
   virtual SSK_ERRCODE set_property(SONG_PROP code, const char *buffer, bool update_file) { return SSKERR_NOT_IMPLEMENTED; }

   ChipSong();
   ChipSong(FILE *file);
   virtual ~ChipSong();

 protected:

   FILE *songfile;
   bool valid;

   unsigned char *filedata;
   unsigned filesize;
};

#endif // _CHIPSONG_H_INCLUDED
