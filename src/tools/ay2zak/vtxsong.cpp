#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "vtxsong.h"
#include "unlzh.h"

VtxSong::VtxSong(FILE *file) : ChipSong(file)
{
   if (!filedata || filesize < sizeof(VTXHDR)+10) return;
   memcpy(&hdr, filedata, sizeof(hdr));
   if (hdr.sig != VTXv2_SIG_AY && hdr.sig != VTXv2_SIG_YM) return;
   get_string_fields();
   valid = 1;
}

void VtxSong::get_string_fields()
{
   char *start = (char*)filedata + sizeof(VTXHDR);
   title = start; start += strlen(start)+1;
   author = start; start += strlen(start)+1;
   program = start; start += strlen(start)+1;
   tracker = start; start += strlen(start)+1;
   comment = start; start += strlen(start)+1;
   pdata = (unsigned char*)start;
}

SSK_ERRCODE VtxSong::get_property(SONG_PROP code, char *buffer, size_t bufsize)
{
   char *str;
   char tmp[32];
   switch (code) {
      case SP_TITLE:
         str = title;
         break;
      case SP_AUTHOR:
         str = author;
         break;
      case SP_ALBUM:
         str = program;
         break;
      case SP_COMMENT:
         str = comment;
         break;
      case SP_PLAYTIME:
         sprintf(tmp, "%d", hdr.vtxsize/14*20);
         str = tmp;
         break;
      default:
         return SSKERR_NOT_IMPLEMENTED;
   }
   if (strlen(str)+1 > bufsize)
      return SSKERR_BUFFER_TOO_SMALL;
   strcpy(buffer, str);
   return SSKERR_OK;
}


void VtxSong::set_chip(ChipPlayer *pl)
{
   const SNDCHIP_VOLTAB *vt = SNDR_VOL_YM;
   SNDCHIP::CHIP_TYPE ct = SNDCHIP::CHIP_YM;

   if (hdr.sig == VTXv2_SIG_AY) ct = SNDCHIP::CHIP_AY, vt = SNDR_VOL_AY;
   if (hdr.sig == VTXv2_SIG_YM) ct = SNDCHIP::CHIP_YM, vt = SNDR_VOL_YM;

   const SNDCHIP_PANTAB *pt = SNDR_PAN_ABC;
   static const SNDCHIP_PANTAB * const s_modes[7] =
   {
      SNDR_PAN_MONO, // 0
      SNDR_PAN_ABC, // 1
      SNDR_PAN_ACB, // 2
      SNDR_PAN_BAC, // 3
      SNDR_PAN_BCA, // 4
      SNDR_PAN_CAB, // 5
      SNDR_PAN_CBA  // 6
   };

   if (hdr.stereomode < 7) pt = s_modes[hdr.stereomode];

   pl->set_volumes(ct, 0x7FFF, vt, pt);
}

ChipPlayer *VtxSong::CreatePlayer(unsigned sample_fq)
{
   unsigned char *udata = (unsigned char*)malloc(hdr.vtxsize);
   decode_buffer(udata, hdr.vtxsize, pdata, filesize - ((int)pdata-(int)filedata));

   unsigned nrec = hdr.vtxsize / 14;
   unsigned char *newb = (unsigned char*)malloc(nrec*14);

   for (unsigned rec = 0; rec < nrec; rec++)
      for (unsigned j = 0; j < 14; j++)
         newb[rec*14+j] = udata[j*nrec+rec];

   free(udata);

   ChipPlayer *pl = new ChipPlayer();
   set_chip(pl);
   pl->set_timings(hdr.chipfq, hdr.chipfq, sample_fq);

   uint64_t t = 0; unsigned frame = 0;

   for (unsigned x = 0; x < hdr.vtxsize; x += 14, frame++) {

      for (int reg = 0; reg < 14; reg++) {
         if (reg == 13 && newb[x+reg] == 0xFF) continue;
         pl->reg_out(t, reg, newb[x+reg]);
      }
      t += hdr.chipfq / hdr.intfq;
      if (hdr.loop == frame) pl->set_loop();
   }

   free(newb);
   return pl;
}
