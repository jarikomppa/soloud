SoLoud
======

Free, straightforward audio engine for games.

![ScreenShot](https://raw.github.com/jarikomppa/soloud/master/soloud.png)

Zlib/LibPng licensed. Portable. Small. Simple.

Preface
-------

When looking to add audio to your game, you have basically the choice to toss a lot of money at
one of the fully-fledged engines (FMOD, BASS, WWISE, IrrKlang), try one of the free alternatives
(SDL_Mixer, OpenAL), or roll your own.

The choice depends a lot on what kind of project you're running. If it's a multi-platform 
console title, outsourcing the audio engine and paying a lot of money for it makes sense. 

Whichever way you go, you'll eventually hit some issues, where the audio engine doesn't do what
you need, is buggy, or some such. As a paying customer, you'll probably get good support.

The games I'm working on don't have such budgets; I've used FMOD for free projects in the past,
and while it's nice, it's way, way too expensive for a hobbyist.

I don't like OpenAL. 

SDL_Mixer has odd restrictions. While I could develop SDL_Mixer further, and fight over every 
single submit, I figured I'd just roll my own. Anyway, this way the audio engine can be used
without SDL, too.

Audio isn't supposed to be that hard. Right?

Philosophy
----------

Most of game audio is of the "fire and forget" type; when an event occurs, you fire the audio
and forget about it. In the rare cases where you need to tweak the audio once it's live, you 
can take the audio instance's handle and use it to change the panning, volume, sample rate,
or just stop the sound.

Typical use cases should be very simple, while complex ones shouldn't be impossible.

Technicalities
--------------

All internal sample data is in floats to ease processing. When did you last see a sub-gigahz 
computing device?

Resampling is currently only done in a point-sample basis. There's no assembler optimizations
anywhere; contributions are welcome.

Sounds play in free channels, or if none are available, the oldest sound is killed. You can 
also protect sounds so that your background music doesn't get bumped out.

Don't like the behavior? Not to worry, the source code is pretty short.

Samples can play once or be set to loop.

Structure
---------

SoLoud audio engine consists of three kinds of objects:

1. Audio Factories, which contain unique data, such as loaded wav files.
2. Audio Producers, which refer to the factories that created them, but include their own read pointers, volume, sample rate, etc.
3. SoLoud engine itself, which handles the channel management and mixing.

SoLoud is rather modular. In addition to the core, you only need to include the modules you want - if you want speech but no wave files, you don't need to include the wave module; if you have no need for speech, don't include the speech module. And so on.

Current modules include:
- Simple wav loader which also can load ogg files (decompressing them into memory; streaming support coming later). 
- Speech synth
- A simple sine wave audio example.
- A simple audio-breaking filter example.
- SDL back-end

Usage example (actual docs TBD)
-------------------------------
```C++
// Headers; engine, wav loader
#include "soloud.h"
#include "soloud_wav.h"

...

// Couple globals. Don't need to be global.
SoLoud::Soloud gSoLoud;
SoLoud::Wav gSample;

...

// Initialize engine. Channels, sample rate, buffer (in samples), flags.
gSoLoud.init(8, 44100, 8192, 0);

// Load a wave file.
gSample.load("pew_pew.wav");

// Perform SDL initialization. For other back-ends, different call would be made.
SoLoud::sdl_init(&gSoLoud);

...

// Play sound.
gSoLoud.play(gSample);

...

// Cleanup.
SoLoud::sdl_deinit(&gSL);

```
