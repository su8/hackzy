This is Qt 6 version of **hackzy**. 

---

# Compile with:

```bash
cmake
make -j8 # 8 cores to use in parallel compile
sudo make install # if on linux or *BSD
```

Open Qt Creator and import this project folder if you want to edit/compile **hackzy** from the Qt Creator itself. To compile and run the program from Qt Creator, press **CTRL** + **r**.

If on windows, you'll have to download the online installer and install qt 6 manually from the GUI installer.

If on windows, replace **srandom** with **srand**:

https://github.com/su8/hackzy/blob/0c4ccd0b06954cedc8ca28cc9f4006da4ea79815/qt6_GUI/mainwindow.cpp#L326