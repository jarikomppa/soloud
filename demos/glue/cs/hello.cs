using System;

namespace hello
{
    class Program
    {
        static void Main(string[] args)
        {

            SoLoud.Soloud soloud = new SoLoud.Soloud();  // SoLoud engine core
            SoLoud.Speech speech = new SoLoud.Speech();  // A sound source (speech, in this case)

	        // Configure sound source
	        speech.setText("Hello c sharp world!");

	        // initialize SoLoud.
	        soloud.init();

	        // Play the sound source (we could do this several times if we wanted)
	        soloud.play(speech);

	        // Wait until sounds have finished
	        while (soloud.getActiveVoiceCount() > 0)
	        {
		        // Still going, sleep for a bit
                Console.Write(".");    
	        }

	        // Clean up SoLoud
	        soloud.deinit();
        }
    }
}
