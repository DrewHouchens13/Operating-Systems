#!/bin/bash
# VM Setup Script for Project 2
# Run this script on your Ubuntu VM after transferring files.
#
# Usage: chmod +x vm_setup.sh && ./vm_setup.sh

echo "=========================================="
echo "  Project 2 VM Setup Script"
echo "=========================================="

# Step 1: Install required tools
echo ""
echo "[Step 1] Installing build tools and kernel headers..."
sudo apt-get update
sudo apt-get install -y gcc-12 make dos2unix
sudo apt-get install -y linux-headers-$(uname -r)

# Step 2: Convert files from DOS to Unix format
echo ""
echo "[Step 2] Converting line endings to Unix format..."
find . -name "*.c" -o -name "Makefile" -o -name "*.sh" -o -name "*.txt" | while read f; do
    dos2unix "$f" 2>/dev/null
done
echo "  Line ending conversion complete."

# Step 3: Build hello module
echo ""
echo "[Step 3] Building hello module..."
cd hello
make clean 2>/dev/null
make
if [ $? -ne 0 ]; then
    echo "[ERROR] hello module build failed!"
    exit 1
fi
echo "[SUCCESS] hello module built."
cd ..

# Step 4: Build print_self module
echo ""
echo "[Step 4] Building print_self module..."
cd print_self
make clean 2>/dev/null
make
if [ $? -ne 0 ]; then
    echo "[ERROR] print_self module build failed!"
    exit 1
fi
echo "[SUCCESS] print_self module built."
cd ..

# Step 5: Build print_other module
echo ""
echo "[Step 5] Building print_other module..."
cd print_other
make clean 2>/dev/null
make
if [ $? -ne 0 ]; then
    echo "[ERROR] print_other module build failed!"
    exit 1
fi
echo "[SUCCESS] print_other module built."
cd ..

echo ""
echo "=========================================="
echo "  All modules built successfully!"
echo "=========================================="
echo ""
echo "Quick reference:"
echo ""
echo "--- hello module ---"
echo "  Load:    sudo insmod hello/hello.ko"
echo "  Check:   sudo dmesg -T | tail"
echo "  Verify:  sudo lsmod | grep hello"
echo "  Remove:  sudo rmmod hello"
echo "  Check:   sudo dmesg -T | tail"
echo ""
echo "--- print_self module ---"
echo "  Load:    sudo insmod print_self/print_self.ko"
echo "  Check:   sudo dmesg -T | tail -20"
echo "  Remove:  sudo rmmod print_self"
echo ""
echo "--- print_other module ---"
echo "  Find a PID:  pgrep bash"
echo "  Load:    sudo insmod print_other/print_other.ko pid=<PID>"
echo "  Check:   sudo dmesg -T | tail -20"
echo "  Remove:  sudo rmmod print_other"
