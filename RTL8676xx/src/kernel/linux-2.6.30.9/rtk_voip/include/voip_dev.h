#ifndef __VOIP_DEV_H__
#define __VOIP_DEV_H__

#include <linux/fs.h>

// ========================================================
// MAJOR and MINOR of dev 
// ========================================================
#define VOIP_DEV_MAJOR	243
#define VOIP_DEV_NAME	"voip"

/* This is minor number */
#define VOIP_DEV_LCM0		0
#define VOIP_DEV_DECT_SITEL	8	// SPI 
#define VOIP_DEV_DECT_DSPG	9	// SPI 
#define VOIP_DEV_IVR8K		16

#define VOIP_DD_DEV_START	60	/* data dump dev - start */
#define VOIP_DEV_JBC0		60
#define VOIP_DEV_JBC1		61
#define VOIP_DEV_JBC2		62
#define VOIP_DEV_JBC3		63
#define VOIP_DEV_PCM0_TX	64
#define VOIP_DEV_PCM0_RX	65
#define VOIP_DD_DEV_END		65	/* data dump dev - end */

// ========================================================
// register / unregister functions 
// ========================================================
/* register a voip dev */
extern int register_voip_chrdev( unsigned int minor, unsigned int num, const char *name, const struct file_operations *fop );

/* unregister a voip dev */
extern void unregister_voip_chrdev( unsigned int minor, unsigned int num );

// ========================================================
// data dump (DD) dev functions 
// (minor = VOIP_DD_DEV_START ~ VOIP_DD_DEV_END)
// ========================================================
/* writing function for data dump dev */
extern void ddinst_write( unsigned int minor, char *buff, size_t count );

#endif /* __VOIP_DEV_H__ */

