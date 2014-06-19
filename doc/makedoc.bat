@echo off
echo ---------------------------------------------------------------------
echo ---------------------------------------------------------------------
echo Generating HTML docs

call pandoc -s --toc --default-image-extension=png -o soloud.html intro.mmd legal.mmd quickstart.mmd premake.mmd concepts.mmd faq.mmd examples.mmd codegen.mmd c_api.mmd python_api.mmd basics.mmd attributes.mmd faders.mmd voicegroups.mmd coremisc.mmd audiosource.mmd wav.mmd wavstream.mmd speech.mmd sfxr.mmd modplug.mmd newsoundsources.mmd mixbus.mmd filters.mmd biquadfilter.mmd echofilter.mmd fftfilter.mmd lofifilter.mmd backends.mmd

echo ---------------------------------------------------------------------
echo ---------------------------------------------------------------------
echo Generating HTML pages

call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png intro.mmd -o index.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png legal.mmd -o legal.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png downloads.mmd -o downloads.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png quickstart.mmd -o quickstart.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png premake.mmd -o premake.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png concepts.mmd -o concepts.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png faq.mmd -o faq.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png examples.mmd -o examples.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png codegen.mmd -o codegen.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png c_api.mmd -o c_api.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png python_api.mmd -o python_api.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png basics.mmd -o basics.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png attributes.mmd -o attributes.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png faders.mmd -o faders.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png voicegroups.mmd -o voicegroups.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png coremisc.mmd -o coremisc.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png audiosource.mmd -o audiosource.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png wav.mmd -o wav.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png wavstream.mmd -o wavstream.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png speech.mmd -o speech.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png sfxr.mmd -o sfxr.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png modplug.mmd -o modplug.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png newsoundsources.mmd -o newsoundsources.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png mixbus.mmd -o mixbus.html 
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png filters.mmd -o filters.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png biquadfilter.mmd -o biquadfilter.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png echofilter.mmd -o echofilter.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png fftfilter.mmd -o fftfilter.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png fftfilter.mmd -o lofifilter.html
call pandoc --template=html.pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png backends.mmd -o backends.html

echo Fixing pandocs code blocks..

perl -p -i.bak -e "s/code>/code>\n/g" index.html
perl -p -i.bak -e "s/code>/code>\n/g" legal.html
perl -p -i.bak -e "s/code>/code>\n/g" downloads.html
perl -p -i.bak -e "s/code>/code>\n/g" quickstart.html
perl -p -i.bak -e "s/code>/code>\n/g" premake.html
perl -p -i.bak -e "s/code>/code>\n/g" concepts.html
perl -p -i.bak -e "s/code>/code>\n/g" faq.html
perl -p -i.bak -e "s/code>/code>\n/g" examples.html
perl -p -i.bak -e "s/code>/code>\n/g" codegen.html
perl -p -i.bak -e "s/code>/code>\n/g" c_api.html
perl -p -i.bak -e "s/code>/code>\n/g" python_api.html
perl -p -i.bak -e "s/code>/code>\n/g" basics.html
perl -p -i.bak -e "s/code>/code>\n/g" attributes.html
perl -p -i.bak -e "s/code>/code>\n/g" faders.html
perl -p -i.bak -e "s/code>/code>\n/g" voicegroups.html
perl -p -i.bak -e "s/code>/code>\n/g" coremisc.html
perl -p -i.bak -e "s/code>/code>\n/g" audiosource.html
perl -p -i.bak -e "s/code>/code>\n/g" wav.html
perl -p -i.bak -e "s/code>/code>\n/g" wavstream.html
perl -p -i.bak -e "s/code>/code>\n/g" speech.html
perl -p -i.bak -e "s/code>/code>\n/g" sfxr.html
perl -p -i.bak -e "s/code>/code>\n/g" modplug.html
perl -p -i.bak -e "s/code>/code>\n/g" newsoundsources.html
perl -p -i.bak -e "s/code>/code>\n/g" mixbus.html
perl -p -i.bak -e "s/code>/code>\n/g" filters.html
perl -p -i.bak -e "s/code>/code>\n/g" biquadfilter.html
perl -p -i.bak -e "s/code>/code>\n/g" echofilter.html
perl -p -i.bak -e "s/code>/code>\n/g" fftfilter.html
perl -p -i.bak -e "s/code>/code>\n/g" lofifilter.html
perl -p -i.bak -e "s/code>/code>\n/g" backends.html

echo ---------------------------------------------------------------------
echo ---------------------------------------------------------------------
echo Generating epub

