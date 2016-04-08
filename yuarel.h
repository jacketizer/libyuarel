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
	char *fragment; /* fragment, default: NULL */
};

/**
 * Parse a URL to a struct.
 *
 * The URL string should be in the following format:
 *
 * scheme ":" [ "//" ] host [ ":" port ] [ "/" ] [ path ] [ "?" query ] [ "#" fragment ]
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
 * pointers to each path part will be stored in **parts. Double slashes will be
 * treated as one.
 *
 * *path:     the path to split.
 * **parts:   a pointer to an array of (char *) where to store the result.
 * max_parts: max number of parts to parse.
 *
 * Returns the number of parsed items. -1 on error.
 */
extern int yuarel_split_path(char *path, char **parts, int max_parts);

/**
 * Parse a query string into a key/value struct.
 *
 * No data is copied, the equal sign and delimiters are used as null
 * terminators and then pointers to each parameter key and value will be stored
 * in *values.
 *
 * *query:     the query string to parse.
 * delimiter   the character that seperates the key/value pairs from eachother.
 * **params:   a pointer to an array of (char *) where to store the result.
 * max_values: max number of parameters to parse.
 *
 * Returns the number of parsed items. -1 on error.
 */
extern int yuarel_tokenize_query(char *query, char delimiter, char **params, int max_params);

#endif /* INC_YUAREL_H */
