#include "minunit.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <yuarel.h>

#if !defined(_POSIX_C_SOURCE) || _POSIX_C_SOURCE < 200809L
// https://stackoverflow.com/questions/26284110/strdup-confused-about-warnings-implicit-declaration-makes-pointer-with
// https://stackoverflow.com/questions/12575267/which-macro-is-being-used-with-strdup-on-linux
char *strdup(const char *src)
{
    // Space for length plus nul
    char *dst = malloc(strlen(src) + 1);
    if (dst == NULL)
    {
        // No memory
        return NULL;
    }
    // Copy the characters
    strcpy(dst, src);
    // Return the new string
    return dst;
}
#endif

int tests_run;

static int strcmp_wrap(const char *str, const char *str2)
{
    if (NULL == str && NULL == str2)
    {
        return 0;
    }
    if (NULL == str)
    {
        return 1;
    }
    if (NULL == str2)
    {
        return -1;
    }

    return strcmp(str, str2);
}

#define assert_struct(as_url, as_scheme, as_user, as_pass, as_host, as_port, as_path, as_query, as_fragment)     \
    mu_silent_assert("should set the scheme attribute correctly", 0 == strcmp_wrap(as_url.scheme, as_scheme));   \
    mu_silent_assert("should set the username attribute correctly", 0 == strcmp_wrap(as_url.username, as_user)); \
    mu_silent_assert("should set the password attribute correctly", 0 == strcmp_wrap(as_url.password, as_pass)); \
    mu_silent_assert("should set the host attribute correctly", 0 == strcmp_wrap(as_url.host, as_host));         \
    mu_silent_assert("should set the port attribute correctly", as_port == as_url.port);                         \
    mu_silent_assert("should set the path attribute correctly", 0 == strcmp_wrap(as_url.path, as_path));         \
    mu_silent_assert("should set the query attribute correctly", 0 == strcmp_wrap(as_url.query, as_query));      \
    mu_silent_assert("should set the fragment attribute correctly", 0 == strcmp_wrap(as_url.fragment, as_fragment));

static const char *test_parse_http_url_ok()
{
    int rc;
    struct yuarel url;
    char *url_string;

    /* Minimal URL */
    url_string = strdup("http://example.com");
    rc = yuarel_parse(&url, url_string);
    mu_assert("minimal HTTP URL", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "example.com", 0, NULL, NULL, NULL);
    free(url_string);

    /* With path (/) */
    url_string = strdup("http://example.com/");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with path ('/')", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "example.com", 0, "", NULL, NULL);
    free(url_string);

    /* With path */
    url_string = strdup("http://example.com/path");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with path ('/path')", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "example.com", 0, "path", NULL, NULL);
    free(url_string);

    /* With port */
    url_string = strdup("http://example.com:80");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with port only", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "example.com", 80, NULL, NULL, NULL);
    free(url_string);

    /* With query */
    url_string = strdup("http://example.com?query=only");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with query only", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "example.com", 0, NULL, "query=only", NULL);
    free(url_string);

    /* With fragment */
    url_string = strdup("http://example.com#frag=f1");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with fragment only", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "example.com", 0, NULL, NULL, "frag=f1");
    free(url_string);

    /* With credentials */
    url_string = strdup("http://u:p@example.com");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with credentials only", -1 != rc);
    assert_struct(url, "http", "u", "p", "example.com", 0, NULL, NULL, NULL);
    free(url_string);

    /* With credentials (user only) */
    url_string = strdup("http://u@example.com");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with user credential only", -1 != rc);
    assert_struct(url, "http", "u", NULL, "example.com", 0, NULL, NULL, NULL);
    free(url_string);

    /* With port and path */
    url_string = strdup("http://example.com:8080/port/and/path");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with port and path", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "example.com", 8080, "port/and/path", NULL, NULL);
    free(url_string);

    /* With port and query */
    url_string = strdup("http://example.com:8080?query=portANDquery");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with port and query", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "example.com", 8080, NULL, "query=portANDquery", NULL);
    free(url_string);

    /* With port and fragment */
    url_string = strdup("http://example.com:8080#f1");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with port and fragment", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "example.com", 8080, NULL, NULL, "f1");
    free(url_string);

    /* With port and credentials */
    url_string = strdup("http://u:p@example.com:8080");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with port and credentials", -1 != rc);
    assert_struct(url, "http", "u", "p", "example.com", 8080, NULL, NULL, NULL);
    free(url_string);

    /* With port and credentials (user only) */
    url_string = strdup("http://u@example.com:8080");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with port and user credential only", -1 != rc);
    assert_struct(url, "http", "u", NULL, "example.com", 8080, NULL, NULL, NULL);
    free(url_string);

    /* With path and query */
    url_string = strdup("http://example.com/path/and/query?q=yes");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with path and query", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "example.com", 0, "path/and/query", "q=yes", NULL);
    free(url_string);

    /* With path and fragment */
    url_string = strdup("http://example.com/path/and#fragment");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with path and fragment", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "example.com", 0, "path/and", NULL, "fragment");
    free(url_string);

    /* With query and fragment */
    url_string = strdup("http://example.com?q=yes#f1");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with query and fragment", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "example.com", 0, NULL, "q=yes", "f1");
    free(url_string);

    /* With query and credentials */
    url_string = strdup("http://u:p@example.com?q=yes");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with query and credentials", -1 != rc);
    assert_struct(url, "http", "u", "p", "example.com", 0, NULL, "q=yes", NULL);
    free(url_string);

    /* With query and credentials (user only) */
    url_string = strdup("http://u@example.com?q=yes");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with query and credentials", -1 != rc);
    assert_struct(url, "http", "u", NULL, "example.com", 0, NULL, "q=yes", NULL);
    free(url_string);

    /* With empty credentials */
    url_string = strdup("http://:@example.com");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with empty credentials", -1 != rc);
    assert_struct(url, "http", "", "", "example.com", 0, NULL, NULL, NULL);
    free(url_string);

    /* With empty credentials and port */
    url_string = strdup("http://:@example.com:89");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with empty credentials and port", -1 != rc);
    assert_struct(url, "http", "", "", "example.com", 89, NULL, NULL, NULL);
    free(url_string);

    /* Full URL */
    url_string = strdup("https://jack:password@localhost:8989/path/to/test?query=yes&q=jack#fragment1");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with port, path and query", -1 != rc);
    assert_struct(url, "https", "jack", "password", "localhost", 8989, "path/to/test", "query=yes&q=jack", "fragment1");
    free(url_string);

    return 0;
}

