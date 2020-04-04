#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "chipsong.h"
#include "vtxsong.h"
#include "pt3song.h"
#include "ptxsong.h"

ChipSong::ChipSong()
{
   filedata = 0;
   songfile = 0;
   filesize = 0;
   valid = false;
}

ChipSong::ChipSong(FILE *file)
{
   valid = false;

   songfile = file;
   fseek(file, 0, SEEK_END);
   filesize = ftell(file); fseek(file, 0, SEEK_SET);
   filedata = (unsigned char*)malloc(filesize);
   if (!filedata) return;
   fread(filedata, 1, filesize, file);
}

ChipSong::~ChipSong()
{
   if (filedata) free(filedata);
   if (songfile) fclose(songfile);
}

#ifndef _WIN32_WCE
ChipSong *ChipSong::CreateFromFile(const char *filename, OPEN_MODE mode)
{
   FILE *file = fopen(filename, (mode == OPEN_READONLY)? "rb" : "w+b");
   if (!file) return 0;

   const char *extpos = strrchr(filename, '.');
   if (!extpos) extpos = ""; else extpos++;

   ChipSong *song = 0;

   if (!_stricmp(extpos, "vtx")) song = new VtxSong(file);
   if (!_stricmp(extpos, "pt2")) song = new PtXSong(file); // pt2 or pt3
   if (!_stricmp(extpos, "pt3")) song = new PT3Song(file);

   if (song && !song->valid) { song->songfile = 0; delete song; song = 0; }

   if (song) {
      if (mode == OPEN_READONLY)
         fclose(song->songfile), song->songfile = 0;
   } else
      fclose(file);

   return song;
}
#endif

#ifdef _WIN32
ChipSong *ChipSong::CreateFromFile(const wchar_t *filename, OPEN_MODE mode)
{
   FILE *file = _wfopen(filename, (mode == OPEN_READONLY)? L"rb" : L"w+b");
   if (!file) return 0;

   wchar_t *extpos = wcsrchr((wchar_t*)filename, '.'); // bug in clib: 1st param declared without 'const'
   if (!extpos) extpos = L""; else extpos++;

   ChipSong *song = 0;

   if (!_wcsicmp(extpos, L"vtx")) song = new VtxSong(file);
   if (!_wcsicmp(extpos, L"pt2")) song = new PtXSong(file); // pt2 or pt3
   if (!_wcsicmp(extpos, L"pt3")) song = new PT3Song(file);

   if (song && !song->valid) { song->songfile = 0; delete song; song = 0; }

   if (song) {
      if (mode == OPEN_READONLY)
         fclose(song->songfile), song->songfile = 0;
   } else
      fclose(file);

   return song;
}
#endif