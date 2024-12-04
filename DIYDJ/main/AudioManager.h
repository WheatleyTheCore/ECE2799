#include <Audio.h>
#include "Arduino.h"

extern Audio audio;

struct AudioState
{
  string head_track;
  string top_track;
  string bottom_track;
};

class AudioManager
{
public:
  struct AudioState nextState;
  AudioManager();
  int initialize();
  struct AudioState updateTracks(struct AudioState newAudioState);
  void audioLoop();
  string getTrackFromState();
  string trackToPlay;

private:
  struct AudioState currentState; // should technically be private with getter and setter function
  string currentlyPlayingTrack;
};
