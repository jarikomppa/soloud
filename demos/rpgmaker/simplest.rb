# Define a couple of variables
soloud = SoLoud::Soloud.new  # SoLoud engine core
speech = SoLoud::Speech.new  # A sound source (speech, in this case)

# Configure sound source
speech.setText("1 2 3   1 2 3   Hello world. Welcome to So-Loud.");

# initialize SoLoud.
soloud.init()

# Play the sound source (we could do this several times if we wanted)
soloud.play(speech);

# Wait until sounds have finished
while (soloud.getActiveVoiceCount() > 0)
  # Still going, sleep for a bit
  Graphics.wait(100)
end

# Clean up SoLoud
soloud.deinit();

# All done.
