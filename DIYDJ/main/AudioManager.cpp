#include "AudioManager.h"

AudioManager::AudioManager()
{
  this->trackToPlay = "";
  this->currentlyPlayingTrack = "top1";
  this->currentState.head_track = "";
  this->currentState.top_track = "";
  this->currentState.bottom_track = "";
  nextState.head_track = "";
  nextState.top_track = "";
  nextState.bottom_track = "";
  audio.setVolume(21);
}

struct AudioState AudioManager::updateTracks(struct AudioState newAudioState)
{
  bool statesAreTheSame = (this->currentState.head_track == newAudioState.head_track) && (this->currentState.top_track == newAudioState.top_track) && (this->currentState.bottom_track == newAudioState.bottom_track);
  if (statesAreTheSame)
  {
    return this->currentState;
  } else {
    this->currentState = newAudioState;
    this->trackToPlay = getTrackFromState();
    return newAudioState;
  }
}

string AudioManager::getTrackFromState() {
  string rawHeadName, rawTopName, rawBottomName;

  if (this->currentState.head_track == "") {
    rawHeadName = "";
  } else {
    size_t headlastindex = this->currentState.head_track.find_last_of(".");
    rawHeadName = this->currentState.head_track.substr(0, headlastindex);
  }
  if (this->currentState.top_track == "") {
    rawTopName = "";
  } else {
    size_t toplastindex = this->currentState.top_track.find_last_of(".");
    rawTopName = this->currentState.top_track.substr(0, toplastindex);
  }
  if (this->currentState.bottom_track == "") {
    rawBottomName = "";
  } else {
    size_t bottomlastindex = this->currentState.bottom_track.find_last_of(".");
    rawBottomName = this->currentState.bottom_track.substr(0, bottomlastindex);
  }

  return (rawHeadName + rawTopName + rawBottomName + ".mp3");
}

void AudioManager::audioLoop() {
  audio.loop();
  if (!audio.isRunning()) {
    // we have hit the end of the loop!!!
    //    string nextTrackToPlay = getTrackFromState();
    if (trackToPlay.c_str() == "" && (currentlyPlayingTrack != "")) {
      audio.stopSong();
      this->currentlyPlayingTrack = "";
    } else {
      audio.connecttoFS(SD, trackToPlay.c_str());
      this->currentlyPlayingTrack = trackToPlay;

    }
  }
}
