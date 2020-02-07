#!/usr/bin/env python3
from shutil import copyfile
import os
import glob
import time

sources = [
"AUTHORS",
"CONTRIBUTING",
"LICENSE",
"readme.txt",
"soloud.png",
"soloud_inv.png",
"bin/soloud_x86.dll",
"bin/soloud_x64.dll",
"bin/codegen_x86.exe",
"bin/c_test_x86.exe",
"bin/enumerate_x86.exe",
"bin/env_x86.exe",
"bin/lutgen_x86.exe",
"bin/megademo_x86.exe",
"bin/null_x86.exe",
"bin/piano_x86.exe",
"bin/sanity_x86.exe",
"bin/SDL2.dll",
"bin/simplest_x86.exe",
"bin/welcome_x86.exe",
"bin/audio/9 (102 BPM)_Seq01.wav",
"bin/audio/9 (102 BPM)_Seq02.wav",
"bin/audio/9 (102 BPM)_Seq03.wav",
"bin/audio/9 (102 BPM)_Seq04.wav",
"bin/audio/9 (102 BPM)_Seq05.wav",
"bin/audio/9 (102 BPM)_Seq06.wav",
"bin/audio/9 (102 BPM)_Seq07.wav",
"bin/audio/9 (102 BPM)_Seq08.wav",
"bin/audio/9 (102 BPM)_Seq09.wav",
"bin/audio/9 (102 BPM)_Seq10.wav",
"bin/audio/AKWF_c604_0024.wav",
"bin/audio/algebra_loop.ogg",
"bin/audio/BRUCE.S3M",
"bin/audio/Jakim - Aboriginal Derivatives.mon",
"bin/audio/Modulation.sid.dump",
"bin/audio/plonk_dry.ogg",
"bin/audio/plonk_wet.ogg",
"bin/audio/rainy_ambience.ogg",
"bin/audio/ted_storm.prg.dump",
"bin/audio/tetsno.ogg",
"bin/audio/delphi_loop.ogg",
"bin/audio/war_loop.ogg",
"bin/audio/windy_ambience.ogg",
"bin/audio/wavformats/ch1.flac",
"bin/audio/wavformats/ch1.mp3",
"bin/audio/wavformats/ch1.ogg",
"bin/audio/wavformats/ch1_16bit.wav",
"bin/audio/wavformats/ch1_24bit.wav",
"bin/audio/wavformats/ch1_32bit.wav",
"bin/audio/wavformats/ch1_8bit.wav",
"bin/audio/wavformats/ch1_alaw.wav",
"bin/audio/wavformats/ch1_double.wav",
"bin/audio/wavformats/ch1_float.wav",
"bin/audio/wavformats/ch1_imaadpcm.wav",
"bin/audio/wavformats/ch1_msadpcm.wav",
"bin/audio/wavformats/ch1_ulaw.wav",
"bin/audio/wavformats/ch2.flac",
"bin/audio/wavformats/ch2.mp3",
"bin/audio/wavformats/ch2.ogg",
"bin/audio/wavformats/ch2_16bit.wav",
"bin/audio/wavformats/ch2_24bit.wav",
"bin/audio/wavformats/ch2_32bit.wav",
"bin/audio/wavformats/ch2_8bit.wav",
"bin/audio/wavformats/ch2_alaw.wav",
"bin/audio/wavformats/ch2_double.wav",
"bin/audio/wavformats/ch2_float.wav",
"bin/audio/wavformats/ch2_imaadpcm.wav",
"bin/audio/wavformats/ch2_msadpcm.wav",
"bin/audio/wavformats/ch2_ulaw.wav",
"bin/audio/wavformats/ch4.flac",
"bin/audio/wavformats/ch4.ogg",
"bin/audio/wavformats/ch4_16bit.wav",
"bin/audio/wavformats/ch4_24bit.wav",
"bin/audio/wavformats/ch4_32bit.wav",
"bin/audio/wavformats/ch4_8bit.wav",
"bin/audio/wavformats/ch4_alaw.wav",
"bin/audio/wavformats/ch4_double.wav",
"bin/audio/wavformats/ch4_float.wav",
"bin/audio/wavformats/ch4_imaadpcm.wav",
"bin/audio/wavformats/ch4_msadpcm.wav",
"bin/audio/wavformats/ch4_ulaw.wav",
"bin/audio/wavformats/ch6.flac",
"bin/audio/wavformats/ch6.ogg",
"bin/audio/wavformats/ch6_1.flac",
"bin/audio/wavformats/ch6_16bit.wav",
"bin/audio/wavformats/ch6_2.flac",
"bin/audio/wavformats/ch6_24bit.wav",
"bin/audio/wavformats/ch6_3.flac",
"bin/audio/wavformats/ch6_32bit.wav",
"bin/audio/wavformats/ch6_4.flac",
"bin/audio/wavformats/ch6_5.flac",
"bin/audio/wavformats/ch6_6.flac",
"bin/audio/wavformats/ch6_8bit.wav",
"bin/audio/wavformats/ch6_alaw.wav",
"bin/audio/wavformats/ch6_double.wav",
"bin/audio/wavformats/ch6_float.wav",
"bin/audio/wavformats/ch6_imaadpcm.wav",
"bin/audio/wavformats/ch6_msadpcm.wav",
"bin/audio/wavformats/ch6_ulaw.wav",
"bin/audio/wavformats/ch8.flac",
"bin/audio/wavformats/ch8.ogg",
"bin/audio/wavformats/ch8_1.flac",
"bin/audio/wavformats/ch8_16bit.wav",
"bin/audio/wavformats/ch8_2.flac",
"bin/audio/wavformats/ch8_24bit.wav",
"bin/audio/wavformats/ch8_3.flac",
"bin/audio/wavformats/ch8_32bit.wav",
"bin/audio/wavformats/ch8_4.flac",
"bin/audio/wavformats/ch8_5.flac",
"bin/audio/wavformats/ch8_6.flac",
"bin/audio/wavformats/ch8_7.flac",
"bin/audio/wavformats/ch8_8.flac",
"bin/audio/wavformats/ch8_8bit.wav",
"bin/audio/wavformats/ch8_alaw.wav",
"bin/audio/wavformats/ch8_double.wav",
"bin/audio/wavformats/ch8_float.wav",
"bin/audio/wavformats/ch8_imaadpcm.wav",
"bin/audio/wavformats/ch8_msadpcm.wav",
"bin/audio/wavformats/ch8_ulaw.wav",
"bin/audio/thebutton/ack.ogg",
"bin/audio/thebutton/button1.mp3",
"bin/audio/thebutton/button2.mp3",
"bin/audio/thebutton/button3.mp3",
"bin/audio/thebutton/button4.mp3",
"bin/audio/thebutton/button5.mp3",
"bin/audio/thebutton/button6.mp3",
"bin/audio/thebutton/button7.mp3",
"bin/audio/thebutton/cough.mp3",
"bin/audio/thebutton/sigh.mp3",
"bin/audio/thebutton/thankyou.mp3",
"bin/graphics/alien.png",
"bin/graphics/env_bg.png",
"bin/graphics/env_walker.png",
"bin/graphics/soloud_bg.png",
"build/genie.lua",
"build/readme.txt",
"contrib/CMakeLists.txt",
"contrib/Configure.cmake",
"contrib/demos.cmake",
"contrib/README.md",
"contrib/src.cmake",
"contrib/cmake/FindSDL2.cmake",
"contrib/cmake/Install.cmake",
"contrib/cmake/InstallExport.cmake",
"contrib/cmake/OptionDependentOnPackage.cmake",
"contrib/cmake/PrintOptionStatus.cmake",
"demos/android/default.properties",
"demos/android/jni/Android.mk",
"demos/android/jni/Application.mk",
"demos/android/jni/main.cpp",
"demos/common/soloud_demo_framework.cpp",
"demos/common/soloud_demo_framework.h",
"demos/common/stb_image.h",
"demos/common/glew/GL/glew.c",
"demos/common/glew/GL/glew.h",
"demos/common/glew/GL/glxew.h",
"demos/common/glew/GL/LICENSE.txt",
"demos/common/glew/GL/README.txt",
"demos/common/glew/GL/wglew.h",
"demos/common/imgui/imconfig.h",
"demos/common/imgui/imgui.cpp",
"demos/common/imgui/imgui.h",
"demos/common/imgui/imgui_demo.cpp",
"demos/common/imgui/imgui_draw.cpp",
"demos/common/imgui/imgui_internal.h",
"demos/common/imgui/LICENSE.txt",
"demos/common/imgui/stb_rect_pack.h",
"demos/common/imgui/stb_textedit.h",
"demos/common/imgui/stb_truetype.h",
"demos/c_test/main.c",
"demos/emscripten/build_monotone",
"demos/emscripten/build_piano",
"demos/enumerate/main.cpp",
"demos/env/main.cpp",
"demos/glue/cs/hello.cs",
"demos/glue/gamemaker/hello.txt",
"demos/glue/python/test_gfx.py",
"demos/glue/python/test_python.py",
"demos/glue/rpgmaker/c_test.rb",
"demos/glue/rpgmaker/simplest.rb",
"demos/glue/rpgmaker/space.rb",
"demos/glue/ruby/test_ruby.rb",
"demos/megademo/3dtest.cpp",
"demos/megademo/main.cpp",
"demos/megademo/mixbusses.cpp",
"demos/megademo/monotone.cpp",
"demos/megademo/multimusic.cpp",
"demos/megademo/pewpew.cpp",
"demos/megademo/radiogaga.cpp",
"demos/megademo/space.cpp",
"demos/megademo/speakers.cpp",
"demos/megademo/speechfilter.cpp",
"demos/megademo/tedsid.cpp",
"demos/megademo/thebutton.cpp",
"demos/megademo/virtualvoices.cpp",
"demos/megademo/wavformats.cpp",
"demos/null/main.cpp",
"demos/piano/main.cpp",
"demos/piano/soloud_basicwave.cpp",
"demos/piano/soloud_basicwave.h",
"demos/piano/soloud_padsynth.cpp",
"demos/piano/soloud_padsynth.h",
"demos/simplest/main.cpp",
"demos/welcome/main.cpp",
"doc/soloud.epub",
"doc/soloud.html",
"doc/soloud.mobi",
"doc/soloud.pdf",
"glue/readme.txt",
"glue/rpgmaker_soloud.rb",
"glue/soloud.bmx",
"glue/soloud.cs",
"glue/soloud.d",
"glue/soloud.gmez",
"glue/soloud.py",
"glue/soloud.rb",
"include/soloud.h",
"include/soloud_audiosource.h",
"include/soloud_bassboostfilter.h",
"include/soloud_biquadresonantfilter.h",
"include/soloud_bus.h",
"include/soloud_c.h",
"include/soloud_dcremovalfilter.h",
"include/soloud_echofilter.h",
"include/soloud_error.h",
"include/soloud_fader.h",
"include/soloud_fft.h",
"include/soloud_fftfilter.h",
"include/soloud_file.h",
"include/soloud_file_hack_off.h",
"include/soloud_file_hack_on.h",
"include/soloud_filter.h",
"include/soloud_flangerfilter.h",
"include/soloud_internal.h",
"include/soloud_lofifilter.h",
"include/soloud_monotone.h",
"include/soloud_openmpt.h",
"include/soloud_queue.h",
"include/soloud_robotizefilter.h",
"include/soloud_sfxr.h",
"include/soloud_speech.h",
"include/soloud_tedsid.h",
"include/soloud_thread.h",
"include/soloud_vic.h",
"include/soloud_vizsn.h",
"include/soloud_wav.h",
"include/soloud_waveshaperfilter.h",
"include/soloud_wavstream.h",
"scripts/checkapidoc.py",
"scripts/checksanity.py",
"scripts/compile_gamemaker_dll.bat",
"scripts/gen_autoit.py",
"scripts/gen_blitzmax.py",
"scripts/gen_cs.py",
"scripts/gen_d.py",
"scripts/gen_gamemaker.py",
"scripts/gen_python.py",
"scripts/gen_rpgmaker.py",
"scripts/gen_ruby.py",
"scripts/makerel.py",
"scripts/makeglue.py",
"scripts/make_gmez.bat",
"scripts/readme.txt",
"scripts/soloud_codegen.py",
"src/audiosource/monotone/soloud_monotone.cpp",
"src/audiosource/openmpt/soloud_openmpt.cpp",
"src/audiosource/openmpt/soloud_openmpt_dll.c",
"src/audiosource/sfxr/soloud_sfxr.cpp",
"src/audiosource/speech/darray.cpp",
"src/audiosource/speech/darray.h",
"src/audiosource/speech/Elements.def",
"src/audiosource/speech/klatt.cpp",
"src/audiosource/speech/klatt.h",
"src/audiosource/speech/legal_readme.txt",
"src/audiosource/speech/resonator.cpp",
"src/audiosource/speech/resonator.h",
"src/audiosource/speech/soloud_speech.cpp",
"src/audiosource/speech/tts.cpp",
"src/audiosource/speech/tts.h",
"src/audiosource/tedsid/sid.cpp",
"src/audiosource/tedsid/sid.h",
"src/audiosource/tedsid/soloud_tedsid.cpp",
"src/audiosource/tedsid/ted.cpp",
"src/audiosource/tedsid/ted.h",
"src/audiosource/vic/soloud_vic.cpp",
"src/audiosource/vizsn/soloud_vizsn.cpp",
"src/audiosource/wav/dr_flac.h",
"src/audiosource/wav/dr_impl.cpp",
"src/audiosource/wav/dr_mp3.h",
"src/audiosource/wav/dr_wav.h",
"src/audiosource/wav/soloud_wav.cpp",
"src/audiosource/wav/soloud_wavstream.cpp",
"src/audiosource/wav/stb_vorbis.c",
"src/audiosource/wav/stb_vorbis.h",
"src/backend/alsa/soloud_alsa.cpp",
"src/backend/coreaudio/soloud_coreaudio.cpp",
"src/backend/null/soloud_null.cpp",
"src/backend/openal/soloud_openal.cpp",
"src/backend/openal/soloud_openal_dll.c",
"src/backend/opensles/soloud_opensles.cpp",
"src/backend/oss/soloud_oss.cpp",
"src/backend/portaudio/soloud_portaudio.cpp",
"src/backend/portaudio/soloud_portaudio_dll.c",
"src/backend/sdl/soloud_sdl1.cpp",
"src/backend/sdl/soloud_sdl1_dll.c",
"src/backend/sdl/soloud_sdl2.cpp",
"src/backend/sdl/soloud_sdl2_dll.c",
"src/backend/sdl2_static/soloud_sdl2_static.cpp",
"src/backend/sdl_static/soloud_sdl_static.cpp",
"src/backend/vita_homebrew/soloud_vita_homebrew.cpp",
"src/backend/wasapi/soloud_wasapi.cpp",
"src/backend/winmm/soloud_winmm.cpp",
"src/backend/xaudio2/soloud_xaudio2.cpp",
"src/core/soloud.cpp",
"src/core/soloud_audiosource.cpp",
"src/core/soloud_bus.cpp",
"src/core/soloud_core_3d.cpp",
"src/core/soloud_core_basicops.cpp",
"src/core/soloud_core_faderops.cpp",
"src/core/soloud_core_filterops.cpp",
"src/core/soloud_core_getters.cpp",
"src/core/soloud_core_setters.cpp",
"src/core/soloud_core_voicegroup.cpp",
"src/core/soloud_core_voiceops.cpp",
"src/core/soloud_fader.cpp",
"src/core/soloud_fft.cpp",
"src/core/soloud_fft_lut.cpp",
"src/core/soloud_file.cpp",
"src/core/soloud_filter.cpp",
"src/core/soloud_queue.cpp",
"src/core/soloud_thread.cpp",
"src/c_api/soloud.def",
"src/c_api/soloud_c.cpp",
"src/filter/soloud_bassboostfilter.cpp",
"src/filter/soloud_biquadresonantfilter.cpp",
"src/filter/soloud_dcremovalfilter.cpp",
"src/filter/soloud_echofilter.cpp",
"src/filter/soloud_fftfilter.cpp",
"src/filter/soloud_flangerfilter.cpp",
"src/filter/soloud_lofifilter.cpp",
"src/filter/soloud_robotizefilter.cpp",
"src/filter/soloud_waveshaperfilter.cpp",
"src/tools/codegen/main.cpp",
"src/tools/lutgen/main.cpp",
"src/tools/resamplerlab/main.cpp",
"src/tools/resamplerlab/stb_image_write.c",
"src/tools/resamplerlab/stb_image_write.h",
"src/tools/sanity/sanity.cpp",
"src/tools/tedsid2dump/cbmtune.cpp",
"src/tools/tedsid2dump/cbmtune.h",
"src/tools/tedsid2dump/cpu.cpp",
"src/tools/tedsid2dump/cpu.h",
"src/tools/tedsid2dump/cpu7501asm.h",
"src/tools/tedsid2dump/main.cpp",
"src/tools/tedsid2dump/mem.h",
"src/tools/tedsid2dump/psid.h",
"src/tools/tedsid2dump/readme.txt",
"src/tools/tedsid2dump/roms.h",
"src/tools/tedsid2dump/sid.cpp",
"src/tools/tedsid2dump/sid.h",
"src/tools/tedsid2dump/tedmem.cpp",
"src/tools/tedsid2dump/tedmem.h",
"src/tools/tedsid2dump/tedplay.cpp",
"src/tools/tedsid2dump/tedplay.h",
"src/tools/tedsid2dump/tedsound.cpp",
"src/tools/tedsid2dump/types.h",
"demos/megademo/annex.cpp",
"demos/megademo/filterfolio.cpp",
"demos/piano/RtMidi.cpp",
"demos/piano/RtMidi.h",
"src/audiosource/noise/soloud_noise.cpp",
"src/audiosource/tedsid/readme.txt",
"src/backend/jack/soloud_jack.cpp",
"src/backend/miniaudio/miniaudio.h",
"src/backend/miniaudio/soloud_miniaudio.cpp",
"src/backend/nosound/soloud_nosound.cpp",
"src/core/soloud_misc.cpp",
"src/filter/soloud_freeverbfilter.cpp",
"include/soloud_freeverbfilter.h",
"include/soloud_misc.h",
"include/soloud_noise.h"
]

