#!/bin/bash
rm -f *.o main.exe

gcc src/*.c -Iinclude -o main.exe -Wall \
&& echo "[OK] Build success" \
&& ./main.exe \
|| echo "[ERROR] Build failed"