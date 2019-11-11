#ifndef _VM_NODE_H_
#define _VM_NODE_H_

#include <sys/queue.h>

struct node {
    LIST_HEAD(,vmspace)         node_vmspace;
    uint64_t                    node_nr_nodes;
};

typedef struct node *       node_t;
#endif