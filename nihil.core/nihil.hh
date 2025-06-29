/*
* This source code is released into the public domain.
 */

#ifndef NIHIL_HH_INCLUDED
#define NIHIL_HH_INCLUDED

#if __has_include(<sys/param.h>)
#	include <sys/param.h>
#endif

#if defined(__FreeBSD_version)

/* fexecve() added in FreeBSD 8.0 */
#if (__FreeBSD_version >= 800000)
#	define NIHIL_HAVE_FEXECVE
#endif

/* getenv_r() added in FreeBSD 15.0 */
#if (__FreeBSD_version >= 1500000)
#	define NIHIL_HAVE_GETENV_R
#endif

#endif // defined(__FreeBSD_version)

#endif // !NIHIL_HH_INCLUDED
