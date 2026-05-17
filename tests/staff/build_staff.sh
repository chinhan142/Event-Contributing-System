#!/bin/sh
set -e

ROOT_DIR="$(cd "$(dirname "$0")/../.." && pwd)"
cd "$ROOT_DIR"

mkdir -p tests/staff/output
gcc tests/staff/test_staff.c \
  src/staff.c src/user.c src/auth.c src/event.c src/fileio.c \
  src/init.c src/menuBCN.c src/menuStaff.c src/report.c \
  src/utils.c src/menuSearch.c \
  -Iinclude -o tests/staff/output/test_staff.exe -Wall
./tests/staff/output/test_staff.exe
