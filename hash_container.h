#ifndef __HASH_CONTAINER_H__
#define __HASH_CONTAINER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum
{
    HASH_NO_ERROR,
    HASH_EXIST,
    HASH_NOT_FOUND,
    HASH_ARGS,
    HASH_MEMORY,
};

#define TABLE_MAX_LOAD 0.75

#define HASH_CONTAINER_HEADER(N, T) \
        typedef struct { \
            const char* key; \
            T data; \
        } N ## _entry_t; \
        \
        typedef struct { \
            size_t count; \
            size_t capacity; \
            size_t index; \
            N ## _entry_t* entries; \
        } N ## _table_t; \
        \
        N ## _table_t* N ## create(void); \
        int N ## destroy(N ## _table_t* table); \
        int N ## insert(N ## _table_t* table, const char* key, T* data); \
        int N ## find(N ## _table_t* table, const char* key, T* data); \
        int N ## reset(N ## _table_t* table); \
        N ## _entry_t* N ## iterate(N ## _table_t* tab);

#define HASH_CONTAINER_FUNCTIONS(N, T) \
        HASH_CONTAINER_HASH(N, T) \
        HASH_CONTAINER_SLOT(N, T) \
        HASH_CONTAINER_GROW(N, T) \
        HASH_CONTAINER_CREATE(N, T) \
        HASH_CONTAINER_DESTROY(N, T) \
        HASH_CONTAINER_INSERT(N, T) \
        HASH_CONTAINER_FIND(N, T) \
        HASH_CONTAINER_RESET(N, T) \
        HASH_CONTAINER_ITERATE(N, T)


#define HASH_CONTAINER_HASH(N, T) \
        static uint32_t N ## _hash(const char* key) { \
            uint32_t hash = 2166136261u; \
            for(int i = 0; i < (int)strlen(key); i++) { \
                hash ^= key[i]; \
                hash *= 16777619; \
            } \
            return hash; }

#define HASH_CONTAINER_SLOT(N, T) \
        static N ## _entry_t* N ## _slot(N ## _entry_t* ent, size_t cap, const char* key) { \
            uint32_t index = N ## _hash(key) & (cap - 1); \
            while(1) { \
                N ## _entry_t* entry = &ent[index]; \
                if((entry->key == NULL) || (!strcmp(key, entry->key))) { \
                    return entry; \
                } \
                index = (index + 1) & (cap - 1); }}

#define HASH_CONTAINER_GROW(N, T) \
        static void N ## _grow(N ## _table_t* tab) { \
            if(tab->count + 2 > tab->capacity * TABLE_MAX_LOAD) { \
                size_t capacity = tab->capacity << 1; \
                N ## _entry_t* entries = (N ## _entry_t*)calloc(capacity, sizeof(N ## _entry_t)); \
                if(entries == NULL) { \
                    fprintf(stderr, "cannot allocate %lu bytes for hash table", capacity * sizeof(N ## _entry_t)); \
                    exit(1); \
                } \
                if(tab->entries != NULL) {  \
                    for(int i = 0; i < (int)tab->capacity; i++) { \
                        if(tab->entries[i].key != NULL) { \
                            N ## _entry_t* ent = N ## _slot(entries, capacity, tab->entries[i].key); \
                            ent->key = tab->entries[i].key; \
                            memcpy(&ent->data, &tab->entries[i].data, sizeof(T)); \
                        } \
                    } \
                    free(tab->entries); \
                } \
                tab->entries = entries; \
                tab->capacity = capacity; }}

#define HASH_CONTAINER_CREATE(N, T) \
        N ## _table_t* N ## _create(void) { \
            N ## _table_t* tab; \
            tab = malloc(sizeof(N ## _table_t)); \
            if(tab == NULL) { \
                fprintf(stderr, "cannot allocate %lu bytes for hash table structure", sizeof(N ## _table_t)); \
                exit(1); \
            } \
            tab->capacity = 0x01 << 3; \
            tab->count = 1; \
            tab->entries = (N ## _entry_t*)calloc(tab->capacity, sizeof(N ## _entry_t)); \
            if(tab->entries == NULL) { \
                fprintf(stderr, "cannot allocate %lu bytes for hash table entries", tab->capacity * sizeof(N ## _entry_t)); \
                free(tab); \
                exit(1); \
            } \
            return tab; }

#define HASH_CONTAINER_DESTROY(N, T) \
        void N ## _destroy(N ## _table_t* tab) { \
            if(tab != NULL) { \
                if(tab->entries != NULL) { \
                    for(int i = 0; i < (int)tab->capacity; i++) { \
                        if(tab->entries[i].key != NULL) \
                            free((void*)tab->entries[i].key); \
                    } \
                    free(tab->entries); \
                } \
                free(tab); }}

/*
 * Refuse to replace an entry.
 */
#define HASH_CONTAINER_INSERT(N, T) \
        int N ## _insert(N ## _table_t* tab, const char* key, T* data) { \
            N ## _grow(tab);\
            N ## _entry_t* entry = N ## _slot(tab->entries, tab->capacity, key); \
            if(entry != NULL && entry->key == NULL) { \
                entry->key = strdup(key); \
                if(entry->key == NULL) { \
                    fprintf(stderr, "cannot allocate %lu bytes for hash table key", strlen(key)); \
                    return HASH_MEMORY; \
                } \
                memcpy(&entry->data, data, (sizeof(T))); \
                tab->count++; \
            } \
            else return HASH_EXIST; \
            return HASH_NO_ERROR; }


/*
 * Return a pointer to the data that the key is bound to. If the key is not
 * found, then return NULL.
 */
#define HASH_CONTAINER_FIND(N, T) \
        int N ## _find(N ## _table_t* tab, const char* key, T* data) { \
            N ## _entry_t* entry = N ## _slot(tab->entries, tab->capacity, key); \
            if(entry->key != NULL) { \
                memcpy(data, &entry->data, sizeof(T)); \
                return HASH_NO_ERROR; \
            } return HASH_NOT_FOUND; }

/*
 * Reset the internal index to zero for iterating the list.
 */
#define HASH_CONTAINER_RESET(N, T) \
        void N ## _reset(N ## _table_t* tab) { tab->index = 0; }

/*
 * Iterate all of the keys in the hash table. Used for various dump utilities.
 *
 * Make reset != 0 to reset to the beginning of the table.
 */
#define HASH_CONTAINER_ITERATE(N, T)  \
        N ## _entry_t* N ## _iterate(N ## _table_t* tab) { \
            for(; tab->index < tab->capacity; tab->index++) { \
                if(tab->entries[tab->index].key != NULL) { \
                    return &tab->entries[tab->index]; \
                }} \
            return NULL; }

#endif

