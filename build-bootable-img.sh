#!/bin/bash
if [ $UID -ne 0 ]
then
    echo "Please use root or sudo to run this script for loop device mount"
    exit 1
fi

dd if=/dev/zero of=image.img bs=512 count=2880

LOOP_DEV=$(losetup --find --show ./image.img)
dd if=boot.bin of=$LOOP_DEV bs=512 count=1

mkdir -p img-mount
mount $LOOP_DEV img-mount/
cp init.bin kernel.sys img-mount/
umount ./img-mount
losetup -d $LOOP_DEV
rm -rf img-mount
