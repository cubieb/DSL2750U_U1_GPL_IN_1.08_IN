/* flashMem.c
 *
 * This header file contains the defines for the flash memory layout and
 * the prototypes for the flash API routines
 *
 */


#define Intel_IMAGE1_START_SECTOR            0
#define Intel_IMAGE1_MAX_SECTORS            15
#define Intel_BOOTLINE_START_SECTOR         38
#define Intel_BOOTLINE_SIZE                  1
#define Intel_CONFIG_TABLE_START_SECTOR     30
#define Intel_CONFIG_TABLE_SIZE              1

#define AMD_IMAGE1_START_SECTOR              4
#define AMD_IMAGE1_MIDDLE_SECTOR             19
#define AMD_IMAGE1_MAX_SECTORS               23
#define AMD_BOOTLINE_START_SECTOR            0
#define AMD_BOOTLINE_SIZE                    1
#define AMD_CONFIG_TABLE_START_SECTOR        2
#define AMD_CONFIG_TABLE_SIZE                1

#define FLASH_STARTING_ADDRESS              0xFFE00000

/********************** Prototypes ****************************/

void flashReadSector(int sector, int offset, UCHAR *buf, int bufLen);
void flashReadString( int sector, int offset, UCHAR *buf, int bufLen);
STATUS flashWriteString( int sector, int offset, UCHAR *buf, int bufLen );
STATUS flashReadBootLine( UCHAR *buf, int *bootlineLen);
void flashWriteBootLine( UCHAR *buf, int bufLen );
STATUS flashReadConfigTable( UCHAR *buf, int *configLen);
void flashWriteConfigTable( UCHAR *buf, int bufLen );
STATUS flashReadImage( int imageNum, char *buf);
int flashWriteImage( int imageNum, char *buf, int bufLen );
char imageLengthSave(char index, int size);
int FlashWriteVW(volatile char *pDest, char *pSource,int Length);
void wtSect(volatile char *adr, char *ptr, int size);
char imageStringGet(char index, char *addr);