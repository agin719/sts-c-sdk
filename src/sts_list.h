#ifndef _STS_LIST_H_
#define _STS_LIST_H_

// from kernel list
typedef struct sts_list_s sts_list_t;

struct sts_list_s {
    sts_list_t *next, *prev;
};

#define sts_list_head_init(name) {&(name), &(name)}

#define sts_list_init(ptr) do {                  \
        (ptr)->next = (ptr);                    \
        (ptr)->prev = (ptr);                    \
    } while (0)

static inline void __sts_list_add(sts_list_t *list, sts_list_t *prev, sts_list_t *next)
{
    next->prev = list;
    list->next = next;
    list->prev = prev;
    prev->next = list;
}

// list head to add it before
static inline void sts_list_add_tail(sts_list_t *list, sts_list_t *head)
{
    __sts_list_add(list, head->prev, head);
}

static inline void __sts_list_del(sts_list_t *prev, sts_list_t *next)
{
    next->prev = prev;
    prev->next = next;
}

// deletes entry from list
static inline void sts_list_del(sts_list_t *entry)
{
    __sts_list_del(entry->prev, entry->next);
    sts_list_init(entry);
}

// tests whether a list is empty
static inline int sts_list_empty(const sts_list_t *head)
{
    return (head->next == head);
}

// move list to new_list
static inline void sts_list_movelist(sts_list_t *list, sts_list_t *new_list)
{
    if (!sts_list_empty(list)) {
        new_list->prev = list->prev;
        new_list->next = list->next;
        new_list->prev->next = new_list;
        new_list->next->prev = new_list;
        sts_list_init(list);
    } else {
        sts_list_init(new_list);
    }
}

static inline int sts_list_size(sts_list_t *head) 
{
    int len = 0;
    sts_list_t *node = head->next;        
    for ( ; node != head; node = node->next) {
        len++;                             
    }
    return len;
}


#define STS_OFFSETOF(type, member) \
    ((long) (((char *) (&(((type*)0)->member))) - ((char *) 0)))
    
// get last
#define sts_list_get_last(list, type, member)                           \
    sts_list_empty(list) ? NULL : sts_list_entry((list)->prev, type, member)

// get first
#define sts_list_get_first(list, type, member)                          \
    sts_list_empty(list) ? NULL : sts_list_entry((list)->next, type, member)

#define sts_list_entry(ptr, type, member) \
    (type *)( (char *)ptr - STS_OFFSETOF(type, member) )

// traversing
#define sts_list_for_each_entry(postp, pos, head, member)                      \
    for (pos = sts_list_entry((head)->next, postp, member);      \
         &pos->member != (head);                                        \
         pos = sts_list_entry(pos->member.next, postp, member))

#define sts_list_for_each_entry_reverse(postp, pos, head, member)              \
    for (pos = sts_list_entry((head)->prev, postp, member);      \
         &pos->member != (head);                                        \
         pos = sts_list_entry(pos->member.prev, postp, member))

#define sts_list_for_each_entry_safe(postp, pos, n, head, member)              \
    for (pos = sts_list_entry((head)->next, postp, member),      \
                 n = sts_list_entry(pos->member.next, postp, member); \
         &pos->member != (head);                                        \
         pos = n, n = sts_list_entry(n->member.next, postp, member))

#define sts_list_for_each_entry_safe_reverse(postp, pos, n, head, member)      \
    for (pos = sts_list_entry((head)->prev, postp, member),      \
                 n = sts_list_entry(pos->member.prev, postp, member); \
         &pos->member != (head);                                        \
         pos = n, n = sts_list_entry(n->member.prev, postp, member))

#endif
