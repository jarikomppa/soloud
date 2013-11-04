@echo off
call pandoc -s --toc --default-image-extension=png -o soloud.html intro.mmd legal.mmd quickstart.mmd concepts.mmd faq.mmd basics.mmd attributes.mmd faders.mmd coremisc.mmd audiosource.mmd wav.mmd wavstream.mmd speech.mmd newsoundsources.mmd mixbus.mmd filters.mmd biquadfilter.mmd echofilter.mmd fftfilter.mmd backends.mmd

call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png intro.mmd -o index.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png legal.mmd -o legal.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png downloads.mmd -o downloads.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png quickstart.mmd -o quickstart.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png concepts.mmd -o concepts.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png faq.mmd -o faq.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png basics.mmd -o basics.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png attributes.mmd -o attributes.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png faders.mmd -o faders.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png coremisc.mmd -o coremisc.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png audiosource.mmd -o audiosource.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png wav.mmd -o wav.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png wavstream.mmd -o wavstream.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png speech.mmd -o speech.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png newsoundsources.mmd -o newsoundsources.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png mixbus.mmd -o mixbus.html 
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png filters.mmd -o filters.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png biquadfilter.mmd -o biquadfilter.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png echofilter.mmd -o echofilter.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png fftfilter.mmd -o fftfilter.html
call pandoc -B htmlpre.txt -A htmlpost.txt --default-image-extension=png backends.mmd -o backends.html

call pandoc -N --toc  --epub-cover-image=images/cover.png -t epub3 --default-image-extension=png -S --epub-stylesheet=epub.css --epub-metadata=metadata.xml -o SoLoud.epub title.txt intro.mmd legal.mmd quickstart.mmd concepts.mmd faq.mmd basics.mmd attributes.mmd faders.mmd coremisc.mmd audiosource.mmd wav.mmd wavstream.mmd speech.mmd newsoundsources.mmd mixbus.mmd filters.mmd biquadfilter.mmd echofilter.mmd fftfilter.mmd backends.mmd
call pandoc --listings --default-image-extension=pdf --chapters attributes.mmd -o attributes.tex 
call pandoc --listings --default-image-extension=pdf --chapters audiosource.mmd -o audiosource.tex
call pandoc --listings --default-image-extension=pdf --chapters backends.mmd -o backends.tex
call pandoc --listings --default-image-extension=pdf --chapters basics.mmd -o basics.tex
call pandoc --listings --default-image-extension=pdf --chapters biquadfilter.mmd -o biquadfilter.tex
call pandoc --listings --default-image-extension=pdf --chapters concepts.mmd -o concepts.tex
call pandoc --listings --default-image-extension=pdf --chapters coremisc.mmd -o coremisc.tex 
call pandoc --listings --default-image-extension=pdf --chapters echofilter.mmd -o echofilter.tex
call pandoc --listings --default-image-extension=pdf --chapters faders.mmd -o faders.tex
call pandoc --listings --default-image-extension=pdf --chapters faq.mmd -o faq.tex
call pandoc --listings --default-image-extension=pdf --chapters fftfilter.mmd -o fftfilter.tex
call pandoc --listings --default-image-extension=pdf --chapters filters.mmd -o filters.tex
call pandoc --listings --default-image-extension=pdf --chapters intro.mmd -o intro.tex
call pandoc --listings --default-image-extension=pdf --chapters legal.mmd -o legal.tex
call pandoc --listings --default-image-extension=pdf --chapters mixbus.mmd -o mixbus.tex
call pandoc --listings --default-image-extension=pdf --chapters newsoundsources.mmd -o newsoundsources.tex
call pandoc --listings --default-image-extension=pdf --chapters quickstart.mmd -o quickstart.tex
call pandoc --listings --default-image-extension=pdf --chapters speech.mmd -o speech.tex
call pandoc --listings --default-image-extension=pdf --chapters wav.mmd -o wav.tex
call pandoc --listings --default-image-extension=pdf --chapters wavstream.mmd -o wavstream.tex
del *.aux *.toc *.out *.log *.lg *.4ct *.4tc *.idv *.tmp *.xdv *.xref
xelatex SoLoud.tex
xelatex SoLoud.tex
echo ---------------
echo soloud.pdf soloud.epub soloud.html + bunch of .html files