notfound = []
root = "../"

def missingfiles(globpath):
    global notfound
    for x in glob.glob(globpath):    
        x=x.replace('\\','/')
        if os.path.isfile(x):
            if x[len(root):] not in sources:        
                #print(x, "not in sources")
                if stringindemos(x[x.rfind("/")+1:]):
                    notfound.append(x)
        else:
            missingfiles(x+"/*")

def missingsources(globpath):
    global notfound
    for x in glob.glob(globpath):    
        if os.path.isfile(x):
            if x[-4:] not in [".bak", ".pyc", ".dll"] :
                x=x.replace('\\','/')
                if x[len(root):] not in sources:        
                    notfound.append(x)
        else:
            missingsources(x+"/*")
    

def checkfile(findstring, fname):
    """ Checks whether the string can be found in a file """
    if findstring in open(fname).read():
        #print(findstring, "found in", fname)
        return True
    return False

def stringindemos(findstring):
    for x in sources:       
        if "demos/" in x:
            if checkfile(findstring, root+x):
                return True
    return False    

def checkuse(findstring):
    global notfound
    if stringindemos(findstring):
        return
    notfound.append(findstring)


def agecheck(fname):
    age = (time.time()-os.path.getmtime(fname)) / (60*60*24)
    #print(fname, "is", age, "days old.")
    if age > 1:
        notfound.append(fname)

