
#include "RtMidi.h"
#include <stdio.h>
#include <memory>
#include <unistd.h>
using namespace std;





#define MIDI_NOTE_OFF            0x80
#define MIDI_NOTE_ON             0x90
#define MIDI_CONTROL_CHANGE      0xB0
#define MIDI_PROGRAM_CHANGE      0xC0
#define MIDI_PITCH_BEND          0xE0
#define MIDI_AFTERTOUCH          0xD0
#define MIDI_POLY_AFTERTOUCH     0xA0

#define MIDI_SYSEX               0xF0
#define MIDI_TIME_CODE           0xF1
#define MIDI_SONG_POS_POINTER    0xF2
#define MIDI_SONG_SELECT         0xF3
#define MIDI_TUNE_REQUEST        0xF6
#define MIDI_SYSEX_END           0xF7
#define MIDI_TIME_CLOCK          0xF8
#define MIDI_START               0xFA
#define MIDI_CONTINUE            0xFB
#define MIDI_STOP                0xFC
#define MIDI_ACTIVE_SENSING      0xFE
#define MIDI_SYSTEM_RESET        0xFF

string getStatusString(int status) {
	switch(status) {
		case MIDI_NOTE_OFF        : return "NOTE OFF          ";
		case MIDI_NOTE_ON         : return "NOTE ON           ";
		case MIDI_CONTROL_CHANGE  : return "CONTROL CHANGE    ";
		case MIDI_PROGRAM_CHANGE  : return "PROGRAM CHANGE    ";
		case MIDI_PITCH_BEND      : return "PITCH BEND        ";
		case MIDI_AFTERTOUCH      : return "AFTERTOUCH        ";
		case MIDI_POLY_AFTERTOUCH : return "POLY AFTERTOUCH   ";
		case MIDI_SYSEX           : return "SYSEX             ";
		case MIDI_TIME_CODE       : return "TIME CODE         ";
		case MIDI_SONG_POS_POINTER: return "SONG POS POINTER  ";
		case MIDI_SONG_SELECT     : return "SONG SELECT       ";
		case MIDI_TUNE_REQUEST    : return "TUNE REQUEST      ";
		case MIDI_SYSEX_END       : return "SYSEX END         ";
		case MIDI_TIME_CLOCK      : return "TIME CLOCK        ";
		case MIDI_START           : return "START             ";
		case MIDI_CONTINUE        : return "CONTINUE          ";
		case MIDI_STOP            : return "STOP              ";
		case MIDI_ACTIVE_SENSING  : return "ACTIVE SENSING    ";
		case MIDI_SYSTEM_RESET    : return "SYSTEM RESET      ";
		default: 				    return "(UNKNOWN)         ";
	}

}



bool showExtras = false;

void printMidiDevices() {
	// RtMidiIn constructor
	shared_ptr<RtMidiIn> midiIn;
	try {
		midiIn = std::make_shared<RtMidiIn>();
	}catch ( RtMidiError &error ) {
		error.printMessage();
		exit( EXIT_FAILURE );
	}

	// Check inputs.
	uint32_t nPorts = midiIn->getPortCount();
	printf("There are %u MIDI inputs.\n", nPorts);
	for ( unsigned int i=0; i<nPorts; i++ ) {
		try {
			auto portName = midiIn->getPortName(i);
			printf("[%u]: %s\n", i, portName.c_str());
		} catch ( RtMidiError &error ) {
			error.printMessage();
		}
	}
}


void printExtras(const vector<uint8_t> &msg, uint8_t status, uint8_t channel) {


	if(status<MIDI_SYSEX) {
		printf("ch: %u   ", channel);
	}
	switch(status) {
		case MIDI_NOTE_ON:
		case MIDI_NOTE_OFF:
			printf("note: %u   vol: %u", msg[1], msg[2]);
			break;
		case MIDI_CONTROL_CHANGE:
			printf("ctrl: %u   val: %u", msg[1], msg[2]);
			break;

		case MIDI_PROGRAM_CHANGE:
			printf("program: %u", msg[1]);
			break;

		case MIDI_PITCH_BEND: {
			int16_t bend =  (int) (msg[2] << 7) + (int) msg[1];
			printf("bend: %d", bend);
		}
		break;

		case MIDI_AFTERTOUCH:
			printf("pressure: %u", msg[1]);
			break;

		case MIDI_POLY_AFTERTOUCH:
			printf("note: %u  pressure: %u", msg[1], msg[2]);
			break;
	}
}

void runMidiLog(int inPort) {
	auto midiIn = std::make_shared<RtMidiIn>();
	midiIn->openPort(inPort);
	
	// Don't ignore sysex, timing, or active sensing messages.
	midiIn->ignoreTypes(!showExtras, !showExtras, !showExtras);

	vector<uint8_t> message;

	while(1) {
		auto stamp = midiIn->getMessage( &message );
		auto numBytes = message.size();
		
		if(numBytes>0) {
			uint8_t status = 0;
			uint8_t channel = 0;
			if(message[0] >= MIDI_SYSEX) {
				status = message[0] & 0xFF;
			} else {
				status = message[0] & 0xF0;
				channel = (int) (message[0] & 0x0F)+1;
			}

			printf("%s", getStatusString(status).c_str());
			for (int i = 0; i < numBytes; i++) {

				printf("0x%02x\t", message[i]);
			}
			printExtras(message, status, channel);
			printf("\n");
		}

		usleep(1000*10);
	}
}


void printUsage(char *c) {
	printf("\nusage: \n"
		"\t%s [n] [0|1]\n\n"
		"Where n is the port number - to find out the port number,\n"
		"run %s with no arguments to list and enumerate all ports.\n"
		"the 0 or 1 is optional and turns off or on exta messages\n"
		"like sysex/timing/active sensing.", c, c);
}

int main(int argc, char **argv) {
	if(argc==1) {
		printMidiDevices();
	} else if(argc>1) {
		int index = 0;
		try {
			index = stoi(argv[1]);
		} catch(std::invalid_argument &e) {
			printf("Invalid argument - must be a number\n");
			printUsage(argv[0]);
			return 1;
		}
		if(argc>2) {
			showExtras = (argv[2][0]=='1');
		}

		runMidiLog(index);		
	} else {
		printUsage(argv[0]);
		return 1;
	}
	return 0;
}
