
#include <common.h>
#include <error.h>
#include "rtl8676_asicregs.h"



/* LOCAL SUBPROGRAM SPECIFICATIONS
 */
static __inline__ void tableAccessForeword(u32, u32, void *);

int swTable_addEntry(u32 tableType, u32 eidx, void *entryContent_P)
{

    REG32(SWTCR0) = REG32(SWTCR0) | EN_STOP_TLU;
    while ((REG32(SWTCR0) & STOP_TLU_READY) == 0);

    tableAccessForeword(tableType, eidx, entryContent_P);
        
    /* Activate add command */
    REG32(SWTACR) = ACTION_START | CMD_ADD;
    
    /* Wait for command done */
    while ( (REG32(SWTACR) & ACTION_MASK) != ACTION_DONE );

    REG32(SWTCR0) = REG32(SWTCR0) & ~EN_STOP_TLU;

    /* Check status */
    if ( (REG32(SWTASR) & TABSTS_MASK) != TABSTS_SUCCESS )
        return ECOLLISION;
    else
        return 0;
}



int swTable_modifyEntry(u32 tableType, u32 eidx, void *entryContent_P)
{
    REG32(SWTCR0) = REG32(SWTCR0) | EN_STOP_TLU;
    while ((REG32(SWTCR0) & STOP_TLU_READY) == 0);

    tableAccessForeword(tableType, eidx, entryContent_P);
        
    /* Activate add command */
    REG32(SWTACR) = ACTION_START | CMD_MODIFY;
    
    /* Wait for command done */
    while ( (REG32(SWTACR) & ACTION_MASK) != ACTION_DONE );

    REG32(SWTCR0) = REG32(SWTCR0) & ~EN_STOP_TLU;

    /* Check status */
    if ( (REG32(SWTASR) & TABSTS_MASK) != TABSTS_SUCCESS )
        return EEMPTY;
    else
        return 0;
}



int swTable_forceAddEntry(u32 tableType, u32 eidx, void *entryContent_P)
{
    REG32(SWTCR0) = REG32(SWTCR0) | EN_STOP_TLU;
    while ((REG32(SWTCR0) & STOP_TLU_READY) == 0);

    tableAccessForeword(tableType, eidx, entryContent_P);
        
    /* Activate add command */
    REG32(SWTACR) = ACTION_START | CMD_FORCE;
    
    /* Wait for command done */
    while ( (REG32(SWTACR) & ACTION_MASK) != ACTION_DONE );

    REG32(SWTCR0) = REG32(SWTCR0) & ~EN_STOP_TLU;

    /* Check status */
    if ( (REG32(SWTASR) & TABSTS_MASK) == TABSTS_SUCCESS )
        return 0;
        
    /* There might be something wrong */
	else
		return -1;

}


int swTable_readEntry(u32 tableType, u32 eidx, void *entryContent_P)
{
    u32 *    entryAddr;

    REG32(SWTCR0) = REG32(SWTCR0) | EN_STOP_TLU;
    while ((REG32(SWTCR0) & STOP_TLU_READY) == 0);

    
    entryAddr = (u32 *) (table_access_addr_base(tableType) + eidx * TABLE_ENTRY_DISTANCE);
    
    /* Wait for command ready */
    while ( (REG32(SWTACR) & ACTION_MASK) != ACTION_DONE );
    
    /* Read registers according to entry width of each table */
    *((u32 *)entryContent_P + 7) = *(entryAddr + 7);
    *((u32 *)entryContent_P + 6) = *(entryAddr + 6);
    *((u32 *)entryContent_P + 5) = *(entryAddr + 5);
    *((u32 *)entryContent_P + 4) = *(entryAddr + 4);
    *((u32 *)entryContent_P + 3) = *(entryAddr + 3);
    *((u32 *)entryContent_P + 2) = *(entryAddr + 2);
    *((u32 *)entryContent_P + 1) = *(entryAddr + 1);
    *((u32 *)entryContent_P + 0) = *(entryAddr + 0);

    REG32(SWTCR0) = REG32(SWTCR0) & ~EN_STOP_TLU;

    return 0;
}



//static __inline__ void tableAccessForeword(u32 tableType, u32 eidx,     void *entryContent_P)
void tableAccessForeword(u32 tableType, u32 eidx,     void *entryContent_P)
{

    /* Wait for command done */
    while ( (REG32(SWTACR) & ACTION_MASK) != ACTION_DONE );
    
    /* Write registers according to entry width of each table */
    REG32(TCR7) = *((u32 *)entryContent_P + 7);
    REG32(TCR6) = *((u32 *)entryContent_P + 6);
    REG32(TCR5) = *((u32 *)entryContent_P + 5);
    REG32(TCR4) = *((u32 *)entryContent_P + 4);
    REG32(TCR3) = *((u32 *)entryContent_P + 3);
    REG32(TCR2) = *((u32 *)entryContent_P + 2);
    REG32(TCR1) = *((u32 *)entryContent_P + 1);
    REG32(TCR0) = *(u32 *)entryContent_P;
    
    /* Fill address */
    REG32(SWTAA) = table_access_addr_base(tableType) + eidx * TABLE_ENTRY_DISTANCE;
}

