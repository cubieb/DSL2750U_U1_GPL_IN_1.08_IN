
#
# Intel's UPnP MicroStack/1.0, Sample MakeFile */
# Intel Device Builder - Build#1.0.2777
#

# List all object files here
OFILES = \
	CdsDidlSerializer.o \
CdsErrors.o \
CdsMediaClass.o \
CdsObject.o \
FileIoAbstraction.o \
DlnaHttp.o \
DlnaHttpClient.o \
DlnaHttpServer.o \
DLNAProtocolInfo.o \
MediaServerAbstraction.o \
DownloadController.o \
DmsIntegration.o \
FileSystemEnumerator.o \
MimeTypes.o \
UTF8Utils.o \
Main.o \
	ILibParsers.o\
	ILibAsyncSocket.o\
	ILibAsyncUDPSocket.o\
	ILibAsyncServerSocket.o\
	ILibWebClient.o\
	ILibWebServer.o\
	ILibThreadPool.o\
	MediaServer_MicroStack.o\
    nls_cp54936.o \
    nls_base.o \


ifdef FFMPEG
OFILES += MetaInfo.o
endif


# Compiler command name
# CC = gcc

# Compiler flags applied to all files
CFLAGS = -Os -DTBS -I./ -I../  -I./CdsObjects  -I./FileAbstractionFiles  -I./HttpFiles  -I./ProtocolInfoParser  -I./MediaServerAbstraction  -I./DownloadController  -I./DmsIntegration  -I./DmsIntegration/FileSystemEnumerator  -I./StringUtils -DINCLUDE_FEATURE_UPLOAD -Os -Wall -D_POSIX -D_DEBUG -D_VERBOSE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE -D_LARGEFILE_SOURCE -D__GLIBC_HAVE_LONG_LONG
LIBS = -lm -lpthread 
# Name of the executable
ifdef LIBID3TAG
OFILES +=    AudioGetTagInfo.o 
CFLAGS += -I../libid3tag -DLIBID3TAG
LIBS += -L../libid3tag -L$(TMPDISTDIR)/lib -lid3tag -lz
endif

ifdef FFMPEG
CFLAGS += -I$(TMPDISTDIR)/include -DFFMPEG
LIBS += -L$(TMPDISTDIR)/lib -lavformat -lavutil -lavcodec 
endif

VPATH =  ./ ../  ./CdsObjects  ./FileAbstractionFiles  ./HttpFiles  ./ProtocolInfoParser  ./MediaServerAbstraction  ./DownloadController  ./DmsIntegration  ./DmsIntegration/FileSystemEnumerator  ./StringUtils 
EXENAME = Main

# Builds all object files and executable
${EXENAME} : $(OFILES)
	$(CC) $(CFLAGS) -o ${EXENAME} $(OFILES) $(LIBS)

# Macro rule for all object files.
$(OFILES) : \
	MediaServer_MicroStack.h\

test:test.c nls_base.o nls_cp54936.o UTF8Utils.o
	gcc -g -o test test.c nls_base.o nls_cp54936.o UTF8Utils.o

# Clean up
clean :
	rm -f $(OFILES) ${EXENAME} ${EXENAME}.exe

install:
	cp Main $(FSROOT)/sbin/dlna
	$(STRIP) $(FSROOT)/sbin/dlna
	#cp -Prf /opt/tbs_toolchains/realtek/rsdk-1.2.8/linux/uclibc/mips-linux/lib/libuClibc*.so* ${FSROOT}/lib
	#cp -Prf /opt/tbs_toolchains/realtek/rsdk-1.2.8/linux/uclibc/mips-linux/lib/libpthread*.so* ${FSROOT}/lib
