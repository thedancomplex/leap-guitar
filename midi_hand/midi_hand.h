// midiout.cpp
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include "RtMidi.h"

RtMidiOut *midiout;

int midi_cleanup();
int midi_init();
int midi_send();

//class MidiSender : public MidiSend {
class MidiSender {
  public:
    virtual int midi_init(void);
    virtual int midi_send(void);
    virtual int midi_cleanup(void);
  private:
};

int MidiSender::midi_init()
{
  RtMidiOut *midiout = new RtMidiOut();
  // Check available ports.
  unsigned int nPorts = midiout->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No ports available!\n";
    midi_cleanup();
  }
  // Open first available port.
  midiout->openPort( 0 );
  return 0;
}

int MidiSender::midi_send()
{
  std::vector<unsigned char> message;
  // Send out a series of MIDI messages.
  // Program change: 192, 5
  message.push_back( 192 );
  message.push_back( 5 );
  midiout->sendMessage( &message );
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
  return 0;
}

int MidiSender::midi_cleanup()
{
  delete midiout;
  return 0;
}
