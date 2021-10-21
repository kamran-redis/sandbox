#ifndef INTERVALTREE_ALLOC_H
#define INTERVALTREE_ALLOC_H
#define intervaltree_malloc RedisModule_Alloc
#define intervaltree_realloc RedisModule_Realloc
#define intervaltree_free RedisModule_Free
extern void *(*RedisModule_Alloc)(size_t bytes);
extern void *(*RedisModule_Realloc)(void *ptr, size_t bytes);
extern void (*RedisModule_Free)(void *ptr);
#endif