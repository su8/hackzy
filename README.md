[![C/C++ build](https://github.com/su8/hackzy/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/su8/hackzy/actions/workflows/c-cpp.yml) [![CodeQL](https://github.com/su8/hackzy/actions/workflows/codeql.yml/badge.svg)](https://github.com/su8/hackzy/actions/workflows/codeql.yml) [![flawfinder](https://github.com/su8/hackzy/actions/workflows/flawfinder.yml/badge.svg)](https://github.com/su8/hackzy/actions/workflows/flawfinder.yml)

![](qt6_GUI/media/snap.jpg)

The picture above does **NOT** represent this game.

Terminal hacker and  **GUI** game. For the GUI version click on [qt6_GUI](https://github.com/su8/hackzy/tree/main/qt6_GUI) folder and compile it according to the README.md docs.

## Installation

If on windows, replace **srandom** with **srand**.

https://github.com/su8/hackzy/blob/a69c66e252bd5e9f78ca2f4a6498edd9318e747e/main.cpp#L304

Tested with [Visual Studio Code Editor](https://code.visualstudio.com/download), but you need to install [MingW](https://github.com/niXman/mingw-builds-binaries/releases/download/12.2.0-rt_v10-rev0/x86_64-12.2.0-release-posix-seh-rt_v10-rev0.7z) once downloaded extract it to **C:\MingW**, then re-open [Visual Studio Code Editor](https://code.visualstudio.com/download), you might want to install C\C++ extensions if you plan to write C\C++ code with the editor. If you plan to contribute to this project go to **File->Preferences->Settings** and type to search for **cppStandard** and set it to c17 to both C++ and C.

I use **One Monokai** theme for the [VScode Editor](https://code.visualstudio.com/download)

In [Visual Studio Code Editor](https://code.visualstudio.com/download), go to **Terminal->Configure Tasks...->Create tasks.json from template** and copy and paste this into it:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
        "type": "cppbuild",
        "label": "C/C++",
        "command": "C:\\MingW\\bin\\g++.exe",
        "args": [
            "-fdiagnostics-color=always",
            "-std=c++17",
            "-ggdb",
            "-lpthread",
            "-Wall",
            "-Wextra",
            "-O2",
            "-pipe",
            "-pedantic",
            "-Wundef",
            "-Wshadow",
            "-W",
            "-Wwrite-strings",
            "-Wcast-align",
            "-Wstrict-overflow=5",
            "-Wconversion",
            "-Wpointer-arith",
            "-Wformat=2",
            "-Wsign-compare",
            "-Wendif-labels",
            "-Wredundant-decls",
            "-Winit-self",
            "${file}",
            "-o",
            "${fileDirname}/${fileBasenameNoExtension}"
        ],
        "options": {
            "cwd": "C:\\MingW\\bin"
        },
        "problemMatcher": [
            "$gcc"
        ],
        "group": {
            "kind": "build",
            "isDefault": true
        },
        "detail": "compiler: C:\\MingW\\bin\\g++.exe"
    }
]
}
```

To compile the program press **CTRL** + **SHIFT** + **B** , wait until it compiles, after that press **CTRL** + **\`** and paste this `cp -r C:\Users\YOUR_USERNAME_GOES_HERE\Desktop\main.exe C:\MingW\bin;cd C:\MingW\bin;.\main.exe`

If on Linux or *BSD

```bash
make -j8 # 8 cores to use in parallel compile, you can add your cpu threads too
sudo make install
```

---

I was inspired by https://github.com/The-Council-of-Wills/HacknetPlusPlus to create this game. I do use their **help** text.

---

The game was entirely written from my tablet with **CxxDroid** up until the 59th commit, afterwards the game is written from my brand new windows laptop.
