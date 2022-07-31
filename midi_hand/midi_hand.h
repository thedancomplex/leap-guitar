// midiout.cpp
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include "RtMidi.h"

/*
int midi_cleanup();
int midi_init();
int midi_send();
*/




//class MidiSender : public MidiSend {
class MidiSender {
  public:
    virtual int midi_init(void);
    virtual int midi_send(void);
    virtual int midi_cleanup(void);
  private:
    RtMidiOut *midiout;
};

int MidiSender::midi_init()
{
  //RtMidiOut *midiout = new RtMidiOut();
  // Check available ports.
  std::cout << "start midi init" << std::endl;
  unsigned int nPorts = this->midiout->getPortCount();
  std::cout << "start midi init part 1" << std::endl;
  if ( nPorts == 0 ) {
    std::cout << "No ports available!\n";
    midi_cleanup();
  }
  std::cout << "start midi init part 2" << std::endl;
  // Open first available port.
  this->midiout->openPort( 0 );
  std::cout << "Opened Midi port 0 - "
  << std::endl;
  return 0;
}

int MidiSender::midi_send()
{
  std::vector<unsigned char> message;
  // Send out a series of MIDI messages.
  // Program change: 192, 5
  message.push_back( 192 );
  message.push_back( 5 );
  this->midiout->sendMessage( &message );
  /*
  // Control Change: 176, 7, 100 (volume)
  message[0] = 176;
  message[1] = 7;
  message.push_back( 100 );
  midiout->sendMessage( &message );
  // Note On: 144, 64, 90
  message[0] = 144;
  message[1] = 64;
  message[2] = 90;
  midiout->sendMessage( &message );

  usleep( 500000 ); // Platform-dependent ... see example in tests directory.
  // Note Off: 128, 64, 40
  message[0] = 128;
  message[1] = 64;
  message[2] = 40;
  midiout->sendMessage( &message );
  */
  return 0;
}

int MidiSender::midi_cleanup()
{
  delete this->midiout;
  return 0;
}