#################################################################

# Check that all of the files listed actually exist

for x in sources:
    if not os.path.isfile(root+x):
        print(root+x, "not found, is the root",root,"correct?")
        exit()
    
print("All listed files exist.")

#
# Verify that there are no new assets that are referenced in the demos
# but not listed for release
#

missingsources(root+"demos/*")
missingsources(root+"include/*")
missingsources(root+"src/audiosource/*")
missingsources(root+"src/backend/*")
missingsources(root+"src/core/*")
missingsources(root+"src/filter/*")

if len(notfound) > 0:
    print("Source files/directories found, but not in the list; edit makerel.py or remove them:")
    for x in notfound:
        print(x)
    exit()

missingfiles(root+"bin/audio/*")
missingfiles(root+"bin/graphics/*")

if len(notfound) > 0:
    print("Data files found in directory and source files, but not in the list:")
    for x in notfound:
        print(x)
    exit()

print("No unlisted, used data files found.")

#
# Age check all exe, dll and glue files
#

for x in sources:
    if (".exe" in x) or (".dll" in x and "soloud" in x) or ("glue/" in x and "demos/" not in x and ".txt" not in x) or ("doc/" in x):
        agecheck(root+x)

if len(notfound) > 0:
    print("Following files are too old (older than 1 day):")
    for x in notfound:
        print(x)
    exit()

