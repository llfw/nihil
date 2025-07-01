// This source code is released into the public domain.

#ifndef NIHIL_HH_INCLUDED
#define NIHIL_HH_INCLUDED

#define NIHIL_HAVE_FEXECVE 0
#define NIHIL_HAVE_GETENV_R 0

#if __has_include(<sys/param.h>)
#	include <sys/param.h>
#endif

#if defined(__FreeBSD_version)

/* fexecve() added in FreeBSD 8.0 */
#	if (__FreeBSD_version >= 800000)
#		undef NIHIL_HAVE_FEXECVE
#		define NIHIL_HAVE_FEXECVE 1
#	endif

/* getenv_r() added in FreeBSD 15.0 */
#	if (__FreeBSD_version >= 1500000)
#		undef NIHIL_HAVE_GETENV_R
#		define NIHIL_HAVE_GETENV_R 1
#	endif

#endif // defined(__FreeBSD_version)

#endif // !NIHIL_HH_INCLUDED
