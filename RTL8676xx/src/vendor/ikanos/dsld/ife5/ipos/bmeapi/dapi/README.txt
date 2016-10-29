
STEPS TO GENERATE Vx180Code.cbin FROM NEW Vx180Code.h
-----------------------------------------------------
1. When a new Vx180Code.h file is obtained, we must compile the arrtobin.c file to create a new arrtobin executable. This is done by using the command:
	 gcc -I./zlib-1.2.3 ./zlib-1.2.3/libz.a arrtobin.c -o arrtobin ./zlib-1.2.3/libz.a
on the shell prompt of the host. This also requires the zlib library which needs to be compiled for the host machine in the directory : fusiv_src/user/vd
sld_vx180/ipos/bmeapi/dapi. Please follow the below steps for this:-
	a) Copy zlib-1.2.3.tar.bz2 from buildroot/dl directory.
	b) Untar this file into current directory
	c) cd zlib-1.2.3
	d) Execute ./configure
	e) Execute make
	f) cd ..

2. On the command line, execute the command "./arrtobin". This will generate
the compressed binary file Vx180Code.cbin from the header file Vx180Code.h
