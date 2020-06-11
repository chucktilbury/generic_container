
#include "generic_list.h"

GENERIC_LIST_FUNCTIONS(test, char*)

int main(void) {

    char* strs[] = {"foo", "barzippoblart", "baz", "bacon", "eggs", "potatoes", "onions", "nuclear", "powered", "chicken", NULL};
    test_list_t* list = test_create_list();
    char* ptr;

    printf("nitems: %lu\n", list->nitems);
    printf("capacity: %lu\n", list->capacity);

    for(int i = 0; strs[i] != NULL; i++)
        test_append_list(list, &strs[i]);

    printf("nitems: %lu\n", list->nitems);
    printf("capacity: %lu\n", list->capacity);

    int i = 0;
    while(test_get_list(list, i++, &ptr) == LIST_NO_ERROR)
        printf("%d: value: %s\n", i, ptr);

    printf("nitems: %lu\n", list->nitems);
    printf("capacity: %lu\n", list->capacity);

    i = 1;
    test_reset_list(list);
    while(test_iterate_list(list, &ptr) == LIST_NO_ERROR)
        printf("%d: value: %s\n", i++, ptr);

    printf("nitems: %lu\n", list->nitems);
    printf("capacity: %lu\n", list->capacity);

    test_insert_list(list, 3, &strs[5]);

    i = 1;
    test_reset_list(list);
    while(test_iterate_list(list, &ptr) == LIST_NO_ERROR)
        printf("%d: value: %s\n", i++, ptr);

    printf("nitems: %lu\n", list->nitems);
    printf("capacity: %lu\n", list->capacity);

    test_destroy_list(list);
}
