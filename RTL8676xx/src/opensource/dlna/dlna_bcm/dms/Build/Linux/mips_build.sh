rm -f libavcodec.so
rm -f libavformat.so
rm -f libavutil.so
ln -sf ../../Dependencies/Libraries/Linux/Mips/Shared/libavcodec.so.52 libavcodec.so
ln -sf ../../Dependencies/Libraries/Linux/Mips/Shared/libavformat.so.52 libavformat.so
ln -sf ../../Dependencies/Libraries/Linux/Mips/Shared/libavutil.so.49 libavutil.so
make clean -f Makefile_Mips_Framework
make clean -f Makefile_Mips_server
make -f Makefile_Mips_Framework
make -f Makefile_Mips_server