#!/bin/bash

dd if=/dev/zero of=file.img bs=1M count=50
sudo losetup /dev/loop0 file.img
sudo mkfs.vfat -F 32 /dev/loop0 -n "labely"
sudo mount /dev/loop0 dir
