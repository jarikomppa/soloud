#!/usr/bin/env python3
from soloud import *

with Soloud() as audiolib:
	audiolib.init()
	audiolib.set_global_volume(10)
	
	speech = Speech()
	
	flanger = FlangerFilter()
	speech.set_filter(0, flanger)		
	
	t = "Hello Python (OOP) World!"
	speech.set_text(t)
	
	print(t)
	audiolib.play(speech)

	print("Enter text to speak (empty string quits)")
	while t != "":
		t = input(": ")

		speech.set_text(t);
		audiolib.play(speech);

	speech.close()

print("Bye")
