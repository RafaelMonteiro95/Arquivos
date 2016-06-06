#include <stdio.h>
#include <stdlib.h>

#include "btree.h"

/*estruturas*/
struct item {
	int key;
	int offset;
};

struct bTree {
	int lastRRN;
	int rootRRN;
	FILE* file; //arquivo
	bTreeNode* root;
};

struct bTreeNode {
	item* items;
	int* children;
	int rrn;
};

/*funcoes*/

/*bTree*/
/*gera uma estrutura de arvore b
	retorno: ponteiro para a estrutura gerada*/
bTree* newBTree()
{
	bTree* bt = (bTree*)malloc(sizeof(bTree));

	bt->file = fopen("moviedb.idx","w+");
	bt->lastRRN = 1;
	bt->rootRRN = 1;

	//crio novo no raiz
	bt->root = newNode();
	bt->root->rrn = 1;

	/*escrevendo cabecalho*/
	char dummy = 'a';
	//rrn da raiz
	fwrite(&(bt->root->rrn),sizeof(int),1,bt->file);

	/*escrevendo raiz no arquivo bin*/
	writeNode(bt,bt->root,1);

	/* teste*/
	item in;
	in.key = 10;
	in.offset = 20;

	insertInNode(bt->root,in);
	writeNode(bt,bt->root,1);
	printf("%d\n",searchInNode(bt->root,10));

	readToNode(bt,bt->root,1);
	int i;
	for(i = 0; i < MAX_ELEMENTS; i++){
		printf("i:%d\nkey:%d\noffset:%d\n\n",i,bt->root->items[i].key,bt->root->items[i].offset);
	}

	printf("%d\n",searchBTree(bt,10));
	/*aqui acaba o teste*/

	return bt;
}

/*libera a memoria utilizada na estrutura arvore b*/
void deleteTree(bTree* bt)
{
	fclose(bt->file);
	deleteNode(bt->root);
	free(bt);
}

/*bTreeNode*/

//aloca um novo bTreeNode
bTreeNode* newNode()
{
	int i;

	//alocando
	bTreeNode* n = malloc(sizeof(bTreeNode));
	n->items = malloc(sizeof(item)*MAX_ELEMENTS);
	n->children = malloc(sizeof(int)*MAX_CHILDREN);

	//preenchendo
	for(i = 0; i < MAX_ELEMENTS; i++)
	{
		n->items[i].key = EMPTY;
		n->items[i].offset = EMPTY;
		n->children[i] = EMPTY;
	}
	n->children[i] = EMPTY; //preenche ultimo ponteiro

	return n;
}

/*Verifica se um no eh folha
	retorna 1 caso seja folha, 0 caso nao seja)*/
int isLeaf(bTreeNode* n)
{
	int i;
	for(i = 0; i < MAX_CHILDREN; i++)
	{
		if(n->children[i] != EMPTY) return 0;
	}

	return 1;
}

/*Verifica se um no esta vazio
	retorna 1 caso esteja, 0 caso nao esteja*/
int isEmpty(bTreeNode* n)
{

	int i;
	for(i = 0; i < MAX_ELEMENTS; i++)
	{
		if(n->items[i].key != EMPTY) return 0;
	}

	return 1;	
}

/*Verifica se um no esta cheio
	retorna 1 caso esteja, 0 caso nao esteja*/
int isFull(bTreeNode* n)
{

	int i;
	for(i = 0; i < MAX_ELEMENTS; i++)
	{
		if(n->items[i].key == EMPTY) return 0;
	}

	return 1;	
}


/*escreve os dados de um bTreeNode no arquivo
	bt: arvore
	n: no
	rrn: rrn onde o no deve ser escrito*/
void writeNode(bTree* bt, bTreeNode* n, int rrn)
{
	//posiciono ponteiro de leitura
	fseek(bt->file,(NODE_SIZE*rrn),SEEK_SET);
	//escrevo items
	fwrite(n->items,sizeof(item),MAX_ELEMENTS,bt->file);
	//escrevo filhos
	fwrite(n->children,sizeof(int),MAX_CHILDREN,bt->file);
}

