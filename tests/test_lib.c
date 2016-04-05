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
	free(url_string);

	/* With path */
	url_string = strdup("http://example.com/path");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with path", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "http"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "example.com"));
	mu_silent_assert("should set the path attribute correctly", 0 == strcmp(url.path, "path"));
	mu_silent_assert("should set the port attribute correctly", 0 == url.port);
	mu_silent_assert("should set the query attribute correctly", NULL == url.query);
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
	free(url_string);

	/* Full URL */
	url_string = strdup("https://localhost:8989/path/to/test?query=yes&q=jack");
	rc = yuarel_parse(&url, url_string);
	mu_assert("with port, path and query", -1 != rc);
	mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp(url.scheme, "https"));
	mu_silent_assert("should set the host attribute correctly", 0 == strcmp(url.host, "localhost"));
	mu_silent_assert("should set the port attribute correctly", 8989 == url.port);
	mu_silent_assert("should set the path attribute correctly", 0 == strcmp(url.path, "path/to/test"));
	mu_silent_assert("should set the query attribute correctly", 0 == strcmp(url.query, "query=yes&q=jack"));
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

	return 0;
}

static unsigned char *
all_tests()
{
	mu_group("yuarel_parse() with an HTTP URL");
	mu_run_test(test_parse_http_url_ok);

	mu_group("yuarel_parse() with faulty values");
	mu_run_test(test_parse_url_fail);

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
