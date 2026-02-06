#include <stdio.h>
#include <yuarel.h>

#define MAX_PART_COUNT 3
#define MAX_PARAM_COUNT 6

int main(void)
{
    int p = 0;
    struct yuarel url = {0};
    char *parts[MAX_PART_COUNT] = {NULL};
    struct yuarel_param params[MAX_PARAM_COUNT] = {0};
    char url_string[] = "http://localhost:8989/path/to/test?flag1&query=yes&flag2&param1=no&flag3&greet=hello%20world#frag=1";

    if (-1 == yuarel_parse(&url, url_string))
    {
        fprintf(stderr, "Could not parse url!\n");
        return 1;
    }

    printf("Struct values:\n");
    printf("\tscheme:\t\t%s\n", url.scheme);
    printf("\thost:\t\t%s\n", url.host);
    printf("\tport:\t\t%d\n", url.port);
    printf("\tpath:\t\t%s\n", url.path);
    printf("\tquery:\t\t%s\n", url.query);
    printf("\tfragment:\t%s\n", url.fragment);

    if (3 != yuarel_split_path(url.path, parts, MAX_PART_COUNT))
    {
        fprintf(stderr, "Could not split path!\n");
        return 1;
    }

    printf("\nPath parts: '%s', '%s', '%s'\n\n", parts[0], parts[1], parts[2]);

    printf("Query string parameters:\n");

    p = yuarel_parse_query(url.query, '&', params, MAX_PARAM_COUNT);
    while (p-- > 0)
    {
        yuarel_url_decode(params[p].key);
        yuarel_url_decode(params[p].val);
        printf("\t%s: %s\n", params[p].key, params[p].val);
    }
}