/*le os dados de uma bTree e os guarda em um bTreeNode
	os dados lidos sao referentes ao rrn "rrn"*/
void readToNode(bTree* bt, bTreeNode* n, int rrn)
{
	//setando ponteiro p/ posicao correta
	fseek(bt->file,NODE_SIZE*rrn,SEEK_SET);
	//lendo items
	fread(n->items,sizeof(item),MAX_ELEMENTS,bt->file);
	//lendo children
	fread(n->children,sizeof(int),MAX_CHILDREN,bt->file);
}


/*libera memoria utilizada por um bTreeNode*/
void deleteNode(bTreeNode* n)
{
	free(n->children);
	free(n->items);
	free(n);
}

/*Insere um item em um no nd
	soh busca uma posicao e insere
		retorna 1 se der certo, 0 se der errado*/
int insertInNode(bTreeNode* n, item it)
{
	int i, j;

	if(isEmpty(n)){
		n->items[0] = it;
		return 1;
	} else {
		for(i = 0; i < MAX_ELEMENTS; i++){
			//procuro uma chave maior
			if(n->items[i].key > it.key){
				//shifto o restante pra direita
				for(j = i; j < MAX_ELEMENTS; j++){
					n->items[j+1] = n->items[j]; 
				}
				n->items[i] = it;
				return 1;
			}
		}
	}
	return 0;
}

/*Verifica se uma chave key esta em n
	retorna o offset da chave, caso encontrado
	retorna -1, caso nao tenha encontrado*/
int searchInNode(bTreeNode* n, int key)
{
	int i, j;

	for(i = 0; i < MAX_ELEMENTS; i++){
		//procuro uma chave igual
		if(n->items[i].key == key){
			return n->items[i].offset;
		}
	}
	return -1;
}

/*Verifica em qual filho de n devo buscar a chave
	retorna o rrn do filho, caso encontrado
	retorna -1, caso nao tenha encontrado*/
int searchInChildren(bTreeNode* n, int key)
{
	int i, j;

	for(i = 0; i < MAX_ELEMENTS; i++){
		//procuro uma chave maior
		if(n->items[i].key < key){
			break;
		}
	}
	return n->children[i];
}


//metodo recursivo
int _searchBTree(bTree* bt, int key, int rrn)
{
	if(rrn == EMPTY) return -1;
	else {
		//carrego o no
		bTreeNode* n = newNode(); 
		readToNode(bt,n,rrn);
		//busco se a chave está nele
		int result = searchInNode(n,key);
		//caso tenha achado
		if(result != EMPTY){
			return result;
		} else {
			//caso nao tenha encontrado, procuro nos filhos
			return _searchBTree(bt,key,searchInChildren(n,key));
		}
	}
}

/*interface para o metodo recursivo
busca primeiro na raiz, que ja esta carregada
	se encontrar, retorna o encontrado
	se nao, chama o metodo recursivo que busca na arvore*/
int searchBTree(bTree* bt, int key)
{
	//busco na raiz
	int result = searchInNode(bt->root,key);
	if(result != EMPTY) return result;
	else {
		//caso nao esteja na raiz, busco na arvore
		return _searchBTree(bt,key,searchInChildren(bt->root,key));
	}


}

/*================ falta implementar/terminar de implementar =======*/

int split();
void promoteItem(bTreeNode* parent, item it);

void insertBTree(bTree* bt, int key, int offset)
{
	item it;
	it.key = key;
	it.offset = offset;

	//caso o no raiz seja folha, insiro nele
	if(isLeaf(bt->root)){
		//caso nao esteja cheio, insiro
		if(!isFull(bt->root)){
			insertInNode(bt->root,it);
			//escrevo no arquivo
			writeNode(bt,bt->root,bt->root->rrn);
		} else {
			//crio uma nova raiz

			//faco split 1-to-2 na raiz
			// split();
		}
	} else {
		//caso o no raiz tenha filhos, busco e insiro
	}
}


