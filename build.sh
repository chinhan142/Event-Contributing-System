#!/bin/bash
rm -f *.o main.exe
gcc *.c -o main.exe -Wall && echo "[OK] Build success" && ./main.exe || echo "[ERROR] Build failed"