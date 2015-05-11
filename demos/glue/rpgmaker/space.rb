gSoloud  = SoLoud::Soloud.new
gSpeech  = SoLoud::Speech.new 
gLofi    = SoLoud::LofiFilter.new
gFlanger = SoLoud::FlangerFilter.new
gReso    = SoLoud::BiquadResonantFilter.new
gSpeechBus = SoLoud::Bus.new
gMusicBus  = SoLoud::Bus.new

gSoloud.init();
gSoloud.setVisualizationEnable(1);
gSoloud.setGlobalVolume(3);
gSoloud.setPostClipScaler(0.75);

gSoloud.play(gSpeechBus);
gSoloud.play(gMusicBus);

gSpeech.setFilter(1, gFlanger);
gSpeech.setFilter(0, gLofi);
gSpeech.setFilter(2, gReso);
gLofi.setParams(8000,4);
gFlanger.setParams(0.002,100);
gReso.setParams(SoLoud::BiquadResonantFilter::BANDPASS, 8000, 1000, 0.5);

gSpeech.setText(<<-EOS)
What the alien has to say might
appear around here if this
wasn't just a dummy mockup..

..........
This is a demo of getting
visualization data from different
parts of the audio pipeline.

..........

..........

..........
EOS

gSpeech.setLooping(1);	

speechhandle = gSpeechBus.play(gSpeech, 3, -0.25);
gSoloud.setRelativePlaySpeed(speechhandle, 1.2);

gSoloud.oscillateFilterParameter(speechhandle, 0, SoLoud::LofiFilter::SAMPLERATE, 2000, 8000, 4);

gSpeechBus.setVisualizationEnable(1);
gMusicBus.setVisualizationEnable(1);

rgss_stop