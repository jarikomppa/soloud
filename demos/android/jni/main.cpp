/*
SoLoud audio engine
Copyright (c) 2013-2015 Jari Komppa

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

#include <jni.h>
#include <errno.h>
#include <stdio.h>

#include <android/asset_manager.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#include "soloud.h"
#include "soloud_file.h"
#include "soloud_speech.h"
#include "soloud_thread.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "soloud-android", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "soloud-android", __VA_ARGS__))

//////////////////////////////////////////////////////////////////////////
// Example Android File implementation to use AAssetManager.
class AndroidFile : public SoLoud::File
{
public:
	AndroidFile( AAssetManager* AssetManager, const char* FileName )
	{
		Asset_ = AAssetManager_open( AssetManager, FileName, AASSET_MODE_UNKNOWN );
		Position_ = 0;
	}

	virtual ~AndroidFile()
	{
		if( Asset_ )
		{
			AAsset_close( Asset_ );
		}
	}

	int eof()
	{
		return Position_ >= length();
	}

	unsigned int read( unsigned char *aDst, unsigned int aBytes )
	{
		AAsset_seek( Asset_, Position_, SEEK_SET );
		AAsset_read( Asset_, aDst, aBytes );
		Position_ += aBytes;
		return aBytes;
	}
	
	unsigned int length()
	{
		return static_cast< unsigned int >( AAsset_getLength( Asset_ ) );
	}

	void seek( int aOffset )
	{
		Position_ = aOffset;
	}

	unsigned int pos()
	{
		return Position_;
	}
	
	virtual FILE * getFilePtr() { return 0; }
	virtual unsigned char * getMemPtr() { return 0; }

private:
	AAsset* Asset_;
	unsigned int Position_;
};

void android_main(struct android_app* state)
{
	// Make sure glue isn't stripped.
	app_dummy();

	// Define a couple of variables
	SoLoud::Soloud soloud;  // SoLoud engine core
	SoLoud::Speech speech;  // A sound source (speech, in this case)

	// Configure sound source
	speech.setText("1 2 3   1 2 3   Hello world. Welcome to So-Loud on Android!");

	// initialize SoLoud.
	soloud.init();

	// Play the sound source (we could do this several times if we wanted)
	soloud.play(speech);

	// Wait for mixing thread.
	SoLoud::Thread::sleep(1000);

	// Wait until sounds have finished
	while (soloud.getActiveVoiceCount() > 0)
	{
		// Still going, sleep for a bit
		SoLoud::Thread::sleep(100);
	}

	// Clean up SoLoud
	soloud.deinit();

	LOGI( "Exiting." );
}
//END_INCLUDE(all)
