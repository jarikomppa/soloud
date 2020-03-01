#ifndef _ERRCODE_H_INCLUDED
#define _ERRCODE_H_INCLUDED

enum SSK_ERRCODE
{
   SSKERR_OK = 0,
   SSKERR_NOT_IMPLEMENTED,
   SSKERR_OPEN_READONLY,        // when updating song, opened for r/o
   SSKERR_BUFFER_TOO_SMALL,
   SSKERR_PROP_TOO_LONG,        // new property value can't fit to file format
   SSKERR_PROP_NOT_INTEGER,
   SSKERR_PROP_INVALID,

};

const char *get_errdesc(SSK_ERRCODE code);


#endif // _ERRCODE_H_INCLUDED