static const char *test_parse_http_url_ipv6_ok()
{
    int rc;
    struct yuarel url;
    char *url_string;

    /* Minimal URL with IPv6 hostname */
    url_string = strdup("http://[3ffe:2a00:100:7031::1]");
    rc = yuarel_parse(&url, url_string);
    mu_assert("minimal http URL with IPv6 hostname", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "3ffe:2a00:100:7031::1", 0, NULL, NULL, NULL);

    /* With IPv6 hostname and path (/) */
    url_string = strdup("http://[3ffe:2a00:100:7031::1]/");
    rc = yuarel_parse(&url, url_string);
    mu_assert("With IPv6 hostname and path ('/')", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "3ffe:2a00:100:7031::1", 0, "", NULL, NULL);
    free(url_string);

    /* With IPv6 hostname and path */
    url_string = strdup("http://[3ffe:2a00:100:7031::1]/path");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with IPv6 hostname and path", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "3ffe:2a00:100:7031::1", 0, "path", NULL, NULL);
    free(url_string);

    /* With IPv6 hostname and port */
    url_string = strdup("http://[3ffe:2a00:100:7031::1]:80");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with IPv6 hostname and port", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "3ffe:2a00:100:7031::1", 80, NULL, NULL, NULL);
    free(url_string);

    /* With IPv6 hostname and credentials */
    url_string = strdup("http://u:p@[3ffe:2a00:100:7031::1]");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with IPv6 hostname and credentials", -1 != rc);
    assert_struct(url, "http", "u", "p", "3ffe:2a00:100:7031::1", 0, NULL, NULL, NULL);

    /* With IPv6 hostname, port and path */
    url_string = strdup("http://[3ffe:2a00:100:7031::1]:8080/port/and/path");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with IPv6 hostname, port and path", -1 != rc);
    assert_struct(url, "http", NULL, NULL, "3ffe:2a00:100:7031::1", 8080, "port/and/path", NULL, NULL);
    free(url_string);

    /* With IPv6 hostname and empty credentials */
    url_string = strdup("http://:@[3ffe:2a00:100:7031::1]");
    rc = yuarel_parse(&url, url_string);
    mu_assert("with IPv6 hostname and empty credentials", -1 != rc);
    assert_struct(url, "http", "", "", "3ffe:2a00:100:7031::1", 0, NULL, NULL, NULL);
    free(url_string);

    return 0;
}

