/*
 * This is a header that generates list functions and datastructures that
 * store a specific type of data. The data type and the name are specified
 * and the macros do the rest. The macros are provided to create both a
 * header file and an implementation file.
 *
 * In the header file, these two macros must be defined. The header file
 * must have a guard macro or the names will get confused.
 *
 * #define GENERIC_LIST_NAME name
 * #define GENERIC_LIST_DATA_TYPE int
 *
 * The macro GENERIC_LIST_HEADERS is provided next.
 *
 * In the implementation file, the macro GENERIC_LIST_FUNCTIONS is expanded.
 *
 * The implementation is a dynamic list of the data type specified. This list
 * can be accessed directly, by index, or as a stack. The list can also be
 * iterated.
 *
 * (get ready for serious preprocessor abuse (don't even try to debug it))
 *
 */
#ifndef __LIST_CONTAINER_H__
#define __LIST_CONTAINER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    LIST_NO_ERROR,  // success
    LIST_END,       // returned by iterate past end of list (not an error)
    LIST_EMPTY,     // error returned when attempt to read empty list
    LIST_UNDERRUN,  // error returned by pop on underrun
    LIST_OVERRUN,   // error return if index > nitems by insert of get
    LIST_MEMORY,    // error where memory cannot be allocated
    LIST_BAD_ARGS,  // error where bad args are supplied
} list_error_t;

#define GENERIC_LIST_HEADER(N, T) \
    typedef struct { \
        size_t capacity; \
        size_t nitems; \
        size_t index; \
        T* list; \
    } N ## _list_t; \
    \
    list_error_t N ## _init_list(N ## _list_t* list); \
    N ## _list_t* N ## _create_list(void); \
    list_error_t N ## _destroy_list(N ## _list_t* list); \
    list_error_t N ## _append_list(N ## _list_t* list, T *data); \
    list_error_t N ## _insert_list(N ## _list_t* list, size_t index, T *data); \
    list_error_t N ## _get_list(N ## _list_t* list, size_t index, T *data); \
    list_error_t N ## _reset_list(N ## _list_t* list); \
    list_error_t N ## _iterate_list(N ## _list_t* list, T *data); \
    list_error_t N ## _push_list(N ## _list_t* list, T *data); \
    list_error_t N ## _pop_list(N ## _list_t* list, T *data); \
    T* N ## _get_raw_list(N ## _list_t* list);


#define GENERIC_LIST_FUNCTIONS(N, T) \
    GENERIC_LIST_RESIZE(N, T) \
    GENERIC_LIST_INIT(N, T) \
    GENERIC_LIST_CREATE(N, T) \
    GENERIC_LIST_DESTROY(N, T) \
    GENERIC_LIST_APPEND(N, T) \
    GENERIC_LIST_INSERT(N, T) \
    GENERIC_LIST_GET(N, T) \
    GENERIC_LIST_RESET(N, T) \
    GENERIC_LIST_ITERATE(N, T) \
    GENERIC_LIST_PUSH(N, T) \
    GENERIC_LIST_POP(N, T) \
    GENERIC_LIST_GET_RAW(N, T)

#define GENERIC_LIST_RESIZE(N, T) \
    static list_error_t resize_list(N ## _list_t* list) { \
        if(list == NULL) \
            return LIST_BAD_ARGS; \
        if(list->nitems + 2 > list->capacity) { \
            list->capacity = list->capacity << 1; \
            list->list = realloc(list->list, list->capacity * sizeof(T)); \
            if(list->list == NULL) { \
                return LIST_MEMORY; \
            } \
        } \
        return LIST_NO_ERROR; \
    }

#define GENERIC_LIST_INIT(N, T) \
    list_error_t N ## _init_list(N ## _list_t* list) { \
        if(list == NULL) \
            return LIST_BAD_ARGS; \
        list->capacity = 0x01 << 3; \
        list->nitems = 0; \
        list->index = 0; \
        list->list = (T*)calloc(list->capacity, sizeof(T)); \
        if(list->list == NULL) {\
            return LIST_MEMORY; \
        } \
        return LIST_NO_ERROR; \
    }

