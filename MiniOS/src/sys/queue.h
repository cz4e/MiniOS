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
                                                            SLIST_NEXT((elm),field) = SLIST_FIRST((head));  \
                                                            SLIST_FIRST((head)) = (elm);                    \
                                                        }while(0)

#define SLIST_INSERT_AFTER(obj,elm,field)               do{\
                                                            SLIST_NEXT((elm),field) = SLIST_NEXT((obj),field);  \
                                                            SLIST_NEXT((obj),field) = (elm);                    \
                                                        }while(0)

#define SLIST_FOREACH(var,head,field)                   \
    for((var) = SLIST_FIRST((head));                    \
    (var);                                              \
    (var) = SLIST((var),field))

#define SLIST_HEAD_INIT(head,field)                     \
    (SLIST_FISRT((head)) = NULL)     
                                                  
#define LIST_HEAD(name,type)                            \
struct name {                                           \
    struct type *lh_first;                              \
}

#define LIST_ENTRY(type)                                \
struct {                                                \
    struct type *le_next;                               \
    struct type **le_prev;                              \
}



#define LIST_EMPTY(head)                ((head)->lh_first == NULL)
#define LIST_NEXT(elm,field)            ((elm)->field.le_next)
#define LIST_PREV(elm,field)            ((elm)->field.le_prev)
#define LIST_FIRST(head)                ((head)->le_first)
#define LIST_HEAD_INIT(head,field)                      \
    (LIST_FIRST((head)) = NULL)

#define LIST_ENTRY_INIT(elm,field)                      \
    (LIST_NEXT((elm),field) = LIST_PREV((elm),field) = NULL)

#define LIST_INSERT_HEAD(head,elm,field)                do{\
                                                            LIST_PREV((elm),field) = LIST_FIRST((head));            \
                                                            LIST_PREV(LIST_FIRST((head)),field) = &(elm);           \
                                                            (head)->lh_first = (elm);                               \
                                                        }while(0)

#define LIST_INSERT_AFTER(obj,elm,field)                do{\
                                                            LIST_NEXT((elm),field) = LIST_NEXT((obj),field);                \
                                                            LIST_PREV(LIST_NEXT((obj),field),field) = &(elm);               \
                                                            LIST_PREV((elm),field) = &(obj);                                \
                                                            LIST_NEXT((obj),field) = (elm);                                 \
                                                        }while(0)

#define LIST_INSERT_BEFOR(obj,elm,field)                do{\
                                                            LIST_PREV((elm),field) = LIST_PREV((obj),field);        \
                                                            LIST_NEXT(*(LIST_PREV((obj),field))) = (elm);           \
                                                            LIST_NEXT((elm),field) = (obj);                         \
                                                        }while(0)

#define LIST_REMOVE_HEAD(head,elm,field)                do{\
                                                            LIST_FIRST((head)) =  LIST_NEXT((elm),field);           \
                                                            LIST_PREV(LIST_NEXT((elm),field),field) = NULL;         \
                                                        }while(0)

#define LIST_REMOVE(elm,field)                          do{\
                                                            if(LIST_NEXT((elm),field)) {                            \
                                                                LIST_PREV(LIST_NEXT((elm),field),field) = LIST_PREV((elm),field);   \
                                                                LIST_NEXT(*LIST_PREV((elm),field),field) = LIST_NEXT((elm),filed);  \
                                                            } else {                                                                \
                                                                LIST_NEXT(*LIST_PREV((elm),field)) = NULL;                          \
                                                            }                                                                       \
                                                        }while(0)
#define LIST_FOREACH(var,head,field)                    \
        for((var) = LIST_FIRST((head));                 \
        (var);                                          \
        (var) = LIST_NEXT((var),field))                 \

#define LIST_INSERT_BY_KEY(obj,var,head,field,key)      \
        LIST_FOREACH((var),(head),field) {              \
            if((obj)->key > (var)->key) {               \
                if((head)->lh_first == (var)) {         \
                    LIST_INSERT_HEAD((head),(obj),field);     \
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