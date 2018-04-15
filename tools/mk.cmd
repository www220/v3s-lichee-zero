@echo off
@del rtthread.bin
@del rtthread.mk
@del rtthread-with-spl.bin
@del rtthread-with-tf.img
@echo mkimage...
@copy ..\rtthread.bin rtthread.bin
@mkimage.exe -A arm -T firmware -C none -O u-boot -a 0x41000000 -e 0 -n "RT-Thread for sunxi board" -d rtthread.bin rtthread.mk
@echo mksplbin...
@copy sunxi-spl.bin /b + rtthread.mk /b rtthread-with-spl.bin
@echo mktfbin...
@copy sunxi-tf.bin rtthread-with-tf.img
@dd.exe if=rtthread-with-spl.bin of=rtthread-with-tf.img bs=1k seek=8
