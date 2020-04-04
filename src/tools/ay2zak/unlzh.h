#ifndef _UNLZH_H_INCLUDED
#define _UNLZH_H_INCLUDED

// this function is not thread-safe!
void decode_buffer(unsigned char *dst, unsigned dstsize, unsigned char *src, unsigned srcsize);

#endif // _UNLZH_H_INCLUDED
