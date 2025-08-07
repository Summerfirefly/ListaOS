LOOP_DEV=$(losetup -f)
IMG_PATH="./build/image.img"

dd if=/dev/zero of=${IMG_PATH} bs=1M count=16

sudo losetup $LOOP_DEV ${IMG_PATH}
sudo fdisk $LOOP_DEV <<EOF
g
n
1


w
EOF
sudo partprobe $LOOP_DEV
sudo mkfs.fat -F 16 -h 4 ${LOOP_DEV}p1

sudo dd if=./build/boot.bin of=${LOOP_DEV} bs=1 count=446
sudo dd if=./build/init.bin of=${LOOP_DEV}p1 bs=512 count=3 seek=1

mkdir tmpmnt
sudo mount ${LOOP_DEV}p1 tmpmnt
sudo cp build/kernel.sys tmpmnt/
sudo umount tmpmnt
rm -rf tmpmnt

sudo losetup -d $LOOP_DEV
