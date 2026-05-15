#!/bin/bash
rm -rf bin/
mkdir -p bin/
gcc src/*.c -o bin/main.exe -Wall -Iinclude && echo "[OK] Build success" && ./bin/main.exe || echo "[ERROR] Build failed"