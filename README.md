# TTF file reader
Simple `.ttf` font viewer I wrote out of pure curiosity.

TTF file parsing is implemented manually and based on [Apple TrueType Reference Manual](https://developer.apple.com/fonts/TrueType-Reference-Manual). 
Rendering, input processing and IO is done using [SDL2](https://github.com/libsdl-org/SDL/tree/SDL2).

## Building
Don't forget to download `SDL` submodule before (one-time action):
```bash
git submodule update --init --recursive
```
This is a regular `CMake` project so just create a `build` directory for build files:
```bash
mkdir build
cd build
cmake ..
cmake --build .
```

## Usage
1) Drag&Drop `.ttf` file into window
2) Use:
	* `WASD` to move glyph around (`Shift` to to if faster)
	* `LKIO` to show/hide straight lines, beziers, control lines of beziers or em-boxes.
	* Left/Right arrows to change glyphs
	* Up/Down arrows to change glyph size (`Shift` to to if faster)