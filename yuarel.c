#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "yuarel.h"

int
yuarel_parse(struct yuarel *url, char *u)
{
	memset(url, 0, sizeof (struct yuarel));

	if (NULL == url || NULL == u) {
		return -1;
	}

	/* Scheme */
	url->scheme = u;
	u = strchr(u, ':');
	if (NULL == u) {
		return -1;
	}
	*(u++) = '\0';

	/* Rewind to after :// */
	while ('/' == *(u++))
		;

	/* Host */
	u--;
	if ('\0' == *u) {
		return -1;
	}
	url->host = u;

	/* (Query) */
	u = strchr(u, '?');
	if (NULL != u) {
		*(u++) = '\0';
		url->query = u;
	}

	/* (Path) */
	u = strchr(url->host, '/');
	if (NULL != u && u < url->query) {
		*(u++) = '\0';
		url->path = u;
	}

	/* (Port) */
	u = strchr(url->host, ':');
	if (NULL != u && (!url->query || u < url->query) && (!url->path || u < url->path)) {
		*(u++) = '\0';
		if ('\0' == *u) {
			return -1;
		}

		if (url->path && url->path > u) {
			char *p = strndup(u, url->path - u);
			url->port = atoi(p);
			free(p);
		} else {
			url->port = atoi(u);
		}
	}

	return 0;
}
