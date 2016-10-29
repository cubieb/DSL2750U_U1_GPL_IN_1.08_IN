SRC_PATH=$(shell pwd)
DST_DIR = ../bin

.PHONY: xmltool imgbuilder squashfs_tool testtools install sstrip uboot_mkimage mktree
CC=gcc
ALL : xmltool imgbuilder squashfs_tool testtools sstrip mktree
imgbuilder:
	make -C mkimage_ex
uboot_mkimage:
	make -C uboot_mkimage
squashfs_tool:
#	patch -p1<squashfs/sqlzma2u-3.3.patch  -d ./squashfs/squashfs3.3/
	make -C sqlzma
xmltool:
	if [ ! -d "$(DST_DIR)" ]; then mkdir -p $(DST_DIR); fi
	make -C mkxml
testtools:
	make -C testtools
sstrip:
	make -C sstrip	
mktree:
	make -C mktree
#lzma:
#	mkdir -p lzma/lzma443
#	tar jxf lzma/lzma443.tar.bz2 -C lzma/lzma443
#	find lzma/lzma443 -name makefile.gcc | xargs dos2unix
#	make -C lzma/lzma443/C/7zip/Compress/LZMA_Alone/ -f makefile.gcc
install:testtools imgbuilder squashfs_tool xmltool sstrip uboot_mkimage mktree
	cp ./mkimage_ex/mkimage_ex $(DST_DIR)
	cp sqlzma/mksquashfs $(DST_DIR)
	cp sqlzma/lzma457/CPP/7zip/Compress/LZMA_Alone/lzma $(DST_DIR)
	cp ./testtools/formatcsv $(DST_DIR)
	cp ./sstrip/sstrip $(DST_DIR)
	cp ./uboot_mkimage/mkimage $(DST_DIR)/uboot_mkimage
	cp ./mktree/mktree $(DST_DIR)
clean:
#	rm -f ../bin/mkimage ../bin/mksquashfs ../bin/lzma *.o
#	rm -rf lzma/lzma443 squashfs/squashfs3.2-r2
	make -C mkimage_ex clean
	make -C sqlzma clean
	make -C mkxml clean
	make -C testtools clean
	make -C sstrip clean
	make -C uboot_mkimage clean
	make -C mktree clean
	rm -rf $(DST_DIR)
