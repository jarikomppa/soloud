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
mFileHandle=0;
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
return PHYSFS_eof(mFileHandle); //weather we received at the end of the file
}

unsigned int PhysfsFile::read(unsigned char *aDst, unsigned int aBytes)
{
return (unsigned int) PHYSFS_read(mFileHandle, aDst, aBytes, 1);
}

unsigned int PhysfsFile::length()
{
int pos=PHYSFS_tell(mFileHandle); //determine our position in the file
PHYSFS_seek(mFileHandle, PHYSFS_fileLength(mFileHandle)+pos); //go to the end of file
int len=PHYSFS_tell(mFileHandle); //get the length of file
PHYSFS_seek(mFileHandle, len-pos); //go back to our previous position in the file
return len;
}

void PhysfsFile::seek(int aOffset)
{
PHYSFS_seek(mFileHandle, aOffset); //move to aOffset
}

unsigned int PhysfsFile::pos()
{
return PHYSFS_tell(mFileHandle); //get the position of the file
}

PhysfsFile::~PhysfsFile()
{
if(mFileHandle)
{
PHYSFS_close(mFileHandle); //close it
}
}

result PhysfsFile::open(const char *aFilename)
{
if(PHYSFS_exists(aFilename)==0)
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
