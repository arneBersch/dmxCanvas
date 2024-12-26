# dmxCanvas
dmxCanvas is a Free and Open Source DMX Media Server software.

It currently only runs on Ubuntu Linux, however, AppImage and Windows support is planned, too.

See the [Releases page](https://github.com/arneBersch/dmxCanvas/releases/) to download the latest version.

## Features
- Unlimited Virtual Beams with Pan, Tilt, Zoom and CMY Color Mixing.
- Control via sACN (ESTA E1.31) input.
- Patch any Object to any DMX address.

## Planned Features
- other Virtual Beam features (like gobo, frost, prism, shutter, framing)
- ArtNet input
- Image Viewer Objects
- Video Player Objects

## Known Limitations
- Input is only possible in sACN Universe 1
- Some sACN features are not implemented yet (like priority or synchronization) and will therefore not work.

## How to contribute
Please DO NOT post feature requests.
I am aware that there are many things that you can't do with dmxCanvas (see the Known Limitations and Planned Features above), but most of them will get implemented over time.
However, you can help by testing dmxCanvas, reporting bugs in the Issues tab and therefore making this software more stable.
Please check this points before creating an Issue:
1. First of all, make sure you're using the latest version of dmxCanvas.
    In case you don't, upgrade dmxCanvas and check if the problem still occurs.
2. Always check the Changelog and the open Issues.
    Did somebody report this bug yet? 
3. Try to reduce the number of steps needed for recreating the bug to a minimal amount.
    If possible, provide step-by-step instructions.
4. Open a new Issue and describe the problem as accurate as possible.

## Copyright and License
Copyright (c) Arne Bersch (zoeglfrex-dmx@web.de)

dmxCanvas is licensed under the GNU General Public License 3.0
