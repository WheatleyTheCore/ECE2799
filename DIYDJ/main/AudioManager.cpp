#include "AudioManager.h"

AudioManager::AudioManager()
{
  
  audio = new Audio(true, I2S_DAC_CHANNEL_BOTH_EN);
  currentState.head_track = "";
  currentState.top_track = "";
  currentState.bottom_track = "";
  nextState.head_track = "";
  nextState.top_track = "";
  nextState.bottom_track = "";
  return 1;
}

AudioManager::updateTracks(struct AudioState current, struct AudioState next)
{
  bool statesAreTheSame = curent.head_track = next.head_track &&curent.top_track = next.top_track &&curent.bottom_track = next.bottom_track;
  if (statesAreTheSame)
  {
    return current;
  } else {
    currentState = next;
  }
}

AudioManager::getTrackFromState() {
  size_t headlastindex = currentState.head_track.find_last_of("."); 
  String rawHeadName = currentState.head_track.substr(0, headlastindex); 

  size_t toplastindex = currentState.top_track.find_last_of("."); 
  String rawTopName = currentState.top_track.substr(0, toplastindex); 

  size_t bottomlastindex = currentState.bottom_track.find_last_of("."); 
  String rawBottomName = currentState.bottom_track.substr(0, bottomlastindex); 

  return rawHeadName + rawTopName + rawBottomName + ".mp3"
}

AudioManager::audioLoop() {
  audio.loop();
  if (!audio.isRunning()) {
    // we have hit the end of the loop!!!
    String nextTrackToPlay = getTrackFromState();
    audio.connecttoFS(SD, nextTrackToPlay); 
  }
}