static const char *test_parse_http_rel_url_ok()
{
    int rc;
    struct yuarel url;
    char *url_string;

    /* Minimal relative URL */
    url_string = strdup("/");
    rc = yuarel_parse(&url, url_string);
    mu_assert("minimal relative URL", -1 != rc);
    assert_struct(url, NULL, NULL, NULL, NULL, 0, "", NULL, NULL);
    free(url_string);

    /* Path only */
    url_string = strdup("/hejsan");
    rc = yuarel_parse(&url, url_string);
    mu_assert("path only", -1 != rc);
    assert_struct(url, NULL, NULL, NULL, NULL, 0, "hejsan", NULL, NULL);
    free(url_string);

    /* Path and query */
    url_string = strdup("/hejsan?q=yes");
    rc = yuarel_parse(&url, url_string);
    mu_assert("path only", -1 != rc);
    assert_struct(url, NULL, NULL, NULL, NULL, 0, "hejsan", "q=yes", NULL);
    free(url_string);

    /* Path and fragment */
    url_string = strdup("/hejsan#fragment");
    rc = yuarel_parse(&url, url_string);
    mu_assert("path and fragment", -1 != rc);
    assert_struct(url, NULL, NULL, NULL, NULL, 0, "hejsan", NULL, "fragment");
    free(url_string);

    /* Path, query and fragment */
    url_string = strdup("/?q=yes&q2=no#fragment");
    rc = yuarel_parse(&url, url_string);
    mu_assert("path, query and fragment", -1 != rc);
    assert_struct(url, NULL, NULL, NULL, NULL, 0, "", "q=yes&q2=no", "fragment");
    free(url_string);

    return 0;
}

static const char *test_parse_url_fail()
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

    /* Missing credentials */
    url_string = strdup("rtsp://@hostname:8910/path");
    rc = yuarel_parse(&url, url_string);
    mu_assert("missing credentials should return -1", -1 == rc);
    free(url_string);

    return 0;
}

static const char *test_parse_file_url_ok()
{
    int rc;
    struct yuarel url;
    char *url_string;

    /* Minimal file URL */
    url_string = strdup("file:///home/john/file.txt");
    rc = yuarel_parse(&url, url_string);
    mu_assert("minimal file URL", -1 != rc);
    assert_struct(url, "file", NULL, NULL, NULL, 0, "/home/john/file.txt", NULL, NULL);
    free(url_string);

    /* File URL with directory */
    url_string = strdup("file:///home/john/");
    rc = yuarel_parse(&url, url_string);
    mu_assert("file URL with directory", -1 != rc);
    assert_struct(url, "file", NULL, NULL, NULL, 0, "/home/john/", NULL, NULL);
    free(url_string);

    /* File URL with path */
    url_string = strdup("file:///home/john/file.txt");
    rc = yuarel_parse(&url, url_string);
    mu_assert("file URL with path", -1 != rc);
    assert_struct(url, "file", NULL, NULL, NULL, 0, "/home/john/file.txt", NULL, NULL);
    free(url_string);

    /* File URL with absolute path */
    url_string = strdup("file:///absolute/path/to/file.txt");
    rc = yuarel_parse(&url, url_string);
    mu_assert("file URL with absolute path", -1 != rc);
    assert_struct(url, "file", NULL, NULL, NULL, 0, "/absolute/path/to/file.txt", NULL, NULL);
    free(url_string);

    return 0;
}

static const char *test_split_path_ok()
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

