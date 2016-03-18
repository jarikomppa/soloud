/*
SoLoud audio engine
Copyright (c) 2013-2015 Jari Komppa

file class for PhysicsFS filesystem
Copyright (se) 2016 amir ramezani

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

   3. This notice may not be removed or altered from any source
   distribution.
*/

 #ifndef SOLOUD_PHYSFS_H
#define SOLOUD_PHYSFS_H

#include <physfs.h>
#include "soloud_file.h"

namespace SoLoud
{

//this class is used for reading from and writing into physics FS files
class PhysfsFile: public File
{
PHYSFS_File *mFileHandle;
public:

virtual int eof();
virtual unsigned int read(unsigned char *aDst, unsigned int aBytes);
virtual unsigned int length();
virtual void seek(int aOffset);
virtual unsigned int pos();
virtual ~PhysfsFile();
PhysfsFile();
PhysfsFile(PHYSFS_File *fp);
result open(const char *aFilename);
virtual PHYSFS_File *getPhysfsFilePtr();
};
};

#endif
