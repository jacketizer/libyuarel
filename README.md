libyuarel
=========

<versionBadge>![Version 1.0.0](https://img.shields.io/badge/version-1.0.0-blue.svg)</versionBadge>
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C](https://img.shields.io/badge/Language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))
[![CI/CD Status Badge](https://github.com/mofosyne/libyuarel/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/mofosyne/libyuarel/actions)

Very simple and well tested C library for parsing URLs and querystrings 
with zero-copy and no mallocs. The library does not do any validation on
the URL, neither before nor after it is parsed. The different parts are 
parsed by searching for special characters like `:` and `/`. For a URL 
should be able to be parsed by yuarel, it has to be constructed in one 
of the following formats:

*Absolute URL:*
scheme ":" [ "//" ] [ username ":" password "@" ] host [ ":" port ] [ "/" ] [ path ] [ "?" query ] [ "#" fragment ]

*Relative URL:*
path [ "?" query ] [ "#" fragment ]

Parts within `[` and `]` are optional. A minimal URL could look like this:

`a:b` or `/`

Due to the fact that the library isn't copying any strings and instead points
to the parts in the URL string, the first `/` in the path will be replaced with
a null terminator. Therefore, the first slash will be missing in the path.

## To install directly in your C project via clib

The library consists of two files, easily integrated into your project manually or via [clib](https://github.com/clibs/clib)

```bash
clib install mofosyne/libyuarel
```

or copy these files manually to your source folder:

```bash
yuarel.c
yuarel.h
```
## To build and install libyuarel library

```sh
$ make && make check && sudo make install
```

Which by default will install both the dynamic library `libyuarel.so.1`
as well as the static library `libyuarel.a` to `/usr/lib` with `yuarel.h`
copied to `/usr/include`.

## Try it

Compile the example in `examples/`:

```sh
$ make examples
```

Run the example program:

```sh
$ ./simple
```

You should expect to see an output like so:

```
Struct values:
        scheme:         http
        host:           localhost
        port:           8989
        path:           path/to/test
        query:          query=yes&param1=no
        fragment:       frag=1

Path parts: 'path', 'to', 'test'

Query string parameters:
        param1: no
        query: yes
```

If you see this error

```bash
$ make examples
gcc examples/simple.c -lyuarel -o simple
examples/simple.c:3:10: fatal error: yuarel.h: No such file or directory
    3 | #include <yuarel.h>
      |          ^~~~~~~~~~
compilation terminated.
make: *** [Makefile:48: examples] Error 1
```

Then you have likely not successfully installed libyuarel library
as explained in previous section.

## The structs

The struct that holds the parsed URL looks like this:

```C
struct yuarel {
	char *scheme;	/* scheme, without ":" and "//" */
	char *username;	/* username, default: NULL */
	char *password;	/* password, default: NULL */
	char *host;	/* hostname or IP address */
	int port;	/* port, default: 0 */
	char *path;	/* path, without leading "/", default: NULL */
	char *query; 	/* query, default: NULL */
	char *fragment;	/* fragment, default: NULL */
};
```

The struct that holds a parsed query string parameter looks like this:

```C
struct yuarel_param {
	char *key;
	char *val;
};
```

## Library functions

### Parse a URL to a struct

```C
int yuarel_parse(struct yuarel *url, char *url_str)
```

`struct yuarel *url`: a pointer to the struct where to store the parsed values.
`char *url_str`: a pointer to the url to be parsed (null terminated).

**Note that the url string will be modified by the function.**

Returns 0 on success, otherwise -1.

### Split a path into several strings

```C
int yuarel_split_path(char *path, char **parts, int max_parts)
```

No data is copied, the slashed are used as null terminators and then
pointers to each path part will be stored in `parts`.

`char *path`: the path to split. The string will be modified.
`char **parts`: a pointer to an array of `(char *)` where to store the result.
`int max_parts`: max number of parts to parse.

**Note that the path string will be modified by the function.**

Returns the number of parsed items. -1 on error.

### Parse a query string

```C
int yuarel_parse_query(char *query, char delimiter, struct yuarel_param *params, int max_params)
```

`char *query`: the query string to parse. The string will be modified.
`char delimiter`: the character that separates the key/value pairs from each other.
`struct yuarel_param *params`: an array of `(struct yuarel_param)` where to store the result.
`int max_values`: max number of parameters to parse.

The query string should be a null terminated string of parameters separated by
a delimiter. Each parameter are checked for the equal sign character. If it
appears in the parameter, it will be used as a null terminator and the part
that comes after it will be the value of the parameter.

No data are copied, the equal sign and delimiters are used as null
terminators and then pointers to each parameter key and value will be stored
in the yuarel_param struct.

**Note that the query string will be modified by the function.**

Returns the number of parsed items. -1 on error.

## How to use it:

Compile with `-lyuarel`.

```C
#include <stdlib.h>
#include <stdio.h>
#include <yuarel.h>

int main(void)
{
	int p;
	struct yuarel url;
	char *parts[3];
	char url_string[] = "http://localhost:8989/path/to/test?query=yes#frag=1";

	if (-1 == yuarel_parse(&url, url_string)) {
		fprintf(stderr, "Could not parse url!\n");
		return 1;
	}

	printf("scheme:\t%s\n", url.scheme);
	printf("host:\t%s\n", url.host);
	printf("port:\t%d\n", url.port);
	printf("path:\t%s\n", url.path);
	printf("query:\t%s\n", url.query);
	printf("fragment:\t%s\n", url.fragment);

	if (3 != yuarel_split_path(url.path, parts, 3)) {
		fprintf(stderr, "Could not split path!\n");
		return 1;
	}

	printf("path parts: %s, %s, %s\n", parts[0], parts[1], parts[2]);

	printf("Query string parameters:\n");

	p = yuarel_parse_query(url.query, '&', params, 3);
	while (p-- > 0) {
		printf("\t%s: %s\n", params[p].key, params[p].val);
	}
}
```

## Maintainer History

* 2016-04-01 : Originally Created By [jacketizer](https://github.com/jacketizer) under [jacketizer/libyuarel](https://github.com/jacketizer/libyuarel)
* 2025-01-27 : Forked by Mofosyne to make it avaliable under clib for better discoverbility by other C programmers, in addition to seeing that this project has not been maintained for some time (With [issue tickets](https://github.com/jacketizer/libyuarel/issues) appearing abandoned for quite a while).
	- Did some investigation and found [user without password does not parse #13](https://github.com/jacketizer/libyuarel/issues/13#issue-502758138)
		- There was an out of repo patch for it's usage in a seperate software named [flux-core](https://github.com/garlick/flux-core/commit/9f9c59b0b584df21061ade70ed4cdc49ce707ebd). Looks important enough to include compaired to other changes shown in the original [jaketizer/libyuarel github network graph](https://github.com/jacketizer/libyuarel/network).
