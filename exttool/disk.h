/*
 *************************************************************************
 *
 * flash.h --
 *
 *	Declarations for the Flash layer. 
 * 
 *      NOTE: The Flash layer is not thread safe. If your LFS is multi-threaded
 *      you'll have to put the proper synchronization functions around your
 *      calls to these routines.
 *
 *
 *************************************************************************
 */

#ifndef _FLASH_H
#define _FLASH_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *************************************************************************
 *
 * Useful definitions.
 *
 *************************************************************************
 */

typedef int Boolean;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

/*
 *************************************************************************
 *
 * The following are flags to pass to Flash_Open.
 *
 * FLASH_SILENT	don't print statistics when Flash_Close is called.
 * FLASH_ASYNC	don't simulate synchronous flash operations. By default
 *		Flash delays for 10ms on every read and write to simulate
 *		flash access times.
 *
 *************************************************************************
 */

typedef u_int Flash_Flags;

#define FLASH_SILENT	0x1	
#define FLASH_ASYNC	0x2	

/*
 *************************************************************************
 *
 * Flash sector size. Don't change this, I haven't tested with other values.
 *
 *************************************************************************
 */

#define FLASH_SECTOR_SIZE 512

/*
 *************************************************************************
 *
 * Handle for a flash returned by Flash_Open and passed to the other routines.
 *
 *************************************************************************
 */

typedef void *Flash;

/*
 *************************************************************************
 * Flash
 * Flash_Open
 *
 * Parameters:
 *
 *  	char 		*file	-- name of the flash file to open.
 *  	Flash_Flags	flags	-- see above
 *  	u_int		*sectors -- # of sectors in the flash
 *
 * Returns:
 *	Flash handle on success, NULL otherwise and errno is set.
 *
 *
 * Flash_Open opens a flash and returns a handle for it that is used
 * in subsequent calls to Flash_Read, Flash_Write, and Flash_Close. 
 * The specified file must exist. The flash size in sectors is returned
 * in "sectors".
 *
 *************************************************************************
 */

Flash	Flash_Open(char *file, Flash_Flags flags, u_int *sectors);

/*
 *************************************************************************
 * int
 * Flash_Read
 *
 * Parameters:
 *
 *  	Flash 		flash	-- flash to read
 *	u_int		offset -- starting offset, in sectors
 * 	u_int		count -- # of sectors to read
 *	void		*buffer -- buffer into which flash is read
 *
 * Returns:
 *	0 on success, 1 otherwise and errno is set.
 *
 *
 * Flash_Read reads "count" sectors from "flash" into "buffer" 
 * starting at sector "offset". 
 *
 *************************************************************************
 */

int	Flash_Read(Flash flash, u_int offset, u_int count, void *buffer);

/*
 *************************************************************************
 * int
 * Flash_Write
 *
 * Parameters:
 *
 *  	Flash 		flash	-- flash to write
 *	u_int		offset -- starting offset, in sectors
 * 	u_int		count -- # of sectors to write
 *	void		*buffer -- buffer from which flash is written
 *
 * Returns:
 *	0 on success, 1 otherwise and errno is set.
 *
 *
 * Flash_Write writes "count" sectors from "buffer" to "flash" 
 * starting at sector "offset". 
 *
 *************************************************************************
 */

int	Flash_Write(Flash flash, u_int offset, u_int count, void *buffer);

/*
 *************************************************************************
 * int
 * Flash_Close
 *
 * Parameters:
 *
 *  	Flash 		flash	-- flash to close
 *
 * Returns:
 *	0 on success, 1 otherwise and errno is set.
 *
 *
 * Flash_Close closes the flash.
 *
 *************************************************************************
 */

int	Flash_Close(Flash flash);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