static const char *test_parse_query_ok()
{
    int rc;
    char *q;
    struct yuarel_param params[10];

    /* One param query */
    q = strdup("q=yes");
    rc = yuarel_parse_query(q, '&', params, 10);
    mu_assert("single parameter with value", 1 == rc);
    mu_silent_assert("first param key should be 'q'", 0 == strcmp("q", params[0].key));
    mu_silent_assert("first param val should be 'yes'", 0 == strcmp("yes", params[0].val));
    free(q);

    /* One param query without value */
    q = strdup("q");
    rc = yuarel_parse_query(q, '&', params, 10);
    mu_assert("single parameter without value", 1 == rc);
    mu_silent_assert("first param key should be 'q'", 0 == strcmp("q", params[0].key));
    mu_silent_assert("first param val should be NULL", NULL == params[0].val);
    free(q);

    /* Two param query */
    q = strdup("query=yes&a1=hello");
    rc = yuarel_parse_query(q, '&', params, 10);
    mu_assert("multiple params with value", 2 == rc);
    mu_silent_assert("first param key should be 'query'", 0 == strcmp("query", params[0].key));
    mu_silent_assert("first param val should be 'yes'", 0 == strcmp("yes", params[0].val));
    mu_silent_assert("second param key should be 'a1'", 0 == strcmp("a1", params[1].key));
    mu_silent_assert("second param val should be 'hello'", 0 == strcmp("hello", params[1].val));
    free(q);

    /* Two param query, one without value */
    q = strdup("query=yes&forceHttps");
    rc = yuarel_parse_query(q, '&', params, 10);
    mu_assert("multiple params one without value", 2 == rc);
    mu_silent_assert("first param key should be 'query'", 0 == strcmp("query", params[0].key));
    mu_silent_assert("first param val should be 'yes'", 0 == strcmp("yes", params[0].val));
    mu_silent_assert("second param key should be 'forceHttps'", 0 == strcmp("forceHttps", params[1].key));
    mu_silent_assert("second param val should be NULL", NULL == params[1].val);
    free(q);

    /* Three param query, all without value */
    q = strdup("query&forceHttps&log");
    rc = yuarel_parse_query(q, '&', params, 10);
    mu_assert("multiple params all without value", 3 == rc);
    mu_silent_assert("first param key should be 'query'", 0 == strcmp("query", params[0].key));
    mu_silent_assert("first param val should be NULL", NULL == params[0].val);
    mu_silent_assert("second param key should be 'forceHttps'", 0 == strcmp("forceHttps", params[1].key));
    mu_silent_assert("second param val should be NULL", NULL == params[1].val);
    mu_silent_assert("third param key should be 'log'", 0 == strcmp("log", params[2].key));
    mu_silent_assert("third param val should be NULL", NULL == params[2].val);
    free(q);

    /* Param with empty value */
    q = strdup("param=&query=no");
    rc = yuarel_parse_query(q, '&', params, 10);
    mu_assert("param with empty value", 2 == rc);
    mu_silent_assert("first param key should be 'param'", 0 == strcmp("param", params[0].key));
    mu_silent_assert("first param val should be ''", 0 == strcmp("", params[0].val));
    mu_silent_assert("second param key should be 'query'", 0 == strcmp("query", params[1].key));
    mu_silent_assert("second param val should be 'no'", 0 == strcmp("no", params[1].val));
    free(q);

    /* Double delimiter */
    q = strdup("param=jack&&query=no");
    rc = yuarel_parse_query(q, '&', params, 10);
    mu_assert("double delimiter", 3 == rc);
    mu_silent_assert("first param key should be 'param'", 0 == strcmp("param", params[0].key));
    mu_silent_assert("first param val should be 'jack'", 0 == strcmp("jack", params[0].val));
    mu_silent_assert("second param key should be ''", 0 == strcmp("", params[1].key));
    mu_silent_assert("second param val should be NULL", NULL == params[1].val);
    mu_silent_assert("third param key should be 'query'", 0 == strcmp("query", params[2].key));
    mu_silent_assert("third param val should be 'no'", 0 == strcmp("no", params[2].val));
    free(q);

    /* Delimiter in beginning */
    q = strdup("&param=jack&query=no");
    rc = yuarel_parse_query(q, '&', params, 10);
    mu_assert("delimiter in beginning", 3 == rc);
    mu_silent_assert("first param key should be ''", 0 == strcmp("", params[0].key));
    mu_silent_assert("first param val should be NULL", NULL == params[0].val);
    mu_silent_assert("second param key should be 'param'", 0 == strcmp("param", params[1].key));
    mu_silent_assert("second param val should be 'jack'", 0 == strcmp("jack", params[1].val));
    mu_silent_assert("third param key should be 'query'", 0 == strcmp("query", params[2].key));
    mu_silent_assert("third param val should be 'no'", 0 == strcmp("no", params[2].val));
    free(q);

    /* Delimiter at the end */
    q = strdup("param=jack&query=no&");
    rc = yuarel_parse_query(q, '&', params, 10);
    mu_assert("delimiter at the end", 3 == rc);
    mu_silent_assert("first param key should be 'param'", 0 == strcmp("param", params[0].key));
    mu_silent_assert("first param val should be 'jack'", 0 == strcmp("jack", params[0].val));
    mu_silent_assert("second param key should be 'query'", 0 == strcmp("query", params[1].key));
    mu_silent_assert("second param val should be 'no'", 0 == strcmp("no", params[1].val));
    mu_silent_assert("third param key should be ''", 0 == strcmp("", params[2].key));
    mu_silent_assert("third param val should be NULL", NULL == params[2].val);
    free(q);

    /* More kv in querystring than max parameters */
    q = strdup("a=b&c=d");
    rc = yuarel_parse_query(q, '&', params, 1);
    mu_assert("more kv in querystring than max parameters", 1 == rc);
    mu_silent_assert("first param key should be 'a'", 0 == strcmp("a", params[0].key));
    mu_silent_assert("first param val should be 'b'", 0 == strcmp("b", params[0].val));
    free(q);

    /* Key Value with present but empty value */
    q = strdup("a=b&f=&c=d");
    rc = yuarel_parse_query(q, '&', params, 3);
    mu_assert("key Value with present but empty value", 3 == rc);
    mu_silent_assert("first param key should be 'a'", 0 == strcmp("a", params[0].key));
    mu_silent_assert("first param val should be 'b'", 0 == strcmp("b", params[0].val));
    mu_silent_assert("first param key should be 'f'", 0 == strcmp("f", params[1].key));
    mu_silent_assert("first param key should be '' ", 0 == strcmp("", params[1].val));
    mu_silent_assert("first param key should be 'c'", 0 == strcmp("c", params[2].key));
    mu_silent_assert("first param val should be 'd'", 0 == strcmp("d", params[2].val));
    free(q);

    return 0;
}

