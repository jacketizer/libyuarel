# libyuarel

Simple C library for parsing URLs with zero-copy and no mallocs. The library
does not do any validation on the URL, neither before nor after it is parsed.
The different parts are parsed by searching for special characters like `:`
and `/`. For a URL should be able to be parsed by yuarel, it has to be
constructed like this:

`scheme ":" [ "//" ] host [ ":" port ] [ "/" ] [ path ] [ "?" query ]`

Parts within `[` and `]` are optional. A minimal URL could look like this:

`proto:hostname`

Due to the fact that the library isn't copying any strings and instead points
to the parts in the URL string, the first `/` in the path will be used as a
null terminator. Therefore, the first slash will be missing in the path.

## To build

```sh
$ make && sudo make install
```

## Try it

Compile the example in `examples/`:

```sh
$ make examples
```

Run the example program:

```sh
$ ./simple
```

## Library functions

### `int yuarel_parse(struct yuarel *url, char *url_str)`

Parse a URL to a struct.

*url: a pointer to the struct where to store the parsed values.
*url_str: a pointer to the url to be parsed (null terminated).

Returns 0 on success, otherwise -1.

### `int yuarel_split_path(char *path, char **parts, int max_parts)`

Split a path into several strings.

No data is copied, the slashed are used as null terminators and then
pointers to each path part will be stored in `**parts`.

*path: the path to split.
**parts: a pointer to an array of `(char *)` where to store the result.
max_parts: max number of parts to parse.

## How to use it:

Compile with `-lyuarel`.

```C
#include <stdlib.h>
#include <stdio.h>
#include <yuarel.h>

int main(void)
{
	struct yuarel url;
	char *parts[3];
	char url_string[] = "http://localhost:8989/path/to/test?query=yes";

	if (-1 == yuarel_parse(&url, url_string)) {
		fprintf(stderr, "Could not parse url!\n");
		return 1;
	}

	printf("scheme:\t%s\n", url.scheme);
	printf("host:\t%s\n", url.host);
	printf("port:\t%d\n", url.port);
	printf("path:\t%s\n", url.path);
	printf("query:\t%s\n", url.query);

	if (3 != yuarel_split_path(url.path, parts, 3)) {
		fprintf(stderr, "Could not split path!\n");
		return 1;
	}

	printf("path parts: %s, %s, %s\n", parts[0], parts[1], parts[2]);
}

```
