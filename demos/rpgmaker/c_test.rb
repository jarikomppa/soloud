soloud = SoLoud::Soloud.new
speech = SoLoud::Speech.new

speech.setText("1 2 3       A B C        Doooooo    Reeeeee    Miiiiii    Faaaaaa    Soooooo    Laaaaaa    Tiiiiii    Doooooo!");
soloud.init(
  SoLoud::Soloud::CLIP_ROUNDOFF | SoLoud::Soloud::ENABLE_VISUALIZATION, 
  SoLoud::Soloud::AUTO, 
  SoLoud::Soloud::AUTO, 
  SoLoud::Soloud::AUTO
);

soloud.setGlobalVolume(4);
soloud.play(speech);

printf("Playing..\n");

sprite = Sprite.new
sprite.bitmap = Bitmap.new(544, 416)
halfWidth = 544 / 2.0
i = 0
while (soloud.getActiveVoiceCount > 0)
  v = soloud.calcFFT
  z = (v[10] * halfWidth).to_i
  z = 544 if z > 544
  
  sprite.bitmap.clear  
  sprite.bitmap.fill_rect(0, 0, z, 40, Color.new(0, 255, 0))
  
  Graphics.update
end
  
printf("\nFinished.\n");

soloud.deinit
  
speech.destroy
soloud.destroy

printf("Cleanup done.\n");
