#ifndef INC_YUAREL_H
#define INC_YUAREL_H

/**
 * The struct where the parsed values will be stored:
 *
 * scheme ":" [ "//" ] host [ ":" port ] [ "/" ] [ path ] [ "?" query ]
 *
 * Note: to make sure that no strings are copied, the first slash "/" in the
 * path will be used to NULL terminate the hostname if no port is supplied.
 */
struct yuarel {
	char *scheme; /* scheme, without ":" and "//" */
	char *host; /* hostname or IP address */
	int port; /* port, default: 0 */
	char *path; /* path, without leading "/", default: NULL */
	char *query; /* query, default: NULL */
};

/**
 * Parse a URL to a struct.
 *
 * The URL string should be in the following format:
 *
 * scheme ":" [ "//" ] host [ ":" port ] [ "/" ] [ path ] [ "?" query ]
 *
 * The following parts will be parsed to the coresponding struct member.
 *
 * *url is a pointer to the struct where to store the parsed values.
 * *url_str is a pointer to the url to be parsed (NULL terminated).
 *
 * Returns 0 on success, otherwise -1.
 */
extern int yuarel_parse(struct yuarel *url, char *url_str);

#endif /* INC_YUAREL_H */
