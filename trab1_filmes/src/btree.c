#include <stdio.h>
#include <stdlib.h>

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

//#include "btree.h"

/*estruturas*/
typedef struct item {
	int key;
	int offset;
}item;

typedef struct bTreeNode {
	item* items;
	int* children;
	int rrn;
}bTreeNode;

typedef struct bTree {
	int lastRRN;
	int rootRRN;
	FILE* file; //arquivo
	bTreeNode* root;
}bTree;


/*funcoes*/

/*bTreeNode*/

//aloca um novo bTreeNode


void debuga(char* s){
	printf("%s\n",s);
}

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
	//5 primeiros conjuntos de 8 bytes
	fwrite(n->items,sizeof(item),MAX_ELEMENTS,bt->file);
	//escrevo filhos
	//6 outros conjuntos de 4 bytes
	fwrite(n->children,sizeof(int),MAX_CHILDREN,bt->file);
}

/*le os dados de uma bTree e os guarda em um bTreeNode
	os dados lidos sao referentes ao rrn "rrn"*/
void readToNode(bTree* bt, bTreeNode* n, int rrn)
{
	n->rrn = rrn;
	//setando ponteiro p/ posicao correta
	fseek(bt->file,NODE_SIZE*rrn,SEEK_SET);
	//lendo items
	fread(n->items,sizeof(item),MAX_ELEMENTS,bt->file);
	//lendo children
	fread(n->children,sizeof(int),MAX_CHILDREN,bt->file);
}

bTreeNode* getNodeFromFile(bTree* bt, int rrn){
	bTreeNode* ret = newNode();
	readToNode(bt,ret,rrn);
	return ret;
}


/*libera memoria utilizada por um bTreeNode*/
void deleteNode(bTreeNode* n)
{
	free(n->children);
	free(n->items);
	free(n);
}

/*libera a memoria utilizada na estrutura arvore b*/
void deleteTree(bTree* bt)
{
	fclose(bt->file);
	deleteNode(bt->root);
	free(bt);
}

/*Insere um item em um no nd
	soh busca uma posicao e insere
		retorna 1 se der certo, 0 se der errado*/
