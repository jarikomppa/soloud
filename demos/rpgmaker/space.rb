gSoloud  = Soloud.new
gSpeech  = Speech.new
gLofi    = LofiFilter.new
gFlanger = FlangerFilter.new
gReso    = BiquadResonantFilter.new
gSpeechBus = Bus.new
gMusicBus  = Bus.new

gSoloud.init();
gSoloud.set_visualization_enable(1);
gSoloud.set_global_volume(3);
gSoloud.set_post_clip_scaler(0.75);

gSoloud.play(gSpeechBus);
gSoloud.play(gMusicBus);

gSpeech.set_filter(1, gFlanger);
gSpeech.set_filter(0, gLofi);
gSpeech.set_filter(2, gReso);
gLofi.set_params(8000,4);
gFlanger.set_params(0.002,100);
gReso.set_params(BiquadResonantFilter::BANDPASS, 8000, 1000, 0.5);

gSpeech.set_text(<<-EOS)
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

gSpeech.set_looping(1);	

speechhandle = gSpeechBus.play(gSpeech, 3, -0.25);
gSoloud.set_relative_play_speed(speechhandle, 1.2);

gSoloud.oscillate_filter_parameter(speechhandle, 0, LofiFilter::SAMPLERATE, 2000, 8000, 4);

gSpeechBus.set_visualization_enable(1);
gMusicBus.set_visualization_enable(1);

rgss_stop