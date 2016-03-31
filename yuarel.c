#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "yuarel.h"

#define find_or_return(search_string, find_char) search_string = strchr(search_string, find_char);\
		if (NULL == search_string) return -1

#define terminate(str_ptr) *(str_ptr++) = '\0'

int
yuarel_parse(struct yuarel *url, char *u)
{
	memset(url, 0, sizeof (struct yuarel));

	if (NULL == url || NULL == u) {
		return -1;
	}

	/* Scheme */
	url->scheme = u;
	find_or_return(u, ':');
	terminate(u);

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
		terminate(u);
		url->query = u;
	}

	/* (Path) */
	u = strchr(url->host, '/');
	if (NULL != u && u < url->query) {
		terminate(u);
		url->path = u;
	}

	/* (Port) */
	u = strchr(url->host, ':');
	if (NULL != u && (!url->query || u < url->query) && (!url->path || u < url->path)) {
		terminate(u);
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
