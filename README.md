# cliArcade
> Play retro games in your linux terminal.
### Why?
there's hundreds of similar projects out there but what makes this one unique is the ability to choose from 6 different rendering resolutions.
### Video Demos
> [!TIP]
> If the output doesn't look right it is probably because of the font your terminal is using, use one of these recommended terminal emulators instead: [kitty](https://sw.kovidgoyal.net/kitty), [wizterm](https://wezfurlong.org/wezterm), [foot](https://codeberg.org/dnkl/foot).

|[![vid](https://img.youtube.com/vi/ZEhrxV3D07o/default.jpg)](https://youtu.be/ZEhrxV3D07o)<br>Snake|[![vid](https://img.youtube.com/vi/YGyJRs-reIk/default.jpg)](https://youtu.be/YGyJRs-reIk)<br>Oscillators|
|:-:|:-:|
|[![vid](https://img.youtube.com/vi/GkgLRtLd3XY/default.jpg)](https://youtu.be/GkgLRtLd3XY)<br>Manual Entry|[![vid](https://img.youtube.com/vi/2-pmjYV7ReA/default.jpg)](https://youtu.be/2-pmjYV7ReA)<br>Replicator
### How Does It Work?
I was inspired by [the unicode implementation of braille characters](https://en.wikipedia.org/wiki/Braille_Patterns), So I followed a similar approach and made a lookup array to index into:

https://github.com/3m4r5/cliArcade/blob/d3e2cef8b0cd7963b4133203e4a9cc37649577e0/terminal.cpp#L151-L160

<details><summary>Project File Structure</summary>

> [!NOTE]
> The entire project results in a single object file because this project is small and the compile time is not an issue.

```
cliArcade
 ├─README.md
 ├─main.cpp
 ├─terminal.cpp          // Utilities for interacting with the terminal.
 ├─baseMenu.cpp          // A base class for creating a selection menu.
 ├─mainMenu.cpp          // A menu for choosing the game.
 ├─resolutionMenu.cpp    // A menu for choosing the resolution.
 ├─games.h               // Included games.
 └─games
    ├─snake
    │  └─snake.cpp
    └─gameOfLife
       ├─gameOfLife.cpp
       ├─patternMenu.cpp // A menu to choose pattern importing method.
       ├─fileMenu.cpp    // A file filter and picker.
       ├─formatMenu.cpp  // A menu for choosing the input format.
       ├─decode.cpp      // Methods for decoding pattern formats.
       └─pattern_files/
```
</details><details><summary>Build Instructions</summary>

\
1- Download the source code:
```sh
wget https://github.com/3m4r5/cliArcade/archive/refs/heads/main.zip
```
2- unzip the downloaded file:
```sh
unzip main.zip
```
and optionally remove the compressed archive:
```sh
rm main.zip
```
3- cd into the main directory:
```sh
cd cliArcade-main/
```
4- compile:
```sh
g++ main.cpp -o cliArcade
```
or
```sh
clang++ main.cpp -o cliArcade
```
5- run the executable:
```sh
./cliArcade
```
</details><details><summary>Roadmap</summary>

Here's several improvements that could be made:
- Support more platforms (windows, macos).
- Add more games like tetris or pacman.
- Support more [file formats](https://conwaylife.com/wiki/File_formats):
- [x] .cells
- [x] .rle
- [ ] life 1.05
- [ ] life 1.06
- [ ] apgcode
- [ ] .mc
- [ ] .mcl
- [ ] .plf
- [ ] .l
- [ ] .rule

- [store neighbors count for each cell](https://youtu.be/dAfWKmKF34).
- Add support for color theming using ansii escape codes.
- Utilize the gpu for parallelization.
</details>
