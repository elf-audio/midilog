# midilog
Command line utility that runs on the mac that lists available midi ports and prints out midi messages as they are received. It's mostly copy/pasted from the excellent RtMidi tutorial, with a little help also from [ofxMidi](https://github.com/danomatika/ofxMidi)

To compile, download or clone the repo then,
```
$ cd to/midilog/dir
$ git submodule init
$ git submodule update
$ make
```

The executable will appear in same dir called midilog. 


## Usage

```
$ midilog [n] [0|1]
```

Where n is the port number - to find out the port number (optional), run `midilog` with no arguments to list and enumerate all ports. The 0 or 1 at the end is optional and turns off or on exta messages like sysex/timing/active sensing.


eg. typing,
`$ midilog` by itself will just list the ports. 
`$ midilog 4` will open the 4th port, and 
`$midilog 3 1` will open the third port, and print out everything including sysex, timing + active sensing.


Output looks like this for listing inputs: 

```
$ midilog
```

```
There are 6 MIDI inputs.
[0]: IAC Driver Bus 1
[1]: UltraLite mk3 Hybrid MIDI Port
[2]: UltraLite mk3 Hybrid Sync Port
[3]: MidiStomp
[4]: from Max 1
[5]: from Max 2
```

And like this for live midi messages (e.g. opening UltraLite and showing all data)

```
$ midilog 1 1
```

```
PITCH BEND        0xe0	0x00	0x69	ch: 1   bend: 13440
NOTE ON           0x90	0x26	0x08	ch: 1   note: 38   vol: 8
NOTE OFF          0x80	0x26	0x00	ch: 1   note: 38   vol: 0
NOTE ON           0x90	0x26	0x7b	ch: 1   note: 38   vol: 123
NOTE OFF          0x80	0x26	0x00	ch: 1   note: 38   vol: 0
NOTE ON           0x90	0x26	0x1f	ch: 1   note: 38   vol: 31
NOTE ON           0x90	0x28	0x06	ch: 1   note: 40   vol: 6
NOTE OFF          0x80	0x26	0x00	ch: 1   note: 38   vol: 0
NOTE OFF          0x80	0x28	0x00	ch: 1   note: 40   vol: 0
NOTE ON           0x90	0x29	0x49	ch: 1   note: 41   vol: 73
CONTROL CHANGE    0xb0	0x05	0x3d	ch: 1   ctrl: 5   val: 61
CONTROL CHANGE    0xb0	0x05	0x3c	ch: 1   ctrl: 5   val: 60
CONTROL CHANGE    0xb0	0x05	0x3b	ch: 1   ctrl: 5   val: 59
CONTROL CHANGE    0xb0	0x05	0x3a	ch: 1   ctrl: 5   val: 58
CONTROL CHANGE    0xb0	0x05	0x39	ch: 1   ctrl: 5   val: 57
PROGRAM CHANGE    0xc0	0x03	ch: 1   program: 3
PROGRAM CHANGE    0xc0	0x00	ch: 1   program: 0
PROGRAM CHANGE    0xc0	0x02	ch: 1   program: 2
CONTINUE          0xfb	
TIME CLOCK        0xf8	
TIME CLOCK        0xf8	
STOP              0xfc	
START             0xfa	

```