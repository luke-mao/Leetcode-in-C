#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

const int DICT_INITIAL_SIZE = 9973;
const int DICTNODE_INITIAL_SIZE = 10;


// define all structures and functions


// we need to create a dictionary
// dictionary[value] = the index of this value
// better to have a node, the node contains both index and the value

struct _Node {
    int index;
    int value;
};

typedef struct _Node *Node;

struct _DictNode {
    int top;    // next index to add
    int size;   // total positions available
    struct _Node** list;  // an array of nodes
};

typedef struct _DictNode* DictNode;

struct _Dict {
    int size;
    struct _DictNode** list;
};

typedef struct _Dict* Dict;     // array


// some basic implementation about the dictionary and the node
Node CreateNode(int index, int value);
Node DestroyNode(Node);

DictNode CreateDictNode(int size); // start size = 10
DictNode DestroyDictNode(DictNode);

Dict CreateDict(int size);
Dict DestroyDict(Dict);

void InsertIntoDictNode(DictNode dn, Node n);
void InsertIntoDict(int index, int value, int target, Dict d);

int SearchDictNode(DictNode dn, int value, int target);
int SearchDict(Dict d, int value, int target);

int* twoSum(int* nums, int numsSize, int target, int* returnSize);



// 
int main (void){
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int target = 8;
    int returnSize = 2; // I think this is irrelevant

    int* answer = twoSum(a, 10, target, &returnSize);

    printf("The answer is %d %d", answer[0], answer[1]);
    
    free(answer);

    return 0;
}



// functions

int* twoSum(int* nums, int numsSize, int target, int* returnSize){
    // returnSize = 2
    int* result = (int*) malloc(*returnSize * sizeof(int));
    assert(result != NULL);

    // create the dictionary
    Dict d = CreateDict(DICT_INITIAL_SIZE);

    for (int i = 0; i < numsSize; i++){
        int i2 = SearchDict(d, nums[i], target);

        if (i2 == -1){
            // not found, insert into dict
            InsertIntoDict(i, nums[i], target, d);
        }
        else{
            // find, return the two indexes in any order
            result[0] = i; 
            result[1] = i2;
            break;
        }
    }

    return result;
}


Node CreateNode(int index, int value){
    assert(index >= 0);

    Node n = (Node) malloc(sizeof(struct _Node));
    assert(n != NULL);

    n->index = index;
    n->value = value;

    return n;
}



Node DestroyNode(Node n){
    assert(n != NULL);
    free(n);
    n = NULL;
    return n;
}


// start size = 10
DictNode CreateDictNode(int size){
    assert(size > 0);

    DictNode dn = (DictNode) malloc(sizeof(struct _DictNode));
    assert(dn != NULL);

    dn->top = 0;
    dn->size = size;
    dn->list = (Node*) malloc(dn->size * sizeof(Node));
    assert(dn->list != NULL);

    for (int i = 0; i < dn->size; i++){
        dn->list[i] = NULL;
    }

    return dn;
}


DictNode DestroyDictNode(DictNode dn){
    assert(dn != NULL);
    assert(dn->size >= 1);

    for (int i = 0; i < dn->top; i++){
        dn->list[i] = DestroyNode(dn->list[i]);
    }

    free(dn->list);
    dn->list = NULL;

    free(dn);
    dn = NULL;

    return dn;
}


Dict CreateDict(int size){
    assert(size > 0);

    Dict d = (Dict) malloc(sizeof(struct _Dict));
    assert(d != NULL);

    d->size = size;

    d->list = (DictNode*) malloc(size * sizeof(DictNode));
    assert(d->list != NULL);

    for (int i = 0; i < d->size; i++){
        d->list[i] = NULL;
    }

    return d;
}


Dict DestroyDict(Dict d){
    assert(d != NULL);
    
    for (int i = 0; i < d->size; i++){
        d->list[i] = DestroyDictNode(d->list[i]);
    }

    free(d->list);
    d->list = NULL;

    free(d);
    d = NULL;

    return d;
}


void InsertIntoDictNode(DictNode dn, Node n){
    assert(dn != NULL && n != NULL);

    // check if the dn is full
    if (dn->top == dn->size){
        // increase the size
        // simply double the size
        dn->size *= 2;
        dn->list = (Node*) realloc(dn->list, dn->size * sizeof(Node));
        assert(dn->list != NULL);
    }

    dn->list[dn->top] = n;
    dn->top += 1;

    return;
}


void InsertIntoDict(int index, int value, int target, Dict d){
    assert(index >= 0);
    assert(d != NULL);

    // create a node first
    Node n = CreateNode(index, value);

    // now insert the node into the dictnode
    // key = target - value
    // but here, key = (target - value) % d->size

    int key = (target - value ) % d->size;

    if (d->list[key] == NULL){
        d->list[key] = CreateDictNode(DICTNODE_INITIAL_SIZE);
    }

    InsertIntoDictNode(d->list[key], n);

    return;
}


int SearchDictNode(DictNode dn, int value, int target){
    assert(dn != NULL);

    int result_index = -1;

    // iterate all inside this dn->list
    for (int i = 0; i < dn->top; i++){
        if (value + dn->list[i]->value == target){
            result_index = dn->list[i]->index;
            break;            
        }
    }

    return result_index;
}


int SearchDict(Dict d, int value, int target){
    assert(d != NULL);

    int key = (target - value) % d->size;

    if (d->list[key] == NULL){
        return -1;
    }
    else{
        return SearchDictNode(d->list[key], value, target);
    }
}







