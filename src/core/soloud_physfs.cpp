/*
SoLoud audio engine
Copyright (c) 2013-2015 Jari Komppa

file class for physicsFS filesystem
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

#include "soloud_error.h"
#include "soloud_physfs.h"

namespace SoLoud
{

PhysfsFile::PhysfsFile()
{
PHYSFS_close(mFileHandle);
}

PhysfsFile::PhysfsFile(const char* aFilename)
{
open(aFilename);
}

PhysfsFile::PhysfsFile(PHYSFS_File *fp):
mFileHandle(fp)
{

}

int PhysfsFile::eof()
{
return PHYSFS_eof(mFileHandle);
}

unsigned int PhysfsFile::read(unsigned char *aDst, unsigned int aBytes)
{
return PHYSFS_read(mFileHandle, aDst, aBytes, 1);
}

unsigned int PhysfsFile::length()
{
return PHYSFS_fileLength(mFileHandle);
}

void PhysfsFile::seek(int aOffset)
{
PHYSFS_seek(mFileHandle, aOffset);
}

unsigned int PhysfsFile::pos()
{
return PHYSFS_tell(mFileHandle);
}

PhysfsFile::~PhysfsFile()
{
PHYSFS_close(mFileHandle);
}

result PhysfsFile::open(const char *aFilename)
{
if(!PHYSFS_exists(aFilename))
{
return FILE_NOT_FOUND;
}
mFileHandle=PHYSFS_openRead(aFilename);
if(!mFileHandle)
{
return FILE_LOAD_FAILED;
}
return SO_NO_ERROR;
}

PHYSFS_File *PhysfsFile::getPhysfsFilePtr()
{
return mFileHandle;
}

}
