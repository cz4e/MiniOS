#ifndef _SYS_QUEUE_H_
#define _SYS_QUEUE_H_

#ifndef NULL
#define NULL        (void *)0
#endif
#define SLIST_HEAD(name,type)                           \
struct name {                                           \
    struct type *slh_first;                             \
}

#define SLIST_ENTRY(type)                               \
struct {                                                \
    struct type *sle_next;                              \
}

#define SLIST_EMPTY(head)               ((head)->slh_first)
#define SLIST_NEXT(elm,head)            ((head)->field.sle_next)
#define SLIST_FIRST(head)               ((head)->slh_first)
#define SLIST_INSERT_HEAD(head,elm,field)               do{\
                                                            (elm)->field.sle_next = SLIST_FIRST((head))     \
                                                            (head)->slh_first = &(elm)->field;              \
                                                        }while(0)

#define SLIST_INSERT_AFTER(obj,elm,field)               do{\
                                                            SLIST_NEXT((elm),field) = SLIST_NEXT((obj),field);  \
                                                            SLIST_NEXT((obj),field) = &(elm)->field;            \
                                                        }while(0)

#define SLIST_FOREACH(var,head,field)                   \
    for((var) = SLIST_FIRST((head));                    \
    (var);                                              \
    (var) = SLIST((var),field))

#define SLIST_HEAD_INIT(elm,field)                      \
    ((elm)->field.slh_first = NULL)     
                                                  
#define LIST_HEAD(name,type)                            \
struct name {                                           \
    struct type *lh_first;                              \
}

#define LIST_ENTRY(type)                                \
struct {                                                \
    struct type *le_next;                               \
    struct type **le_prev;                              \
}

#define LIST_HEAD_INIT(elm,field)                       \
    ((elm)->field.lh_first = NULL)

#define LIST_ENTRY_INIT(elm,field)                      \
    ((elm)->field.le_next = NULL,(elm)->field.le_prev = NULL)
#define LIST_EMPTY(head)                ((head)->lh_first == NULL)
#define LIST_NEXT(elm,field)            ((elm)->field.le_next)
#define LIST_PREV(elm,field)            ((elm)->field.le_prev)
#define LIST_FIRST(head)                ((head)->le_first)
#define LIST_INSERT_HEAD(head,elm,field)                do{\
                                                            (elm)->field.le_next = LIST_FIRST((head));      \
                                                            LIST_PREV(LIST_FIRST((head)),field) = (elm);    \
                                                            (head)->lh_first = &(elm)->field;               \
                                                        }while(0)

#define LIST_INSERT_AFTER(obj,elm,field)                do{\
                                                            (elm)->field.le_next = LIST_NEXT(obj,field);                \
                                                            LIST_PREV(LIST_NEXT(obj,field),field) = &(elm)->field;      \
                                                            (elm)->field.le_prev = &(obj)->field;                       \
                                                            LIST_NEXT(obj,field) = (elm);                               \
                                                        }while(0)

#define LIST_INSERT_BEFOR(obj,elm,field)                do{\

                                                        }while(0)
#define LIST_REMOVE_HEAD(head,elm)                      do{\


                                                        }while(0)
#define LIST_FOREACH(var,head,field)                    \
        for((var) = LIST_FIRST((head));                 \
        (var);                                          \
        (var) = LIST_NEXT((var),field))                 \

#define LIST_INSERT_BY_KEY(obj,var,head,field,key)      \
        LIST_FOREACH((var),(head),field) {              \
            if((obj)->key > (var)->key) {               \
                if((head)->lh_first == (var)) {         \
                    LIST_INSERT_HEAD((head),(obj));     \
                } else {                                \
                    LIST_INSERT_BEFOR((var),(obj),field);\
                }                                       \
                break;                                  \
            }                                           \
            if(LIST_NEXT((var),field) == NULL) {        \
                LIST_INSERT_AFTER((var),(obj),field);   \
                break;                                  \
            }                                           \
        }
#endif