#!/bin/sh
set -e

mkdir -p output
gcc test_staff.c ../staff.c ../user.c ../auth.c ../event.c ../fileio.c ../init.c ../menuBCN.c ../menuStaff.c ../report.c ../utils.c -o output/test_staff.exe -Wall
./output/test_staff.exe