print("All prebuilt binaries are fresh enough.")

#    
# Check that all of the listed assets are referenced in demos
#    

for x in sources:
    if "bin/audio" in x or "bin/graphics" in x:
        checkuse(x[x.rfind("/")+1:])
 
if len(notfound) > 0:
    print("Data files not found in any of the demo sources:")
    for x in notfound:
        print(x)
    exit()

print("All listed assets can be found in at least one demo source.")

#
# Target directory
#

datestring = time.strftime("%Y%m%d")
targetdir = root + "rel/soloud" + datestring + "/"

if os.path.exists(targetdir):
    print("Target directory already exists.")
    exit()

#
# Copy files
#

print("Copying minimal to", targetdir)

for x in sources:
    if ("src/" in x
        or "include/" in x 
        or "build/" in x
        or (".dll" in x and "soloud" in x)
        or "/" not in x 
        or ("glue/" in x and "demos" not in x)
        or "demos/simplest" in x
        or "contrib/" in x
        or ".html" in x):
        dst = targetdir+x
        dstdir = dst[:dst.rfind("/")]
        if not os.path.exists(dstdir):
            os.makedirs(dstdir)
        copyfile(root+x, dst)

#
# Zip'em up
#

print("making","soloud"+datestring+"_lite.zip")

os.system("7z A -r -bd -tzip "+root+"rel/"+"soloud_"+datestring+"_lite.zip "+targetdir+" > 7z.log")

#
# Copy files
#

print("Copying all to", targetdir)

for x in sources:    
    dst = targetdir+x
    dstdir = dst[:dst.rfind("/")]
    if not os.path.exists(dstdir):
        os.makedirs(dstdir)
    copyfile(root+x, dst)

#
# Zip'em up
#

print("making","soloud"+datestring+".zip")

os.system("7z A -r -bd -tzip "+root+"rel/"+"soloud_"+datestring+".zip "+targetdir+" > 7z.log")

print("All done.")
