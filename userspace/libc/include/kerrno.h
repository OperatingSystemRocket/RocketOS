#pragma once

#define EPERM		 1	/* Operation not permitted */
#define ENOENT		 2	/* No such file or directory */
#define ENOFILE 	 2	/* Microsoft legacy alias for ENOENT */
#define ESRCH		 3	/* No such process */
#define EINTR		 4	/* Interrupted function call */
#define EIO		 5	/* Input/output error */
#define ENXIO		 6	/* No such device or address */
#define E2BIG		 7	/* Arg list too long */
#define ENOEXEC 	 8	/* Exec format error */
#define EBADF		 9	/* Bad file descriptor */
#define ECHILD		10	/* No child processes */
#define EAGAIN		11	/* Resource temporarily unavailable */
#define ENOMEM		12	/* Not enough space */
#define EACCES		13	/* Permission denied */
#define EFAULT		14	/* Bad address */

/* 15 - Unknown Error */

#define EBUSY		16	/* Device or resource busy */
#define EEXIST		17	/* File exists */
#define EXDEV		18	/* Improper link (cross-device link) */
#define ENODEV		19	/* No such device */
#define ENOTDIR 	20	/* Not a directory */
#define EISDIR		21	/* Is a directory */

/* Microsoft's non-standard _get_errno() and _set_errno(), which are
 * declared in <stdlib.h>, and for which we provide in-line support on
 * legacy Windows versions, (also in <stdlib.h>), demand exposure of
 * EINVAL within <stdlib.h>, (for legacy support), regardless of the
 * state of _ERRNO_H.
 */
#define EINVAL		22	/* Invalid argument */

/* The remaining error codes are to be exposed only when <errno.h> has
 * been included explicitly.
 */
#define ENFILE		23	/* Too many open files in system */
#define EMFILE		24	/* Too many open files */
#define ENOTTY		25	/* Inappropriate I/O control operation */

/* 26 - Unknown Error */

#define EFBIG		27	/* File too large */
#define ENOSPC		28	/* No space left on device */
#define ESPIPE		29	/* Invalid seek (seek on a pipe?) */
#define EROFS		30	/* Read-only file system */
#define EMLINK		31	/* Too many links */
#define EPIPE		32	/* Broken pipe */
#define EDOM		33	/* Domain error (math functions) */
#define ERANGE		34	/* Result too large (possibly too small) */

/* 35 - Unknown Error */

#define EDEADLK 	36	/* Resource deadlock avoided (non-Cyg) */
#define EDEADLOCK	36	/* Microsoft legacy alias for EDEADLK */

/* 37 - Unknown Error */

#define ENAMETOOLONG	38	/* Filename too long (91 in Cyg?) */
#define ENOLCK		39	/* No locks available (46 in Cyg?) */
#define ENOSYS		40	/* Function not implemented (88 in Cyg?) */
#define ENOTEMPTY	41	/* Directory not empty (90 in Cyg?) */
#define EILSEQ		42	/* Illegal byte sequence */

extern int errno;
