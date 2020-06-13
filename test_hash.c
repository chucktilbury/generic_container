
#include "hash_container.h"

HASH_CONTAINER_HEADER(test0, char*)
HASH_CONTAINER_FUNCTIONS(test0, char*)

HASH_CONTAINER_HEADER(test1, int)
HASH_CONTAINER_FUNCTIONS(test1, int)

int main(void)
{
    char* strs[] = {"foo",      "bar",       "baz",      "bacon",   "eggs",    "potatoes",  "onions",
                    "knuckles", "are",       "dragging", "hoops",   "of",      "chocolate", "almonds",
                    "with",     "sprinkles", "and",      "cyanide", "log",     "balls",     "eaten",
                    "by",       "unicorns",  "as",       "pink",    "stripes", "given",     "to",
                    "nuclear",  "pound",     "cake",     "candles", "snards",  "and",       "snipes",    NULL};

    test0_table_t* tab = test0_create();
    char* buf;

    printf("\ninsert entries\n");
    printf("  table capacity: %lu\n", tab->capacity);
    printf("  table count: %lu\n", tab->count);
    for(int i = 0; strs[i] != NULL; i++)
        test0_insert(tab, strs[i], &strs[i]);

    printf("\nfind entries\n");
    printf("  table capacity: %lu\n", tab->capacity);
    printf("  table count: %lu\n", tab->count);

    for(int i = 0; strs[i] != NULL; i++)
    {
        test0_find(tab, strs[i], &buf);
        printf("%3d. value: %s\n", i+1, buf);
    }

    test0_find(tab, "poopoo", &buf);
    printf("glop = %s\n", buf);

    test0_destroy(tab);

    test1_table_t* tab1 = test1_create();
    int val;

    printf("\ninsert entries\n");
    printf("  table capacity: %lu\n", tab1->capacity);
    printf("  table count: %lu\n", tab1->count);
    for(int i = 0; strs[i] != NULL; i++)
        test1_insert(tab1, strs[i], &i);

    printf("\nfind entries\n");
    printf("  table capacity: %lu\n", tab1->capacity);
    printf("  table count: %lu\n", tab1->count);

    for(int i = 0; strs[i] != NULL; i++)
    {
        test1_find(tab1, strs[i], &val);
        printf("%3d. key: %s, value: %d\n", i+1, strs[i], val);
    }

    test1_find(tab1, "poopoo", &val);
    printf("glop = %d\n", val);

    test1_destroy(tab1);
}


