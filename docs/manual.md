# dmxCanvas Manual

## Installation
Get the latest version of dmxCanvas from the [Releases page](https://github.com/arneBersch/dmxCanvas/releases/) and make it executable.
You can find the DMX charts for dmxCanvas [here](dmxCharts.md).

## Input
For setting up sACN input, you only need to set the sACN universe.
When dmxCanvas receives a sACN signal, it will show the Name and the IP Address of the Source.

## Virtual Beams
For setting up a Virtual Beam, just follow these steps:
1. Add an Object.
2. Set the Object's type to "Virtual Beam" and the required Mode.
3. Set the Object's address (just like a physical lighting fixture).
4. Check for any incoming sACN packets.
5. Open the Canvas.

## Images
For setting up a Image, follow these steps:
1. Add an Object.
2. Set the Object's type to "Image" and the required Mode.
3. Set the Object's address (just like a physical lighting fixture).
4. Select your Image folder (which has to contain Images numbered like 1.png, 2.jpeg, 3.svg, ...).
5. Check for any incoming sACN packets.
6. Open the Canvas.
