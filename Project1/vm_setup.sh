#!/bin/bash
# VM Setup Script for Project 1
# Run this script on your Ubuntu VM after transferring files
#
# Usage: chmod +x vm_setup.sh && ./vm_setup.sh

echo "=========================================="
echo "  Project 1 VM Setup Script"
echo "=========================================="

# Step 1: Install required tools if not present
echo ""
echo "[Step 1] Installing build tools..."
sudo apt-get update
sudo apt-get install -y make gcc dos2unix

# Step 2: Convert files from DOS to Unix format
echo ""
echo "[Step 2] Converting line endings to Unix format..."
dos2unix list.h list.c list_test.c Makefile README.txt 2>/dev/null || echo "dos2unix not needed or files already Unix format"

# Step 3: Build the project
echo ""
echo "[Step 3] Building the project with make..."
make clean 2>/dev/null
make

# Check if build was successful
if [ $? -eq 0 ]; then
    echo ""
    echo "[SUCCESS] Build completed without errors!"
    echo ""
    echo "[Step 4] Running tests..."
    echo "=========================================="
    ./list_test
    echo "=========================================="
    echo ""
    echo "[DONE] All tests completed!"
else
    echo ""
    echo "[ERROR] Build failed! Check the error messages above."
    exit 1
fi
