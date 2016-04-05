#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "yuarel.h"

/**
 * Parse a non null terminated string into an integer.
 *
 * str: pointer to the string containing the number.
 * len: Number of characters to parse.
 */
static inline int
natoi(char *str, size_t len)
{
	int i, r = 0;
	for (i = 0; i < len; i++) {
		r *= 10;
		r += str[i] - '0';
	}

	return r;
}

/**
 * Parse a URL string to a struct.
 *
 * url: pointer to the struct where to store the parsed URL parts.
 * u:   the URL string to be parsed.
 *
 * Returns 0 on success, otherwise -1.
 */
int
yuarel_parse(struct yuarel *url, char *u)
{
	if (NULL == url || NULL == u) {
		return -1;
	}

	memset(url, 0, sizeof (struct yuarel));

	/* Scheme */
	url->scheme = u;
	u = strchr(u, ':');
	if (NULL == u || url->scheme == u) {
		return -1;
	}
	*(u++) = '\0'; // Replace ':' with NULL

	/* Rewind to after // */
	while ('/' == *u) u++;

	/* Host */
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
	if (NULL != u && (!url->query || u < url->query)) {
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

		if (url->path) {
			url->port = natoi(u, url->path - u - 1);
		} else {
			url->port = atoi(u);
		}
	}

	/* Check that host is not empty */
	if ('\0' == *(url->host)) {
		return -1;
	}

	return 0;
}

/**
 * Split a path into several strings.
 *
 * No data is copied, the slashed are used as null terminators and then
 * pointers to each path part will be stored in **parts.
 *
 * *path:     the path to split.
 * **parts:   a pointer to an array of (char *) where to store the result.
 * max_parts: max number of parts to parse.
 */
int
yuarel_split_path(char *path, char **parts, int max_parts)
{
	int i = 0;

	/* Rewind to after slashes */
	while ('/' == *path) path++;

	parts[i++] = path;

	while (i < max_parts && (path = strchr(path, '/')) != NULL) {
		*path = '\0';
		parts[i++] = ++path;
	}

	return i;
}
