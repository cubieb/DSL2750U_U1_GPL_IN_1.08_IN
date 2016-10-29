#ifndef _LINUX_GHASH_H_
#define _LINUX_GHASH_H_

#define USER_LEVEL_HASH

#define create_hashtable(hsize) \
         hash_create(lh_strhash, equal_str, hsize)

struct hashentry;
struct _hashtable;
typedef struct _hashtable   hashtable;

struct hashentry
{
    void             *key;
    void             *data;

    struct hashentry *next;
};

struct _hashtable
{
    unsigned int      (*gethash)(void *);
    int               (*compare)(void *, void *);
    int               hashsize;
    int               count;
    struct hashentry **hashlist;
};

#define hashindex(key, tab) ((tab->gethash)(key) % (tab->hashsize -1))

hashtable *hash_create(unsigned int (*keyfunc)(void *),
                       int (*comparefunc)(void *,void *),
                       int size);
void hash_free(hashtable *tab);
void hash_insert(void *key, void *data, hashtable *tab);
void hash_remove(void *key, hashtable *tab);
void *hash_value(void *key, hashtable *tab);
void hash_for_each_do(hashtable *tab, int (cb)(void *, void *));
int hash_count(hashtable *tab);

#define DEFAULT_HASH_SIZE 256

#endif

