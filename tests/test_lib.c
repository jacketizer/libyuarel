#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <yuarel.h>
#include "minunit.h"

int tests_run;

static unsigned char *
test_parse_http_url_ok()
{
	int rc;
	struct yuarel url;
	char *url_string;

	/* Minimal URL */
	url_string = strdup("http://example.com");
	rc = yuarel_parse(&url, url_string);
	mu_assert("minimal HTTP URL", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "http"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "example.com"));
	mu_silent_assert("should set the port attribute correctly", 0 == url.port);
	mu_silent_assert("should set the path attribute correctly", NULL == url.path);
	mu_silent_assert("should set the query attribute correctly", NULL == url.query);
	mu_silent_assert("should set the fragment attribute correctly", NULL == url.fragment);
	free(url_string);

	/* With path (/) */
	url_string = strdup("http://example.com/");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with path ('/')", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "http"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "example.com"));
	mu_silent_assert("should set the path attribute correctly", 0 == strcmp(url.path, ""));
	mu_silent_assert("should set the port attribute correctly", 0 == url.port);
	mu_silent_assert("should set the query attribute correctly", NULL == url.query);
	mu_silent_assert("should set the fragment attribute correctly", NULL == url.fragment);
	free(url_string);

	/* With path */
	url_string = strdup("http://example.com/path");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with path ('/path')", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "http"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "example.com"));
	mu_silent_assert("should set the path attribute correctly", 0 == strcmp(url.path, "path"));
	mu_silent_assert("should set the port attribute correctly", 0 == url.port);
	mu_silent_assert("should set the query attribute correctly", NULL == url.query);
	mu_silent_assert("should set the fragment attribute correctly", NULL == url.fragment);
	free(url_string);

	/* With port */
	url_string = strdup("http://example.com:80");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with port only", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "http"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "example.com"));
	mu_silent_assert("should set the port attribute correctly", 80 == url.port);
	mu_silent_assert("should set the path attribute correctly", NULL == url.path);
	mu_silent_assert("should set the query attribute correctly", NULL == url.query);
	mu_silent_assert("should set the fragment attribute correctly", NULL == url.fragment);
	free(url_string);

	/* With query */
	url_string = strdup("http://example.com?query=only");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with query only", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "http"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "example.com"));
	mu_silent_assert("should set the query attribute correctly", 0 == strcmp(url.query, "query=only"));
	mu_silent_assert("should set the port attribute correctly", 0 == url.port);
	mu_silent_assert("should set the path attribute correctly", NULL == url.path);
	mu_silent_assert("should set the fragment attribute correctly", NULL == url.fragment);
	free(url_string);

	/* With fragment */
	url_string = strdup("http://example.com#frag=f1");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with fragment only", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "http"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "example.com"));
	mu_silent_assert("should set the fragment attribute correctly", 0 == strcmp(url.fragment, "frag=f1"));
	mu_silent_assert("should set the port attribute correctly", 0 == url.port);
	mu_silent_assert("should set the path attribute correctly", NULL == url.path);
	mu_silent_assert("should set the query attribute correctly", NULL == url.query);
	free(url_string);

	/* With port and path */
	url_string = strdup("http://example.com:8080/port/and/path");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with port and path", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "http"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "example.com"));
	mu_silent_assert("should set the port attribute correctly", 8080 == url.port);
	mu_silent_assert("should set the path attribute correctly", 0 == strcmp(url.path, "port/and/path"));
	mu_silent_assert("should set the query attribute correctly", NULL == url.query);
	mu_silent_assert("should set the fragment attribute correctly", NULL == url.fragment);
	free(url_string);

	/* With port and query */
	url_string = strdup("http://example.com:8080?query=portANDquery");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with port and query", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "http"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "example.com"));
	mu_silent_assert("should set the port attribute correctly", 8080 == url.port);
	mu_silent_assert("should set the query attribute correctly", 0 == strcmp(url.query, "query=portANDquery"));
	mu_silent_assert("should set the path attribute correctly", NULL == url.path);
	mu_silent_assert("should set the fragment attribute correctly", NULL == url.fragment);
	free(url_string);

	/* With port and fragment */
	url_string = strdup("http://example.com:8080#f1");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with port and fragment", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "http"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "example.com"));
	mu_silent_assert("should set the port attribute correctly", 8080 == url.port);
	mu_silent_assert("should set the fragment attribute correctly", 0 == strcmp(url.fragment, "f1"));
	mu_silent_assert("should set the path attribute correctly", NULL == url.path);
	mu_silent_assert("should set the query attribute correctly", NULL == url.query);
	free(url_string);

	/* With path and query */
	url_string = strdup("http://example.com/path/and/query?q=yes");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with path and query", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "http"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "example.com"));
	mu_silent_assert("should set the path attribute correctly", 0 == strcmp(url.path, "path/and/query"));
	mu_silent_assert("should set the query attribute correctly", 0 == strcmp(url.query, "q=yes"));
	mu_silent_assert("should set the port attribute correctly", 0 == url.port);
	mu_silent_assert("should set the fragment attribute correctly", NULL == url.fragment);
	free(url_string);

	/* With path and fragment */
	url_string = strdup("http://example.com/path/and#fragment");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with path and fragment", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "http"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "example.com"));
	mu_silent_assert("should set the path attribute correctly", 0 == strcmp(url.path, "path/and"));
	mu_silent_assert("should set the fragment attribute correctly", 0 == strcmp(url.fragment, "fragment"));
	mu_silent_assert("should set the port attribute correctly", 0 == url.port);
	mu_silent_assert("should set the query attribute correctly", NULL == url.query);
	free(url_string);

	/* With query and fragment */
	url_string = strdup("http://example.com?q=yes#f1");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with query and fragment", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "http"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "example.com"));
	mu_silent_assert("should set the query attribute correctly", 0 == strcmp(url.query, "q=yes"));
	mu_silent_assert("should set the fragment attribute correctly", 0 == strcmp(url.fragment, "f1"));
	mu_silent_assert("should set the port attribute correctly", 0 == url.port);
	mu_silent_assert("should set the path attribute correctly", NULL == url.path);
	free(url_string);

	/* Full URL */
	url_string = strdup("https://localhost:8989/path/to/test?query=yes&q=jack#fragment1");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with port, path and query", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "https"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "localhost"));
	mu_silent_assert("should set the port attribute correctly", 8989 == url.port);
	mu_silent_assert("should set the path attribute correctly", 0 == strcmp(url.path, "path/to/test"));
	mu_silent_assert("should set the query attribute correctly", 0 == strcmp(url.query, "query=yes&q=jack"));
	mu_silent_assert("should set the fragment attribute correctly", 0 == strcmp(url.fragment, "fragment1"));
	free(url_string);

	return 0;
}