call pandoc -N --toc  --epub-cover-image=images/cover.png -t epub3 --default-image-extension=png -S --epub-stylesheet=epub.css --epub-metadata=metadata.xml -o SoLoud.epub title.txt intro.mmd legal.mmd quickstart.mmd premake.mmd concepts.mmd faq.mmd examples.mmd codegen.mmd c_api.mmd python_api.mmd basics.mmd attributes.mmd faders.mmd voicegroups.mmd coremisc.mmd audiosource.mmd wav.mmd wavstream.mmd speech.mmd sfxr.mmd modplug.mmd newsoundsources.mmd mixbus.mmd filters.mmd biquadfilter.mmd echofilter.mmd fftfilter.mmd lofifilter.mmd backends.mmd

echo ---------------------------------------------------------------------
echo ---------------------------------------------------------------------
echo Generating mobi

kindlegen SoLoud.epub -c2

echo ---------------------------------------------------------------------
echo ---------------------------------------------------------------------
echo Generating latex

call pandoc --listings --default-image-extension=pdf --chapters attributes.mmd -o attributes.tex 
call pandoc --listings --default-image-extension=pdf --chapters audiosource.mmd -o audiosource.tex
call pandoc --listings --default-image-extension=pdf --chapters backends.mmd -o backends.tex
call pandoc --listings --default-image-extension=pdf --chapters basics.mmd -o basics.tex
call pandoc --listings --default-image-extension=pdf --chapters codegen.mmd -o codegen.tex
call pandoc --listings --default-image-extension=pdf --chapters c_api.mmd -o c_api.tex
call pandoc --listings --default-image-extension=pdf --chapters python_api.mmd -o python_api.tex
call pandoc --listings --default-image-extension=pdf --chapters biquadfilter.mmd -o biquadfilter.tex
call pandoc --listings --default-image-extension=pdf --chapters concepts.mmd -o concepts.tex
call pandoc --listings --default-image-extension=pdf --chapters voicegroups.mmd -o voicegroups.tex 
call pandoc --listings --default-image-extension=pdf --chapters coremisc.mmd -o coremisc.tex 
call pandoc --listings --default-image-extension=pdf --chapters echofilter.mmd -o echofilter.tex
call pandoc --listings --default-image-extension=pdf --chapters faders.mmd -o faders.tex
call pandoc --listings --default-image-extension=pdf --chapters faq.mmd -o faq.tex
call pandoc --listings --default-image-extension=pdf --chapters examples.mmd -o examples.tex
call pandoc --listings --default-image-extension=pdf --chapters fftfilter.mmd -o fftfilter.tex
call pandoc --listings --default-image-extension=pdf --chapters lofifilter.mmd -o lofifilter.tex
call pandoc --listings --default-image-extension=pdf --chapters filters.mmd -o filters.tex
call pandoc --listings --default-image-extension=pdf --chapters intro.mmd -o intro.tex
call pandoc --listings --default-image-extension=pdf --chapters legal.mmd -o legal.tex
call pandoc --listings --default-image-extension=pdf --chapters mixbus.mmd -o mixbus.tex
call pandoc --listings --default-image-extension=pdf --chapters newsoundsources.mmd -o newsoundsources.tex
call pandoc --listings --default-image-extension=pdf --chapters quickstart.mmd -o quickstart.tex
call pandoc --listings --default-image-extension=pdf --chapters premake.mmd -o premake.tex
call pandoc --listings --default-image-extension=pdf --chapters speech.mmd -o speech.tex
call pandoc --listings --default-image-extension=pdf --chapters sfxr.mmd -o sfxr.tex
call pandoc --listings --default-image-extension=pdf --chapters modplug.mmd -o modplug.tex
call pandoc --listings --default-image-extension=pdf --chapters wav.mmd -o wav.tex
call pandoc --listings --default-image-extension=pdf --chapters wavstream.mmd -o wavstream.tex
del *.aux *.toc *.out *.log *.lg *.4ct *.4tc *.idv *.tmp *.xdv *.xref

echo ---------------------------------------------------------------------
echo ---------------------------------------------------------------------
echo Generating pdf

xelatex SoLoud.tex
xelatex SoLoud.tex

del *.aux *.toc *.out *.log *.lg *.4ct *.4tc *.idv *.tmp *.xdv *.xref

echo ---------------------------------------------------------------------
echo ---------------------------------------------------------------------
echo All done:
echo soloud.pdf soloud.epub soloud.mobi soloud.html + bunch of .html files
