#include "RtMidi.h"

void printMidiDevices() {
	try {
		RtMidiIn midiin;
	} catch (RtMidiError &error) {
		// Handle the exception here
		error.printMessage();
	}
}


int main(int argc, char **argv) {
	
	printMidiDevices();
	return 0;
}