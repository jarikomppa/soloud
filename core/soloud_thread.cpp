/*
SoLoud audio engine
Copyright (c) 2013 Jari Komppa

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

#include "soloud_thread.h"

#ifdef _MSC_VER
#define WINDOWS_VERSION
#endif

#ifdef WINDOWS_VERSION
#include <Windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

namespace SoLoud
{
	namespace Thread
	{
#ifdef WINDOWS_VERSION
		void * createMutex()
		{
			CRITICAL_SECTION * cs = new CRITICAL_SECTION;
			InitializeCriticalSectionAndSpinCount(cs, 100);
			return (void*)cs;
		}

		void destroyMutex(void *aHandle)
		{
			CRITICAL_SECTION *cs = (CRITICAL_SECTION*)aHandle;
			DeleteCriticalSection(cs);
			delete cs;
		}

		void lockMutex(void *aHandle)
		{
			CRITICAL_SECTION *cs = (CRITICAL_SECTION*)aHandle;
			if (cs)
			{
				EnterCriticalSection(cs);
			}
		}

		void unlockMutex(void *aHandle)
		{
			CRITICAL_SECTION *cs = (CRITICAL_SECTION*)aHandle;
			if (cs)
			{
				LeaveCriticalSection(cs);
			}
		}

		struct soloud_thread_data
		{
			threadFunction mFunc;
			void *mParam;
		};

		static DWORD WINAPI threadfunc(LPVOID d)
		{
			soloud_thread_data *p = (soloud_thread_data *)d;
			p->mFunc(p->mParam);
			delete p;
			return 0;
		}

		void createThread(threadFunction aThreadFunction, void *aParameter)
		{
			soloud_thread_data *d = new soloud_thread_data;
			d->mFunc = aThreadFunction;
			d->mParam = aParameter;

			CreateThread(NULL,0,threadfunc,d,0,NULL);
		}

		void sleep(int aMSec)
		{
			Sleep(aMSec);
		}

#else // pthreads
		void * createMutex()
		{
			pthread_mutex_t *mutex;
			mutex = new pthread_mutex_t;
		
			pthread_mutexattr_t attr;
			pthread_mutexattr_init(&attr);

			pthread_mutex_init(mutex, &attr);
		
			return (void*)mutex;
		}

		void destroyMutex(void *aHandle)
		{
			pthread_mutex_t *mutex = (pthread_mutex_t*)aHandle;

			if (mutex)
			{
				pthread_mutex_destroy(mutex);
				delete mutex;
			}
		}

		void lockMutex(void *aHandle)
		{
			pthread_mutex_t *mutex = (pthread_mutex_t*)aHandle;
			if (mutex)
			{
				pthread_mutex_lock(mutex);
			}
		}

		void unlockMutex(void *aHandle)
		{
			pthread_mutex_t *mutex = (pthread_mutex_t*)aHandle;
			if (mutex)
			{
				pthread_mutex_unlock(mutex);
			}
		}

		struct soloud_thread_data
		{
			threadFunction mFunc;
			void *mParam;
		};

		static void * threadfunc(void * d)
		{
			soloud_thread_data *p = (soloud_thread_data *)d;
			p->mFunc(p->mParam);
			delete p;
			return 0;
		}

		void createThread(threadFunction aThreadFunction, void *aParameter)
		{
			soloud_thread_data *d = new soloud_thread_data;
			d->mFunc = aThreadFunction;
			d->mParam = aParameter;

			pthread_t thread;
			pthread_create(&thread, NULL, threadfunc, (void*)d);
		}

		void sleep(int aMSec)
		{
			usleep(aMSec / 1000);
		}
#endif
	}
}
