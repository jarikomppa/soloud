// This is a generated file (via gen_consoledata.py). Do not edit.

void call_Soloud_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_destroy((Soloud *)gVar[gCmd[3]].p);
}

void call_Soloud_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  Soloud * res = (Soloud *)Soloud_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_Soloud_init()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_init((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_initEx()
{
  if (gCmd.size() != 9)  {
    printf("Invalid number of parameters (6 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_initEx((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (unsigned int)getVarOrInt(gCmd[5]), (unsigned int)getVarOrInt(gCmd[6]), (unsigned int)getVarOrInt(gCmd[7]), (unsigned int)getVarOrInt(gCmd[8]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_deinit()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_deinit((Soloud *)gVar[gCmd[3]].p);
}

void call_Soloud_getVersion()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_getVersion((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_getBackendId()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_getBackendId((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_getBackendChannels()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_getBackendChannels((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_getBackendSamplerate()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_getBackendSamplerate((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_getBackendBufferSize()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_getBackendBufferSize((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_setSpeakerPosition()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_setSpeakerPosition((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_play()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_play((Soloud *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_playEx()
{
  if (gCmd.size() != 9)  {
    printf("Invalid number of parameters (6 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_playEx((Soloud *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p, (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (int)getVarOrInt(gCmd[7]), (unsigned int)getVarOrInt(gCmd[8]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_playClocked()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_playClocked((Soloud *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]), (AudioSource *)gVar[gCmd[5]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_playClockedEx()
{
  if (gCmd.size() != 9)  {
    printf("Invalid number of parameters (6 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_playClockedEx((Soloud *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]), (AudioSource *)gVar[gCmd[5]].p, (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]), (unsigned int)getVarOrInt(gCmd[8]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_play3d()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_play3d((Soloud *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p, (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_play3dEx()
{
  if (gCmd.size() != 14)  {
    printf("Invalid number of parameters (11 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_play3dEx((Soloud *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p, (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]), (float)getVarOrFloat(gCmd[8]), (float)getVarOrFloat(gCmd[9]), (float)getVarOrFloat(gCmd[10]), (float)getVarOrFloat(gCmd[11]), (int)getVarOrInt(gCmd[12]), (unsigned int)getVarOrInt(gCmd[13]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_play3dClocked()
{
  if (gCmd.size() != 9)  {
    printf("Invalid number of parameters (6 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_play3dClocked((Soloud *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]), (AudioSource *)gVar[gCmd[5]].p, (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]), (float)getVarOrFloat(gCmd[8]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_play3dClockedEx()
{
  if (gCmd.size() != 14)  {
    printf("Invalid number of parameters (11 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_play3dClockedEx((Soloud *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]), (AudioSource *)gVar[gCmd[5]].p, (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]), (float)getVarOrFloat(gCmd[8]), (float)getVarOrFloat(gCmd[9]), (float)getVarOrFloat(gCmd[10]), (float)getVarOrFloat(gCmd[11]), (float)getVarOrFloat(gCmd[12]), (unsigned int)getVarOrInt(gCmd[13]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_playBackground()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_playBackground((Soloud *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_playBackgroundEx()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_playBackgroundEx((Soloud *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p, (float)getVarOrFloat(gCmd[5]), (int)getVarOrInt(gCmd[6]), (unsigned int)getVarOrInt(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_seek()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_seek((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (double)getVarOrFloat(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_stop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_stop((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
}

void call_Soloud_stopAll()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_stopAll((Soloud *)gVar[gCmd[3]].p);
}

void call_Soloud_stopAudioSource()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Soloud_stopAudioSource((Soloud *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p);
}

void call_Soloud_countAudioSource()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)Soloud_countAudioSource((Soloud *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_setFilterParameter()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setFilterParameter((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (unsigned int)getVarOrInt(gCmd[5]), (unsigned int)getVarOrInt(gCmd[6]), (float)getVarOrFloat(gCmd[7]));
}

void call_Soloud_getFilterParameter()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)Soloud_getFilterParameter((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (unsigned int)getVarOrInt(gCmd[5]), (unsigned int)getVarOrInt(gCmd[6]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_fadeFilterParameter()
{
  if (gCmd.size() != 9)  {
    printf("Invalid number of parameters (6 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_fadeFilterParameter((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (unsigned int)getVarOrInt(gCmd[5]), (unsigned int)getVarOrInt(gCmd[6]), (float)getVarOrFloat(gCmd[7]), (double)getVarOrFloat(gCmd[8]));
}

void call_Soloud_oscillateFilterParameter()
{
  if (gCmd.size() != 10)  {
    printf("Invalid number of parameters (7 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_oscillateFilterParameter((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (unsigned int)getVarOrInt(gCmd[5]), (unsigned int)getVarOrInt(gCmd[6]), (float)getVarOrFloat(gCmd[7]), (float)getVarOrFloat(gCmd[8]), (double)getVarOrFloat(gCmd[9]));
}

void call_Soloud_getStreamTime()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Soloud_getStreamTime((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_getStreamPosition()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Soloud_getStreamPosition((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_getPause()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_getPause((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_getVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)Soloud_getVolume((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_getOverallVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)Soloud_getOverallVolume((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_getPan()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)Soloud_getPan((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_getSamplerate()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)Soloud_getSamplerate((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_getProtectVoice()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_getProtectVoice((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_getActiveVoiceCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_getActiveVoiceCount((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_getVoiceCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_getVoiceCount((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_isValidVoiceHandle()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_isValidVoiceHandle((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_getRelativePlaySpeed()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)Soloud_getRelativePlaySpeed((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_getPostClipScaler()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)Soloud_getPostClipScaler((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_getMainResampler()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_getMainResampler((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_getGlobalVolume()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)Soloud_getGlobalVolume((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_getMaxActiveVoiceCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_getMaxActiveVoiceCount((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_getLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_getLooping((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_getAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_getAutoStop((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_getLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Soloud_getLoopPoint((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_setLoopPoint()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setLoopPoint((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (double)getVarOrFloat(gCmd[5]));
}

void call_Soloud_setLooping()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setLooping((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Soloud_setAutoStop()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setAutoStop((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Soloud_setMaxActiveVoiceCount()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_setMaxActiveVoiceCount((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_setInaudibleBehavior()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setInaudibleBehavior((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]), (int)getVarOrInt(gCmd[6]));
}

void call_Soloud_setGlobalVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setGlobalVolume((Soloud *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Soloud_setPostClipScaler()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setPostClipScaler((Soloud *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Soloud_setMainResampler()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setMainResampler((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
}

void call_Soloud_setPause()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setPause((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Soloud_setPauseAll()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setPauseAll((Soloud *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Soloud_setRelativePlaySpeed()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_setRelativePlaySpeed((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_setProtectVoice()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setProtectVoice((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Soloud_setSamplerate()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setSamplerate((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Soloud_setPan()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setPan((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Soloud_setPanAbsolute()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setPanAbsolute((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]));
}

void call_Soloud_setChannelVolume()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setChannelVolume((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (unsigned int)getVarOrInt(gCmd[5]), (float)getVarOrFloat(gCmd[6]));
}

void call_Soloud_setVolume()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setVolume((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Soloud_setDelaySamples()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setDelaySamples((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (unsigned int)getVarOrInt(gCmd[5]));
}

void call_Soloud_fadeVolume()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_fadeVolume((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (double)getVarOrFloat(gCmd[6]));
}

void call_Soloud_fadePan()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_fadePan((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (double)getVarOrFloat(gCmd[6]));
}

void call_Soloud_fadeRelativePlaySpeed()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_fadeRelativePlaySpeed((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (double)getVarOrFloat(gCmd[6]));
}

void call_Soloud_fadeGlobalVolume()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_fadeGlobalVolume((Soloud *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (double)getVarOrFloat(gCmd[5]));
}

void call_Soloud_schedulePause()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_schedulePause((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (double)getVarOrFloat(gCmd[5]));
}

void call_Soloud_scheduleStop()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_scheduleStop((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (double)getVarOrFloat(gCmd[5]));
}

void call_Soloud_oscillateVolume()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_oscillateVolume((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (double)getVarOrFloat(gCmd[7]));
}

void call_Soloud_oscillatePan()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_oscillatePan((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (double)getVarOrFloat(gCmd[7]));
}

void call_Soloud_oscillateRelativePlaySpeed()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_oscillateRelativePlaySpeed((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (double)getVarOrFloat(gCmd[7]));
}

void call_Soloud_oscillateGlobalVolume()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_oscillateGlobalVolume((Soloud *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (double)getVarOrFloat(gCmd[6]));
}

void call_Soloud_setGlobalFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  Soloud_setGlobalFilter((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_Soloud_setVisualizationEnable()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_setVisualizationEnable((Soloud *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Soloud_getApproximateVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)Soloud_getApproximateVolume((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_getLoopCount()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_getLoopCount((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_getInfo()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)Soloud_getInfo((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (unsigned int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_createVoiceGroup()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Soloud_createVoiceGroup((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_destroyVoiceGroup()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_destroyVoiceGroup((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_addVoiceToGroup()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_addVoiceToGroup((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (unsigned int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_isVoiceGroup()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_isVoiceGroup((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_isVoiceGroupEmpty()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_isVoiceGroupEmpty((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_update3dAudio()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_update3dAudio((Soloud *)gVar[gCmd[3]].p);
}

void call_Soloud_set3dSoundSpeed()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Soloud_set3dSoundSpeed((Soloud *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Soloud_get3dSoundSpeed()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)Soloud_get3dSoundSpeed((Soloud *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Soloud_set3dListenerParameters()
{
  if (gCmd.size() != 13)  {
    printf("Invalid number of parameters (10 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_set3dListenerParameters((Soloud *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]), (float)getVarOrFloat(gCmd[8]), (float)getVarOrFloat(gCmd[9]), (float)getVarOrFloat(gCmd[10]), (float)getVarOrFloat(gCmd[11]), (float)getVarOrFloat(gCmd[12]));
}

void call_Soloud_set3dListenerParametersEx()
{
  if (gCmd.size() != 16)  {
    printf("Invalid number of parameters (13 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_set3dListenerParametersEx((Soloud *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]), (float)getVarOrFloat(gCmd[8]), (float)getVarOrFloat(gCmd[9]), (float)getVarOrFloat(gCmd[10]), (float)getVarOrFloat(gCmd[11]), (float)getVarOrFloat(gCmd[12]), (float)getVarOrFloat(gCmd[13]), (float)getVarOrFloat(gCmd[14]), (float)getVarOrFloat(gCmd[15]));
}

void call_Soloud_set3dListenerPosition()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_set3dListenerPosition((Soloud *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]));
}

void call_Soloud_set3dListenerAt()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_set3dListenerAt((Soloud *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]));
}

void call_Soloud_set3dListenerUp()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_set3dListenerUp((Soloud *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]));
}

void call_Soloud_set3dListenerVelocity()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_set3dListenerVelocity((Soloud *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]));
}

void call_Soloud_set3dSourceParameters()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_set3dSourceParameters((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]));
}

void call_Soloud_set3dSourceParametersEx()
{
  if (gCmd.size() != 11)  {
    printf("Invalid number of parameters (8 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_set3dSourceParametersEx((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]), (float)getVarOrFloat(gCmd[8]), (float)getVarOrFloat(gCmd[9]), (float)getVarOrFloat(gCmd[10]));
}

void call_Soloud_set3dSourcePosition()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_set3dSourcePosition((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]));
}

void call_Soloud_set3dSourceVelocity()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_set3dSourceVelocity((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]));
}

void call_Soloud_set3dSourceMinMaxDistance()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_set3dSourceMinMaxDistance((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]));
}

void call_Soloud_set3dSourceAttenuation()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_set3dSourceAttenuation((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (unsigned int)getVarOrInt(gCmd[5]), (float)getVarOrFloat(gCmd[6]));
}

void call_Soloud_set3dSourceDopplerFactor()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Soloud_set3dSourceDopplerFactor((Soloud *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Soloud_mix()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Soloud_mix((Soloud *)gVar[gCmd[3]].p, (float *)gVar[gCmd[4]].p, (unsigned int)getVarOrInt(gCmd[5]));
}

void call_Soloud_mixSigned16()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Soloud_mixSigned16((Soloud *)gVar[gCmd[3]].p, (short *)gVar[gCmd[4]].p, (unsigned int)getVarOrInt(gCmd[5]));
}

void call_Ay_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Ay_destroy((Ay *)gVar[gCmd[3]].p);
}

void call_Ay_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  Ay * res = (Ay *)Ay_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_Ay_setVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Ay_setVolume((Ay *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Ay_setLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Ay_setLooping((Ay *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Ay_setAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Ay_setAutoStop((Ay *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Ay_set3dMinMaxDistance()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Ay_set3dMinMaxDistance((Ay *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Ay_set3dAttenuation()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Ay_set3dAttenuation((Ay *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Ay_set3dDopplerFactor()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Ay_set3dDopplerFactor((Ay *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Ay_set3dListenerRelative()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Ay_set3dListenerRelative((Ay *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Ay_set3dDistanceDelay()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Ay_set3dDistanceDelay((Ay *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Ay_set3dCollider()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Ay_set3dCollider((Ay *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p);
}

void call_Ay_set3dColliderEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Ay_set3dColliderEx((Ay *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p, (int)getVarOrInt(gCmd[5]));
}

void call_Ay_set3dAttenuator()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Ay_set3dAttenuator((Ay *)gVar[gCmd[3]].p, (AudioAttenuator *)gVar[gCmd[4]].p);
}

void call_Ay_setInaudibleBehavior()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Ay_setInaudibleBehavior((Ay *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Ay_setLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Ay_setLoopPoint((Ay *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]));
}

void call_Ay_getLoopPoint()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Ay_getLoopPoint((Ay *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Ay_setFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  Ay_setFilter((Ay *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_Ay_stop()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Ay_stop((Ay *)gVar[gCmd[3]].p);
}

void call_BassboostFilter_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  BassboostFilter_destroy((BassboostFilter *)gVar[gCmd[3]].p);
}

void call_BassboostFilter_getParamCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)BassboostFilter_getParamCount((BassboostFilter *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_BassboostFilter_getParamType()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)BassboostFilter_getParamType((BassboostFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_BassboostFilter_getParamMax()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)BassboostFilter_getParamMax((BassboostFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_BassboostFilter_getParamMin()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)BassboostFilter_getParamMin((BassboostFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_BassboostFilter_setParams()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)BassboostFilter_setParams((BassboostFilter *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_BassboostFilter_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  BassboostFilter * res = (BassboostFilter *)BassboostFilter_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_BiquadResonantFilter_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  BiquadResonantFilter_destroy((BiquadResonantFilter *)gVar[gCmd[3]].p);
}

void call_BiquadResonantFilter_getParamCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)BiquadResonantFilter_getParamCount((BiquadResonantFilter *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_BiquadResonantFilter_getParamType()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)BiquadResonantFilter_getParamType((BiquadResonantFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_BiquadResonantFilter_getParamMax()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)BiquadResonantFilter_getParamMax((BiquadResonantFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_BiquadResonantFilter_getParamMin()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)BiquadResonantFilter_getParamMin((BiquadResonantFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_BiquadResonantFilter_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  BiquadResonantFilter * res = (BiquadResonantFilter *)BiquadResonantFilter_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_BiquadResonantFilter_setParams()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)BiquadResonantFilter_setParams((BiquadResonantFilter *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Bus_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_destroy((Bus *)gVar[gCmd[3]].p);
}

void call_Bus_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  Bus * res = (Bus *)Bus_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_Bus_setFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  Bus_setFilter((Bus *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_Bus_play()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  unsigned int res = (unsigned int)Bus_play((Bus *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Bus_playEx()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  unsigned int res = (unsigned int)Bus_playEx((Bus *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p, (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (int)getVarOrInt(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Bus_playClocked()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  unsigned int res = (unsigned int)Bus_playClocked((Bus *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]), (AudioSource *)gVar[gCmd[5]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Bus_playClockedEx()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  unsigned int res = (unsigned int)Bus_playClockedEx((Bus *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]), (AudioSource *)gVar[gCmd[5]].p, (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Bus_play3d()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  unsigned int res = (unsigned int)Bus_play3d((Bus *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p, (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Bus_play3dEx()
{
  if (gCmd.size() != 13)  {
    printf("Invalid number of parameters (10 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  unsigned int res = (unsigned int)Bus_play3dEx((Bus *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p, (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]), (float)getVarOrFloat(gCmd[8]), (float)getVarOrFloat(gCmd[9]), (float)getVarOrFloat(gCmd[10]), (float)getVarOrFloat(gCmd[11]), (int)getVarOrInt(gCmd[12]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Bus_play3dClocked()
{
  if (gCmd.size() != 9)  {
    printf("Invalid number of parameters (6 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  unsigned int res = (unsigned int)Bus_play3dClocked((Bus *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]), (AudioSource *)gVar[gCmd[5]].p, (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]), (float)getVarOrFloat(gCmd[8]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Bus_play3dClockedEx()
{
  if (gCmd.size() != 13)  {
    printf("Invalid number of parameters (10 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  unsigned int res = (unsigned int)Bus_play3dClockedEx((Bus *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]), (AudioSource *)gVar[gCmd[5]].p, (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]), (float)getVarOrFloat(gCmd[8]), (float)getVarOrFloat(gCmd[9]), (float)getVarOrFloat(gCmd[10]), (float)getVarOrFloat(gCmd[11]), (float)getVarOrFloat(gCmd[12]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Bus_setChannels()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Bus_setChannels((Bus *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Bus_setVisualizationEnable()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_setVisualizationEnable((Bus *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Bus_annexSound()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_annexSound((Bus *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
}

void call_Bus_getApproximateVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)Bus_getApproximateVolume((Bus *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Bus_getActiveVoiceCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Bus_getActiveVoiceCount((Bus *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Bus_getResampler()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Bus_getResampler((Bus *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Bus_setResampler()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_setResampler((Bus *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
}

void call_Bus_setVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_setVolume((Bus *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Bus_setLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_setLooping((Bus *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Bus_setAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_setAutoStop((Bus *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Bus_set3dMinMaxDistance()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_set3dMinMaxDistance((Bus *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Bus_set3dAttenuation()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_set3dAttenuation((Bus *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Bus_set3dDopplerFactor()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_set3dDopplerFactor((Bus *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Bus_set3dListenerRelative()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_set3dListenerRelative((Bus *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Bus_set3dDistanceDelay()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_set3dDistanceDelay((Bus *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Bus_set3dCollider()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Bus_set3dCollider((Bus *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p);
}

void call_Bus_set3dColliderEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Bus_set3dColliderEx((Bus *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p, (int)getVarOrInt(gCmd[5]));
}

void call_Bus_set3dAttenuator()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Bus_set3dAttenuator((Bus *)gVar[gCmd[3]].p, (AudioAttenuator *)gVar[gCmd[4]].p);
}

void call_Bus_setInaudibleBehavior()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_setInaudibleBehavior((Bus *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Bus_setLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_setLoopPoint((Bus *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]));
}

void call_Bus_getLoopPoint()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Bus_getLoopPoint((Bus *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Bus_stop()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Bus_stop((Bus *)gVar[gCmd[3]].p);
}

void call_DCRemovalFilter_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  DCRemovalFilter_destroy((DCRemovalFilter *)gVar[gCmd[3]].p);
}

void call_DCRemovalFilter_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  DCRemovalFilter * res = (DCRemovalFilter *)DCRemovalFilter_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_DCRemovalFilter_setParams()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)DCRemovalFilter_setParams((DCRemovalFilter *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_DCRemovalFilter_setParamsEx()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)DCRemovalFilter_setParamsEx((DCRemovalFilter *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_DCRemovalFilter_getParamCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)DCRemovalFilter_getParamCount((DCRemovalFilter *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_DCRemovalFilter_getParamType()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)DCRemovalFilter_getParamType((DCRemovalFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_DCRemovalFilter_getParamMax()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)DCRemovalFilter_getParamMax((DCRemovalFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_DCRemovalFilter_getParamMin()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)DCRemovalFilter_getParamMin((DCRemovalFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_EchoFilter_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  EchoFilter_destroy((EchoFilter *)gVar[gCmd[3]].p);
}

void call_EchoFilter_getParamCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)EchoFilter_getParamCount((EchoFilter *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_EchoFilter_getParamType()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)EchoFilter_getParamType((EchoFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_EchoFilter_getParamMax()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)EchoFilter_getParamMax((EchoFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_EchoFilter_getParamMin()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)EchoFilter_getParamMin((EchoFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_EchoFilter_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  EchoFilter * res = (EchoFilter *)EchoFilter_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_EchoFilter_setParams()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)EchoFilter_setParams((EchoFilter *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_EchoFilter_setParamsEx()
{
  if (gCmd.size() != 7)  {
    printf("Invalid number of parameters (4 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)EchoFilter_setParamsEx((EchoFilter *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_FFTFilter_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  FFTFilter_destroy((FFTFilter *)gVar[gCmd[3]].p);
}

void call_FFTFilter_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  FFTFilter * res = (FFTFilter *)FFTFilter_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_FFTFilter_getParamCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)FFTFilter_getParamCount((FFTFilter *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_FFTFilter_getParamType()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)FFTFilter_getParamType((FFTFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_FFTFilter_getParamMax()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)FFTFilter_getParamMax((FFTFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_FFTFilter_getParamMin()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)FFTFilter_getParamMin((FFTFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_FlangerFilter_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  FlangerFilter_destroy((FlangerFilter *)gVar[gCmd[3]].p);
}

void call_FlangerFilter_getParamCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)FlangerFilter_getParamCount((FlangerFilter *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_FlangerFilter_getParamType()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)FlangerFilter_getParamType((FlangerFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_FlangerFilter_getParamMax()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)FlangerFilter_getParamMax((FlangerFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_FlangerFilter_getParamMin()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)FlangerFilter_getParamMin((FlangerFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_FlangerFilter_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  FlangerFilter * res = (FlangerFilter *)FlangerFilter_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_FlangerFilter_setParams()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)FlangerFilter_setParams((FlangerFilter *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_FreeverbFilter_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  FreeverbFilter_destroy((FreeverbFilter *)gVar[gCmd[3]].p);
}

void call_FreeverbFilter_getParamCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)FreeverbFilter_getParamCount((FreeverbFilter *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_FreeverbFilter_getParamType()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)FreeverbFilter_getParamType((FreeverbFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_FreeverbFilter_getParamMax()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)FreeverbFilter_getParamMax((FreeverbFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_FreeverbFilter_getParamMin()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)FreeverbFilter_getParamMin((FreeverbFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_FreeverbFilter_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  FreeverbFilter * res = (FreeverbFilter *)FreeverbFilter_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_FreeverbFilter_setParams()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)FreeverbFilter_setParams((FreeverbFilter *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_LofiFilter_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  LofiFilter_destroy((LofiFilter *)gVar[gCmd[3]].p);
}

void call_LofiFilter_getParamCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)LofiFilter_getParamCount((LofiFilter *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_LofiFilter_getParamType()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)LofiFilter_getParamType((LofiFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_LofiFilter_getParamMax()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)LofiFilter_getParamMax((LofiFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_LofiFilter_getParamMin()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)LofiFilter_getParamMin((LofiFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_LofiFilter_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  LofiFilter * res = (LofiFilter *)LofiFilter_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_LofiFilter_setParams()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)LofiFilter_setParams((LofiFilter *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Monotone_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Monotone_destroy((Monotone *)gVar[gCmd[3]].p);
}

void call_Monotone_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  Monotone * res = (Monotone *)Monotone_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_Monotone_setParams()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Monotone_setParams((Monotone *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Monotone_setParamsEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Monotone_setParamsEx((Monotone *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Monotone_load()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Monotone_load((Monotone *)gVar[gCmd[3]].p, (const char *)gCmd[4].c_str());
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Monotone_loadMem()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Monotone_loadMem((Monotone *)gVar[gCmd[3]].p, (const unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Monotone_loadMemEx()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Monotone_loadMemEx((Monotone *)gVar[gCmd[3]].p, (const unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]), (int)getVarOrInt(gCmd[6]), (int)getVarOrInt(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Monotone_loadFile()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)Monotone_loadFile((Monotone *)gVar[gCmd[3]].p, (File *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Monotone_setVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Monotone_setVolume((Monotone *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Monotone_setLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Monotone_setLooping((Monotone *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Monotone_setAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Monotone_setAutoStop((Monotone *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Monotone_set3dMinMaxDistance()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Monotone_set3dMinMaxDistance((Monotone *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Monotone_set3dAttenuation()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Monotone_set3dAttenuation((Monotone *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Monotone_set3dDopplerFactor()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Monotone_set3dDopplerFactor((Monotone *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Monotone_set3dListenerRelative()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Monotone_set3dListenerRelative((Monotone *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Monotone_set3dDistanceDelay()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Monotone_set3dDistanceDelay((Monotone *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Monotone_set3dCollider()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Monotone_set3dCollider((Monotone *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p);
}

void call_Monotone_set3dColliderEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Monotone_set3dColliderEx((Monotone *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p, (int)getVarOrInt(gCmd[5]));
}

void call_Monotone_set3dAttenuator()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Monotone_set3dAttenuator((Monotone *)gVar[gCmd[3]].p, (AudioAttenuator *)gVar[gCmd[4]].p);
}

void call_Monotone_setInaudibleBehavior()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Monotone_setInaudibleBehavior((Monotone *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Monotone_setLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Monotone_setLoopPoint((Monotone *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]));
}

void call_Monotone_getLoopPoint()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Monotone_getLoopPoint((Monotone *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Monotone_setFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  Monotone_setFilter((Monotone *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_Monotone_stop()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Monotone_stop((Monotone *)gVar[gCmd[3]].p);
}

void call_Noise_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_destroy((Noise *)gVar[gCmd[3]].p);
}

void call_Noise_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  Noise * res = (Noise *)Noise_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_Noise_setOctaveScale()
{
  if (gCmd.size() != 14)  {
    printf("Invalid number of parameters (11 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_setOctaveScale((Noise *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (float)getVarOrFloat(gCmd[7]), (float)getVarOrFloat(gCmd[8]), (float)getVarOrFloat(gCmd[9]), (float)getVarOrFloat(gCmd[10]), (float)getVarOrFloat(gCmd[11]), (float)getVarOrFloat(gCmd[12]), (float)getVarOrFloat(gCmd[13]));
}

void call_Noise_setType()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_setType((Noise *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Noise_setVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_setVolume((Noise *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Noise_setLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_setLooping((Noise *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Noise_setAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_setAutoStop((Noise *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Noise_set3dMinMaxDistance()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_set3dMinMaxDistance((Noise *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Noise_set3dAttenuation()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_set3dAttenuation((Noise *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Noise_set3dDopplerFactor()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_set3dDopplerFactor((Noise *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Noise_set3dListenerRelative()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_set3dListenerRelative((Noise *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Noise_set3dDistanceDelay()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_set3dDistanceDelay((Noise *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Noise_set3dCollider()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Noise_set3dCollider((Noise *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p);
}

void call_Noise_set3dColliderEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Noise_set3dColliderEx((Noise *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p, (int)getVarOrInt(gCmd[5]));
}

void call_Noise_set3dAttenuator()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Noise_set3dAttenuator((Noise *)gVar[gCmd[3]].p, (AudioAttenuator *)gVar[gCmd[4]].p);
}

void call_Noise_setInaudibleBehavior()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_setInaudibleBehavior((Noise *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Noise_setLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_setLoopPoint((Noise *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]));
}

void call_Noise_getLoopPoint()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Noise_getLoopPoint((Noise *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Noise_setFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  Noise_setFilter((Noise *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_Noise_stop()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Noise_stop((Noise *)gVar[gCmd[3]].p);
}

void call_Openmpt_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Openmpt_destroy((Openmpt *)gVar[gCmd[3]].p);
}

void call_Openmpt_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  Openmpt * res = (Openmpt *)Openmpt_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_Openmpt_load()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Openmpt_load((Openmpt *)gVar[gCmd[3]].p, (const char *)gCmd[4].c_str());
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Openmpt_loadMem()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Openmpt_loadMem((Openmpt *)gVar[gCmd[3]].p, (const unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Openmpt_loadMemEx()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Openmpt_loadMemEx((Openmpt *)gVar[gCmd[3]].p, (const unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]), (int)getVarOrInt(gCmd[6]), (int)getVarOrInt(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Openmpt_loadFile()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)Openmpt_loadFile((Openmpt *)gVar[gCmd[3]].p, (File *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Openmpt_setVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Openmpt_setVolume((Openmpt *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Openmpt_setLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Openmpt_setLooping((Openmpt *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Openmpt_setAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Openmpt_setAutoStop((Openmpt *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Openmpt_set3dMinMaxDistance()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Openmpt_set3dMinMaxDistance((Openmpt *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Openmpt_set3dAttenuation()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Openmpt_set3dAttenuation((Openmpt *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Openmpt_set3dDopplerFactor()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Openmpt_set3dDopplerFactor((Openmpt *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Openmpt_set3dListenerRelative()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Openmpt_set3dListenerRelative((Openmpt *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Openmpt_set3dDistanceDelay()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Openmpt_set3dDistanceDelay((Openmpt *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Openmpt_set3dCollider()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Openmpt_set3dCollider((Openmpt *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p);
}

void call_Openmpt_set3dColliderEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Openmpt_set3dColliderEx((Openmpt *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p, (int)getVarOrInt(gCmd[5]));
}

void call_Openmpt_set3dAttenuator()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Openmpt_set3dAttenuator((Openmpt *)gVar[gCmd[3]].p, (AudioAttenuator *)gVar[gCmd[4]].p);
}

void call_Openmpt_setInaudibleBehavior()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Openmpt_setInaudibleBehavior((Openmpt *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Openmpt_setLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Openmpt_setLoopPoint((Openmpt *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]));
}

void call_Openmpt_getLoopPoint()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Openmpt_getLoopPoint((Openmpt *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Openmpt_setFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  Openmpt_setFilter((Openmpt *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_Openmpt_stop()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Openmpt_stop((Openmpt *)gVar[gCmd[3]].p);
}

void call_Queue_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Queue_destroy((Queue *)gVar[gCmd[3]].p);
}

void call_Queue_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  Queue * res = (Queue *)Queue_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_Queue_play()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)Queue_play((Queue *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Queue_getQueueCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)Queue_getQueueCount((Queue *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Queue_isCurrentlyPlaying()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)Queue_isCurrentlyPlaying((Queue *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Queue_setParamsFromAudioSource()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)Queue_setParamsFromAudioSource((Queue *)gVar[gCmd[3]].p, (AudioSource *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Queue_setParams()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Queue_setParams((Queue *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Queue_setParamsEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Queue_setParamsEx((Queue *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (unsigned int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Queue_setVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Queue_setVolume((Queue *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Queue_setLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Queue_setLooping((Queue *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Queue_setAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Queue_setAutoStop((Queue *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Queue_set3dMinMaxDistance()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Queue_set3dMinMaxDistance((Queue *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Queue_set3dAttenuation()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Queue_set3dAttenuation((Queue *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Queue_set3dDopplerFactor()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Queue_set3dDopplerFactor((Queue *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Queue_set3dListenerRelative()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Queue_set3dListenerRelative((Queue *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Queue_set3dDistanceDelay()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Queue_set3dDistanceDelay((Queue *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Queue_set3dCollider()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Queue_set3dCollider((Queue *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p);
}

void call_Queue_set3dColliderEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Queue_set3dColliderEx((Queue *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p, (int)getVarOrInt(gCmd[5]));
}

void call_Queue_set3dAttenuator()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Queue_set3dAttenuator((Queue *)gVar[gCmd[3]].p, (AudioAttenuator *)gVar[gCmd[4]].p);
}

void call_Queue_setInaudibleBehavior()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Queue_setInaudibleBehavior((Queue *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Queue_setLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Queue_setLoopPoint((Queue *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]));
}

void call_Queue_getLoopPoint()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Queue_getLoopPoint((Queue *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Queue_setFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  Queue_setFilter((Queue *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_Queue_stop()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Queue_stop((Queue *)gVar[gCmd[3]].p);
}

void call_RobotizeFilter_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  RobotizeFilter_destroy((RobotizeFilter *)gVar[gCmd[3]].p);
}

void call_RobotizeFilter_getParamCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)RobotizeFilter_getParamCount((RobotizeFilter *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_RobotizeFilter_getParamType()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)RobotizeFilter_getParamType((RobotizeFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_RobotizeFilter_getParamMax()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)RobotizeFilter_getParamMax((RobotizeFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_RobotizeFilter_getParamMin()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)RobotizeFilter_getParamMin((RobotizeFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_RobotizeFilter_setParams()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  RobotizeFilter_setParams((RobotizeFilter *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_RobotizeFilter_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  RobotizeFilter * res = (RobotizeFilter *)RobotizeFilter_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_Sfxr_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Sfxr_destroy((Sfxr *)gVar[gCmd[3]].p);
}

void call_Sfxr_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  Sfxr * res = (Sfxr *)Sfxr_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_Sfxr_resetParams()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Sfxr_resetParams((Sfxr *)gVar[gCmd[3]].p);
}

void call_Sfxr_loadParams()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Sfxr_loadParams((Sfxr *)gVar[gCmd[3]].p, (const char *)gCmd[4].c_str());
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Sfxr_loadParamsMem()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Sfxr_loadParamsMem((Sfxr *)gVar[gCmd[3]].p, (unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Sfxr_loadParamsMemEx()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Sfxr_loadParamsMemEx((Sfxr *)gVar[gCmd[3]].p, (unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]), (int)getVarOrInt(gCmd[6]), (int)getVarOrInt(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Sfxr_loadParamsFile()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)Sfxr_loadParamsFile((Sfxr *)gVar[gCmd[3]].p, (File *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Sfxr_loadPreset()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Sfxr_loadPreset((Sfxr *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Sfxr_setVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Sfxr_setVolume((Sfxr *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Sfxr_setLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Sfxr_setLooping((Sfxr *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Sfxr_setAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Sfxr_setAutoStop((Sfxr *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Sfxr_set3dMinMaxDistance()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Sfxr_set3dMinMaxDistance((Sfxr *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Sfxr_set3dAttenuation()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Sfxr_set3dAttenuation((Sfxr *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Sfxr_set3dDopplerFactor()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Sfxr_set3dDopplerFactor((Sfxr *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Sfxr_set3dListenerRelative()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Sfxr_set3dListenerRelative((Sfxr *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Sfxr_set3dDistanceDelay()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Sfxr_set3dDistanceDelay((Sfxr *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Sfxr_set3dCollider()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Sfxr_set3dCollider((Sfxr *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p);
}

void call_Sfxr_set3dColliderEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Sfxr_set3dColliderEx((Sfxr *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p, (int)getVarOrInt(gCmd[5]));
}

void call_Sfxr_set3dAttenuator()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Sfxr_set3dAttenuator((Sfxr *)gVar[gCmd[3]].p, (AudioAttenuator *)gVar[gCmd[4]].p);
}

void call_Sfxr_setInaudibleBehavior()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Sfxr_setInaudibleBehavior((Sfxr *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Sfxr_setLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Sfxr_setLoopPoint((Sfxr *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]));
}

void call_Sfxr_getLoopPoint()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Sfxr_getLoopPoint((Sfxr *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Sfxr_setFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  Sfxr_setFilter((Sfxr *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_Sfxr_stop()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Sfxr_stop((Sfxr *)gVar[gCmd[3]].p);
}

void call_Speech_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Speech_destroy((Speech *)gVar[gCmd[3]].p);
}

void call_Speech_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  Speech * res = (Speech *)Speech_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_Speech_setText()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Speech_setText((Speech *)gVar[gCmd[3]].p, (const char *)gCmd[4].c_str());
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Speech_setParams()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Speech_setParams((Speech *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Speech_setParamsEx()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Speech_setParamsEx((Speech *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (int)getVarOrInt(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Speech_setVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Speech_setVolume((Speech *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Speech_setLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Speech_setLooping((Speech *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Speech_setAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Speech_setAutoStop((Speech *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Speech_set3dMinMaxDistance()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Speech_set3dMinMaxDistance((Speech *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Speech_set3dAttenuation()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Speech_set3dAttenuation((Speech *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Speech_set3dDopplerFactor()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Speech_set3dDopplerFactor((Speech *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Speech_set3dListenerRelative()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Speech_set3dListenerRelative((Speech *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Speech_set3dDistanceDelay()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Speech_set3dDistanceDelay((Speech *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Speech_set3dCollider()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Speech_set3dCollider((Speech *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p);
}

void call_Speech_set3dColliderEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Speech_set3dColliderEx((Speech *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p, (int)getVarOrInt(gCmd[5]));
}

void call_Speech_set3dAttenuator()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Speech_set3dAttenuator((Speech *)gVar[gCmd[3]].p, (AudioAttenuator *)gVar[gCmd[4]].p);
}

void call_Speech_setInaudibleBehavior()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Speech_setInaudibleBehavior((Speech *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Speech_setLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Speech_setLoopPoint((Speech *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]));
}

void call_Speech_getLoopPoint()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Speech_getLoopPoint((Speech *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Speech_setFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  Speech_setFilter((Speech *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_Speech_stop()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Speech_stop((Speech *)gVar[gCmd[3]].p);
}

void call_TedSid_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  TedSid_destroy((TedSid *)gVar[gCmd[3]].p);
}

void call_TedSid_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  TedSid * res = (TedSid *)TedSid_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_TedSid_load()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)TedSid_load((TedSid *)gVar[gCmd[3]].p, (const char *)gCmd[4].c_str());
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_TedSid_loadMem()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)TedSid_loadMem((TedSid *)gVar[gCmd[3]].p, (const unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_TedSid_loadMemEx()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)TedSid_loadMemEx((TedSid *)gVar[gCmd[3]].p, (const unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]), (int)getVarOrInt(gCmd[6]), (int)getVarOrInt(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_TedSid_loadFile()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)TedSid_loadFile((TedSid *)gVar[gCmd[3]].p, (File *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_TedSid_setVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  TedSid_setVolume((TedSid *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_TedSid_setLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  TedSid_setLooping((TedSid *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_TedSid_setAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  TedSid_setAutoStop((TedSid *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_TedSid_set3dMinMaxDistance()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  TedSid_set3dMinMaxDistance((TedSid *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_TedSid_set3dAttenuation()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  TedSid_set3dAttenuation((TedSid *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_TedSid_set3dDopplerFactor()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  TedSid_set3dDopplerFactor((TedSid *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_TedSid_set3dListenerRelative()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  TedSid_set3dListenerRelative((TedSid *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_TedSid_set3dDistanceDelay()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  TedSid_set3dDistanceDelay((TedSid *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_TedSid_set3dCollider()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  TedSid_set3dCollider((TedSid *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p);
}

void call_TedSid_set3dColliderEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  TedSid_set3dColliderEx((TedSid *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p, (int)getVarOrInt(gCmd[5]));
}

void call_TedSid_set3dAttenuator()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  TedSid_set3dAttenuator((TedSid *)gVar[gCmd[3]].p, (AudioAttenuator *)gVar[gCmd[4]].p);
}

void call_TedSid_setInaudibleBehavior()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  TedSid_setInaudibleBehavior((TedSid *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_TedSid_setLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  TedSid_setLoopPoint((TedSid *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]));
}

void call_TedSid_getLoopPoint()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)TedSid_getLoopPoint((TedSid *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_TedSid_setFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  TedSid_setFilter((TedSid *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_TedSid_stop()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  TedSid_stop((TedSid *)gVar[gCmd[3]].p);
}

void call_Vic_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_destroy((Vic *)gVar[gCmd[3]].p);
}

void call_Vic_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  Vic * res = (Vic *)Vic_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_Vic_setModel()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_setModel((Vic *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Vic_getModel()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Vic_getModel((Vic *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Vic_setRegister()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_setRegister((Vic *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (unsigned char)getVarOrInt(gCmd[5]));
}

void call_Vic_getRegister()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned char res = (unsigned char)Vic_getRegister((Vic *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Vic_setVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_setVolume((Vic *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Vic_setLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_setLooping((Vic *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Vic_setAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_setAutoStop((Vic *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Vic_set3dMinMaxDistance()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_set3dMinMaxDistance((Vic *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Vic_set3dAttenuation()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_set3dAttenuation((Vic *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Vic_set3dDopplerFactor()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_set3dDopplerFactor((Vic *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Vic_set3dListenerRelative()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_set3dListenerRelative((Vic *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Vic_set3dDistanceDelay()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_set3dDistanceDelay((Vic *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Vic_set3dCollider()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Vic_set3dCollider((Vic *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p);
}

void call_Vic_set3dColliderEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Vic_set3dColliderEx((Vic *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p, (int)getVarOrInt(gCmd[5]));
}

void call_Vic_set3dAttenuator()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Vic_set3dAttenuator((Vic *)gVar[gCmd[3]].p, (AudioAttenuator *)gVar[gCmd[4]].p);
}

void call_Vic_setInaudibleBehavior()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_setInaudibleBehavior((Vic *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Vic_setLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_setLoopPoint((Vic *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]));
}

void call_Vic_getLoopPoint()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Vic_getLoopPoint((Vic *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Vic_setFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  Vic_setFilter((Vic *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_Vic_stop()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vic_stop((Vic *)gVar[gCmd[3]].p);
}

void call_Vizsn_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vizsn_destroy((Vizsn *)gVar[gCmd[3]].p);
}

void call_Vizsn_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  Vizsn * res = (Vizsn *)Vizsn_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_Vizsn_setText()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vizsn_setText((Vizsn *)gVar[gCmd[3]].p, (char *)gCmd[4].c_str());
}

void call_Vizsn_setVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vizsn_setVolume((Vizsn *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Vizsn_setLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vizsn_setLooping((Vizsn *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Vizsn_setAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vizsn_setAutoStop((Vizsn *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Vizsn_set3dMinMaxDistance()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vizsn_set3dMinMaxDistance((Vizsn *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Vizsn_set3dAttenuation()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vizsn_set3dAttenuation((Vizsn *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Vizsn_set3dDopplerFactor()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vizsn_set3dDopplerFactor((Vizsn *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Vizsn_set3dListenerRelative()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vizsn_set3dListenerRelative((Vizsn *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Vizsn_set3dDistanceDelay()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vizsn_set3dDistanceDelay((Vizsn *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Vizsn_set3dCollider()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Vizsn_set3dCollider((Vizsn *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p);
}

void call_Vizsn_set3dColliderEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Vizsn_set3dColliderEx((Vizsn *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p, (int)getVarOrInt(gCmd[5]));
}

void call_Vizsn_set3dAttenuator()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Vizsn_set3dAttenuator((Vizsn *)gVar[gCmd[3]].p, (AudioAttenuator *)gVar[gCmd[4]].p);
}

void call_Vizsn_setInaudibleBehavior()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vizsn_setInaudibleBehavior((Vizsn *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Vizsn_setLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vizsn_setLoopPoint((Vizsn *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]));
}

void call_Vizsn_getLoopPoint()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Vizsn_getLoopPoint((Vizsn *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Vizsn_setFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  Vizsn_setFilter((Vizsn *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_Vizsn_stop()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Vizsn_stop((Vizsn *)gVar[gCmd[3]].p);
}

void call_Wav_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Wav_destroy((Wav *)gVar[gCmd[3]].p);
}

void call_Wav_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  Wav * res = (Wav *)Wav_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_Wav_load()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Wav_load((Wav *)gVar[gCmd[3]].p, (const char *)gCmd[4].c_str());
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Wav_loadMem()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Wav_loadMem((Wav *)gVar[gCmd[3]].p, (const unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Wav_loadMemEx()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Wav_loadMemEx((Wav *)gVar[gCmd[3]].p, (const unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]), (int)getVarOrInt(gCmd[6]), (int)getVarOrInt(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Wav_loadFile()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)Wav_loadFile((Wav *)gVar[gCmd[3]].p, (File *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Wav_loadRawWave8()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Wav_loadRawWave8((Wav *)gVar[gCmd[3]].p, (unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Wav_loadRawWave8Ex()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)Wav_loadRawWave8Ex((Wav *)gVar[gCmd[3]].p, (unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (unsigned int)getVarOrInt(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Wav_loadRawWave16()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)Wav_loadRawWave16((Wav *)gVar[gCmd[3]].p, (short *)gVar[gCmd[4]].p, (unsigned int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Wav_loadRawWave16Ex()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)Wav_loadRawWave16Ex((Wav *)gVar[gCmd[3]].p, (short *)gVar[gCmd[4]].p, (unsigned int)getVarOrInt(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (unsigned int)getVarOrInt(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Wav_loadRawWave()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)Wav_loadRawWave((Wav *)gVar[gCmd[3]].p, (float *)gVar[gCmd[4]].p, (unsigned int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Wav_loadRawWaveEx()
{
  if (gCmd.size() != 10)  {
    printf("Invalid number of parameters (7 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)Wav_loadRawWaveEx((Wav *)gVar[gCmd[3]].p, (float *)gVar[gCmd[4]].p, (unsigned int)getVarOrInt(gCmd[5]), (float)getVarOrFloat(gCmd[6]), (unsigned int)getVarOrInt(gCmd[7]), (int)getVarOrInt(gCmd[8]), (int)getVarOrInt(gCmd[9]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_Wav_getLength()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Wav_getLength((Wav *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Wav_setVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Wav_setVolume((Wav *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Wav_setLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Wav_setLooping((Wav *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Wav_setAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Wav_setAutoStop((Wav *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Wav_set3dMinMaxDistance()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Wav_set3dMinMaxDistance((Wav *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Wav_set3dAttenuation()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Wav_set3dAttenuation((Wav *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_Wav_set3dDopplerFactor()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Wav_set3dDopplerFactor((Wav *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_Wav_set3dListenerRelative()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Wav_set3dListenerRelative((Wav *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Wav_set3dDistanceDelay()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Wav_set3dDistanceDelay((Wav *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_Wav_set3dCollider()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Wav_set3dCollider((Wav *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p);
}

void call_Wav_set3dColliderEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Wav_set3dColliderEx((Wav *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p, (int)getVarOrInt(gCmd[5]));
}

void call_Wav_set3dAttenuator()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  Wav_set3dAttenuator((Wav *)gVar[gCmd[3]].p, (AudioAttenuator *)gVar[gCmd[4]].p);
}

void call_Wav_setInaudibleBehavior()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Wav_setInaudibleBehavior((Wav *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_Wav_setLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Wav_setLoopPoint((Wav *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]));
}

void call_Wav_getLoopPoint()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)Wav_getLoopPoint((Wav *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_Wav_setFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  Wav_setFilter((Wav *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_Wav_stop()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  Wav_stop((Wav *)gVar[gCmd[3]].p);
}

void call_WaveShaperFilter_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  WaveShaperFilter_destroy((WaveShaperFilter *)gVar[gCmd[3]].p);
}

void call_WaveShaperFilter_setParams()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)WaveShaperFilter_setParams((WaveShaperFilter *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_WaveShaperFilter_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  WaveShaperFilter * res = (WaveShaperFilter *)WaveShaperFilter_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_WaveShaperFilter_getParamCount()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)WaveShaperFilter_getParamCount((WaveShaperFilter *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_WaveShaperFilter_getParamType()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  unsigned int res = (unsigned int)WaveShaperFilter_getParamType((WaveShaperFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_WaveShaperFilter_getParamMax()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)WaveShaperFilter_getParamMax((WaveShaperFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_WaveShaperFilter_getParamMin()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  float res = (float)WaveShaperFilter_getParamMin((WaveShaperFilter *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]));
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_WavStream_destroy()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  WavStream_destroy((WavStream *)gVar[gCmd[3]].p);
}

void call_WavStream_create()
{
  if (gCmd.size() != 3)  {
    printf("Invalid number of parameters (0 expected)\n");
    return;
  }
  WavStream * res = (WavStream *)WavStream_create();
  gVar[gCmd[0].c_str()].p = (void *)res;
}

void call_WavStream_load()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)WavStream_load((WavStream *)gVar[gCmd[3]].p, (const char *)gCmd[4].c_str());
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_WavStream_loadMem()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)WavStream_loadMem((WavStream *)gVar[gCmd[3]].p, (const unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_WavStream_loadMemEx()
{
  if (gCmd.size() != 8)  {
    printf("Invalid number of parameters (5 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)WavStream_loadMemEx((WavStream *)gVar[gCmd[3]].p, (const unsigned char *)gCmd[4].c_str(), (unsigned int)getVarOrInt(gCmd[5]), (int)getVarOrInt(gCmd[6]), (int)getVarOrInt(gCmd[7]));
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_WavStream_loadToMem()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  int res = (int)WavStream_loadToMem((WavStream *)gVar[gCmd[3]].p, (const char *)gCmd[4].c_str());
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_WavStream_loadFile()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)WavStream_loadFile((WavStream *)gVar[gCmd[3]].p, (File *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_WavStream_loadFileToMem()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  int res = (int)WavStream_loadFileToMem((WavStream *)gVar[gCmd[3]].p, (File *)gVar[gCmd[4]].p);
  gVar[gCmd[0].c_str()].i = (int)res;
}

void call_WavStream_getLength()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)WavStream_getLength((WavStream *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_WavStream_setVolume()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  WavStream_setVolume((WavStream *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_WavStream_setLooping()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  WavStream_setLooping((WavStream *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_WavStream_setAutoStop()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  WavStream_setAutoStop((WavStream *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_WavStream_set3dMinMaxDistance()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  WavStream_set3dMinMaxDistance((WavStream *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_WavStream_set3dAttenuation()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  WavStream_set3dAttenuation((WavStream *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (float)getVarOrFloat(gCmd[5]));
}

void call_WavStream_set3dDopplerFactor()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  WavStream_set3dDopplerFactor((WavStream *)gVar[gCmd[3]].p, (float)getVarOrFloat(gCmd[4]));
}

void call_WavStream_set3dListenerRelative()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  WavStream_set3dListenerRelative((WavStream *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_WavStream_set3dDistanceDelay()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  WavStream_set3dDistanceDelay((WavStream *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]));
}

void call_WavStream_set3dCollider()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  WavStream_set3dCollider((WavStream *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p);
}

void call_WavStream_set3dColliderEx()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  WavStream_set3dColliderEx((WavStream *)gVar[gCmd[3]].p, (AudioCollider *)gVar[gCmd[4]].p, (int)getVarOrInt(gCmd[5]));
}

void call_WavStream_set3dAttenuator()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[4]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[4].c_str());
    return;
  }
  WavStream_set3dAttenuator((WavStream *)gVar[gCmd[3]].p, (AudioAttenuator *)gVar[gCmd[4]].p);
}

void call_WavStream_setInaudibleBehavior()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  WavStream_setInaudibleBehavior((WavStream *)gVar[gCmd[3]].p, (int)getVarOrInt(gCmd[4]), (int)getVarOrInt(gCmd[5]));
}

void call_WavStream_setLoopPoint()
{
  if (gCmd.size() != 5)  {
    printf("Invalid number of parameters (2 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  WavStream_setLoopPoint((WavStream *)gVar[gCmd[3]].p, (double)getVarOrFloat(gCmd[4]));
}

void call_WavStream_getLoopPoint()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  double res = (double)WavStream_getLoopPoint((WavStream *)gVar[gCmd[3]].p);
  gVar[gCmd[0].c_str()].f = (int)res;
}

void call_WavStream_setFilter()
{
  if (gCmd.size() != 6)  {
    printf("Invalid number of parameters (3 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  if (gVar.count(gCmd[5]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[5].c_str());
    return;
  }
  WavStream_setFilter((WavStream *)gVar[gCmd[3]].p, (unsigned int)getVarOrInt(gCmd[4]), (Filter *)gVar[gCmd[5]].p);
}

void call_WavStream_stop()
{
  if (gCmd.size() != 4)  {
    printf("Invalid number of parameters (1 expected)\n");
    return;
  }
  if (gVar.count(gCmd[3]) == 0)
  {
    printf("Variable \"%s\" not found\n", gCmd[3].c_str());
    return;
  }
  WavStream_stop((WavStream *)gVar[gCmd[3]].p);
}

void setup_soloud_calls()
{
  gSoloudCalls["Soloud"]["destroy"].p = call_Soloud_destroy;
  gSoloudCalls["Soloud"]["destroy"].info = "void Soloud_destroy( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["create"].p = call_Soloud_create;
  gSoloudCalls["Soloud"]["create"].info = "Soloud * Soloud_create()";
  gSoloudCalls["Soloud"]["init"].p = call_Soloud_init;
  gSoloudCalls["Soloud"]["init"].info = "int Soloud_init( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["initEx"].p = call_Soloud_initEx;
  gSoloudCalls["Soloud"]["initEx"].info = "int Soloud_initEx( \n    Soloud * aSoloud,\n    unsigned int aFlags,\n    unsigned int aBackend,\n    unsigned int aSamplerate,\n    unsigned int aBufferSize,\n    unsigned int aChannels)";
  gSoloudCalls["Soloud"]["deinit"].p = call_Soloud_deinit;
  gSoloudCalls["Soloud"]["deinit"].info = "void Soloud_deinit( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["getVersion"].p = call_Soloud_getVersion;
  gSoloudCalls["Soloud"]["getVersion"].info = "unsigned int Soloud_getVersion( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["getBackendId"].p = call_Soloud_getBackendId;
  gSoloudCalls["Soloud"]["getBackendId"].info = "unsigned int Soloud_getBackendId( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["getBackendChannels"].p = call_Soloud_getBackendChannels;
  gSoloudCalls["Soloud"]["getBackendChannels"].info = "unsigned int Soloud_getBackendChannels( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["getBackendSamplerate"].p = call_Soloud_getBackendSamplerate;
  gSoloudCalls["Soloud"]["getBackendSamplerate"].info = "unsigned int Soloud_getBackendSamplerate( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["getBackendBufferSize"].p = call_Soloud_getBackendBufferSize;
  gSoloudCalls["Soloud"]["getBackendBufferSize"].info = "unsigned int Soloud_getBackendBufferSize( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["setSpeakerPosition"].p = call_Soloud_setSpeakerPosition;
  gSoloudCalls["Soloud"]["setSpeakerPosition"].info = "int Soloud_setSpeakerPosition( \n    Soloud * aSoloud,\n    unsigned int aChannel,\n    float aX,\n    float aY,\n    float aZ)";
  gSoloudCalls["Soloud"]["play"].p = call_Soloud_play;
  gSoloudCalls["Soloud"]["play"].info = "unsigned int Soloud_play( \n    Soloud * aSoloud,\n    AudioSource * aSound)";
  gSoloudCalls["Soloud"]["playEx"].p = call_Soloud_playEx;
  gSoloudCalls["Soloud"]["playEx"].info = "unsigned int Soloud_playEx( \n    Soloud * aSoloud,\n    AudioSource * aSound,\n    float aVolume,\n    float aPan,\n    int aPaused,\n    unsigned int aBus)";
  gSoloudCalls["Soloud"]["playClocked"].p = call_Soloud_playClocked;
  gSoloudCalls["Soloud"]["playClocked"].info = "unsigned int Soloud_playClocked( \n    Soloud * aSoloud,\n    double aSoundTime,\n    AudioSource * aSound)";
  gSoloudCalls["Soloud"]["playClockedEx"].p = call_Soloud_playClockedEx;
  gSoloudCalls["Soloud"]["playClockedEx"].info = "unsigned int Soloud_playClockedEx( \n    Soloud * aSoloud,\n    double aSoundTime,\n    AudioSource * aSound,\n    float aVolume,\n    float aPan,\n    unsigned int aBus)";
  gSoloudCalls["Soloud"]["play3d"].p = call_Soloud_play3d;
  gSoloudCalls["Soloud"]["play3d"].info = "unsigned int Soloud_play3d( \n    Soloud * aSoloud,\n    AudioSource * aSound,\n    float aPosX,\n    float aPosY,\n    float aPosZ)";
  gSoloudCalls["Soloud"]["play3dEx"].p = call_Soloud_play3dEx;
  gSoloudCalls["Soloud"]["play3dEx"].info = "unsigned int Soloud_play3dEx( \n    Soloud * aSoloud,\n    AudioSource * aSound,\n    float aPosX,\n    float aPosY,\n    float aPosZ,\n    float aVelX,\n    float aVelY,\n    float aVelZ,\n    float aVolume,\n    int aPaused,\n    unsigned int aBus)";
  gSoloudCalls["Soloud"]["play3dClocked"].p = call_Soloud_play3dClocked;
  gSoloudCalls["Soloud"]["play3dClocked"].info = "unsigned int Soloud_play3dClocked( \n    Soloud * aSoloud,\n    double aSoundTime,\n    AudioSource * aSound,\n    float aPosX,\n    float aPosY,\n    float aPosZ)";
  gSoloudCalls["Soloud"]["play3dClockedEx"].p = call_Soloud_play3dClockedEx;
  gSoloudCalls["Soloud"]["play3dClockedEx"].info = "unsigned int Soloud_play3dClockedEx( \n    Soloud * aSoloud,\n    double aSoundTime,\n    AudioSource * aSound,\n    float aPosX,\n    float aPosY,\n    float aPosZ,\n    float aVelX,\n    float aVelY,\n    float aVelZ,\n    float aVolume,\n    unsigned int aBus)";
  gSoloudCalls["Soloud"]["playBackground"].p = call_Soloud_playBackground;
  gSoloudCalls["Soloud"]["playBackground"].info = "unsigned int Soloud_playBackground( \n    Soloud * aSoloud,\n    AudioSource * aSound)";
  gSoloudCalls["Soloud"]["playBackgroundEx"].p = call_Soloud_playBackgroundEx;
  gSoloudCalls["Soloud"]["playBackgroundEx"].info = "unsigned int Soloud_playBackgroundEx( \n    Soloud * aSoloud,\n    AudioSource * aSound,\n    float aVolume,\n    int aPaused,\n    unsigned int aBus)";
  gSoloudCalls["Soloud"]["seek"].p = call_Soloud_seek;
  gSoloudCalls["Soloud"]["seek"].info = "int Soloud_seek( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    double aSeconds)";
  gSoloudCalls["Soloud"]["stop"].p = call_Soloud_stop;
  gSoloudCalls["Soloud"]["stop"].info = "void Soloud_stop( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["stopAll"].p = call_Soloud_stopAll;
  gSoloudCalls["Soloud"]["stopAll"].info = "void Soloud_stopAll( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["stopAudioSource"].p = call_Soloud_stopAudioSource;
  gSoloudCalls["Soloud"]["stopAudioSource"].info = "void Soloud_stopAudioSource( \n    Soloud * aSoloud,\n    AudioSource * aSound)";
  gSoloudCalls["Soloud"]["countAudioSource"].p = call_Soloud_countAudioSource;
  gSoloudCalls["Soloud"]["countAudioSource"].info = "int Soloud_countAudioSource( \n    Soloud * aSoloud,\n    AudioSource * aSound)";
  gSoloudCalls["Soloud"]["setFilterParameter"].p = call_Soloud_setFilterParameter;
  gSoloudCalls["Soloud"]["setFilterParameter"].info = "void Soloud_setFilterParameter( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    unsigned int aFilterId,\n    unsigned int aAttributeId,\n    float aValue)";
  gSoloudCalls["Soloud"]["getFilterParameter"].p = call_Soloud_getFilterParameter;
  gSoloudCalls["Soloud"]["getFilterParameter"].info = "float Soloud_getFilterParameter( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    unsigned int aFilterId,\n    unsigned int aAttributeId)";
  gSoloudCalls["Soloud"]["fadeFilterParameter"].p = call_Soloud_fadeFilterParameter;
  gSoloudCalls["Soloud"]["fadeFilterParameter"].info = "void Soloud_fadeFilterParameter( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    unsigned int aFilterId,\n    unsigned int aAttributeId,\n    float aTo,\n    double aTime)";
  gSoloudCalls["Soloud"]["oscillateFilterParameter"].p = call_Soloud_oscillateFilterParameter;
  gSoloudCalls["Soloud"]["oscillateFilterParameter"].info = "void Soloud_oscillateFilterParameter( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    unsigned int aFilterId,\n    unsigned int aAttributeId,\n    float aFrom,\n    float aTo,\n    double aTime)";
  gSoloudCalls["Soloud"]["getStreamTime"].p = call_Soloud_getStreamTime;
  gSoloudCalls["Soloud"]["getStreamTime"].info = "double Soloud_getStreamTime( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["getStreamPosition"].p = call_Soloud_getStreamPosition;
  gSoloudCalls["Soloud"]["getStreamPosition"].info = "double Soloud_getStreamPosition( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["getPause"].p = call_Soloud_getPause;
  gSoloudCalls["Soloud"]["getPause"].info = "int Soloud_getPause( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["getVolume"].p = call_Soloud_getVolume;
  gSoloudCalls["Soloud"]["getVolume"].info = "float Soloud_getVolume( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["getOverallVolume"].p = call_Soloud_getOverallVolume;
  gSoloudCalls["Soloud"]["getOverallVolume"].info = "float Soloud_getOverallVolume( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["getPan"].p = call_Soloud_getPan;
  gSoloudCalls["Soloud"]["getPan"].info = "float Soloud_getPan( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["getSamplerate"].p = call_Soloud_getSamplerate;
  gSoloudCalls["Soloud"]["getSamplerate"].info = "float Soloud_getSamplerate( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["getProtectVoice"].p = call_Soloud_getProtectVoice;
  gSoloudCalls["Soloud"]["getProtectVoice"].info = "int Soloud_getProtectVoice( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["getActiveVoiceCount"].p = call_Soloud_getActiveVoiceCount;
  gSoloudCalls["Soloud"]["getActiveVoiceCount"].info = "unsigned int Soloud_getActiveVoiceCount( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["getVoiceCount"].p = call_Soloud_getVoiceCount;
  gSoloudCalls["Soloud"]["getVoiceCount"].info = "unsigned int Soloud_getVoiceCount( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["isValidVoiceHandle"].p = call_Soloud_isValidVoiceHandle;
  gSoloudCalls["Soloud"]["isValidVoiceHandle"].info = "int Soloud_isValidVoiceHandle( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["getRelativePlaySpeed"].p = call_Soloud_getRelativePlaySpeed;
  gSoloudCalls["Soloud"]["getRelativePlaySpeed"].info = "float Soloud_getRelativePlaySpeed( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["getPostClipScaler"].p = call_Soloud_getPostClipScaler;
  gSoloudCalls["Soloud"]["getPostClipScaler"].info = "float Soloud_getPostClipScaler( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["getMainResampler"].p = call_Soloud_getMainResampler;
  gSoloudCalls["Soloud"]["getMainResampler"].info = "unsigned int Soloud_getMainResampler( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["getGlobalVolume"].p = call_Soloud_getGlobalVolume;
  gSoloudCalls["Soloud"]["getGlobalVolume"].info = "float Soloud_getGlobalVolume( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["getMaxActiveVoiceCount"].p = call_Soloud_getMaxActiveVoiceCount;
  gSoloudCalls["Soloud"]["getMaxActiveVoiceCount"].info = "unsigned int Soloud_getMaxActiveVoiceCount( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["getLooping"].p = call_Soloud_getLooping;
  gSoloudCalls["Soloud"]["getLooping"].info = "int Soloud_getLooping( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["getAutoStop"].p = call_Soloud_getAutoStop;
  gSoloudCalls["Soloud"]["getAutoStop"].info = "int Soloud_getAutoStop( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["getLoopPoint"].p = call_Soloud_getLoopPoint;
  gSoloudCalls["Soloud"]["getLoopPoint"].info = "double Soloud_getLoopPoint( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["setLoopPoint"].p = call_Soloud_setLoopPoint;
  gSoloudCalls["Soloud"]["setLoopPoint"].info = "void Soloud_setLoopPoint( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    double aLoopPoint)";
  gSoloudCalls["Soloud"]["setLooping"].p = call_Soloud_setLooping;
  gSoloudCalls["Soloud"]["setLooping"].info = "void Soloud_setLooping( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    int aLooping)";
  gSoloudCalls["Soloud"]["setAutoStop"].p = call_Soloud_setAutoStop;
  gSoloudCalls["Soloud"]["setAutoStop"].info = "void Soloud_setAutoStop( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    int aAutoStop)";
  gSoloudCalls["Soloud"]["setMaxActiveVoiceCount"].p = call_Soloud_setMaxActiveVoiceCount;
  gSoloudCalls["Soloud"]["setMaxActiveVoiceCount"].info = "int Soloud_setMaxActiveVoiceCount( \n    Soloud * aSoloud,\n    unsigned int aVoiceCount)";
  gSoloudCalls["Soloud"]["setInaudibleBehavior"].p = call_Soloud_setInaudibleBehavior;
  gSoloudCalls["Soloud"]["setInaudibleBehavior"].info = "void Soloud_setInaudibleBehavior( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["Soloud"]["setGlobalVolume"].p = call_Soloud_setGlobalVolume;
  gSoloudCalls["Soloud"]["setGlobalVolume"].info = "void Soloud_setGlobalVolume( \n    Soloud * aSoloud,\n    float aVolume)";
  gSoloudCalls["Soloud"]["setPostClipScaler"].p = call_Soloud_setPostClipScaler;
  gSoloudCalls["Soloud"]["setPostClipScaler"].info = "void Soloud_setPostClipScaler( \n    Soloud * aSoloud,\n    float aScaler)";
  gSoloudCalls["Soloud"]["setMainResampler"].p = call_Soloud_setMainResampler;
  gSoloudCalls["Soloud"]["setMainResampler"].info = "void Soloud_setMainResampler( \n    Soloud * aSoloud,\n    unsigned int aResampler)";
  gSoloudCalls["Soloud"]["setPause"].p = call_Soloud_setPause;
  gSoloudCalls["Soloud"]["setPause"].info = "void Soloud_setPause( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    int aPause)";
  gSoloudCalls["Soloud"]["setPauseAll"].p = call_Soloud_setPauseAll;
  gSoloudCalls["Soloud"]["setPauseAll"].info = "void Soloud_setPauseAll( \n    Soloud * aSoloud,\n    int aPause)";
  gSoloudCalls["Soloud"]["setRelativePlaySpeed"].p = call_Soloud_setRelativePlaySpeed;
  gSoloudCalls["Soloud"]["setRelativePlaySpeed"].info = "int Soloud_setRelativePlaySpeed( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aSpeed)";
  gSoloudCalls["Soloud"]["setProtectVoice"].p = call_Soloud_setProtectVoice;
  gSoloudCalls["Soloud"]["setProtectVoice"].info = "void Soloud_setProtectVoice( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    int aProtect)";
  gSoloudCalls["Soloud"]["setSamplerate"].p = call_Soloud_setSamplerate;
  gSoloudCalls["Soloud"]["setSamplerate"].info = "void Soloud_setSamplerate( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aSamplerate)";
  gSoloudCalls["Soloud"]["setPan"].p = call_Soloud_setPan;
  gSoloudCalls["Soloud"]["setPan"].info = "void Soloud_setPan( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aPan)";
  gSoloudCalls["Soloud"]["setPanAbsolute"].p = call_Soloud_setPanAbsolute;
  gSoloudCalls["Soloud"]["setPanAbsolute"].info = "void Soloud_setPanAbsolute( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aLVolume,\n    float aRVolume)";
  gSoloudCalls["Soloud"]["setChannelVolume"].p = call_Soloud_setChannelVolume;
  gSoloudCalls["Soloud"]["setChannelVolume"].info = "void Soloud_setChannelVolume( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    unsigned int aChannel,\n    float aVolume)";
  gSoloudCalls["Soloud"]["setVolume"].p = call_Soloud_setVolume;
  gSoloudCalls["Soloud"]["setVolume"].info = "void Soloud_setVolume( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aVolume)";
  gSoloudCalls["Soloud"]["setDelaySamples"].p = call_Soloud_setDelaySamples;
  gSoloudCalls["Soloud"]["setDelaySamples"].info = "void Soloud_setDelaySamples( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    unsigned int aSamples)";
  gSoloudCalls["Soloud"]["fadeVolume"].p = call_Soloud_fadeVolume;
  gSoloudCalls["Soloud"]["fadeVolume"].info = "void Soloud_fadeVolume( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aTo,\n    double aTime)";
  gSoloudCalls["Soloud"]["fadePan"].p = call_Soloud_fadePan;
  gSoloudCalls["Soloud"]["fadePan"].info = "void Soloud_fadePan( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aTo,\n    double aTime)";
  gSoloudCalls["Soloud"]["fadeRelativePlaySpeed"].p = call_Soloud_fadeRelativePlaySpeed;
  gSoloudCalls["Soloud"]["fadeRelativePlaySpeed"].info = "void Soloud_fadeRelativePlaySpeed( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aTo,\n    double aTime)";
  gSoloudCalls["Soloud"]["fadeGlobalVolume"].p = call_Soloud_fadeGlobalVolume;
  gSoloudCalls["Soloud"]["fadeGlobalVolume"].info = "void Soloud_fadeGlobalVolume( \n    Soloud * aSoloud,\n    float aTo,\n    double aTime)";
  gSoloudCalls["Soloud"]["schedulePause"].p = call_Soloud_schedulePause;
  gSoloudCalls["Soloud"]["schedulePause"].info = "void Soloud_schedulePause( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    double aTime)";
  gSoloudCalls["Soloud"]["scheduleStop"].p = call_Soloud_scheduleStop;
  gSoloudCalls["Soloud"]["scheduleStop"].info = "void Soloud_scheduleStop( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    double aTime)";
  gSoloudCalls["Soloud"]["oscillateVolume"].p = call_Soloud_oscillateVolume;
  gSoloudCalls["Soloud"]["oscillateVolume"].info = "void Soloud_oscillateVolume( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aFrom,\n    float aTo,\n    double aTime)";
  gSoloudCalls["Soloud"]["oscillatePan"].p = call_Soloud_oscillatePan;
  gSoloudCalls["Soloud"]["oscillatePan"].info = "void Soloud_oscillatePan( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aFrom,\n    float aTo,\n    double aTime)";
  gSoloudCalls["Soloud"]["oscillateRelativePlaySpeed"].p = call_Soloud_oscillateRelativePlaySpeed;
  gSoloudCalls["Soloud"]["oscillateRelativePlaySpeed"].info = "void Soloud_oscillateRelativePlaySpeed( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aFrom,\n    float aTo,\n    double aTime)";
  gSoloudCalls["Soloud"]["oscillateGlobalVolume"].p = call_Soloud_oscillateGlobalVolume;
  gSoloudCalls["Soloud"]["oscillateGlobalVolume"].info = "void Soloud_oscillateGlobalVolume( \n    Soloud * aSoloud,\n    float aFrom,\n    float aTo,\n    double aTime)";
  gSoloudCalls["Soloud"]["setGlobalFilter"].p = call_Soloud_setGlobalFilter;
  gSoloudCalls["Soloud"]["setGlobalFilter"].info = "void Soloud_setGlobalFilter( \n    Soloud * aSoloud,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["Soloud"]["setVisualizationEnable"].p = call_Soloud_setVisualizationEnable;
  gSoloudCalls["Soloud"]["setVisualizationEnable"].info = "void Soloud_setVisualizationEnable( \n    Soloud * aSoloud,\n    int aEnable)";
  gSoloudCalls["Soloud"]["getApproximateVolume"].p = call_Soloud_getApproximateVolume;
  gSoloudCalls["Soloud"]["getApproximateVolume"].info = "float Soloud_getApproximateVolume( \n    Soloud * aSoloud,\n    unsigned int aChannel)";
  gSoloudCalls["Soloud"]["getLoopCount"].p = call_Soloud_getLoopCount;
  gSoloudCalls["Soloud"]["getLoopCount"].info = "unsigned int Soloud_getLoopCount( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["getInfo"].p = call_Soloud_getInfo;
  gSoloudCalls["Soloud"]["getInfo"].info = "float Soloud_getInfo( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    unsigned int aInfoKey)";
  gSoloudCalls["Soloud"]["createVoiceGroup"].p = call_Soloud_createVoiceGroup;
  gSoloudCalls["Soloud"]["createVoiceGroup"].info = "unsigned int Soloud_createVoiceGroup( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["destroyVoiceGroup"].p = call_Soloud_destroyVoiceGroup;
  gSoloudCalls["Soloud"]["destroyVoiceGroup"].info = "int Soloud_destroyVoiceGroup( \n    Soloud * aSoloud,\n    unsigned int aVoiceGroupHandle)";
  gSoloudCalls["Soloud"]["addVoiceToGroup"].p = call_Soloud_addVoiceToGroup;
  gSoloudCalls["Soloud"]["addVoiceToGroup"].info = "int Soloud_addVoiceToGroup( \n    Soloud * aSoloud,\n    unsigned int aVoiceGroupHandle,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Soloud"]["isVoiceGroup"].p = call_Soloud_isVoiceGroup;
  gSoloudCalls["Soloud"]["isVoiceGroup"].info = "int Soloud_isVoiceGroup( \n    Soloud * aSoloud,\n    unsigned int aVoiceGroupHandle)";
  gSoloudCalls["Soloud"]["isVoiceGroupEmpty"].p = call_Soloud_isVoiceGroupEmpty;
  gSoloudCalls["Soloud"]["isVoiceGroupEmpty"].info = "int Soloud_isVoiceGroupEmpty( \n    Soloud * aSoloud,\n    unsigned int aVoiceGroupHandle)";
  gSoloudCalls["Soloud"]["update3dAudio"].p = call_Soloud_update3dAudio;
  gSoloudCalls["Soloud"]["update3dAudio"].info = "void Soloud_update3dAudio( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["set3dSoundSpeed"].p = call_Soloud_set3dSoundSpeed;
  gSoloudCalls["Soloud"]["set3dSoundSpeed"].info = "int Soloud_set3dSoundSpeed( \n    Soloud * aSoloud,\n    float aSpeed)";
  gSoloudCalls["Soloud"]["get3dSoundSpeed"].p = call_Soloud_get3dSoundSpeed;
  gSoloudCalls["Soloud"]["get3dSoundSpeed"].info = "float Soloud_get3dSoundSpeed( \n    Soloud * aSoloud)";
  gSoloudCalls["Soloud"]["set3dListenerParameters"].p = call_Soloud_set3dListenerParameters;
  gSoloudCalls["Soloud"]["set3dListenerParameters"].info = "void Soloud_set3dListenerParameters( \n    Soloud * aSoloud,\n    float aPosX,\n    float aPosY,\n    float aPosZ,\n    float aAtX,\n    float aAtY,\n    float aAtZ,\n    float aUpX,\n    float aUpY,\n    float aUpZ)";
  gSoloudCalls["Soloud"]["set3dListenerParametersEx"].p = call_Soloud_set3dListenerParametersEx;
  gSoloudCalls["Soloud"]["set3dListenerParametersEx"].info = "void Soloud_set3dListenerParametersEx( \n    Soloud * aSoloud,\n    float aPosX,\n    float aPosY,\n    float aPosZ,\n    float aAtX,\n    float aAtY,\n    float aAtZ,\n    float aUpX,\n    float aUpY,\n    float aUpZ,\n    float aVelocityX,\n    float aVelocityY,\n    float aVelocityZ)";
  gSoloudCalls["Soloud"]["set3dListenerPosition"].p = call_Soloud_set3dListenerPosition;
  gSoloudCalls["Soloud"]["set3dListenerPosition"].info = "void Soloud_set3dListenerPosition( \n    Soloud * aSoloud,\n    float aPosX,\n    float aPosY,\n    float aPosZ)";
  gSoloudCalls["Soloud"]["set3dListenerAt"].p = call_Soloud_set3dListenerAt;
  gSoloudCalls["Soloud"]["set3dListenerAt"].info = "void Soloud_set3dListenerAt( \n    Soloud * aSoloud,\n    float aAtX,\n    float aAtY,\n    float aAtZ)";
  gSoloudCalls["Soloud"]["set3dListenerUp"].p = call_Soloud_set3dListenerUp;
  gSoloudCalls["Soloud"]["set3dListenerUp"].info = "void Soloud_set3dListenerUp( \n    Soloud * aSoloud,\n    float aUpX,\n    float aUpY,\n    float aUpZ)";
  gSoloudCalls["Soloud"]["set3dListenerVelocity"].p = call_Soloud_set3dListenerVelocity;
  gSoloudCalls["Soloud"]["set3dListenerVelocity"].info = "void Soloud_set3dListenerVelocity( \n    Soloud * aSoloud,\n    float aVelocityX,\n    float aVelocityY,\n    float aVelocityZ)";
  gSoloudCalls["Soloud"]["set3dSourceParameters"].p = call_Soloud_set3dSourceParameters;
  gSoloudCalls["Soloud"]["set3dSourceParameters"].info = "void Soloud_set3dSourceParameters( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aPosX,\n    float aPosY,\n    float aPosZ)";
  gSoloudCalls["Soloud"]["set3dSourceParametersEx"].p = call_Soloud_set3dSourceParametersEx;
  gSoloudCalls["Soloud"]["set3dSourceParametersEx"].info = "void Soloud_set3dSourceParametersEx( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aPosX,\n    float aPosY,\n    float aPosZ,\n    float aVelocityX,\n    float aVelocityY,\n    float aVelocityZ)";
  gSoloudCalls["Soloud"]["set3dSourcePosition"].p = call_Soloud_set3dSourcePosition;
  gSoloudCalls["Soloud"]["set3dSourcePosition"].info = "void Soloud_set3dSourcePosition( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aPosX,\n    float aPosY,\n    float aPosZ)";
  gSoloudCalls["Soloud"]["set3dSourceVelocity"].p = call_Soloud_set3dSourceVelocity;
  gSoloudCalls["Soloud"]["set3dSourceVelocity"].info = "void Soloud_set3dSourceVelocity( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aVelocityX,\n    float aVelocityY,\n    float aVelocityZ)";
  gSoloudCalls["Soloud"]["set3dSourceMinMaxDistance"].p = call_Soloud_set3dSourceMinMaxDistance;
  gSoloudCalls["Soloud"]["set3dSourceMinMaxDistance"].info = "void Soloud_set3dSourceMinMaxDistance( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["Soloud"]["set3dSourceAttenuation"].p = call_Soloud_set3dSourceAttenuation;
  gSoloudCalls["Soloud"]["set3dSourceAttenuation"].info = "void Soloud_set3dSourceAttenuation( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["Soloud"]["set3dSourceDopplerFactor"].p = call_Soloud_set3dSourceDopplerFactor;
  gSoloudCalls["Soloud"]["set3dSourceDopplerFactor"].info = "void Soloud_set3dSourceDopplerFactor( \n    Soloud * aSoloud,\n    unsigned int aVoiceHandle,\n    float aDopplerFactor)";
  gSoloudCalls["Soloud"]["mix"].p = call_Soloud_mix;
  gSoloudCalls["Soloud"]["mix"].info = "void Soloud_mix( \n    Soloud * aSoloud,\n    float * aBuffer,\n    unsigned int aSamples)";
  gSoloudCalls["Soloud"]["mixSigned16"].p = call_Soloud_mixSigned16;
  gSoloudCalls["Soloud"]["mixSigned16"].info = "void Soloud_mixSigned16( \n    Soloud * aSoloud,\n    short * aBuffer,\n    unsigned int aSamples)";
  gSoloudCalls["Ay"]["destroy"].p = call_Ay_destroy;
  gSoloudCalls["Ay"]["destroy"].info = "void Ay_destroy( \n    Ay * aAy)";
  gSoloudCalls["Ay"]["create"].p = call_Ay_create;
  gSoloudCalls["Ay"]["create"].info = "Ay * Ay_create()";
  gSoloudCalls["Ay"]["setVolume"].p = call_Ay_setVolume;
  gSoloudCalls["Ay"]["setVolume"].info = "void Ay_setVolume( \n    Ay * aAy,\n    float aVolume)";
  gSoloudCalls["Ay"]["setLooping"].p = call_Ay_setLooping;
  gSoloudCalls["Ay"]["setLooping"].info = "void Ay_setLooping( \n    Ay * aAy,\n    int aLoop)";
  gSoloudCalls["Ay"]["setAutoStop"].p = call_Ay_setAutoStop;
  gSoloudCalls["Ay"]["setAutoStop"].info = "void Ay_setAutoStop( \n    Ay * aAy,\n    int aAutoStop)";
  gSoloudCalls["Ay"]["set3dMinMaxDistance"].p = call_Ay_set3dMinMaxDistance;
  gSoloudCalls["Ay"]["set3dMinMaxDistance"].info = "void Ay_set3dMinMaxDistance( \n    Ay * aAy,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["Ay"]["set3dAttenuation"].p = call_Ay_set3dAttenuation;
  gSoloudCalls["Ay"]["set3dAttenuation"].info = "void Ay_set3dAttenuation( \n    Ay * aAy,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["Ay"]["set3dDopplerFactor"].p = call_Ay_set3dDopplerFactor;
  gSoloudCalls["Ay"]["set3dDopplerFactor"].info = "void Ay_set3dDopplerFactor( \n    Ay * aAy,\n    float aDopplerFactor)";
  gSoloudCalls["Ay"]["set3dListenerRelative"].p = call_Ay_set3dListenerRelative;
  gSoloudCalls["Ay"]["set3dListenerRelative"].info = "void Ay_set3dListenerRelative( \n    Ay * aAy,\n    int aListenerRelative)";
  gSoloudCalls["Ay"]["set3dDistanceDelay"].p = call_Ay_set3dDistanceDelay;
  gSoloudCalls["Ay"]["set3dDistanceDelay"].info = "void Ay_set3dDistanceDelay( \n    Ay * aAy,\n    int aDistanceDelay)";
  gSoloudCalls["Ay"]["set3dCollider"].p = call_Ay_set3dCollider;
  gSoloudCalls["Ay"]["set3dCollider"].info = "void Ay_set3dCollider( \n    Ay * aAy,\n    AudioCollider * aCollider)";
  gSoloudCalls["Ay"]["set3dColliderEx"].p = call_Ay_set3dColliderEx;
  gSoloudCalls["Ay"]["set3dColliderEx"].info = "void Ay_set3dColliderEx( \n    Ay * aAy,\n    AudioCollider * aCollider,\n    int aUserData)";
  gSoloudCalls["Ay"]["set3dAttenuator"].p = call_Ay_set3dAttenuator;
  gSoloudCalls["Ay"]["set3dAttenuator"].info = "void Ay_set3dAttenuator( \n    Ay * aAy,\n    AudioAttenuator * aAttenuator)";
  gSoloudCalls["Ay"]["setInaudibleBehavior"].p = call_Ay_setInaudibleBehavior;
  gSoloudCalls["Ay"]["setInaudibleBehavior"].info = "void Ay_setInaudibleBehavior( \n    Ay * aAy,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["Ay"]["setLoopPoint"].p = call_Ay_setLoopPoint;
  gSoloudCalls["Ay"]["setLoopPoint"].info = "void Ay_setLoopPoint( \n    Ay * aAy,\n    double aLoopPoint)";
  gSoloudCalls["Ay"]["getLoopPoint"].p = call_Ay_getLoopPoint;
  gSoloudCalls["Ay"]["getLoopPoint"].info = "double Ay_getLoopPoint( \n    Ay * aAy)";
  gSoloudCalls["Ay"]["setFilter"].p = call_Ay_setFilter;
  gSoloudCalls["Ay"]["setFilter"].info = "void Ay_setFilter( \n    Ay * aAy,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["Ay"]["stop"].p = call_Ay_stop;
  gSoloudCalls["Ay"]["stop"].info = "void Ay_stop( \n    Ay * aAy)";
  gSoloudCalls["BassboostFilter"]["destroy"].p = call_BassboostFilter_destroy;
  gSoloudCalls["BassboostFilter"]["destroy"].info = "void BassboostFilter_destroy( \n    BassboostFilter * aBassboostFilter)";
  gSoloudCalls["BassboostFilter"]["getParamCount"].p = call_BassboostFilter_getParamCount;
  gSoloudCalls["BassboostFilter"]["getParamCount"].info = "int BassboostFilter_getParamCount( \n    BassboostFilter * aBassboostFilter)";
  gSoloudCalls["BassboostFilter"]["getParamType"].p = call_BassboostFilter_getParamType;
  gSoloudCalls["BassboostFilter"]["getParamType"].info = "unsigned int BassboostFilter_getParamType( \n    BassboostFilter * aBassboostFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["BassboostFilter"]["getParamMax"].p = call_BassboostFilter_getParamMax;
  gSoloudCalls["BassboostFilter"]["getParamMax"].info = "float BassboostFilter_getParamMax( \n    BassboostFilter * aBassboostFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["BassboostFilter"]["getParamMin"].p = call_BassboostFilter_getParamMin;
  gSoloudCalls["BassboostFilter"]["getParamMin"].info = "float BassboostFilter_getParamMin( \n    BassboostFilter * aBassboostFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["BassboostFilter"]["setParams"].p = call_BassboostFilter_setParams;
  gSoloudCalls["BassboostFilter"]["setParams"].info = "int BassboostFilter_setParams( \n    BassboostFilter * aBassboostFilter,\n    float aBoost)";
  gSoloudCalls["BassboostFilter"]["create"].p = call_BassboostFilter_create;
  gSoloudCalls["BassboostFilter"]["create"].info = "BassboostFilter * BassboostFilter_create()";
  gSoloudCalls["BiquadResonantFilter"]["destroy"].p = call_BiquadResonantFilter_destroy;
  gSoloudCalls["BiquadResonantFilter"]["destroy"].info = "void BiquadResonantFilter_destroy( \n    BiquadResonantFilter * aBiquadResonantFilter)";
  gSoloudCalls["BiquadResonantFilter"]["getParamCount"].p = call_BiquadResonantFilter_getParamCount;
  gSoloudCalls["BiquadResonantFilter"]["getParamCount"].info = "int BiquadResonantFilter_getParamCount( \n    BiquadResonantFilter * aBiquadResonantFilter)";
  gSoloudCalls["BiquadResonantFilter"]["getParamType"].p = call_BiquadResonantFilter_getParamType;
  gSoloudCalls["BiquadResonantFilter"]["getParamType"].info = "unsigned int BiquadResonantFilter_getParamType( \n    BiquadResonantFilter * aBiquadResonantFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["BiquadResonantFilter"]["getParamMax"].p = call_BiquadResonantFilter_getParamMax;
  gSoloudCalls["BiquadResonantFilter"]["getParamMax"].info = "float BiquadResonantFilter_getParamMax( \n    BiquadResonantFilter * aBiquadResonantFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["BiquadResonantFilter"]["getParamMin"].p = call_BiquadResonantFilter_getParamMin;
  gSoloudCalls["BiquadResonantFilter"]["getParamMin"].info = "float BiquadResonantFilter_getParamMin( \n    BiquadResonantFilter * aBiquadResonantFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["BiquadResonantFilter"]["create"].p = call_BiquadResonantFilter_create;
  gSoloudCalls["BiquadResonantFilter"]["create"].info = "BiquadResonantFilter * BiquadResonantFilter_create()";
  gSoloudCalls["BiquadResonantFilter"]["setParams"].p = call_BiquadResonantFilter_setParams;
  gSoloudCalls["BiquadResonantFilter"]["setParams"].info = "int BiquadResonantFilter_setParams( \n    BiquadResonantFilter * aBiquadResonantFilter,\n    int aType,\n    float aFrequency,\n    float aResonance)";
  gSoloudCalls["Bus"]["destroy"].p = call_Bus_destroy;
  gSoloudCalls["Bus"]["destroy"].info = "void Bus_destroy( \n    Bus * aBus)";
  gSoloudCalls["Bus"]["create"].p = call_Bus_create;
  gSoloudCalls["Bus"]["create"].info = "Bus * Bus_create()";
  gSoloudCalls["Bus"]["setFilter"].p = call_Bus_setFilter;
  gSoloudCalls["Bus"]["setFilter"].info = "void Bus_setFilter( \n    Bus * aBus,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["Bus"]["play"].p = call_Bus_play;
  gSoloudCalls["Bus"]["play"].info = "unsigned int Bus_play( \n    Bus * aBus,\n    AudioSource * aSound)";
  gSoloudCalls["Bus"]["playEx"].p = call_Bus_playEx;
  gSoloudCalls["Bus"]["playEx"].info = "unsigned int Bus_playEx( \n    Bus * aBus,\n    AudioSource * aSound,\n    float aVolume,\n    float aPan,\n    int aPaused)";
  gSoloudCalls["Bus"]["playClocked"].p = call_Bus_playClocked;
  gSoloudCalls["Bus"]["playClocked"].info = "unsigned int Bus_playClocked( \n    Bus * aBus,\n    double aSoundTime,\n    AudioSource * aSound)";
  gSoloudCalls["Bus"]["playClockedEx"].p = call_Bus_playClockedEx;
  gSoloudCalls["Bus"]["playClockedEx"].info = "unsigned int Bus_playClockedEx( \n    Bus * aBus,\n    double aSoundTime,\n    AudioSource * aSound,\n    float aVolume,\n    float aPan)";
  gSoloudCalls["Bus"]["play3d"].p = call_Bus_play3d;
  gSoloudCalls["Bus"]["play3d"].info = "unsigned int Bus_play3d( \n    Bus * aBus,\n    AudioSource * aSound,\n    float aPosX,\n    float aPosY,\n    float aPosZ)";
  gSoloudCalls["Bus"]["play3dEx"].p = call_Bus_play3dEx;
  gSoloudCalls["Bus"]["play3dEx"].info = "unsigned int Bus_play3dEx( \n    Bus * aBus,\n    AudioSource * aSound,\n    float aPosX,\n    float aPosY,\n    float aPosZ,\n    float aVelX,\n    float aVelY,\n    float aVelZ,\n    float aVolume,\n    int aPaused)";
  gSoloudCalls["Bus"]["play3dClocked"].p = call_Bus_play3dClocked;
  gSoloudCalls["Bus"]["play3dClocked"].info = "unsigned int Bus_play3dClocked( \n    Bus * aBus,\n    double aSoundTime,\n    AudioSource * aSound,\n    float aPosX,\n    float aPosY,\n    float aPosZ)";
  gSoloudCalls["Bus"]["play3dClockedEx"].p = call_Bus_play3dClockedEx;
  gSoloudCalls["Bus"]["play3dClockedEx"].info = "unsigned int Bus_play3dClockedEx( \n    Bus * aBus,\n    double aSoundTime,\n    AudioSource * aSound,\n    float aPosX,\n    float aPosY,\n    float aPosZ,\n    float aVelX,\n    float aVelY,\n    float aVelZ,\n    float aVolume)";
  gSoloudCalls["Bus"]["setChannels"].p = call_Bus_setChannels;
  gSoloudCalls["Bus"]["setChannels"].info = "int Bus_setChannels( \n    Bus * aBus,\n    unsigned int aChannels)";
  gSoloudCalls["Bus"]["setVisualizationEnable"].p = call_Bus_setVisualizationEnable;
  gSoloudCalls["Bus"]["setVisualizationEnable"].info = "void Bus_setVisualizationEnable( \n    Bus * aBus,\n    int aEnable)";
  gSoloudCalls["Bus"]["annexSound"].p = call_Bus_annexSound;
  gSoloudCalls["Bus"]["annexSound"].info = "void Bus_annexSound( \n    Bus * aBus,\n    unsigned int aVoiceHandle)";
  gSoloudCalls["Bus"]["getApproximateVolume"].p = call_Bus_getApproximateVolume;
  gSoloudCalls["Bus"]["getApproximateVolume"].info = "float Bus_getApproximateVolume( \n    Bus * aBus,\n    unsigned int aChannel)";
  gSoloudCalls["Bus"]["getActiveVoiceCount"].p = call_Bus_getActiveVoiceCount;
  gSoloudCalls["Bus"]["getActiveVoiceCount"].info = "unsigned int Bus_getActiveVoiceCount( \n    Bus * aBus)";
  gSoloudCalls["Bus"]["getResampler"].p = call_Bus_getResampler;
  gSoloudCalls["Bus"]["getResampler"].info = "unsigned int Bus_getResampler( \n    Bus * aBus)";
  gSoloudCalls["Bus"]["setResampler"].p = call_Bus_setResampler;
  gSoloudCalls["Bus"]["setResampler"].info = "void Bus_setResampler( \n    Bus * aBus,\n    unsigned int aResampler)";
  gSoloudCalls["Bus"]["setVolume"].p = call_Bus_setVolume;
  gSoloudCalls["Bus"]["setVolume"].info = "void Bus_setVolume( \n    Bus * aBus,\n    float aVolume)";
  gSoloudCalls["Bus"]["setLooping"].p = call_Bus_setLooping;
  gSoloudCalls["Bus"]["setLooping"].info = "void Bus_setLooping( \n    Bus * aBus,\n    int aLoop)";
  gSoloudCalls["Bus"]["setAutoStop"].p = call_Bus_setAutoStop;
  gSoloudCalls["Bus"]["setAutoStop"].info = "void Bus_setAutoStop( \n    Bus * aBus,\n    int aAutoStop)";
  gSoloudCalls["Bus"]["set3dMinMaxDistance"].p = call_Bus_set3dMinMaxDistance;
  gSoloudCalls["Bus"]["set3dMinMaxDistance"].info = "void Bus_set3dMinMaxDistance( \n    Bus * aBus,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["Bus"]["set3dAttenuation"].p = call_Bus_set3dAttenuation;
  gSoloudCalls["Bus"]["set3dAttenuation"].info = "void Bus_set3dAttenuation( \n    Bus * aBus,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["Bus"]["set3dDopplerFactor"].p = call_Bus_set3dDopplerFactor;
  gSoloudCalls["Bus"]["set3dDopplerFactor"].info = "void Bus_set3dDopplerFactor( \n    Bus * aBus,\n    float aDopplerFactor)";
  gSoloudCalls["Bus"]["set3dListenerRelative"].p = call_Bus_set3dListenerRelative;
  gSoloudCalls["Bus"]["set3dListenerRelative"].info = "void Bus_set3dListenerRelative( \n    Bus * aBus,\n    int aListenerRelative)";
  gSoloudCalls["Bus"]["set3dDistanceDelay"].p = call_Bus_set3dDistanceDelay;
  gSoloudCalls["Bus"]["set3dDistanceDelay"].info = "void Bus_set3dDistanceDelay( \n    Bus * aBus,\n    int aDistanceDelay)";
  gSoloudCalls["Bus"]["set3dCollider"].p = call_Bus_set3dCollider;
  gSoloudCalls["Bus"]["set3dCollider"].info = "void Bus_set3dCollider( \n    Bus * aBus,\n    AudioCollider * aCollider)";
  gSoloudCalls["Bus"]["set3dColliderEx"].p = call_Bus_set3dColliderEx;
  gSoloudCalls["Bus"]["set3dColliderEx"].info = "void Bus_set3dColliderEx( \n    Bus * aBus,\n    AudioCollider * aCollider,\n    int aUserData)";
  gSoloudCalls["Bus"]["set3dAttenuator"].p = call_Bus_set3dAttenuator;
  gSoloudCalls["Bus"]["set3dAttenuator"].info = "void Bus_set3dAttenuator( \n    Bus * aBus,\n    AudioAttenuator * aAttenuator)";
  gSoloudCalls["Bus"]["setInaudibleBehavior"].p = call_Bus_setInaudibleBehavior;
  gSoloudCalls["Bus"]["setInaudibleBehavior"].info = "void Bus_setInaudibleBehavior( \n    Bus * aBus,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["Bus"]["setLoopPoint"].p = call_Bus_setLoopPoint;
  gSoloudCalls["Bus"]["setLoopPoint"].info = "void Bus_setLoopPoint( \n    Bus * aBus,\n    double aLoopPoint)";
  gSoloudCalls["Bus"]["getLoopPoint"].p = call_Bus_getLoopPoint;
  gSoloudCalls["Bus"]["getLoopPoint"].info = "double Bus_getLoopPoint( \n    Bus * aBus)";
  gSoloudCalls["Bus"]["stop"].p = call_Bus_stop;
  gSoloudCalls["Bus"]["stop"].info = "void Bus_stop( \n    Bus * aBus)";
  gSoloudCalls["DCRemovalFilter"]["destroy"].p = call_DCRemovalFilter_destroy;
  gSoloudCalls["DCRemovalFilter"]["destroy"].info = "void DCRemovalFilter_destroy( \n    DCRemovalFilter * aDCRemovalFilter)";
  gSoloudCalls["DCRemovalFilter"]["create"].p = call_DCRemovalFilter_create;
  gSoloudCalls["DCRemovalFilter"]["create"].info = "DCRemovalFilter * DCRemovalFilter_create()";
  gSoloudCalls["DCRemovalFilter"]["setParams"].p = call_DCRemovalFilter_setParams;
  gSoloudCalls["DCRemovalFilter"]["setParams"].info = "int DCRemovalFilter_setParams( \n    DCRemovalFilter * aDCRemovalFilter)";
  gSoloudCalls["DCRemovalFilter"]["setParamsEx"].p = call_DCRemovalFilter_setParamsEx;
  gSoloudCalls["DCRemovalFilter"]["setParamsEx"].info = "int DCRemovalFilter_setParamsEx( \n    DCRemovalFilter * aDCRemovalFilter,\n    float aLength)";
  gSoloudCalls["DCRemovalFilter"]["getParamCount"].p = call_DCRemovalFilter_getParamCount;
  gSoloudCalls["DCRemovalFilter"]["getParamCount"].info = "int DCRemovalFilter_getParamCount( \n    DCRemovalFilter * aDCRemovalFilter)";
  gSoloudCalls["DCRemovalFilter"]["getParamType"].p = call_DCRemovalFilter_getParamType;
  gSoloudCalls["DCRemovalFilter"]["getParamType"].info = "unsigned int DCRemovalFilter_getParamType( \n    DCRemovalFilter * aDCRemovalFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["DCRemovalFilter"]["getParamMax"].p = call_DCRemovalFilter_getParamMax;
  gSoloudCalls["DCRemovalFilter"]["getParamMax"].info = "float DCRemovalFilter_getParamMax( \n    DCRemovalFilter * aDCRemovalFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["DCRemovalFilter"]["getParamMin"].p = call_DCRemovalFilter_getParamMin;
  gSoloudCalls["DCRemovalFilter"]["getParamMin"].info = "float DCRemovalFilter_getParamMin( \n    DCRemovalFilter * aDCRemovalFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["EchoFilter"]["destroy"].p = call_EchoFilter_destroy;
  gSoloudCalls["EchoFilter"]["destroy"].info = "void EchoFilter_destroy( \n    EchoFilter * aEchoFilter)";
  gSoloudCalls["EchoFilter"]["getParamCount"].p = call_EchoFilter_getParamCount;
  gSoloudCalls["EchoFilter"]["getParamCount"].info = "int EchoFilter_getParamCount( \n    EchoFilter * aEchoFilter)";
  gSoloudCalls["EchoFilter"]["getParamType"].p = call_EchoFilter_getParamType;
  gSoloudCalls["EchoFilter"]["getParamType"].info = "unsigned int EchoFilter_getParamType( \n    EchoFilter * aEchoFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["EchoFilter"]["getParamMax"].p = call_EchoFilter_getParamMax;
  gSoloudCalls["EchoFilter"]["getParamMax"].info = "float EchoFilter_getParamMax( \n    EchoFilter * aEchoFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["EchoFilter"]["getParamMin"].p = call_EchoFilter_getParamMin;
  gSoloudCalls["EchoFilter"]["getParamMin"].info = "float EchoFilter_getParamMin( \n    EchoFilter * aEchoFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["EchoFilter"]["create"].p = call_EchoFilter_create;
  gSoloudCalls["EchoFilter"]["create"].info = "EchoFilter * EchoFilter_create()";
  gSoloudCalls["EchoFilter"]["setParams"].p = call_EchoFilter_setParams;
  gSoloudCalls["EchoFilter"]["setParams"].info = "int EchoFilter_setParams( \n    EchoFilter * aEchoFilter,\n    float aDelay)";
  gSoloudCalls["EchoFilter"]["setParamsEx"].p = call_EchoFilter_setParamsEx;
  gSoloudCalls["EchoFilter"]["setParamsEx"].info = "int EchoFilter_setParamsEx( \n    EchoFilter * aEchoFilter,\n    float aDelay,\n    float aDecay,\n    float aFilter)";
  gSoloudCalls["FFTFilter"]["destroy"].p = call_FFTFilter_destroy;
  gSoloudCalls["FFTFilter"]["destroy"].info = "void FFTFilter_destroy( \n    FFTFilter * aFFTFilter)";
  gSoloudCalls["FFTFilter"]["create"].p = call_FFTFilter_create;
  gSoloudCalls["FFTFilter"]["create"].info = "FFTFilter * FFTFilter_create()";
  gSoloudCalls["FFTFilter"]["getParamCount"].p = call_FFTFilter_getParamCount;
  gSoloudCalls["FFTFilter"]["getParamCount"].info = "int FFTFilter_getParamCount( \n    FFTFilter * aFFTFilter)";
  gSoloudCalls["FFTFilter"]["getParamType"].p = call_FFTFilter_getParamType;
  gSoloudCalls["FFTFilter"]["getParamType"].info = "unsigned int FFTFilter_getParamType( \n    FFTFilter * aFFTFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["FFTFilter"]["getParamMax"].p = call_FFTFilter_getParamMax;
  gSoloudCalls["FFTFilter"]["getParamMax"].info = "float FFTFilter_getParamMax( \n    FFTFilter * aFFTFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["FFTFilter"]["getParamMin"].p = call_FFTFilter_getParamMin;
  gSoloudCalls["FFTFilter"]["getParamMin"].info = "float FFTFilter_getParamMin( \n    FFTFilter * aFFTFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["FlangerFilter"]["destroy"].p = call_FlangerFilter_destroy;
  gSoloudCalls["FlangerFilter"]["destroy"].info = "void FlangerFilter_destroy( \n    FlangerFilter * aFlangerFilter)";
  gSoloudCalls["FlangerFilter"]["getParamCount"].p = call_FlangerFilter_getParamCount;
  gSoloudCalls["FlangerFilter"]["getParamCount"].info = "int FlangerFilter_getParamCount( \n    FlangerFilter * aFlangerFilter)";
  gSoloudCalls["FlangerFilter"]["getParamType"].p = call_FlangerFilter_getParamType;
  gSoloudCalls["FlangerFilter"]["getParamType"].info = "unsigned int FlangerFilter_getParamType( \n    FlangerFilter * aFlangerFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["FlangerFilter"]["getParamMax"].p = call_FlangerFilter_getParamMax;
  gSoloudCalls["FlangerFilter"]["getParamMax"].info = "float FlangerFilter_getParamMax( \n    FlangerFilter * aFlangerFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["FlangerFilter"]["getParamMin"].p = call_FlangerFilter_getParamMin;
  gSoloudCalls["FlangerFilter"]["getParamMin"].info = "float FlangerFilter_getParamMin( \n    FlangerFilter * aFlangerFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["FlangerFilter"]["create"].p = call_FlangerFilter_create;
  gSoloudCalls["FlangerFilter"]["create"].info = "FlangerFilter * FlangerFilter_create()";
  gSoloudCalls["FlangerFilter"]["setParams"].p = call_FlangerFilter_setParams;
  gSoloudCalls["FlangerFilter"]["setParams"].info = "int FlangerFilter_setParams( \n    FlangerFilter * aFlangerFilter,\n    float aDelay,\n    float aFreq)";
  gSoloudCalls["FreeverbFilter"]["destroy"].p = call_FreeverbFilter_destroy;
  gSoloudCalls["FreeverbFilter"]["destroy"].info = "void FreeverbFilter_destroy( \n    FreeverbFilter * aFreeverbFilter)";
  gSoloudCalls["FreeverbFilter"]["getParamCount"].p = call_FreeverbFilter_getParamCount;
  gSoloudCalls["FreeverbFilter"]["getParamCount"].info = "int FreeverbFilter_getParamCount( \n    FreeverbFilter * aFreeverbFilter)";
  gSoloudCalls["FreeverbFilter"]["getParamType"].p = call_FreeverbFilter_getParamType;
  gSoloudCalls["FreeverbFilter"]["getParamType"].info = "unsigned int FreeverbFilter_getParamType( \n    FreeverbFilter * aFreeverbFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["FreeverbFilter"]["getParamMax"].p = call_FreeverbFilter_getParamMax;
  gSoloudCalls["FreeverbFilter"]["getParamMax"].info = "float FreeverbFilter_getParamMax( \n    FreeverbFilter * aFreeverbFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["FreeverbFilter"]["getParamMin"].p = call_FreeverbFilter_getParamMin;
  gSoloudCalls["FreeverbFilter"]["getParamMin"].info = "float FreeverbFilter_getParamMin( \n    FreeverbFilter * aFreeverbFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["FreeverbFilter"]["create"].p = call_FreeverbFilter_create;
  gSoloudCalls["FreeverbFilter"]["create"].info = "FreeverbFilter * FreeverbFilter_create()";
  gSoloudCalls["FreeverbFilter"]["setParams"].p = call_FreeverbFilter_setParams;
  gSoloudCalls["FreeverbFilter"]["setParams"].info = "int FreeverbFilter_setParams( \n    FreeverbFilter * aFreeverbFilter,\n    float aMode,\n    float aRoomSize,\n    float aDamp,\n    float aWidth)";
  gSoloudCalls["LofiFilter"]["destroy"].p = call_LofiFilter_destroy;
  gSoloudCalls["LofiFilter"]["destroy"].info = "void LofiFilter_destroy( \n    LofiFilter * aLofiFilter)";
  gSoloudCalls["LofiFilter"]["getParamCount"].p = call_LofiFilter_getParamCount;
  gSoloudCalls["LofiFilter"]["getParamCount"].info = "int LofiFilter_getParamCount( \n    LofiFilter * aLofiFilter)";
  gSoloudCalls["LofiFilter"]["getParamType"].p = call_LofiFilter_getParamType;
  gSoloudCalls["LofiFilter"]["getParamType"].info = "unsigned int LofiFilter_getParamType( \n    LofiFilter * aLofiFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["LofiFilter"]["getParamMax"].p = call_LofiFilter_getParamMax;
  gSoloudCalls["LofiFilter"]["getParamMax"].info = "float LofiFilter_getParamMax( \n    LofiFilter * aLofiFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["LofiFilter"]["getParamMin"].p = call_LofiFilter_getParamMin;
  gSoloudCalls["LofiFilter"]["getParamMin"].info = "float LofiFilter_getParamMin( \n    LofiFilter * aLofiFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["LofiFilter"]["create"].p = call_LofiFilter_create;
  gSoloudCalls["LofiFilter"]["create"].info = "LofiFilter * LofiFilter_create()";
  gSoloudCalls["LofiFilter"]["setParams"].p = call_LofiFilter_setParams;
  gSoloudCalls["LofiFilter"]["setParams"].info = "int LofiFilter_setParams( \n    LofiFilter * aLofiFilter,\n    float aSampleRate,\n    float aBitdepth)";
  gSoloudCalls["Monotone"]["destroy"].p = call_Monotone_destroy;
  gSoloudCalls["Monotone"]["destroy"].info = "void Monotone_destroy( \n    Monotone * aMonotone)";
  gSoloudCalls["Monotone"]["create"].p = call_Monotone_create;
  gSoloudCalls["Monotone"]["create"].info = "Monotone * Monotone_create()";
  gSoloudCalls["Monotone"]["setParams"].p = call_Monotone_setParams;
  gSoloudCalls["Monotone"]["setParams"].info = "int Monotone_setParams( \n    Monotone * aMonotone,\n    int aHardwareChannels)";
  gSoloudCalls["Monotone"]["setParamsEx"].p = call_Monotone_setParamsEx;
  gSoloudCalls["Monotone"]["setParamsEx"].info = "int Monotone_setParamsEx( \n    Monotone * aMonotone,\n    int aHardwareChannels,\n    int aWaveform)";
  gSoloudCalls["Monotone"]["load"].p = call_Monotone_load;
  gSoloudCalls["Monotone"]["load"].info = "int Monotone_load( \n    Monotone * aMonotone,\n    const char * aFilename)";
  gSoloudCalls["Monotone"]["loadMem"].p = call_Monotone_loadMem;
  gSoloudCalls["Monotone"]["loadMem"].info = "int Monotone_loadMem( \n    Monotone * aMonotone,\n    const unsigned char * aMem,\n    unsigned int aLength)";
  gSoloudCalls["Monotone"]["loadMemEx"].p = call_Monotone_loadMemEx;
  gSoloudCalls["Monotone"]["loadMemEx"].info = "int Monotone_loadMemEx( \n    Monotone * aMonotone,\n    const unsigned char * aMem,\n    unsigned int aLength,\n    int aCopy,\n    int aTakeOwnership)";
  gSoloudCalls["Monotone"]["loadFile"].p = call_Monotone_loadFile;
  gSoloudCalls["Monotone"]["loadFile"].info = "int Monotone_loadFile( \n    Monotone * aMonotone,\n    File * aFile)";
  gSoloudCalls["Monotone"]["setVolume"].p = call_Monotone_setVolume;
  gSoloudCalls["Monotone"]["setVolume"].info = "void Monotone_setVolume( \n    Monotone * aMonotone,\n    float aVolume)";
  gSoloudCalls["Monotone"]["setLooping"].p = call_Monotone_setLooping;
  gSoloudCalls["Monotone"]["setLooping"].info = "void Monotone_setLooping( \n    Monotone * aMonotone,\n    int aLoop)";
  gSoloudCalls["Monotone"]["setAutoStop"].p = call_Monotone_setAutoStop;
  gSoloudCalls["Monotone"]["setAutoStop"].info = "void Monotone_setAutoStop( \n    Monotone * aMonotone,\n    int aAutoStop)";
  gSoloudCalls["Monotone"]["set3dMinMaxDistance"].p = call_Monotone_set3dMinMaxDistance;
  gSoloudCalls["Monotone"]["set3dMinMaxDistance"].info = "void Monotone_set3dMinMaxDistance( \n    Monotone * aMonotone,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["Monotone"]["set3dAttenuation"].p = call_Monotone_set3dAttenuation;
  gSoloudCalls["Monotone"]["set3dAttenuation"].info = "void Monotone_set3dAttenuation( \n    Monotone * aMonotone,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["Monotone"]["set3dDopplerFactor"].p = call_Monotone_set3dDopplerFactor;
  gSoloudCalls["Monotone"]["set3dDopplerFactor"].info = "void Monotone_set3dDopplerFactor( \n    Monotone * aMonotone,\n    float aDopplerFactor)";
  gSoloudCalls["Monotone"]["set3dListenerRelative"].p = call_Monotone_set3dListenerRelative;
  gSoloudCalls["Monotone"]["set3dListenerRelative"].info = "void Monotone_set3dListenerRelative( \n    Monotone * aMonotone,\n    int aListenerRelative)";
  gSoloudCalls["Monotone"]["set3dDistanceDelay"].p = call_Monotone_set3dDistanceDelay;
  gSoloudCalls["Monotone"]["set3dDistanceDelay"].info = "void Monotone_set3dDistanceDelay( \n    Monotone * aMonotone,\n    int aDistanceDelay)";
  gSoloudCalls["Monotone"]["set3dCollider"].p = call_Monotone_set3dCollider;
  gSoloudCalls["Monotone"]["set3dCollider"].info = "void Monotone_set3dCollider( \n    Monotone * aMonotone,\n    AudioCollider * aCollider)";
  gSoloudCalls["Monotone"]["set3dColliderEx"].p = call_Monotone_set3dColliderEx;
  gSoloudCalls["Monotone"]["set3dColliderEx"].info = "void Monotone_set3dColliderEx( \n    Monotone * aMonotone,\n    AudioCollider * aCollider,\n    int aUserData)";
  gSoloudCalls["Monotone"]["set3dAttenuator"].p = call_Monotone_set3dAttenuator;
  gSoloudCalls["Monotone"]["set3dAttenuator"].info = "void Monotone_set3dAttenuator( \n    Monotone * aMonotone,\n    AudioAttenuator * aAttenuator)";
  gSoloudCalls["Monotone"]["setInaudibleBehavior"].p = call_Monotone_setInaudibleBehavior;
  gSoloudCalls["Monotone"]["setInaudibleBehavior"].info = "void Monotone_setInaudibleBehavior( \n    Monotone * aMonotone,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["Monotone"]["setLoopPoint"].p = call_Monotone_setLoopPoint;
  gSoloudCalls["Monotone"]["setLoopPoint"].info = "void Monotone_setLoopPoint( \n    Monotone * aMonotone,\n    double aLoopPoint)";
  gSoloudCalls["Monotone"]["getLoopPoint"].p = call_Monotone_getLoopPoint;
  gSoloudCalls["Monotone"]["getLoopPoint"].info = "double Monotone_getLoopPoint( \n    Monotone * aMonotone)";
  gSoloudCalls["Monotone"]["setFilter"].p = call_Monotone_setFilter;
  gSoloudCalls["Monotone"]["setFilter"].info = "void Monotone_setFilter( \n    Monotone * aMonotone,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["Monotone"]["stop"].p = call_Monotone_stop;
  gSoloudCalls["Monotone"]["stop"].info = "void Monotone_stop( \n    Monotone * aMonotone)";
  gSoloudCalls["Noise"]["destroy"].p = call_Noise_destroy;
  gSoloudCalls["Noise"]["destroy"].info = "void Noise_destroy( \n    Noise * aNoise)";
  gSoloudCalls["Noise"]["create"].p = call_Noise_create;
  gSoloudCalls["Noise"]["create"].info = "Noise * Noise_create()";
  gSoloudCalls["Noise"]["setOctaveScale"].p = call_Noise_setOctaveScale;
  gSoloudCalls["Noise"]["setOctaveScale"].info = "void Noise_setOctaveScale( \n    Noise * aNoise,\n    float aOct0,\n    float aOct1,\n    float aOct2,\n    float aOct3,\n    float aOct4,\n    float aOct5,\n    float aOct6,\n    float aOct7,\n    float aOct8,\n    float aOct9)";
  gSoloudCalls["Noise"]["setType"].p = call_Noise_setType;
  gSoloudCalls["Noise"]["setType"].info = "void Noise_setType( \n    Noise * aNoise,\n    int aType)";
  gSoloudCalls["Noise"]["setVolume"].p = call_Noise_setVolume;
  gSoloudCalls["Noise"]["setVolume"].info = "void Noise_setVolume( \n    Noise * aNoise,\n    float aVolume)";
  gSoloudCalls["Noise"]["setLooping"].p = call_Noise_setLooping;
  gSoloudCalls["Noise"]["setLooping"].info = "void Noise_setLooping( \n    Noise * aNoise,\n    int aLoop)";
  gSoloudCalls["Noise"]["setAutoStop"].p = call_Noise_setAutoStop;
  gSoloudCalls["Noise"]["setAutoStop"].info = "void Noise_setAutoStop( \n    Noise * aNoise,\n    int aAutoStop)";
  gSoloudCalls["Noise"]["set3dMinMaxDistance"].p = call_Noise_set3dMinMaxDistance;
  gSoloudCalls["Noise"]["set3dMinMaxDistance"].info = "void Noise_set3dMinMaxDistance( \n    Noise * aNoise,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["Noise"]["set3dAttenuation"].p = call_Noise_set3dAttenuation;
  gSoloudCalls["Noise"]["set3dAttenuation"].info = "void Noise_set3dAttenuation( \n    Noise * aNoise,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["Noise"]["set3dDopplerFactor"].p = call_Noise_set3dDopplerFactor;
  gSoloudCalls["Noise"]["set3dDopplerFactor"].info = "void Noise_set3dDopplerFactor( \n    Noise * aNoise,\n    float aDopplerFactor)";
  gSoloudCalls["Noise"]["set3dListenerRelative"].p = call_Noise_set3dListenerRelative;
  gSoloudCalls["Noise"]["set3dListenerRelative"].info = "void Noise_set3dListenerRelative( \n    Noise * aNoise,\n    int aListenerRelative)";
  gSoloudCalls["Noise"]["set3dDistanceDelay"].p = call_Noise_set3dDistanceDelay;
  gSoloudCalls["Noise"]["set3dDistanceDelay"].info = "void Noise_set3dDistanceDelay( \n    Noise * aNoise,\n    int aDistanceDelay)";
  gSoloudCalls["Noise"]["set3dCollider"].p = call_Noise_set3dCollider;
  gSoloudCalls["Noise"]["set3dCollider"].info = "void Noise_set3dCollider( \n    Noise * aNoise,\n    AudioCollider * aCollider)";
  gSoloudCalls["Noise"]["set3dColliderEx"].p = call_Noise_set3dColliderEx;
  gSoloudCalls["Noise"]["set3dColliderEx"].info = "void Noise_set3dColliderEx( \n    Noise * aNoise,\n    AudioCollider * aCollider,\n    int aUserData)";
  gSoloudCalls["Noise"]["set3dAttenuator"].p = call_Noise_set3dAttenuator;
  gSoloudCalls["Noise"]["set3dAttenuator"].info = "void Noise_set3dAttenuator( \n    Noise * aNoise,\n    AudioAttenuator * aAttenuator)";
  gSoloudCalls["Noise"]["setInaudibleBehavior"].p = call_Noise_setInaudibleBehavior;
  gSoloudCalls["Noise"]["setInaudibleBehavior"].info = "void Noise_setInaudibleBehavior( \n    Noise * aNoise,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["Noise"]["setLoopPoint"].p = call_Noise_setLoopPoint;
  gSoloudCalls["Noise"]["setLoopPoint"].info = "void Noise_setLoopPoint( \n    Noise * aNoise,\n    double aLoopPoint)";
  gSoloudCalls["Noise"]["getLoopPoint"].p = call_Noise_getLoopPoint;
  gSoloudCalls["Noise"]["getLoopPoint"].info = "double Noise_getLoopPoint( \n    Noise * aNoise)";
  gSoloudCalls["Noise"]["setFilter"].p = call_Noise_setFilter;
  gSoloudCalls["Noise"]["setFilter"].info = "void Noise_setFilter( \n    Noise * aNoise,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["Noise"]["stop"].p = call_Noise_stop;
  gSoloudCalls["Noise"]["stop"].info = "void Noise_stop( \n    Noise * aNoise)";
  gSoloudCalls["Openmpt"]["destroy"].p = call_Openmpt_destroy;
  gSoloudCalls["Openmpt"]["destroy"].info = "void Openmpt_destroy( \n    Openmpt * aOpenmpt)";
  gSoloudCalls["Openmpt"]["create"].p = call_Openmpt_create;
  gSoloudCalls["Openmpt"]["create"].info = "Openmpt * Openmpt_create()";
  gSoloudCalls["Openmpt"]["load"].p = call_Openmpt_load;
  gSoloudCalls["Openmpt"]["load"].info = "int Openmpt_load( \n    Openmpt * aOpenmpt,\n    const char * aFilename)";
  gSoloudCalls["Openmpt"]["loadMem"].p = call_Openmpt_loadMem;
  gSoloudCalls["Openmpt"]["loadMem"].info = "int Openmpt_loadMem( \n    Openmpt * aOpenmpt,\n    const unsigned char * aMem,\n    unsigned int aLength)";
  gSoloudCalls["Openmpt"]["loadMemEx"].p = call_Openmpt_loadMemEx;
  gSoloudCalls["Openmpt"]["loadMemEx"].info = "int Openmpt_loadMemEx( \n    Openmpt * aOpenmpt,\n    const unsigned char * aMem,\n    unsigned int aLength,\n    int aCopy,\n    int aTakeOwnership)";
  gSoloudCalls["Openmpt"]["loadFile"].p = call_Openmpt_loadFile;
  gSoloudCalls["Openmpt"]["loadFile"].info = "int Openmpt_loadFile( \n    Openmpt * aOpenmpt,\n    File * aFile)";
  gSoloudCalls["Openmpt"]["setVolume"].p = call_Openmpt_setVolume;
  gSoloudCalls["Openmpt"]["setVolume"].info = "void Openmpt_setVolume( \n    Openmpt * aOpenmpt,\n    float aVolume)";
  gSoloudCalls["Openmpt"]["setLooping"].p = call_Openmpt_setLooping;
  gSoloudCalls["Openmpt"]["setLooping"].info = "void Openmpt_setLooping( \n    Openmpt * aOpenmpt,\n    int aLoop)";
  gSoloudCalls["Openmpt"]["setAutoStop"].p = call_Openmpt_setAutoStop;
  gSoloudCalls["Openmpt"]["setAutoStop"].info = "void Openmpt_setAutoStop( \n    Openmpt * aOpenmpt,\n    int aAutoStop)";
  gSoloudCalls["Openmpt"]["set3dMinMaxDistance"].p = call_Openmpt_set3dMinMaxDistance;
  gSoloudCalls["Openmpt"]["set3dMinMaxDistance"].info = "void Openmpt_set3dMinMaxDistance( \n    Openmpt * aOpenmpt,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["Openmpt"]["set3dAttenuation"].p = call_Openmpt_set3dAttenuation;
  gSoloudCalls["Openmpt"]["set3dAttenuation"].info = "void Openmpt_set3dAttenuation( \n    Openmpt * aOpenmpt,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["Openmpt"]["set3dDopplerFactor"].p = call_Openmpt_set3dDopplerFactor;
  gSoloudCalls["Openmpt"]["set3dDopplerFactor"].info = "void Openmpt_set3dDopplerFactor( \n    Openmpt * aOpenmpt,\n    float aDopplerFactor)";
  gSoloudCalls["Openmpt"]["set3dListenerRelative"].p = call_Openmpt_set3dListenerRelative;
  gSoloudCalls["Openmpt"]["set3dListenerRelative"].info = "void Openmpt_set3dListenerRelative( \n    Openmpt * aOpenmpt,\n    int aListenerRelative)";
  gSoloudCalls["Openmpt"]["set3dDistanceDelay"].p = call_Openmpt_set3dDistanceDelay;
  gSoloudCalls["Openmpt"]["set3dDistanceDelay"].info = "void Openmpt_set3dDistanceDelay( \n    Openmpt * aOpenmpt,\n    int aDistanceDelay)";
  gSoloudCalls["Openmpt"]["set3dCollider"].p = call_Openmpt_set3dCollider;
  gSoloudCalls["Openmpt"]["set3dCollider"].info = "void Openmpt_set3dCollider( \n    Openmpt * aOpenmpt,\n    AudioCollider * aCollider)";
  gSoloudCalls["Openmpt"]["set3dColliderEx"].p = call_Openmpt_set3dColliderEx;
  gSoloudCalls["Openmpt"]["set3dColliderEx"].info = "void Openmpt_set3dColliderEx( \n    Openmpt * aOpenmpt,\n    AudioCollider * aCollider,\n    int aUserData)";
  gSoloudCalls["Openmpt"]["set3dAttenuator"].p = call_Openmpt_set3dAttenuator;
  gSoloudCalls["Openmpt"]["set3dAttenuator"].info = "void Openmpt_set3dAttenuator( \n    Openmpt * aOpenmpt,\n    AudioAttenuator * aAttenuator)";
  gSoloudCalls["Openmpt"]["setInaudibleBehavior"].p = call_Openmpt_setInaudibleBehavior;
  gSoloudCalls["Openmpt"]["setInaudibleBehavior"].info = "void Openmpt_setInaudibleBehavior( \n    Openmpt * aOpenmpt,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["Openmpt"]["setLoopPoint"].p = call_Openmpt_setLoopPoint;
  gSoloudCalls["Openmpt"]["setLoopPoint"].info = "void Openmpt_setLoopPoint( \n    Openmpt * aOpenmpt,\n    double aLoopPoint)";
  gSoloudCalls["Openmpt"]["getLoopPoint"].p = call_Openmpt_getLoopPoint;
  gSoloudCalls["Openmpt"]["getLoopPoint"].info = "double Openmpt_getLoopPoint( \n    Openmpt * aOpenmpt)";
  gSoloudCalls["Openmpt"]["setFilter"].p = call_Openmpt_setFilter;
  gSoloudCalls["Openmpt"]["setFilter"].info = "void Openmpt_setFilter( \n    Openmpt * aOpenmpt,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["Openmpt"]["stop"].p = call_Openmpt_stop;
  gSoloudCalls["Openmpt"]["stop"].info = "void Openmpt_stop( \n    Openmpt * aOpenmpt)";
  gSoloudCalls["Queue"]["destroy"].p = call_Queue_destroy;
  gSoloudCalls["Queue"]["destroy"].info = "void Queue_destroy( \n    Queue * aQueue)";
  gSoloudCalls["Queue"]["create"].p = call_Queue_create;
  gSoloudCalls["Queue"]["create"].info = "Queue * Queue_create()";
  gSoloudCalls["Queue"]["play"].p = call_Queue_play;
  gSoloudCalls["Queue"]["play"].info = "int Queue_play( \n    Queue * aQueue,\n    AudioSource * aSound)";
  gSoloudCalls["Queue"]["getQueueCount"].p = call_Queue_getQueueCount;
  gSoloudCalls["Queue"]["getQueueCount"].info = "unsigned int Queue_getQueueCount( \n    Queue * aQueue)";
  gSoloudCalls["Queue"]["isCurrentlyPlaying"].p = call_Queue_isCurrentlyPlaying;
  gSoloudCalls["Queue"]["isCurrentlyPlaying"].info = "int Queue_isCurrentlyPlaying( \n    Queue * aQueue,\n    AudioSource * aSound)";
  gSoloudCalls["Queue"]["setParamsFromAudioSource"].p = call_Queue_setParamsFromAudioSource;
  gSoloudCalls["Queue"]["setParamsFromAudioSource"].info = "int Queue_setParamsFromAudioSource( \n    Queue * aQueue,\n    AudioSource * aSound)";
  gSoloudCalls["Queue"]["setParams"].p = call_Queue_setParams;
  gSoloudCalls["Queue"]["setParams"].info = "int Queue_setParams( \n    Queue * aQueue,\n    float aSamplerate)";
  gSoloudCalls["Queue"]["setParamsEx"].p = call_Queue_setParamsEx;
  gSoloudCalls["Queue"]["setParamsEx"].info = "int Queue_setParamsEx( \n    Queue * aQueue,\n    float aSamplerate,\n    unsigned int aChannels)";
  gSoloudCalls["Queue"]["setVolume"].p = call_Queue_setVolume;
  gSoloudCalls["Queue"]["setVolume"].info = "void Queue_setVolume( \n    Queue * aQueue,\n    float aVolume)";
  gSoloudCalls["Queue"]["setLooping"].p = call_Queue_setLooping;
  gSoloudCalls["Queue"]["setLooping"].info = "void Queue_setLooping( \n    Queue * aQueue,\n    int aLoop)";
  gSoloudCalls["Queue"]["setAutoStop"].p = call_Queue_setAutoStop;
  gSoloudCalls["Queue"]["setAutoStop"].info = "void Queue_setAutoStop( \n    Queue * aQueue,\n    int aAutoStop)";
  gSoloudCalls["Queue"]["set3dMinMaxDistance"].p = call_Queue_set3dMinMaxDistance;
  gSoloudCalls["Queue"]["set3dMinMaxDistance"].info = "void Queue_set3dMinMaxDistance( \n    Queue * aQueue,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["Queue"]["set3dAttenuation"].p = call_Queue_set3dAttenuation;
  gSoloudCalls["Queue"]["set3dAttenuation"].info = "void Queue_set3dAttenuation( \n    Queue * aQueue,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["Queue"]["set3dDopplerFactor"].p = call_Queue_set3dDopplerFactor;
  gSoloudCalls["Queue"]["set3dDopplerFactor"].info = "void Queue_set3dDopplerFactor( \n    Queue * aQueue,\n    float aDopplerFactor)";
  gSoloudCalls["Queue"]["set3dListenerRelative"].p = call_Queue_set3dListenerRelative;
  gSoloudCalls["Queue"]["set3dListenerRelative"].info = "void Queue_set3dListenerRelative( \n    Queue * aQueue,\n    int aListenerRelative)";
  gSoloudCalls["Queue"]["set3dDistanceDelay"].p = call_Queue_set3dDistanceDelay;
  gSoloudCalls["Queue"]["set3dDistanceDelay"].info = "void Queue_set3dDistanceDelay( \n    Queue * aQueue,\n    int aDistanceDelay)";
  gSoloudCalls["Queue"]["set3dCollider"].p = call_Queue_set3dCollider;
  gSoloudCalls["Queue"]["set3dCollider"].info = "void Queue_set3dCollider( \n    Queue * aQueue,\n    AudioCollider * aCollider)";
  gSoloudCalls["Queue"]["set3dColliderEx"].p = call_Queue_set3dColliderEx;
  gSoloudCalls["Queue"]["set3dColliderEx"].info = "void Queue_set3dColliderEx( \n    Queue * aQueue,\n    AudioCollider * aCollider,\n    int aUserData)";
  gSoloudCalls["Queue"]["set3dAttenuator"].p = call_Queue_set3dAttenuator;
  gSoloudCalls["Queue"]["set3dAttenuator"].info = "void Queue_set3dAttenuator( \n    Queue * aQueue,\n    AudioAttenuator * aAttenuator)";
  gSoloudCalls["Queue"]["setInaudibleBehavior"].p = call_Queue_setInaudibleBehavior;
  gSoloudCalls["Queue"]["setInaudibleBehavior"].info = "void Queue_setInaudibleBehavior( \n    Queue * aQueue,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["Queue"]["setLoopPoint"].p = call_Queue_setLoopPoint;
  gSoloudCalls["Queue"]["setLoopPoint"].info = "void Queue_setLoopPoint( \n    Queue * aQueue,\n    double aLoopPoint)";
  gSoloudCalls["Queue"]["getLoopPoint"].p = call_Queue_getLoopPoint;
  gSoloudCalls["Queue"]["getLoopPoint"].info = "double Queue_getLoopPoint( \n    Queue * aQueue)";
  gSoloudCalls["Queue"]["setFilter"].p = call_Queue_setFilter;
  gSoloudCalls["Queue"]["setFilter"].info = "void Queue_setFilter( \n    Queue * aQueue,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["Queue"]["stop"].p = call_Queue_stop;
  gSoloudCalls["Queue"]["stop"].info = "void Queue_stop( \n    Queue * aQueue)";
  gSoloudCalls["RobotizeFilter"]["destroy"].p = call_RobotizeFilter_destroy;
  gSoloudCalls["RobotizeFilter"]["destroy"].info = "void RobotizeFilter_destroy( \n    RobotizeFilter * aRobotizeFilter)";
  gSoloudCalls["RobotizeFilter"]["getParamCount"].p = call_RobotizeFilter_getParamCount;
  gSoloudCalls["RobotizeFilter"]["getParamCount"].info = "int RobotizeFilter_getParamCount( \n    RobotizeFilter * aRobotizeFilter)";
  gSoloudCalls["RobotizeFilter"]["getParamType"].p = call_RobotizeFilter_getParamType;
  gSoloudCalls["RobotizeFilter"]["getParamType"].info = "unsigned int RobotizeFilter_getParamType( \n    RobotizeFilter * aRobotizeFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["RobotizeFilter"]["getParamMax"].p = call_RobotizeFilter_getParamMax;
  gSoloudCalls["RobotizeFilter"]["getParamMax"].info = "float RobotizeFilter_getParamMax( \n    RobotizeFilter * aRobotizeFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["RobotizeFilter"]["getParamMin"].p = call_RobotizeFilter_getParamMin;
  gSoloudCalls["RobotizeFilter"]["getParamMin"].info = "float RobotizeFilter_getParamMin( \n    RobotizeFilter * aRobotizeFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["RobotizeFilter"]["setParams"].p = call_RobotizeFilter_setParams;
  gSoloudCalls["RobotizeFilter"]["setParams"].info = "void RobotizeFilter_setParams( \n    RobotizeFilter * aRobotizeFilter,\n    float aFreq,\n    int aWaveform)";
  gSoloudCalls["RobotizeFilter"]["create"].p = call_RobotizeFilter_create;
  gSoloudCalls["RobotizeFilter"]["create"].info = "RobotizeFilter * RobotizeFilter_create()";
  gSoloudCalls["Sfxr"]["destroy"].p = call_Sfxr_destroy;
  gSoloudCalls["Sfxr"]["destroy"].info = "void Sfxr_destroy( \n    Sfxr * aSfxr)";
  gSoloudCalls["Sfxr"]["create"].p = call_Sfxr_create;
  gSoloudCalls["Sfxr"]["create"].info = "Sfxr * Sfxr_create()";
  gSoloudCalls["Sfxr"]["resetParams"].p = call_Sfxr_resetParams;
  gSoloudCalls["Sfxr"]["resetParams"].info = "void Sfxr_resetParams( \n    Sfxr * aSfxr)";
  gSoloudCalls["Sfxr"]["loadParams"].p = call_Sfxr_loadParams;
  gSoloudCalls["Sfxr"]["loadParams"].info = "int Sfxr_loadParams( \n    Sfxr * aSfxr,\n    const char * aFilename)";
  gSoloudCalls["Sfxr"]["loadParamsMem"].p = call_Sfxr_loadParamsMem;
  gSoloudCalls["Sfxr"]["loadParamsMem"].info = "int Sfxr_loadParamsMem( \n    Sfxr * aSfxr,\n    unsigned char * aMem,\n    unsigned int aLength)";
  gSoloudCalls["Sfxr"]["loadParamsMemEx"].p = call_Sfxr_loadParamsMemEx;
  gSoloudCalls["Sfxr"]["loadParamsMemEx"].info = "int Sfxr_loadParamsMemEx( \n    Sfxr * aSfxr,\n    unsigned char * aMem,\n    unsigned int aLength,\n    int aCopy,\n    int aTakeOwnership)";
  gSoloudCalls["Sfxr"]["loadParamsFile"].p = call_Sfxr_loadParamsFile;
  gSoloudCalls["Sfxr"]["loadParamsFile"].info = "int Sfxr_loadParamsFile( \n    Sfxr * aSfxr,\n    File * aFile)";
  gSoloudCalls["Sfxr"]["loadPreset"].p = call_Sfxr_loadPreset;
  gSoloudCalls["Sfxr"]["loadPreset"].info = "int Sfxr_loadPreset( \n    Sfxr * aSfxr,\n    int aPresetNo,\n    int aRandSeed)";
  gSoloudCalls["Sfxr"]["setVolume"].p = call_Sfxr_setVolume;
  gSoloudCalls["Sfxr"]["setVolume"].info = "void Sfxr_setVolume( \n    Sfxr * aSfxr,\n    float aVolume)";
  gSoloudCalls["Sfxr"]["setLooping"].p = call_Sfxr_setLooping;
  gSoloudCalls["Sfxr"]["setLooping"].info = "void Sfxr_setLooping( \n    Sfxr * aSfxr,\n    int aLoop)";
  gSoloudCalls["Sfxr"]["setAutoStop"].p = call_Sfxr_setAutoStop;
  gSoloudCalls["Sfxr"]["setAutoStop"].info = "void Sfxr_setAutoStop( \n    Sfxr * aSfxr,\n    int aAutoStop)";
  gSoloudCalls["Sfxr"]["set3dMinMaxDistance"].p = call_Sfxr_set3dMinMaxDistance;
  gSoloudCalls["Sfxr"]["set3dMinMaxDistance"].info = "void Sfxr_set3dMinMaxDistance( \n    Sfxr * aSfxr,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["Sfxr"]["set3dAttenuation"].p = call_Sfxr_set3dAttenuation;
  gSoloudCalls["Sfxr"]["set3dAttenuation"].info = "void Sfxr_set3dAttenuation( \n    Sfxr * aSfxr,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["Sfxr"]["set3dDopplerFactor"].p = call_Sfxr_set3dDopplerFactor;
  gSoloudCalls["Sfxr"]["set3dDopplerFactor"].info = "void Sfxr_set3dDopplerFactor( \n    Sfxr * aSfxr,\n    float aDopplerFactor)";
  gSoloudCalls["Sfxr"]["set3dListenerRelative"].p = call_Sfxr_set3dListenerRelative;
  gSoloudCalls["Sfxr"]["set3dListenerRelative"].info = "void Sfxr_set3dListenerRelative( \n    Sfxr * aSfxr,\n    int aListenerRelative)";
  gSoloudCalls["Sfxr"]["set3dDistanceDelay"].p = call_Sfxr_set3dDistanceDelay;
  gSoloudCalls["Sfxr"]["set3dDistanceDelay"].info = "void Sfxr_set3dDistanceDelay( \n    Sfxr * aSfxr,\n    int aDistanceDelay)";
  gSoloudCalls["Sfxr"]["set3dCollider"].p = call_Sfxr_set3dCollider;
  gSoloudCalls["Sfxr"]["set3dCollider"].info = "void Sfxr_set3dCollider( \n    Sfxr * aSfxr,\n    AudioCollider * aCollider)";
  gSoloudCalls["Sfxr"]["set3dColliderEx"].p = call_Sfxr_set3dColliderEx;
  gSoloudCalls["Sfxr"]["set3dColliderEx"].info = "void Sfxr_set3dColliderEx( \n    Sfxr * aSfxr,\n    AudioCollider * aCollider,\n    int aUserData)";
  gSoloudCalls["Sfxr"]["set3dAttenuator"].p = call_Sfxr_set3dAttenuator;
  gSoloudCalls["Sfxr"]["set3dAttenuator"].info = "void Sfxr_set3dAttenuator( \n    Sfxr * aSfxr,\n    AudioAttenuator * aAttenuator)";
  gSoloudCalls["Sfxr"]["setInaudibleBehavior"].p = call_Sfxr_setInaudibleBehavior;
  gSoloudCalls["Sfxr"]["setInaudibleBehavior"].info = "void Sfxr_setInaudibleBehavior( \n    Sfxr * aSfxr,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["Sfxr"]["setLoopPoint"].p = call_Sfxr_setLoopPoint;
  gSoloudCalls["Sfxr"]["setLoopPoint"].info = "void Sfxr_setLoopPoint( \n    Sfxr * aSfxr,\n    double aLoopPoint)";
  gSoloudCalls["Sfxr"]["getLoopPoint"].p = call_Sfxr_getLoopPoint;
  gSoloudCalls["Sfxr"]["getLoopPoint"].info = "double Sfxr_getLoopPoint( \n    Sfxr * aSfxr)";
  gSoloudCalls["Sfxr"]["setFilter"].p = call_Sfxr_setFilter;
  gSoloudCalls["Sfxr"]["setFilter"].info = "void Sfxr_setFilter( \n    Sfxr * aSfxr,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["Sfxr"]["stop"].p = call_Sfxr_stop;
  gSoloudCalls["Sfxr"]["stop"].info = "void Sfxr_stop( \n    Sfxr * aSfxr)";
  gSoloudCalls["Speech"]["destroy"].p = call_Speech_destroy;
  gSoloudCalls["Speech"]["destroy"].info = "void Speech_destroy( \n    Speech * aSpeech)";
  gSoloudCalls["Speech"]["create"].p = call_Speech_create;
  gSoloudCalls["Speech"]["create"].info = "Speech * Speech_create()";
  gSoloudCalls["Speech"]["setText"].p = call_Speech_setText;
  gSoloudCalls["Speech"]["setText"].info = "int Speech_setText( \n    Speech * aSpeech,\n    const char * aText)";
  gSoloudCalls["Speech"]["setParams"].p = call_Speech_setParams;
  gSoloudCalls["Speech"]["setParams"].info = "int Speech_setParams( \n    Speech * aSpeech)";
  gSoloudCalls["Speech"]["setParamsEx"].p = call_Speech_setParamsEx;
  gSoloudCalls["Speech"]["setParamsEx"].info = "int Speech_setParamsEx( \n    Speech * aSpeech,\n    unsigned int aBaseFrequency,\n    float aBaseSpeed,\n    float aBaseDeclination,\n    int aBaseWaveform)";
  gSoloudCalls["Speech"]["setVolume"].p = call_Speech_setVolume;
  gSoloudCalls["Speech"]["setVolume"].info = "void Speech_setVolume( \n    Speech * aSpeech,\n    float aVolume)";
  gSoloudCalls["Speech"]["setLooping"].p = call_Speech_setLooping;
  gSoloudCalls["Speech"]["setLooping"].info = "void Speech_setLooping( \n    Speech * aSpeech,\n    int aLoop)";
  gSoloudCalls["Speech"]["setAutoStop"].p = call_Speech_setAutoStop;
  gSoloudCalls["Speech"]["setAutoStop"].info = "void Speech_setAutoStop( \n    Speech * aSpeech,\n    int aAutoStop)";
  gSoloudCalls["Speech"]["set3dMinMaxDistance"].p = call_Speech_set3dMinMaxDistance;
  gSoloudCalls["Speech"]["set3dMinMaxDistance"].info = "void Speech_set3dMinMaxDistance( \n    Speech * aSpeech,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["Speech"]["set3dAttenuation"].p = call_Speech_set3dAttenuation;
  gSoloudCalls["Speech"]["set3dAttenuation"].info = "void Speech_set3dAttenuation( \n    Speech * aSpeech,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["Speech"]["set3dDopplerFactor"].p = call_Speech_set3dDopplerFactor;
  gSoloudCalls["Speech"]["set3dDopplerFactor"].info = "void Speech_set3dDopplerFactor( \n    Speech * aSpeech,\n    float aDopplerFactor)";
  gSoloudCalls["Speech"]["set3dListenerRelative"].p = call_Speech_set3dListenerRelative;
  gSoloudCalls["Speech"]["set3dListenerRelative"].info = "void Speech_set3dListenerRelative( \n    Speech * aSpeech,\n    int aListenerRelative)";
  gSoloudCalls["Speech"]["set3dDistanceDelay"].p = call_Speech_set3dDistanceDelay;
  gSoloudCalls["Speech"]["set3dDistanceDelay"].info = "void Speech_set3dDistanceDelay( \n    Speech * aSpeech,\n    int aDistanceDelay)";
  gSoloudCalls["Speech"]["set3dCollider"].p = call_Speech_set3dCollider;
  gSoloudCalls["Speech"]["set3dCollider"].info = "void Speech_set3dCollider( \n    Speech * aSpeech,\n    AudioCollider * aCollider)";
  gSoloudCalls["Speech"]["set3dColliderEx"].p = call_Speech_set3dColliderEx;
  gSoloudCalls["Speech"]["set3dColliderEx"].info = "void Speech_set3dColliderEx( \n    Speech * aSpeech,\n    AudioCollider * aCollider,\n    int aUserData)";
  gSoloudCalls["Speech"]["set3dAttenuator"].p = call_Speech_set3dAttenuator;
  gSoloudCalls["Speech"]["set3dAttenuator"].info = "void Speech_set3dAttenuator( \n    Speech * aSpeech,\n    AudioAttenuator * aAttenuator)";
  gSoloudCalls["Speech"]["setInaudibleBehavior"].p = call_Speech_setInaudibleBehavior;
  gSoloudCalls["Speech"]["setInaudibleBehavior"].info = "void Speech_setInaudibleBehavior( \n    Speech * aSpeech,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["Speech"]["setLoopPoint"].p = call_Speech_setLoopPoint;
  gSoloudCalls["Speech"]["setLoopPoint"].info = "void Speech_setLoopPoint( \n    Speech * aSpeech,\n    double aLoopPoint)";
  gSoloudCalls["Speech"]["getLoopPoint"].p = call_Speech_getLoopPoint;
  gSoloudCalls["Speech"]["getLoopPoint"].info = "double Speech_getLoopPoint( \n    Speech * aSpeech)";
  gSoloudCalls["Speech"]["setFilter"].p = call_Speech_setFilter;
  gSoloudCalls["Speech"]["setFilter"].info = "void Speech_setFilter( \n    Speech * aSpeech,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["Speech"]["stop"].p = call_Speech_stop;
  gSoloudCalls["Speech"]["stop"].info = "void Speech_stop( \n    Speech * aSpeech)";
  gSoloudCalls["TedSid"]["destroy"].p = call_TedSid_destroy;
  gSoloudCalls["TedSid"]["destroy"].info = "void TedSid_destroy( \n    TedSid * aTedSid)";
  gSoloudCalls["TedSid"]["create"].p = call_TedSid_create;
  gSoloudCalls["TedSid"]["create"].info = "TedSid * TedSid_create()";
  gSoloudCalls["TedSid"]["load"].p = call_TedSid_load;
  gSoloudCalls["TedSid"]["load"].info = "int TedSid_load( \n    TedSid * aTedSid,\n    const char * aFilename)";
  gSoloudCalls["TedSid"]["loadMem"].p = call_TedSid_loadMem;
  gSoloudCalls["TedSid"]["loadMem"].info = "int TedSid_loadMem( \n    TedSid * aTedSid,\n    const unsigned char * aMem,\n    unsigned int aLength)";
  gSoloudCalls["TedSid"]["loadMemEx"].p = call_TedSid_loadMemEx;
  gSoloudCalls["TedSid"]["loadMemEx"].info = "int TedSid_loadMemEx( \n    TedSid * aTedSid,\n    const unsigned char * aMem,\n    unsigned int aLength,\n    int aCopy,\n    int aTakeOwnership)";
  gSoloudCalls["TedSid"]["loadFile"].p = call_TedSid_loadFile;
  gSoloudCalls["TedSid"]["loadFile"].info = "int TedSid_loadFile( \n    TedSid * aTedSid,\n    File * aFile)";
  gSoloudCalls["TedSid"]["setVolume"].p = call_TedSid_setVolume;
  gSoloudCalls["TedSid"]["setVolume"].info = "void TedSid_setVolume( \n    TedSid * aTedSid,\n    float aVolume)";
  gSoloudCalls["TedSid"]["setLooping"].p = call_TedSid_setLooping;
  gSoloudCalls["TedSid"]["setLooping"].info = "void TedSid_setLooping( \n    TedSid * aTedSid,\n    int aLoop)";
  gSoloudCalls["TedSid"]["setAutoStop"].p = call_TedSid_setAutoStop;
  gSoloudCalls["TedSid"]["setAutoStop"].info = "void TedSid_setAutoStop( \n    TedSid * aTedSid,\n    int aAutoStop)";
  gSoloudCalls["TedSid"]["set3dMinMaxDistance"].p = call_TedSid_set3dMinMaxDistance;
  gSoloudCalls["TedSid"]["set3dMinMaxDistance"].info = "void TedSid_set3dMinMaxDistance( \n    TedSid * aTedSid,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["TedSid"]["set3dAttenuation"].p = call_TedSid_set3dAttenuation;
  gSoloudCalls["TedSid"]["set3dAttenuation"].info = "void TedSid_set3dAttenuation( \n    TedSid * aTedSid,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["TedSid"]["set3dDopplerFactor"].p = call_TedSid_set3dDopplerFactor;
  gSoloudCalls["TedSid"]["set3dDopplerFactor"].info = "void TedSid_set3dDopplerFactor( \n    TedSid * aTedSid,\n    float aDopplerFactor)";
  gSoloudCalls["TedSid"]["set3dListenerRelative"].p = call_TedSid_set3dListenerRelative;
  gSoloudCalls["TedSid"]["set3dListenerRelative"].info = "void TedSid_set3dListenerRelative( \n    TedSid * aTedSid,\n    int aListenerRelative)";
  gSoloudCalls["TedSid"]["set3dDistanceDelay"].p = call_TedSid_set3dDistanceDelay;
  gSoloudCalls["TedSid"]["set3dDistanceDelay"].info = "void TedSid_set3dDistanceDelay( \n    TedSid * aTedSid,\n    int aDistanceDelay)";
  gSoloudCalls["TedSid"]["set3dCollider"].p = call_TedSid_set3dCollider;
  gSoloudCalls["TedSid"]["set3dCollider"].info = "void TedSid_set3dCollider( \n    TedSid * aTedSid,\n    AudioCollider * aCollider)";
  gSoloudCalls["TedSid"]["set3dColliderEx"].p = call_TedSid_set3dColliderEx;
  gSoloudCalls["TedSid"]["set3dColliderEx"].info = "void TedSid_set3dColliderEx( \n    TedSid * aTedSid,\n    AudioCollider * aCollider,\n    int aUserData)";
  gSoloudCalls["TedSid"]["set3dAttenuator"].p = call_TedSid_set3dAttenuator;
  gSoloudCalls["TedSid"]["set3dAttenuator"].info = "void TedSid_set3dAttenuator( \n    TedSid * aTedSid,\n    AudioAttenuator * aAttenuator)";
  gSoloudCalls["TedSid"]["setInaudibleBehavior"].p = call_TedSid_setInaudibleBehavior;
  gSoloudCalls["TedSid"]["setInaudibleBehavior"].info = "void TedSid_setInaudibleBehavior( \n    TedSid * aTedSid,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["TedSid"]["setLoopPoint"].p = call_TedSid_setLoopPoint;
  gSoloudCalls["TedSid"]["setLoopPoint"].info = "void TedSid_setLoopPoint( \n    TedSid * aTedSid,\n    double aLoopPoint)";
  gSoloudCalls["TedSid"]["getLoopPoint"].p = call_TedSid_getLoopPoint;
  gSoloudCalls["TedSid"]["getLoopPoint"].info = "double TedSid_getLoopPoint( \n    TedSid * aTedSid)";
  gSoloudCalls["TedSid"]["setFilter"].p = call_TedSid_setFilter;
  gSoloudCalls["TedSid"]["setFilter"].info = "void TedSid_setFilter( \n    TedSid * aTedSid,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["TedSid"]["stop"].p = call_TedSid_stop;
  gSoloudCalls["TedSid"]["stop"].info = "void TedSid_stop( \n    TedSid * aTedSid)";
  gSoloudCalls["Vic"]["destroy"].p = call_Vic_destroy;
  gSoloudCalls["Vic"]["destroy"].info = "void Vic_destroy( \n    Vic * aVic)";
  gSoloudCalls["Vic"]["create"].p = call_Vic_create;
  gSoloudCalls["Vic"]["create"].info = "Vic * Vic_create()";
  gSoloudCalls["Vic"]["setModel"].p = call_Vic_setModel;
  gSoloudCalls["Vic"]["setModel"].info = "void Vic_setModel( \n    Vic * aVic,\n    int model)";
  gSoloudCalls["Vic"]["getModel"].p = call_Vic_getModel;
  gSoloudCalls["Vic"]["getModel"].info = "int Vic_getModel( \n    Vic * aVic)";
  gSoloudCalls["Vic"]["setRegister"].p = call_Vic_setRegister;
  gSoloudCalls["Vic"]["setRegister"].info = "void Vic_setRegister( \n    Vic * aVic,\n    int reg,\n    unsigned char value)";
  gSoloudCalls["Vic"]["getRegister"].p = call_Vic_getRegister;
  gSoloudCalls["Vic"]["getRegister"].info = "unsigned char Vic_getRegister( \n    Vic * aVic,\n    int reg)";
  gSoloudCalls["Vic"]["setVolume"].p = call_Vic_setVolume;
  gSoloudCalls["Vic"]["setVolume"].info = "void Vic_setVolume( \n    Vic * aVic,\n    float aVolume)";
  gSoloudCalls["Vic"]["setLooping"].p = call_Vic_setLooping;
  gSoloudCalls["Vic"]["setLooping"].info = "void Vic_setLooping( \n    Vic * aVic,\n    int aLoop)";
  gSoloudCalls["Vic"]["setAutoStop"].p = call_Vic_setAutoStop;
  gSoloudCalls["Vic"]["setAutoStop"].info = "void Vic_setAutoStop( \n    Vic * aVic,\n    int aAutoStop)";
  gSoloudCalls["Vic"]["set3dMinMaxDistance"].p = call_Vic_set3dMinMaxDistance;
  gSoloudCalls["Vic"]["set3dMinMaxDistance"].info = "void Vic_set3dMinMaxDistance( \n    Vic * aVic,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["Vic"]["set3dAttenuation"].p = call_Vic_set3dAttenuation;
  gSoloudCalls["Vic"]["set3dAttenuation"].info = "void Vic_set3dAttenuation( \n    Vic * aVic,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["Vic"]["set3dDopplerFactor"].p = call_Vic_set3dDopplerFactor;
  gSoloudCalls["Vic"]["set3dDopplerFactor"].info = "void Vic_set3dDopplerFactor( \n    Vic * aVic,\n    float aDopplerFactor)";
  gSoloudCalls["Vic"]["set3dListenerRelative"].p = call_Vic_set3dListenerRelative;
  gSoloudCalls["Vic"]["set3dListenerRelative"].info = "void Vic_set3dListenerRelative( \n    Vic * aVic,\n    int aListenerRelative)";
  gSoloudCalls["Vic"]["set3dDistanceDelay"].p = call_Vic_set3dDistanceDelay;
  gSoloudCalls["Vic"]["set3dDistanceDelay"].info = "void Vic_set3dDistanceDelay( \n    Vic * aVic,\n    int aDistanceDelay)";
  gSoloudCalls["Vic"]["set3dCollider"].p = call_Vic_set3dCollider;
  gSoloudCalls["Vic"]["set3dCollider"].info = "void Vic_set3dCollider( \n    Vic * aVic,\n    AudioCollider * aCollider)";
  gSoloudCalls["Vic"]["set3dColliderEx"].p = call_Vic_set3dColliderEx;
  gSoloudCalls["Vic"]["set3dColliderEx"].info = "void Vic_set3dColliderEx( \n    Vic * aVic,\n    AudioCollider * aCollider,\n    int aUserData)";
  gSoloudCalls["Vic"]["set3dAttenuator"].p = call_Vic_set3dAttenuator;
  gSoloudCalls["Vic"]["set3dAttenuator"].info = "void Vic_set3dAttenuator( \n    Vic * aVic,\n    AudioAttenuator * aAttenuator)";
  gSoloudCalls["Vic"]["setInaudibleBehavior"].p = call_Vic_setInaudibleBehavior;
  gSoloudCalls["Vic"]["setInaudibleBehavior"].info = "void Vic_setInaudibleBehavior( \n    Vic * aVic,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["Vic"]["setLoopPoint"].p = call_Vic_setLoopPoint;
  gSoloudCalls["Vic"]["setLoopPoint"].info = "void Vic_setLoopPoint( \n    Vic * aVic,\n    double aLoopPoint)";
  gSoloudCalls["Vic"]["getLoopPoint"].p = call_Vic_getLoopPoint;
  gSoloudCalls["Vic"]["getLoopPoint"].info = "double Vic_getLoopPoint( \n    Vic * aVic)";
  gSoloudCalls["Vic"]["setFilter"].p = call_Vic_setFilter;
  gSoloudCalls["Vic"]["setFilter"].info = "void Vic_setFilter( \n    Vic * aVic,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["Vic"]["stop"].p = call_Vic_stop;
  gSoloudCalls["Vic"]["stop"].info = "void Vic_stop( \n    Vic * aVic)";
  gSoloudCalls["Vizsn"]["destroy"].p = call_Vizsn_destroy;
  gSoloudCalls["Vizsn"]["destroy"].info = "void Vizsn_destroy( \n    Vizsn * aVizsn)";
  gSoloudCalls["Vizsn"]["create"].p = call_Vizsn_create;
  gSoloudCalls["Vizsn"]["create"].info = "Vizsn * Vizsn_create()";
  gSoloudCalls["Vizsn"]["setText"].p = call_Vizsn_setText;
  gSoloudCalls["Vizsn"]["setText"].info = "void Vizsn_setText( \n    Vizsn * aVizsn,\n    char * aText)";
  gSoloudCalls["Vizsn"]["setVolume"].p = call_Vizsn_setVolume;
  gSoloudCalls["Vizsn"]["setVolume"].info = "void Vizsn_setVolume( \n    Vizsn * aVizsn,\n    float aVolume)";
  gSoloudCalls["Vizsn"]["setLooping"].p = call_Vizsn_setLooping;
  gSoloudCalls["Vizsn"]["setLooping"].info = "void Vizsn_setLooping( \n    Vizsn * aVizsn,\n    int aLoop)";
  gSoloudCalls["Vizsn"]["setAutoStop"].p = call_Vizsn_setAutoStop;
  gSoloudCalls["Vizsn"]["setAutoStop"].info = "void Vizsn_setAutoStop( \n    Vizsn * aVizsn,\n    int aAutoStop)";
  gSoloudCalls["Vizsn"]["set3dMinMaxDistance"].p = call_Vizsn_set3dMinMaxDistance;
  gSoloudCalls["Vizsn"]["set3dMinMaxDistance"].info = "void Vizsn_set3dMinMaxDistance( \n    Vizsn * aVizsn,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["Vizsn"]["set3dAttenuation"].p = call_Vizsn_set3dAttenuation;
  gSoloudCalls["Vizsn"]["set3dAttenuation"].info = "void Vizsn_set3dAttenuation( \n    Vizsn * aVizsn,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["Vizsn"]["set3dDopplerFactor"].p = call_Vizsn_set3dDopplerFactor;
  gSoloudCalls["Vizsn"]["set3dDopplerFactor"].info = "void Vizsn_set3dDopplerFactor( \n    Vizsn * aVizsn,\n    float aDopplerFactor)";
  gSoloudCalls["Vizsn"]["set3dListenerRelative"].p = call_Vizsn_set3dListenerRelative;
  gSoloudCalls["Vizsn"]["set3dListenerRelative"].info = "void Vizsn_set3dListenerRelative( \n    Vizsn * aVizsn,\n    int aListenerRelative)";
  gSoloudCalls["Vizsn"]["set3dDistanceDelay"].p = call_Vizsn_set3dDistanceDelay;
  gSoloudCalls["Vizsn"]["set3dDistanceDelay"].info = "void Vizsn_set3dDistanceDelay( \n    Vizsn * aVizsn,\n    int aDistanceDelay)";
  gSoloudCalls["Vizsn"]["set3dCollider"].p = call_Vizsn_set3dCollider;
  gSoloudCalls["Vizsn"]["set3dCollider"].info = "void Vizsn_set3dCollider( \n    Vizsn * aVizsn,\n    AudioCollider * aCollider)";
  gSoloudCalls["Vizsn"]["set3dColliderEx"].p = call_Vizsn_set3dColliderEx;
  gSoloudCalls["Vizsn"]["set3dColliderEx"].info = "void Vizsn_set3dColliderEx( \n    Vizsn * aVizsn,\n    AudioCollider * aCollider,\n    int aUserData)";
  gSoloudCalls["Vizsn"]["set3dAttenuator"].p = call_Vizsn_set3dAttenuator;
  gSoloudCalls["Vizsn"]["set3dAttenuator"].info = "void Vizsn_set3dAttenuator( \n    Vizsn * aVizsn,\n    AudioAttenuator * aAttenuator)";
  gSoloudCalls["Vizsn"]["setInaudibleBehavior"].p = call_Vizsn_setInaudibleBehavior;
  gSoloudCalls["Vizsn"]["setInaudibleBehavior"].info = "void Vizsn_setInaudibleBehavior( \n    Vizsn * aVizsn,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["Vizsn"]["setLoopPoint"].p = call_Vizsn_setLoopPoint;
  gSoloudCalls["Vizsn"]["setLoopPoint"].info = "void Vizsn_setLoopPoint( \n    Vizsn * aVizsn,\n    double aLoopPoint)";
  gSoloudCalls["Vizsn"]["getLoopPoint"].p = call_Vizsn_getLoopPoint;
  gSoloudCalls["Vizsn"]["getLoopPoint"].info = "double Vizsn_getLoopPoint( \n    Vizsn * aVizsn)";
  gSoloudCalls["Vizsn"]["setFilter"].p = call_Vizsn_setFilter;
  gSoloudCalls["Vizsn"]["setFilter"].info = "void Vizsn_setFilter( \n    Vizsn * aVizsn,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["Vizsn"]["stop"].p = call_Vizsn_stop;
  gSoloudCalls["Vizsn"]["stop"].info = "void Vizsn_stop( \n    Vizsn * aVizsn)";
  gSoloudCalls["Wav"]["destroy"].p = call_Wav_destroy;
  gSoloudCalls["Wav"]["destroy"].info = "void Wav_destroy( \n    Wav * aWav)";
  gSoloudCalls["Wav"]["create"].p = call_Wav_create;
  gSoloudCalls["Wav"]["create"].info = "Wav * Wav_create()";
  gSoloudCalls["Wav"]["load"].p = call_Wav_load;
  gSoloudCalls["Wav"]["load"].info = "int Wav_load( \n    Wav * aWav,\n    const char * aFilename)";
  gSoloudCalls["Wav"]["loadMem"].p = call_Wav_loadMem;
  gSoloudCalls["Wav"]["loadMem"].info = "int Wav_loadMem( \n    Wav * aWav,\n    const unsigned char * aMem,\n    unsigned int aLength)";
  gSoloudCalls["Wav"]["loadMemEx"].p = call_Wav_loadMemEx;
  gSoloudCalls["Wav"]["loadMemEx"].info = "int Wav_loadMemEx( \n    Wav * aWav,\n    const unsigned char * aMem,\n    unsigned int aLength,\n    int aCopy,\n    int aTakeOwnership)";
  gSoloudCalls["Wav"]["loadFile"].p = call_Wav_loadFile;
  gSoloudCalls["Wav"]["loadFile"].info = "int Wav_loadFile( \n    Wav * aWav,\n    File * aFile)";
  gSoloudCalls["Wav"]["loadRawWave8"].p = call_Wav_loadRawWave8;
  gSoloudCalls["Wav"]["loadRawWave8"].info = "int Wav_loadRawWave8( \n    Wav * aWav,\n    unsigned char * aMem,\n    unsigned int aLength)";
  gSoloudCalls["Wav"]["loadRawWave8Ex"].p = call_Wav_loadRawWave8Ex;
  gSoloudCalls["Wav"]["loadRawWave8Ex"].info = "int Wav_loadRawWave8Ex( \n    Wav * aWav,\n    unsigned char * aMem,\n    unsigned int aLength,\n    float aSamplerate,\n    unsigned int aChannels)";
  gSoloudCalls["Wav"]["loadRawWave16"].p = call_Wav_loadRawWave16;
  gSoloudCalls["Wav"]["loadRawWave16"].info = "int Wav_loadRawWave16( \n    Wav * aWav,\n    short * aMem,\n    unsigned int aLength)";
  gSoloudCalls["Wav"]["loadRawWave16Ex"].p = call_Wav_loadRawWave16Ex;
  gSoloudCalls["Wav"]["loadRawWave16Ex"].info = "int Wav_loadRawWave16Ex( \n    Wav * aWav,\n    short * aMem,\n    unsigned int aLength,\n    float aSamplerate,\n    unsigned int aChannels)";
  gSoloudCalls["Wav"]["loadRawWave"].p = call_Wav_loadRawWave;
  gSoloudCalls["Wav"]["loadRawWave"].info = "int Wav_loadRawWave( \n    Wav * aWav,\n    float * aMem,\n    unsigned int aLength)";
  gSoloudCalls["Wav"]["loadRawWaveEx"].p = call_Wav_loadRawWaveEx;
  gSoloudCalls["Wav"]["loadRawWaveEx"].info = "int Wav_loadRawWaveEx( \n    Wav * aWav,\n    float * aMem,\n    unsigned int aLength,\n    float aSamplerate,\n    unsigned int aChannels,\n    int aCopy,\n    int aTakeOwnership)";
  gSoloudCalls["Wav"]["getLength"].p = call_Wav_getLength;
  gSoloudCalls["Wav"]["getLength"].info = "double Wav_getLength( \n    Wav * aWav)";
  gSoloudCalls["Wav"]["setVolume"].p = call_Wav_setVolume;
  gSoloudCalls["Wav"]["setVolume"].info = "void Wav_setVolume( \n    Wav * aWav,\n    float aVolume)";
  gSoloudCalls["Wav"]["setLooping"].p = call_Wav_setLooping;
  gSoloudCalls["Wav"]["setLooping"].info = "void Wav_setLooping( \n    Wav * aWav,\n    int aLoop)";
  gSoloudCalls["Wav"]["setAutoStop"].p = call_Wav_setAutoStop;
  gSoloudCalls["Wav"]["setAutoStop"].info = "void Wav_setAutoStop( \n    Wav * aWav,\n    int aAutoStop)";
  gSoloudCalls["Wav"]["set3dMinMaxDistance"].p = call_Wav_set3dMinMaxDistance;
  gSoloudCalls["Wav"]["set3dMinMaxDistance"].info = "void Wav_set3dMinMaxDistance( \n    Wav * aWav,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["Wav"]["set3dAttenuation"].p = call_Wav_set3dAttenuation;
  gSoloudCalls["Wav"]["set3dAttenuation"].info = "void Wav_set3dAttenuation( \n    Wav * aWav,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["Wav"]["set3dDopplerFactor"].p = call_Wav_set3dDopplerFactor;
  gSoloudCalls["Wav"]["set3dDopplerFactor"].info = "void Wav_set3dDopplerFactor( \n    Wav * aWav,\n    float aDopplerFactor)";
  gSoloudCalls["Wav"]["set3dListenerRelative"].p = call_Wav_set3dListenerRelative;
  gSoloudCalls["Wav"]["set3dListenerRelative"].info = "void Wav_set3dListenerRelative( \n    Wav * aWav,\n    int aListenerRelative)";
  gSoloudCalls["Wav"]["set3dDistanceDelay"].p = call_Wav_set3dDistanceDelay;
  gSoloudCalls["Wav"]["set3dDistanceDelay"].info = "void Wav_set3dDistanceDelay( \n    Wav * aWav,\n    int aDistanceDelay)";
  gSoloudCalls["Wav"]["set3dCollider"].p = call_Wav_set3dCollider;
  gSoloudCalls["Wav"]["set3dCollider"].info = "void Wav_set3dCollider( \n    Wav * aWav,\n    AudioCollider * aCollider)";
  gSoloudCalls["Wav"]["set3dColliderEx"].p = call_Wav_set3dColliderEx;
  gSoloudCalls["Wav"]["set3dColliderEx"].info = "void Wav_set3dColliderEx( \n    Wav * aWav,\n    AudioCollider * aCollider,\n    int aUserData)";
  gSoloudCalls["Wav"]["set3dAttenuator"].p = call_Wav_set3dAttenuator;
  gSoloudCalls["Wav"]["set3dAttenuator"].info = "void Wav_set3dAttenuator( \n    Wav * aWav,\n    AudioAttenuator * aAttenuator)";
  gSoloudCalls["Wav"]["setInaudibleBehavior"].p = call_Wav_setInaudibleBehavior;
  gSoloudCalls["Wav"]["setInaudibleBehavior"].info = "void Wav_setInaudibleBehavior( \n    Wav * aWav,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["Wav"]["setLoopPoint"].p = call_Wav_setLoopPoint;
  gSoloudCalls["Wav"]["setLoopPoint"].info = "void Wav_setLoopPoint( \n    Wav * aWav,\n    double aLoopPoint)";
  gSoloudCalls["Wav"]["getLoopPoint"].p = call_Wav_getLoopPoint;
  gSoloudCalls["Wav"]["getLoopPoint"].info = "double Wav_getLoopPoint( \n    Wav * aWav)";
  gSoloudCalls["Wav"]["setFilter"].p = call_Wav_setFilter;
  gSoloudCalls["Wav"]["setFilter"].info = "void Wav_setFilter( \n    Wav * aWav,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["Wav"]["stop"].p = call_Wav_stop;
  gSoloudCalls["Wav"]["stop"].info = "void Wav_stop( \n    Wav * aWav)";
  gSoloudCalls["WaveShaperFilter"]["destroy"].p = call_WaveShaperFilter_destroy;
  gSoloudCalls["WaveShaperFilter"]["destroy"].info = "void WaveShaperFilter_destroy( \n    WaveShaperFilter * aWaveShaperFilter)";
  gSoloudCalls["WaveShaperFilter"]["setParams"].p = call_WaveShaperFilter_setParams;
  gSoloudCalls["WaveShaperFilter"]["setParams"].info = "int WaveShaperFilter_setParams( \n    WaveShaperFilter * aWaveShaperFilter,\n    float aAmount)";
  gSoloudCalls["WaveShaperFilter"]["create"].p = call_WaveShaperFilter_create;
  gSoloudCalls["WaveShaperFilter"]["create"].info = "WaveShaperFilter * WaveShaperFilter_create()";
  gSoloudCalls["WaveShaperFilter"]["getParamCount"].p = call_WaveShaperFilter_getParamCount;
  gSoloudCalls["WaveShaperFilter"]["getParamCount"].info = "int WaveShaperFilter_getParamCount( \n    WaveShaperFilter * aWaveShaperFilter)";
  gSoloudCalls["WaveShaperFilter"]["getParamType"].p = call_WaveShaperFilter_getParamType;
  gSoloudCalls["WaveShaperFilter"]["getParamType"].info = "unsigned int WaveShaperFilter_getParamType( \n    WaveShaperFilter * aWaveShaperFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["WaveShaperFilter"]["getParamMax"].p = call_WaveShaperFilter_getParamMax;
  gSoloudCalls["WaveShaperFilter"]["getParamMax"].info = "float WaveShaperFilter_getParamMax( \n    WaveShaperFilter * aWaveShaperFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["WaveShaperFilter"]["getParamMin"].p = call_WaveShaperFilter_getParamMin;
  gSoloudCalls["WaveShaperFilter"]["getParamMin"].info = "float WaveShaperFilter_getParamMin( \n    WaveShaperFilter * aWaveShaperFilter,\n    unsigned int aParamIndex)";
  gSoloudCalls["WavStream"]["destroy"].p = call_WavStream_destroy;
  gSoloudCalls["WavStream"]["destroy"].info = "void WavStream_destroy( \n    WavStream * aWavStream)";
  gSoloudCalls["WavStream"]["create"].p = call_WavStream_create;
  gSoloudCalls["WavStream"]["create"].info = "WavStream * WavStream_create()";
  gSoloudCalls["WavStream"]["load"].p = call_WavStream_load;
  gSoloudCalls["WavStream"]["load"].info = "int WavStream_load( \n    WavStream * aWavStream,\n    const char * aFilename)";
  gSoloudCalls["WavStream"]["loadMem"].p = call_WavStream_loadMem;
  gSoloudCalls["WavStream"]["loadMem"].info = "int WavStream_loadMem( \n    WavStream * aWavStream,\n    const unsigned char * aData,\n    unsigned int aDataLen)";
  gSoloudCalls["WavStream"]["loadMemEx"].p = call_WavStream_loadMemEx;
  gSoloudCalls["WavStream"]["loadMemEx"].info = "int WavStream_loadMemEx( \n    WavStream * aWavStream,\n    const unsigned char * aData,\n    unsigned int aDataLen,\n    int aCopy,\n    int aTakeOwnership)";
  gSoloudCalls["WavStream"]["loadToMem"].p = call_WavStream_loadToMem;
  gSoloudCalls["WavStream"]["loadToMem"].info = "int WavStream_loadToMem( \n    WavStream * aWavStream,\n    const char * aFilename)";
  gSoloudCalls["WavStream"]["loadFile"].p = call_WavStream_loadFile;
  gSoloudCalls["WavStream"]["loadFile"].info = "int WavStream_loadFile( \n    WavStream * aWavStream,\n    File * aFile)";
  gSoloudCalls["WavStream"]["loadFileToMem"].p = call_WavStream_loadFileToMem;
  gSoloudCalls["WavStream"]["loadFileToMem"].info = "int WavStream_loadFileToMem( \n    WavStream * aWavStream,\n    File * aFile)";
  gSoloudCalls["WavStream"]["getLength"].p = call_WavStream_getLength;
  gSoloudCalls["WavStream"]["getLength"].info = "double WavStream_getLength( \n    WavStream * aWavStream)";
  gSoloudCalls["WavStream"]["setVolume"].p = call_WavStream_setVolume;
  gSoloudCalls["WavStream"]["setVolume"].info = "void WavStream_setVolume( \n    WavStream * aWavStream,\n    float aVolume)";
  gSoloudCalls["WavStream"]["setLooping"].p = call_WavStream_setLooping;
  gSoloudCalls["WavStream"]["setLooping"].info = "void WavStream_setLooping( \n    WavStream * aWavStream,\n    int aLoop)";
  gSoloudCalls["WavStream"]["setAutoStop"].p = call_WavStream_setAutoStop;
  gSoloudCalls["WavStream"]["setAutoStop"].info = "void WavStream_setAutoStop( \n    WavStream * aWavStream,\n    int aAutoStop)";
  gSoloudCalls["WavStream"]["set3dMinMaxDistance"].p = call_WavStream_set3dMinMaxDistance;
  gSoloudCalls["WavStream"]["set3dMinMaxDistance"].info = "void WavStream_set3dMinMaxDistance( \n    WavStream * aWavStream,\n    float aMinDistance,\n    float aMaxDistance)";
  gSoloudCalls["WavStream"]["set3dAttenuation"].p = call_WavStream_set3dAttenuation;
  gSoloudCalls["WavStream"]["set3dAttenuation"].info = "void WavStream_set3dAttenuation( \n    WavStream * aWavStream,\n    unsigned int aAttenuationModel,\n    float aAttenuationRolloffFactor)";
  gSoloudCalls["WavStream"]["set3dDopplerFactor"].p = call_WavStream_set3dDopplerFactor;
  gSoloudCalls["WavStream"]["set3dDopplerFactor"].info = "void WavStream_set3dDopplerFactor( \n    WavStream * aWavStream,\n    float aDopplerFactor)";
  gSoloudCalls["WavStream"]["set3dListenerRelative"].p = call_WavStream_set3dListenerRelative;
  gSoloudCalls["WavStream"]["set3dListenerRelative"].info = "void WavStream_set3dListenerRelative( \n    WavStream * aWavStream,\n    int aListenerRelative)";
  gSoloudCalls["WavStream"]["set3dDistanceDelay"].p = call_WavStream_set3dDistanceDelay;
  gSoloudCalls["WavStream"]["set3dDistanceDelay"].info = "void WavStream_set3dDistanceDelay( \n    WavStream * aWavStream,\n    int aDistanceDelay)";
  gSoloudCalls["WavStream"]["set3dCollider"].p = call_WavStream_set3dCollider;
  gSoloudCalls["WavStream"]["set3dCollider"].info = "void WavStream_set3dCollider( \n    WavStream * aWavStream,\n    AudioCollider * aCollider)";
  gSoloudCalls["WavStream"]["set3dColliderEx"].p = call_WavStream_set3dColliderEx;
  gSoloudCalls["WavStream"]["set3dColliderEx"].info = "void WavStream_set3dColliderEx( \n    WavStream * aWavStream,\n    AudioCollider * aCollider,\n    int aUserData)";
  gSoloudCalls["WavStream"]["set3dAttenuator"].p = call_WavStream_set3dAttenuator;
  gSoloudCalls["WavStream"]["set3dAttenuator"].info = "void WavStream_set3dAttenuator( \n    WavStream * aWavStream,\n    AudioAttenuator * aAttenuator)";
  gSoloudCalls["WavStream"]["setInaudibleBehavior"].p = call_WavStream_setInaudibleBehavior;
  gSoloudCalls["WavStream"]["setInaudibleBehavior"].info = "void WavStream_setInaudibleBehavior( \n    WavStream * aWavStream,\n    int aMustTick,\n    int aKill)";
  gSoloudCalls["WavStream"]["setLoopPoint"].p = call_WavStream_setLoopPoint;
  gSoloudCalls["WavStream"]["setLoopPoint"].info = "void WavStream_setLoopPoint( \n    WavStream * aWavStream,\n    double aLoopPoint)";
  gSoloudCalls["WavStream"]["getLoopPoint"].p = call_WavStream_getLoopPoint;
  gSoloudCalls["WavStream"]["getLoopPoint"].info = "double WavStream_getLoopPoint( \n    WavStream * aWavStream)";
  gSoloudCalls["WavStream"]["setFilter"].p = call_WavStream_setFilter;
  gSoloudCalls["WavStream"]["setFilter"].info = "void WavStream_setFilter( \n    WavStream * aWavStream,\n    unsigned int aFilterId,\n    Filter * aFilter)";
  gSoloudCalls["WavStream"]["stop"].p = call_WavStream_stop;
  gSoloudCalls["WavStream"]["stop"].info = "void WavStream_stop( \n    WavStream * aWavStream)";
}
