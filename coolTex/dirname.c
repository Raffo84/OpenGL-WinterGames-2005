/*      $OpenBSD: dirname.c,v 1.10 2003/06/17 21:56:23 millert Exp $    */

/*
 * Copyright (c) 1997 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef HAVE_DIRNAME

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef lint
//static char rcsid[] = "$OpenBSD: dirname.c,v 1.10 2003/06/17 21:56:23 millert Exp $";
#endif /* not lint */

#include <errno.h>
#include <string.h>
#include "libgen.h"

char *
dirname(const char *path)
{
	// The maximal path length
	static char bname[259];
	register const char *endp;

	/* Empty or NULL string gets treated as "." */
	if (path == NULL || *path == '\0') {
		(void)strlcpy(bname, ".", sizeof bname);
		return(bname);
	}

	/* Strip trailing slashes and backslashes */
	endp = path + strlen(path) - 1;
	while (endp > path && (*endp == '/' || *endp == '\\'))
		endp--;

	/* Find the start of the dir */
	while (endp > path && *endp != '/' && *endp != '\\')
		endp--;

	/* Either the dir is "/" or there are no slashes */
	if (endp == path)
	{
		char* temp;
		if (*endp == '/')
			temp = "/";
		else if (*endp =='\\')
			temp = "\\";
		else
			temp = ".";
		///(void)strlcpy(bname, *endp == '/' ? "/" : ".", sizeof bname);
		(void)strlcpy(bname, temp, sizeof bname);
		return(bname);
	}
	else
	{
		do
		{
			endp--;
		}
		while (endp > path && (*endp == '/' || *endp == '\\'));
	}

	if (endp - path + 2 > (int)sizeof(bname)) {
		errno = ENAMETOOLONG;
		return(NULL);
	}
	strlcpy(bname, path, endp - path + 2);
	return(bname);
}

#ifdef __cplusplus
}
#endif

#endif /* HAVE_DIRNAME */
