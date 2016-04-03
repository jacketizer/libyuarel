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
