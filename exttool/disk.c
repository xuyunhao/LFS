#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include "flash.h"

#define assume(expr) {                                                  \
    if (!(expr)) {                                                      \
        printf("%s:%d: failed assumption `%s'\n", __FILE__, __LINE__,   \
            #expr);                                                     \
        abort();                                                        \
    }                                                                   \
}

#define FLASH_READ 0
#define FLASH_WRITE 1

typedef struct FlashInfo {
    char	*file;
    Flash_Flags	flags;
    int		fd;
    int         sectors;
    long long	readOps;
    long long	readSectors;
    long long	writeOps;
    long long	writeSectors;
} FlashInfo;

Flash
Flash_Open(
    char	*file,
    Flash_Flags	flags,
    u_int	*sectors)
{
    FlashInfo	*flash;
    struct stat	buf;
    int		rc;

    fprintf(stderr, "In Flash_Open\n");

    flash = (FlashInfo *) malloc(sizeof(FlashInfo));
    memset(flash, 0, sizeof(*flash));
    assume(flash != NULL);
    flash->file = file;
    flash->flags = flags;
    flash->fd = open(file, O_RDWR);
    if (flash->fd < 0) {
	rc = 1;
	goto done;
    }
    rc = fstat(flash->fd, &buf);
    if (rc != 0) {
	goto done;
    }
    flash->sectors = buf.st_size / FLASH_SECTOR_SIZE;
    *sectors = flash->sectors;
    rc = 0;
done:
    if (rc) {
	free((void *) flash);
	flash = NULL;
    }
    return flash;
}

static int
FlashIO(
    FlashInfo	*flash,
    int		type,
    u_int	offset,
    u_int	count,
    void	*buffer)
{
    off_t	seekOffset;
    off_t	resultOffset;
    int		rc;
    ssize_t	amount;

    if (flash == NULL) {
	rc = 1;
	errno = EINVAL;
	goto done;
    }
    if (offset >= flash->sectors) {
        rc = 1;
        errno = EINVAL;
        goto done;
    }
    seekOffset = offset * FLASH_SECTOR_SIZE;
    resultOffset = lseek(flash->fd, seekOffset, SEEK_SET);
    if (resultOffset != seekOffset) {
	rc = 1;
	goto done;
    }
    switch (type) {
	case FLASH_READ: 
	    amount = read(flash->fd, buffer, count * FLASH_SECTOR_SIZE);
	    break;
	case FLASH_WRITE:
	    amount = write(flash->fd, buffer, count * FLASH_SECTOR_SIZE);
	    break;
	default:
	    fprintf(stderr, "Internal error in FlashIO\n");
	    rc = 1;
	    goto done;
    }
    if (amount != (count * FLASH_SECTOR_SIZE)) {
	rc = 1;
	if (errno == 0) {
	    errno = EIO;
	}
	goto done;
    }
    if ((flash->flags & FLASH_ASYNC) == 0) {
	struct timespec req;
	req.tv_sec = 0;
	req.tv_nsec = 10000000;
	rc = nanosleep(&req, NULL);
	if (rc) {
	    rc = 1;
	    goto done;
	}
    }
    rc = 0;
done:
    return rc;
}

int
Flash_Read(
    Flash	flashHandle,
    u_int	offset,
    u_int	count,
    void	*buffer)
{
    FlashInfo	*flash = (FlashInfo *) flashHandle;
    int		rc;

    rc = FlashIO(flash, FLASH_READ, offset, count, buffer);
    if (rc == 0) {
	flash->readOps++;
	flash->readSectors += count;
    }
    return rc;
}

int
Flash_Write(
    Flash	flashHandle,
    u_int	offset,
    u_int	count,
    void	*buffer)
{
    FlashInfo	*flash = (FlashInfo *) flashHandle;
    int		rc;

    rc = FlashIO(flash, FLASH_WRITE, offset, count, buffer);
    if (rc == 0) {
	flash->writeOps++;
	flash->writeSectors += count;
    }
    return rc;
}


int
Flash_Close(
    Flash	flashHandle)
{
    FlashInfo	*flash = (FlashInfo *) flashHandle;
    int		rc;

    if (flash == NULL) {
	rc = 1;
	errno = EINVAL;
	goto done;
    }
    rc = close(flash->fd);
    if (rc) {
	rc = 1;
	goto done;
    }
    if ((flash->flags & FLASH_SILENT) == 0) {
	fprintf(stderr, "Flash read ops: %lld\n", flash->readOps);
	fprintf(stderr, "Flash read sectors: %lld\n", flash->readSectors);
	fprintf(stderr, "Flash write ops: %lld\n", flash->writeOps);
	fprintf(stderr, "Flash write sectors: %lld\n", flash->writeSectors);
    }
    free((void *) flash);
    rc = 0;
done:
    return rc;
}







