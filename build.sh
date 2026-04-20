#!/bin/bash
gcc *.c -o main -Wall && echo "[OK] Build success" && ./main || echo "[ERROR] Build failed"