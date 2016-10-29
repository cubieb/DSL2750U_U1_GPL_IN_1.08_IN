# Create device files
cd ${FSROOT}
rm dev -rf
mkdir dev
cd dev


for i in $(seq 0 5)
do
mknod -m 664 mtd$i c 90 $(expr $i + $i)
mknod -m 664 mtdblock$i b 31 $i
done

mknod -m 600 mem    c  1 1
mknod -m 666 null   c  1 3
mknod -m 666 zero   c  1 5
mknod -m 644 random c  1 8
mknod -m 600 tty0   c  4 0
mknod -m 600 tty1   c  4 1
mknod -m 600 tty5   c  4 1
mknod -m 600 ttyS0  c  4 64
mknod -m 666 tty    c  5 0
mknod -m 600 console  c  5 1
mknod -m 600 ppp c 108 0
mknod -m 666 urandom c 1 9
#mkdir pty
#for i in $(seq 0 31)
#do
#mknod -m 666 pty/m$i c 2 $i 
#done
mknod -m 666 ptmx c 5 2
mkdir pts
for i in $(seq 0 255)
do
mknod -m 622 pts/$i c 136 $i
done
#mkdir tts
#mknod -m 666 tts/0 c 4 64
ln -s ../var/devlog log 
exit

