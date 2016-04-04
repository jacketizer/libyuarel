#ifndef INC_YUAREL_H
#define INC_YUAREL_H

/**
 * The struct where the parsed values will be stored:
 *
 * scheme ":" [ "//" ] host [ ":" port ] [ "/" ] [ path ] [ "?" query ]
 *
 * Note: to make sure that no strings are copied, the first slash "/" in the
 * path will be used to null terminate the hostname if no port is supplied.
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
 * The following parts will be parsed to the corresponding struct member.
 *
 * *url:     a pointer to the struct where to store the parsed values.
 * *url_str: a pointer to the url to be parsed (null terminated).
 *
 * Returns 0 on success, otherwise -1.
 */
extern int yuarel_parse(struct yuarel *url, char *url_str);

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
extern int yuarel_split_path(char *path, char **parts, int max_parts);

#endif /* INC_YUAREL_H */