#define GENERIC_LIST_CREATE(N, T)  \
    N ## _list_t* N ## _create_list(void) { \
        N ## _list_t* list; \
        list = (N ## _list_t*)malloc(sizeof(N ## _list_t)); \
        if(list == NULL) {\
            return NULL; \
        } \
        if(N ## _init_list(list) != LIST_NO_ERROR) { \
            free(list); \
            return NULL; \
        } \
        return list; \
    }

#define GENERIC_LIST_DESTROY(N, T) \
    list_error_t N ## _destroy_list(N ## _list_t* list) { \
        if(list == NULL) \
            return LIST_BAD_ARGS; \
        if(list->list != NULL && list->nitems != 0) \
            free(list->list); \
        free(list); \
        return LIST_NO_ERROR; \
    }

#define GENERIC_LIST_APPEND(N, T) \
    list_error_t N ## _append_list(N ## _list_t* list, T* data) { \
        if(list == NULL || data == NULL) \
            return LIST_BAD_ARGS; \
        if(resize_list(list) != LIST_NO_ERROR) \
            return LIST_MEMORY; \
        memcpy(&list->list[list->nitems], data, sizeof(T)); \
        list->nitems++; \
        return LIST_NO_ERROR; \
    }

#define GENERIC_LIST_INSERT(N, T) \
    list_error_t N ## _insert_list(N ## _list_t* list, size_t index, T* data) { \
        if(list == NULL || data == NULL) \
            return LIST_BAD_ARGS; \
        if(resize_list(list) != LIST_NO_ERROR) \
            return LIST_MEMORY; \
        if(index < list->nitems) {\
            memmove(&list->list[index+1], &list->list[index], sizeof(T) * (list->nitems - index)); \
            memcpy(&list->list[index], data, sizeof(T)); \
            list->nitems++; \
        } \
        else { \
            memcpy(&list->list[list->nitems], data, sizeof(T)); \
            list->nitems++; \
            return LIST_OVERRUN; \
        } \
        return LIST_NO_ERROR; \
    }

#define GENERIC_LIST_GET(N, T) \
    list_error_t N ## _get_list(N ## _list_t* list, size_t index, T* data) { \
        if(list == NULL || data == NULL) \
            return LIST_BAD_ARGS; \
        if(index < list->nitems) { \
            memcpy(data, &list->list[index], sizeof(T)); \
            return LIST_NO_ERROR; \
        } \
        return LIST_OVERRUN; \
    }

#define GENERIC_LIST_RESET(N, T) \
    list_error_t N ## _reset_list(N ## _list_t* list) { \
        if(list == NULL) \
            return LIST_BAD_ARGS; \
        list->index = 0; \
        return LIST_NO_ERROR; \
    }

#define GENERIC_LIST_ITERATE(N, T) \
    list_error_t N ## _iterate_list(N ## _list_t* list, T* data) { \
        if(list == NULL || data == NULL) \
            return LIST_BAD_ARGS; \
        if(list->index < list->nitems) { \
            memcpy(data, &list->list[list->index], sizeof(T)); \
            list->index++; \
            return LIST_NO_ERROR; \
        } \
        return LIST_END; \
    }

#define GENERIC_LIST_PUSH(N, T) \
    list_error_t N ## _push_list(N ## _list_t* list, T* data) { \
        if(list == NULL || data == NULL) \
            return LIST_BAD_ARGS; \
        return N ## _append_list(list, data); \
    }

#define GENERIC_LIST_POP(N, T) \
    list_error_t N ## _pop_list(N ## _list_t* list, T* data) { \
        if(list == NULL || data == NULL) \
            return LIST_BAD_ARGS; \
        if(list->nitems > 0) { \
            list->nitems--; \
            memcpy(data, &list->list[list->nitems], sizeof(T)); \
            return LIST_NO_ERROR; \
        } \
        return LIST_UNDERRUN; \
    }

#define GENERIC_LIST_GET_RAW(N, T) \
    T* N ## _get_raw_list(N ## _list_t* list) { \
        if(list == NULL) \
            return NULL; \
        return list->list; \
    }

#endif

