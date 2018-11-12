import sys
import pygame
from soloud import *

pygame.init()
screen = pygame.display.set_mode((640,480), 0, 32)

audiolib = Soloud()
audiolib.init(3)
audiolib.set_global_volume(0.5)
speech = Speech()
lofi = LofiFilter()
speech.set_filter(0, lofi)

t = "Wheeeeeeeeeeeeeeeeeee Python and Pygame and visualizations and stuff"
speech.set_text(t)
for x in range(0, 15):
	sph = audiolib.play(speech)
	audiolib.set_relative_play_speed(sph, float(x + 10) / (15-x))

while audiolib.get_active_voice_count() > 0:
    
	for event in pygame.event.get():
		if event.type == pygame.QUIT:
			speech.close()
			audiolib.close()
			sys.exit()
    
	# Redraw the background
	screen.fill((0,0,0,0))

	vob = audiolib.get_wave()
	h = 0
	for x in vob:
		screen.set_at((h*2, int(240 + 200 * x)), (255, 255, 255))
		h += 1
	pygame.display.flip()

pygame.quit()
