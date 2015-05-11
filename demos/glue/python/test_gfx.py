import sys
import pygame
from soloud import *

pygame.init()
screen = pygame.display.set_mode((640,480), 0, 32)

audiolib = Soloud()
audiolib.initEx(3)
audiolib.setGlobalVolume(10)
speech = Speech()
lofi = LofiFilter()
speech.setFilter(0, lofi)

t = "Wheeeeeeeeeeeeeeeeeee Python and Pygame and visualizations and stuff"
speech.setText(t)
for x in range(0, 15):
	sph = audiolib.play(speech)
	audiolib.setRelativePlaySpeed(sph, float(x + 10) / (15-x))

while audiolib.getActiveVoiceCount() > 0:
    
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			speech.close()
			audiolib.close()
			sys.exit()
    
	# Redraw the background
	screen.fill((0,0,0,0))

	vob = audiolib.getWave()
	h = 0
	for x in vob:
		screen.set_at((h*2, int(240 + 200 * x)), (255, 255, 255))
		h += 1
	pygame.display.flip()

pygame.quit()
