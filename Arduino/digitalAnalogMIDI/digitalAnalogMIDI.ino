/* Buttons to USB MIDI Example + AnalogControlChange

   You must select MIDI from the "Tools > USB Type" menu
*/

#include <Bounce.h>

// the MIDI channel number to send messages
const int channel = 1;

// the MIDI continuous controller for each analog input
const int controllerA0 = 10;

// Create Bounce objects for each button.  The Bounce object
// automatically deals with contact chatter or "bounce", and
// it makes detecting changes very simple.
Bounce button0 = Bounce(0, 5);

void setup() {
  // Configure the pins for input mode with pullup resistors.
  pinMode(0, INPUT_PULLUP);
}

// store previously sent values, to detect changes
int previousA0 = -1;

// variable to hold time of last read:
elapsedMillis msec = 0;

void loop() {
  // DIGITAL INPUT
  // update button and send NoteOn/Off message if needed

  // Update the button
  button0.update();

  // Check  button for "falling" edge.
  // Send a MIDI Note On message when each button presses
  if (button0.fallingEdge()) {
    usbMIDI.sendNoteOn(60, 99, channel);  // 60 = C4
  }

  // Check each button for "rising" edge
  // Send a MIDI Note Off message when each button releases
  if (button0.risingEdge()) {
    usbMIDI.sendNoteOff(60, 0, channel);  // 60 = C4
  }

  // ANALOG INPUT
  // Analog read and send CC value

  // only check the analog inputs 50 times per second,
  // to prevent a flood of MIDI messages
  if (msec >= 20) {
    msec = 0; // reset timer

    // analogRead will return a value between 0:1023 (10-bits) divide by 8 and it becomes 0:127
    int n0 = analogRead(A0) / 8;

    // only transmit MIDI messages if analog input changed
    if (n0 != previousA0) {
      usbMIDI.sendControlChange(controllerA0, n0, channel);
      previousA0 = n0;
    }

    // MIDI Controllers should discard incoming MIDI messages.
    // http://forum.pjrc.com/threads/24179-Teensy-3-Ableton-Analog-CC-causes-midi-crash
    while (usbMIDI.read()) {
      // ignore incoming messages
    }
  }
}