int insertInNode(bTreeNode* n, item it)
{
	int i, j;

	if(isEmpty(n)){
		debuga("Folhas estao vazias, entao insere na primeira posicao");
		n->items[0] = it;
		return 1;
	} else {
		debuga("Temos alguma coisa nas folhas nesse caso, entao acha o maior e shifta");
		for(i = 0; i < MAX_ELEMENTS; i++){
			//verifica se tem chave igual
			if(n->items[i].key == it.key)
				return 1;
			//procuro uma chave maior
			if(n->items[i].key > it.key || n->items[i].key == EMPTY){
				//shifto o restante pra direita				
				for(j = MAX_ELEMENTS-1; j >= i; j--){
					n->items[j] = n->items[j-1]; 
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

	return bt;
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

void splitRoot(bTree *btree)
{
	bTreeNode* root = btree->root;
	int token = MAX_ELEMENTS/2, i = 0;	
	bTreeNode *nodeMenorQueARaiz  = newNode();
	bTreeNode *nodeMaiorQueARaiz = newNode(); //Novo no que eh criado no split

	//Atribui novo no > noRaiz
	for(i = token + 1; i < MAX_ELEMENTS; i++){
		nodeMaiorQueARaiz->items[i - (token+1)] = root->items[i]; //coloca os itens restantes da raíz antiga no nó da
		
	}

	//Atribui novo no < noRaiz
	for(i = 0; i < token; i++){
		nodeMenorQueARaiz->items[i] = root->items[i]; //coloca os itens restantes da raíz antiga no nó da
	}

	//Arruma raiz limpando os antigos e colocando o novo valor unico
	root->items[0] = root->items[token];
	for(i = 1; i < MAX_CHILDREN; i++){
		root->items[i].key = -1; //Coloca um valor invalido nos elementos que já foram copiados
		root->items[i].offset = -1; //Coloca um valor invalido nos elementos que já foram copiados
	}
	
	
	//Arruma os filhos da raiz
	root->children[0] = (btree->lastRRN) + 1;
	(btree->lastRRN)++;
	root->children[1] = (btree->lastRRN) + 1;
	(btree->lastRRN)++;

	writeNode(btree,root,btree->rootRRN);
	writeNode(btree,nodeMenorQueARaiz,root->children[0]);
	writeNode(btree,nodeMaiorQueARaiz,root->children[1]);

}

int split2_3(bTreeNode *bt);

void promoteItem(bTreeNode* parent, item it);


void printaEspacos(int n){
	int i;
	for(i=0;i<n;i++){
		printf(" ");
	}
}

void printaArvoreRecursivo(bTree* btree,bTreeNode* node, int profundidade){
	int i=0;
	//printaEspacos(profundidade*5);
	if(!isLeaf(node)){
		printf("\n");
		if(node->children[0]!=EMPTY){
			printaArvoreRecursivo(btree,getNodeFromFile(btree,node->children[0]),profundidade+1);
		}
	}
	printf("\nrrn = %d\n",node->rrn);
	for(i=0;i<MAX_ELEMENTS;i++){		
		if(node->items[i].key != EMPTY){
			printf("(%d, %d) ", node->items[i].key,node->items[i].offset);			
		}else{
			printf("(-1,-1)");
		}
		if(!isLeaf(node)){
			printf("\n");
			if(node->children[i+1]!=EMPTY){
				if(i<=MAX_CHILDREN)
					printaArvoreRecursivo(btree,getNodeFromFile(btree,node->children[i+1]),profundidade+1);
			}
		}		
	
	
	}	

			printf("\n");

}

void printaArvore(bTree* tree){	
	if(tree->root){
		printaArvoreRecursivo(tree,tree->root,1);
		printf("\n");
	}else{
		printf("Arvore nao tem raiz\n");
	}
}


void printaNode(bTreeNode* n){
	int i;
	for(i=0;i<MAX_ELEMENTS;i++){
		printf("(%d,%d)",n->items[i].key,n->items[i].offset);
	}
	printf("\n");
}

item getMenorItemERearruma(bTreeNode* node){
	item ret;
	int i;
	ret.key = node->items[0].key;
	ret.offset = node->items[0].offset;
	for(i=1;i<MAX_ELEMENTS;i++){
		node->items[i-1] = node->items[i];
	}
	node->items[MAX_ELEMENTS-1].key = -1;
	node->items[MAX_ELEMENTS-1].offset = -1;
	return ret;
}
item getMaiorItemERearruma(bTreeNode* node){
	item ret;
	int i =1;
	while(node->items[MAX_ELEMENTS-i].key==-1)
		i++;
	ret.key = node->items[MAX_ELEMENTS-i].key;
	ret.offset = node->items[MAX_ELEMENTS-i].offset;
	node->items[MAX_ELEMENTS-i].key = -1;
	node->items[MAX_ELEMENTS-i].offset = -1;
	return ret;
}

void split2to3(bTree* btree, bTreeNode* node, int rrn, int child,int key, int offset){
		printf("Comecando um split 2-3 com os filhos %d e %d\n",child, child + 1);
		printf("Enquanto adicionamos a key %d\n",key );
	/* 

			| NODE| 
			|
		| CHILD | CHILD +1 |




	*/
	int i;
	bTreeNode* a = getNodeFromFile(btree, node->children[child]);
	printf("Node a: ");
	printaNode(a);
	bTreeNode* b = getNodeFromFile(btree, node->children[child+1]);
	printf("Node b: ");
	printaNode(b);
	bTreeNode* c = newNode();
	printf("Node c: ");
	printaNode(c);
	item itemA;
	item itemB;

	for(i=0;i<3;i++){
		item tmp = getMaiorItemERearruma(b);		
		insertInNode(c,tmp);				
	}
	for(i=0;i<2;i++){
		item tmp = getMaiorItemERearruma(a);
		insertInNode(b,tmp);
	}

	printf("\n\n Depois da primeira parte\n\n");

	printf("Node a: ");
	printaNode(a);
	printf("Node b: ");
	printaNode(b);
	printf("Node c: ");
	printaNode(c);

	itemA = getMenorItemERearruma(b);
	itemB = getMaiorItemERearruma(b);


	printf("\n\n Depois da Segunda parte\n\n");

	printf("Node a: ");
	printaNode(a);
	printf("Node b: ");
	printaNode(b);
	printf("Node c: ");
	printaNode(c);
	printf("Item A : %d\n",itemA.key );
	printf("Item B : %d\n",itemB.key );


	printf("Agora vamos adicionar o numero que estava no Node Pai, no caso eh : %d\n", node->items[child].key);
	if(itemA.key < node->items[child].key){
		insertInNode(b,node->items[child]);
	}else{
		item tmp = getMaiorItemERearruma(a);
		if(tmp.key >node->items[child].key){			
			insertInNode(a,node->items[child]);
			insertInNode(b,itemA);
			itemA = tmp;

		}
	}

	printf("\n\n Depois da Terceira parte\n\n");

	printf("Node a: ");
	printaNode(a);
	printf("Node b: ");
	printaNode(b);
	printf("Node c: ");
	printaNode(c);
	printf("Item A : %d\n",itemA.key );
	printf("Item B : %d\n",itemB.key );


	printf("Agora vamos adicionar o numero que eh da propria chamada : %d\n",key);

	item tmp;
	tmp.key= key;
	tmp.offset = offset;
	if(tmp.key>itemB.key){
		insertInNode(c,tmp);
	}else{
		insertInNode(b,tmp);
	}


	printf("\n\n Depois da Quarta parte\n\n");

	printf("Node a: ");
	printaNode(a);
	printf("Node b: ");
	printaNode(b);
	printf("Node c: ");
	printaNode(c);
	printf("Item A : %d\n",itemA.key );
	printf("Item B : %d\n",itemB.key );



	
	for(i=MAX_ELEMENTS-1;i>child+1;i--){
		node->items[i] = node->items[i-1];
	}
	for(i=MAX_CHILDREN-1;i>child+2;i--){
		node->children[i] = node->children[i-1];
	}
	printaNode(node);
	node->items[child] = itemA;
	node->items[child + 1] = itemB;
	writeNode(btree, a,node->children[child]);
	writeNode(btree, b,node->children[child + 1]);

	node->children[child + 2] = btree->lastRRN+1;
	(btree->lastRRN)++;
	writeNode(btree,c,node->children[child + 2]);

	


}


//Ou seja, insere em node que nao eh Leaf
void insertRecursivo(bTree* btree, bTreeNode* node, int key, int offset, int rrn){
	debuga("Insert recursivo");
	if(isLeaf(node)){
		debuga("Caso em que eh folha");
		if(isFull(node)){
			debuga("NO ESTA CHEIO! ERRO NA RECURSAO!\n");
		}else{
			debuga("Entrou no caso de adicao");
			item it;
			it.key = key;
			it.offset = offset;
			insertInNode(node,it);	

			writeNode(btree,node,rrn);

		}
		
	}else{
		//Primeiro caso trata a excecao do insert ocorrer no primeiro filho!
		debuga("Caso que nao eh folha");
		if(key < node->items[0].key){
			debuga("Entrou no caso de excecao! Ou seja, no no extremo da esquerda");
			bTreeNode* nodeExtremoEsquerdo = getNodeFromFile(btree,node->children[0]);
			if(!isFull(nodeExtremoEsquerdo)){
				debuga("E como o no extremo da esquerda tem espaco adicionamos nele!");
				//Caso 1: Insercao normal
				insertRecursivo(btree,nodeExtremoEsquerdo,key,offset,node->children[0]);
			}
			else{
				debuga("Temos um problema... o no esta cheio... entao tentamos fazer redistribuicao");
				bTreeNode* nodeVizinho = getNodeFromFile(btree,node->children[1]);
				if(isFull(nodeVizinho)){
					debuga("PROBLEMA! O no de redistribuicao tambem esta cheio! Logo precisamos fazer um split 2-3");
					//Caso 2: Aqui temos tanto o child 0 como o child 1 cheio,
					//logo precisamos fazer um split 2-3
					debuga("Precisamos fazer o split 2-to-3");												
				}else{
					debuga("O no vizinho ainda tem espaco, entao fazemos a redistribuicao");
					//Caso 3: Aqui ocorre uma insercao com redistribuicao!
					item maiorDoLadoEsquerdo = getMaiorItemERearruma(nodeExtremoEsquerdo);
					if(maiorDoLadoEsquerdo.key > key){
						debuga("Caso 1 da redistribuicao");
						insertRecursivo(btree,nodeExtremoEsquerdo,key,offset,node->children[0]);
						insertRecursivo(btree,nodeVizinho,node->items[0].key,node->items[0].offset,node->children[1]);
						node->items[0].key = maiorDoLadoEsquerdo.key;
						node->items[0].offset = maiorDoLadoEsquerdo.offset;
						writeNode(btree,node,rrn);
					}else{
						insertRecursivo(btree,nodeExtremoEsquerdo,maiorDoLadoEsquerdo.key,maiorDoLadoEsquerdo.offset,rrn);
						insertRecursivo(btree,nodeVizinho,node->items[0].key,node->items[0].offset,node->children[1]);
						node->items[0].key = key;
						node->items[0].offset = offset;
						writeNode(btree,node,rrn);
					}
				}
			}



		//Insercao nos casos gerais
		}else{	
			int i=0;
			int acho = 0;

			//Procura qual o filho em que o insert deve ocorrer	
			while(key < node->items[i].key || node->items[i].key != EMPTY){
				printf(" i%d   %d node item\n",key,node->items[i].key);				
				i++;
			}

			if(key<node->items[1].key && node->items[1].key != EMPTY){
				i=1;
			}

			printf("%d node item\n",node->items[i].key);
			printf("Node escolhido para colocar o novo items eh %d, com key%d\n",i,key);
		
			//node onde eh esperado que o insert ocorra
			bTreeNode* nodeDeTrabalho = getNodeFromFile(btree,node->children[i]);
			//node onde ocorrera a redistribuicao, caso o nodeDeTrabalho esteja cheio
			bTreeNode* nodeVizinho = getNodeFromFile(btree,node->children[i-1]);


			if(!isFull(nodeDeTrabalho)){
				//Caso 1: Insercao ocorre normalmente!
				insertRecursivo(btree,nodeDeTrabalho,key,offset,node->children[i]);	
			}else{
				if(isFull(nodeVizinho)){
					//Caso 2: Tanto o nodeDeTrabalho como o nodeVizinho estao cheios!
					debuga("Precisamos fazer o split 2-to-3");
					if(isFull(node)){
						printf("FODEU\n" );
						printaArvore(btree);
					}else{
						split2to3(btree,node, rrn, i-1,key, offset);	
					}					
				}else{
					//Caso 3: Aqui ocorre um insert + redistribuicao, ja que o nodeDeTrabalho esta cheio
					item menorItemDoLadoDireito = getMenorItemERearruma(nodeDeTrabalho);
					if(menorItemDoLadoDireito.key < key){
						printf("CASO EM QUE A > D\n");
						insertRecursivo(btree,nodeVizinho,node->items[i-1].key,node->items[i-1].offset,node->children[i-1]);
						node->items[i-1].key = menorItemDoLadoDireito.key;
						node->items[i-1].offset = menorItemDoLadoDireito.offset;
						writeNode(btree,node,rrn);
						insertRecursivo(btree,nodeDeTrabalho,key,offset,node->children[i]);
					}else{
						printf("CASO EM QUE D < A\n");
						insertRecursivo(btree,nodeVizinho,node->items[i-1].key,node->items[i-1].offset,node->children[i-1]);
						node->items[i-1].key = key;
						node->items[i-1].offset = offset;
						writeNode(btree,node,rrn);
					}
				}				
			}			
		}
	}
}

void insertBTree(bTree* bt, int key, int offset){
	item it;
	it.key = key;
	it.offset = offset;

	//caso o no raiz seja folha, insiro nele
	if(isLeaf(bt->root)){
		 debuga("entrou no caso folha");

		//caso nao esteja cheio, insiro
		if(!isFull(bt->root)){
			debuga("Folha ainda nao ta cheia");
			insertInNode(bt->root,it);
			debuga("Insere no node!");
			//escrevo no arquivo
			writeNode(bt,bt->root,bt->root->rrn);
		} else {
			debuga("filhos estao cheios, entao dou o split na raiz");
			splitRoot(bt); //faz o split do noh raiz cheio e atualiza a raiz
			insertBTree(bt,key,offset);
		}
	} else {
		debuga("entrou no caso em que inserimos num nó que nao eh folha");
		insertRecursivo(bt,bt->root,key,offset,bt->rootRRN);
		//caso o no raiz tenha filhos, busco e insiro
	}
}

// typedef struct queue{
// 	int* queue;
// 	int first;
// 	int last;
// } Queue;

// void push(Queue* q, int rrn)
// {
// 	q->queue[q->last] = rrn;
// 	if(q->last == 37){
// 		//se a fila chegou no fim
// 		q->last = 0;
// 	} else {
// 		q->last++;
// 	}
// }

// int pop(Queue* q)
// {
// 	int popped = q->queue[q->first];
// 	if(q->first == 37){
// 		//se a fila chegou no fim
// 		q->first = 0;
// 	} else {
// 		q->first++;
// 	}
// }

// Queue* newQueue(int size){
// 	Queue* q = malloc(sizeof(Queue));
// 	q->queue = malloc(sizeof(int)*size);
// 	q->first = 0;
// 	q->last = 0;
// 	for(i = 0; i < size; i++){
// 		q->queue[i] = -1;
// 	}

// 	return q;
// }

// int filled(Queue* q)
// {
// 	return q->first - q->last;
// }

// /*
// 	uso uma matriz: cada linha eh um subnivel da arvore
// 	cada coluna eh um no da arvore*/
// void printBTree(bTree* bt)
// {
// 	int** matrix = malloc(sizeof(int*)*)
// }