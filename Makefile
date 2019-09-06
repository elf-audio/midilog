

all:
	g++ main.cpp -Irtmidi -c -o main.o
	g++ rtmidi/RtMidi.cpp -D__MACOSX_CORE__ -c -o rtmidi.o
	g++ main.o rtmidi.o -framework CoreMIDI -framework CoreFoundation -framework CoreAudio -o midilog
	rm *.o