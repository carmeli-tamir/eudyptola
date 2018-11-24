#!/bin/bash

sudo umount dir
sudo losetup -d /dev/loop0
sudo rm file.img
