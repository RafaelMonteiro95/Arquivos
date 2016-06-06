#ifndef _BTREE_H_
#define _BTREE_H_


/*defines*/
#define MAX_ELEMENTS 5
#define MAX_CHILDREN 6
/*MIN_OCCUPATION = floor(MAX_ELEMENTS/2)*/
#define MIN_OCCUPATION 2

/*NODE_SIZE = (MAX_ELEMENTS*(sizeof(item))
					+(MAX_CHILDREN*sizeof(int))
  NODE_SIZE = 5*8 + 6*4 = 64 */
#define NODE_SIZE 64

#define EMPTY -1

/*estruturas*/
typedef struct item item;

typedef struct bTree bTree;

typedef struct bTreeNode bTreeNode;

/*metodos*/

/*bTree*/
bTree* newBTree();
void deleteTree(bTree* bt);
int searchBTree(bTree* bt, int key);

/*bTreeNode*/
bTreeNode* newNode();
int isLeaf(bTreeNode* n);
int isEmpty(bTreeNode* n);
int isFull(bTreeNode* n);
int insertInNode(bTreeNode* n,item it);
int searchInNode(bTreeNode* n, int key);
void readToNode(bTree* bt, bTreeNode* n, int rrn);
void writeNode(bTree* bt, bTreeNode* n, int rrn);
void deleteNode(bTreeNode* n);

//todo
int split();
void promoteItem(bTreeNode* parent, item it);
void insertBTree(bTree* bt, int key, int offset);

#endif

// /*estruturas*/
// struct item {
// 	int key;
// 	int offset;
// };

// struct bTree {
// 	FILE* file; //arquivo
// 	bTreeNode* root;
// 	int root;
// };

// struct bTreeNode {
// 	int nElements;
// 	item* items;
// 	int* children;
// };