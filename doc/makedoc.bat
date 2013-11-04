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
del *.aux *.toc *.out *.log
xelatex SoLoud.tex
xelatex SoLoud.tex
start soloud.pdf