static const char *yuarel_url_decode_ok()
{
    char *q;

    // Empty String
    q = strdup("");
    q = yuarel_url_decode(q);
    mu_assert("empty string do nothing", 0 == strcmp("", q));
    free(q);

    // Simple percent encoding
    q = strdup("Hello%20World");
    q = yuarel_url_decode(q);
    mu_assert("percent encoding %20 as space", 0 == strcmp("Hello World", q));
    free(q);

    // Multiple percent-encoded characters
    q = strdup("Hello%20World%21");
    q = yuarel_url_decode(q);
    mu_assert("multiple percent encoding", 0 == strcmp("Hello World!", q));
    free(q);

    // Plus as space (x-www-form-urlencoded)
    q = strdup("Hello+World");
    q = yuarel_url_decode(q);
    mu_assert("plus as space", 0 == strcmp("Hello World", q));
    free(q);

    // Malformed percent encoding (invalid hex)
    q = strdup("abc%xy");
    q = yuarel_url_decode(q);
    mu_assert("invalid percent encoding", 0 == strcmp("abc%xy", q));
    free(q);

    // Decoding multiple valid percent-encoded characters
    q = strdup("abc%20def%3A%2Fgh");
    q = yuarel_url_decode(q);
    mu_assert("decode multiple percent encodings", 0 == strcmp("abc def:/gh", q));
    free(q);

    // Percent encoding with uppercase hex
    q = strdup("Hex%2DTest");
    q = yuarel_url_decode(q);
    mu_assert("percent encoding with uppercase hex", 0 == strcmp("Hex-Test", q));
    free(q);

    // Percent encoding with lowercase hex
    q = strdup("Hello%3Aworld");
    q = yuarel_url_decode(q);
    mu_assert("percent encoding with lowercase hex", 0 == strcmp("Hello:world", q));
    free(q);

    // String with special characters
    q = strdup("Hello%2C%20how%20are%20you%3F");
    q = yuarel_url_decode(q);
    mu_assert("string with special characters", 0 == strcmp("Hello, how are you?", q));
    free(q);

    // URL-encoded key-value pair
    q = strdup("key%3Dvalue");
    q = yuarel_url_decode(q);
    mu_assert("decode key-value pair", 0 == strcmp("key=value", q));
    free(q);

    // Long input string with mixed encodings
    q = strdup("long%20test%20with%20multiple%20encodings%20%3D%20and%20spaces");
    q = yuarel_url_decode(q);
    mu_assert("long input with mixed encodings", 0 == strcmp("long test with multiple encodings = and spaces", q));
    free(q);

    // Percent encoding with multiple consecutive encodings
    q = strdup("Hello%%20World");
    q = yuarel_url_decode(q);
    mu_assert("multiple consecutive percent encodings", 0 == strcmp("Hello% World", q));
    free(q);

    return 0;
}

static const char *all_tests()
{
    mu_group("yuarel_parse() with an HTTP URL");
    mu_run_test(test_parse_http_url_ok);

    mu_group("yuarel_parse() with an HTTP URL (ipv6) ");
    mu_run_test(test_parse_http_url_ipv6_ok);

    mu_group("yuarel_parse() with a relative URL");
    mu_run_test(test_parse_http_rel_url_ok);

    mu_group("yuarel_parse() with faulty values");
    mu_run_test(test_parse_url_fail);

    mu_group("yuarel_parse() with file URL");
    mu_run_test(test_parse_file_url_ok);

    mu_group("yuarel_split_path()");
    mu_run_test(test_split_path_ok);

    mu_group("yuarel_parse_query()");
    mu_run_test(test_parse_query_ok);

    mu_group("yuarel_url_decode()");
    mu_run_test(yuarel_url_decode_ok);

    return 0;
}

int main(void)
{
    const char *result = all_tests();

    if (result != 0)
    {
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
