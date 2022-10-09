[![C/C++ build](https://github.com/su8/hackzy/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/su8/hackzy/actions/workflows/c-cpp.yml)

[![CodeQL](https://github.com/su8/hackzy/actions/workflows/codeql.yml/badge.svg)](https://github.com/su8/hackzy/actions/workflows/codeql.yml)

![](snap.jpg)

The picture above does **NOT** represent this game.

Terminal hacker game.

## Installation

If on windows, replace **srandom** with **srand**.

https://github.com/su8/hackzy/blob/c60b6969460573096d83b9640c882abe0802d638/main.cpp#L304

Tested with Visual Studio Code Editor, but you need to install MingW - https://github.com/niXman/mingw-builds-binaries/releases/download/12.2.0-rt_v10-rev0/x86_64-12.2.0-release-posix-seh-rt_v10-rev0.7z once downloaded extract it to **C:\MingW**, then re-open Visual Studio Code Editor, you might want to install C\C++ extensions if you plan to write C\C++ code with the editor. If you plan to contribute to this project go to **File->Preferences->Settings** and type to search for **cppStandard** and set c++ to c17 to both c++ and C

If on Linux or *BSD

`
make -j8 && 
sudo make install
`

---

I was inspired by https://github.com/The-Council-of-Wills/HacknetPlusPlus to create this game. I do use their **help** text.

---

The game is entirely written from my tablet with CxxDroid, only true up until to the 59th commit, afterwards the game is written from my brand new windows laptop.