static unsigned char *
test_parse_url_fail()
{
	int rc;
	struct yuarel url;
	char *url_string;

	/* Empty */
	url_string = strdup("");
	rc = yuarel_parse(&url, url_string);
	mu_assert("empty string should return -1", -1 == rc);
	free(url_string);

	/* Scheme only */
	url_string = strdup("rtsp://");
	rc = yuarel_parse(&url, url_string);
	mu_assert("scheme only should return -1", -1 == rc);
	free(url_string);

	/* Hostname only */
	url_string = strdup("hostname");
	rc = yuarel_parse(&url, url_string);
	mu_assert("hostname only should return -1", -1 == rc);
	free(url_string);

	/* Path only */
	url_string = strdup("/path/only");
	rc = yuarel_parse(&url, url_string);
	mu_assert("path only should return -1", -1 == rc);
	free(url_string);

	/* Query only */
	url_string = strdup("?query=only");
	rc = yuarel_parse(&url, url_string);
	mu_assert("query only should return -1", -1 == rc);
	free(url_string);

	/* Missing scheme */
	url_string = strdup("://");
	rc = yuarel_parse(&url, url_string);
	mu_assert("missing scheme should return -1", -1 == rc);
	free(url_string);

	/* Missing hostname */
	url_string = strdup("rtsp://:8910/path");
	rc = yuarel_parse(&url, url_string);
	mu_assert("missing hostname should return -1", -1 == rc);
	free(url_string);

	return 0;
}

static unsigned char *
test_split_path_ok()
{
	int rc;
	char *path;
	char *parts[10];

	/* Simple path */
	path = strdup("/this/is/a/path");
	rc = yuarel_split_path(path, parts, 10);
	mu_assert("should be able to parse a regular path", 4 == rc);
	mu_silent_assert("first part should be 'this'", 0 == strcmp("this", parts[0]));
	mu_silent_assert("second part should be 'is'", 0 == strcmp("is", parts[1]));
	mu_silent_assert("third part should be 'a'", 0 == strcmp("a", parts[2]));
	mu_silent_assert("fourth part should be 'path'", 0 == strcmp("path", parts[3]));
	free(path);

	/* Relative path */
	path = strdup("this/is/a/path");
	rc = yuarel_split_path(path, parts, 10);
	mu_assert("should be able to parse a relative path", 4 == rc);
	mu_silent_assert("first part should be 'this'", 0 == strcmp("this", parts[0]));
	mu_silent_assert("second part should be 'is'", 0 == strcmp("is", parts[1]));
	mu_silent_assert("third part should be 'a'", 0 == strcmp("a", parts[2]));
	mu_silent_assert("fourth part should be 'path'", 0 == strcmp("path", parts[3]));
	free(path);

	/* Path with empty parts */
	path = strdup("//this//is/a/path/");
	rc = yuarel_split_path(path, parts, 10);
	mu_assert("should treat multiple slashes as one", 4 == rc);
	mu_silent_assert("first part should be 'this'", 0 == strcmp("this", parts[0]));
	mu_silent_assert("second part should be 'is'", 0 == strcmp("is", parts[1]));
	mu_silent_assert("third part should be 'a'", 0 == strcmp("a", parts[2]));
	mu_silent_assert("fourth part should be 'path'", 0 == strcmp("path", parts[3]));
	free(path);

	/* Just one level */
	path = strdup("/one_level");
	rc = yuarel_split_path(path, parts, 10);
	mu_assert("should be able to parse a path with one level", 1 == rc);
	mu_silent_assert("first part should be 'this'", 0 == strcmp("one_level", parts[0]));
	free(path);

	return 0;
}

static unsigned char *
all_tests()
{
	mu_group("yuarel_parse() with an HTTP URL");
	mu_run_test(test_parse_http_url_ok);

	mu_group("yuarel_parse() with faulty values");
	mu_run_test(test_parse_url_fail);

	mu_group("yuarel_split_path()");
	mu_run_test(test_split_path_ok);

	return 0;
}

int
main(void)
{
	unsigned char *result;

	result = all_tests();
	if (result != 0) {
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
