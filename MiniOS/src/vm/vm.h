#ifndef _VM_VM_H_ 
#define _VM_VM_H_

#define MAX_VM_NAME             256
struct vmspace {
    char name[MAX_VM_NAME  - 1];
};

#endif