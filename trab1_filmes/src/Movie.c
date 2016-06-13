/*Autores:
	BRUNO BACELAR ABE		9292858
	KAUE LOPES DE MORAIS	9277576
	LUCAS ALEXANDRE SOARES	9293265
	RAFAEL AUGUSTO MONTEIRO	9293095
*/
	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Movie.h"
#include "btree.h"

/*Struct que representa o registro de um filme
	Campos:
		int idFilme: 		ID do filme
		int anoLancamento: 	Ano de lancamento do filme
		int duracaoFilme: 	Tempo de duracao do filme, em minutos
		char* generoFilme: 	Genero(s) do filme. 
		char* tituloFilme: 	Titulo do filme
		char* descFilme: 	Descricao do filme
		char* producao: 	Pais(es) de producao do filme
*/
struct filme
{
	int idFilme;
	int anoLancamento;
	int duracaoFilme;
	char *generoFilme;
	char *tituloFilme;
	char *descFilme;
	char *producao;
};

/*	Funcao que aloca a memoria e inicializa uma estrutura Filme
	Parametros: nenhum
	Retorno: Filme *f: ponteiro para a estrutura filme gerada
						devidamente inicializada*/
Filme *Inicialize_Struct()
 {
		/*Aloca memoria para o time Filme*/
	 Filme *f = malloc(sizeof(Filme));

		/*Inicializa a variavel inteira com zero*/
		f->idFilme = 0;
		f->anoLancamento = 0;
		f->duracaoFilme = 0;

		/*Aloca memoria para uso das Strings*/
		f->generoFilme = (char*)malloc(sizeof(char)*100);
		f->tituloFilme = (char*)malloc(sizeof(char)*100);
		f->descFilme = (char*)malloc(sizeof(char)*500);
		f->producao = (char*)malloc(sizeof(char)*100);


	return f;
 }

/*	Funcao que libera a memoria utilizada pela estrutura Filme
	Parametros: Filme **f: endereco do ponteiro da estrutura
	Retorno: nenhum*/
void Destroy_Struct(Filme **f)
 {
 		if(!f || !(*f)) return;

		 /*Funcao contraria a Inicializa_Struct, essa desaloca toda a Struct utilizada*/
		if((*f)->generoFilme)free((*f)->generoFilme);
		if((*f)->tituloFilme)free((*f)->tituloFilme);
		if((*f)->descFilme)free((*f)->descFilme);
		if((*f)->producao)free((*f)->producao);
		free((*f));
		*f = NULL;
 }

// Setters
void SetId(Filme *f, int id) { f->idFilme = id; }
void SetYear(Filme *f, int year) { f->anoLancamento = year; }
void SetDuration(Filme *f, int duration) { f->duracaoFilme = duration; }
void SetProduction(Filme *f, char *production) { strcpy(f->producao, production); }
void SetGenre(Filme *f, char *genre) { strcpy(f->generoFilme, genre); }
void SetTitle(Filme *f, char *title) { strcpy(f->tituloFilme, title); }
void SetDescription(Filme *f, char *description) { strcpy(f->descFilme, description); }

// Getters
int GetId(Filme *f) { return f->idFilme; }
int GetYear(Filme *f) { return f->anoLancamento; }
int GetDuration(Filme *f) { return f->duracaoFilme; }
char *GetProduction(Filme *f) { return f->producao; }
char *GetGenre(Filme *f) { return f->generoFilme; }
char *GetTitle(Filme *f) { return f->tituloFilme; }
char *GetDescription(Filme *f) { return f->descFilme; }


/*	Funcao que imprime os valores de uma estrutura Filme
	Parametros: Filme *f: endereco de memoria da estrutura
	Retorno: nenhum*/
void PrintFilme(Filme *filme){

	if(!filme) return;

	printf("\nID: %d\n", filme->idFilme);
	printf("Lancamento: %d\n", filme->anoLancamento);
	printf("Duracao: %dmin \n", filme->duracaoFilme);
	printf("Genero: %s\n", filme->generoFilme);
	printf("Titulo: %s\n", filme->tituloFilme);
	printf("Descricao: %s\n", filme->descFilme);
	printf("Producao: %s\n", filme->producao);

}

/*	Funcao que gera as IDs aleatorias sem numeros repetidos
	Parametros: nenhum
	Retorno: int* values: vetor contendo numeros de 1 a 100 ordenados
							aleatoriamente*/
int* Rand_Num()
{
	int i, temp;
	int* values = NULL;
	char* used = NULL;

	//aloco vetor com os valores
	values = (int*)malloc(100*sizeof(int));
	//aloco um vetor que marca quais valores foram usados
	used = (char*)calloc(100,sizeof(char));

	for(i = 0; i < 100; i++){
		//gero um numero aleatorio valido
		do{
			temp = (rand() % 100);
			if(temp >= 100)printf("%d\n",temp);
		}while(used[temp]); //sempre verificando se ele foi usado
		//insiro esse numero + 1 (pra ficar de 1 a 100)
		values[i] = temp+1;
		//marco que o numero foi usado
		used[temp] = 1;
	}

	//libero a memoria utilizada
	free(used);

	//retorno vetor com numeros aleatorios
	return values;
}

/*	Funcao que escreve uma string str em um arquivo textual
	Parametros: 
		FILE*  fp: ponteiro do arquivo utilizado
		char* str: ponteiro da string com o texto a ser escrito
	Retorno: nenhum */
void fprintf_str(FILE* fp, char* str)
{
	int i = 0;
	int size = strlen(str);
	while(i < size){
		if(str[i] == '\0') break;
		fprintf(fp,"%c",str[i++]);
	}
}

/*	Funcao que lê uma string de um arquivo textual
	a leitura é realizada até que um caractere '\n' seja encontrado
	Parametros: 
		FILE*  fp: ponteiro do arquivo utilizado
		char* str: ponteiro onde a string lida sera armazenada
	Retorno: o tamanho da string lida excluindo o caractere \0*/
int freadline(FILE* fp, char* string)
{
	char* buffer = NULL;
	int sizeCounter = 0;
	int curSize = 100;
	char c;

	//realoco o tamanho da buffer passada
	buffer = malloc(sizeof(char)*curSize);

	//leio do arquivo até encontrar um delimitadores
	do{
		c = fgetc(fp);
		//caso seja um caractere valido
		if(c != '\n'){
			sizeCounter++;
			//verifico se preciso ampliar o buffer
			if(sizeCounter >= curSize){
				buffer = (char*)realloc(buffer,sizeof(char)*(++curSize));
			}
			//guardo o valor lido
			buffer[sizeCounter-1] = c;
		}
	} while(c != '\n');
	buffer[sizeCounter] = '\0';

	strcpy(string,buffer);
	free(buffer);

	//retorno o numero de caracteres lidos e escritos em string
	return sizeCounter;
}

/*Funcao que prepara o ponteiro de leitura do arquivo txt para um reg
	que ainda nao foi inserido no arquivo bin
  A posicao escolhida eh aleatoria, sorteada entre os 100 registros do
  	arquivo txt.
	Parametros:
		FILE* fp: ponteiro do arquivo txt
		char* used: vetor que armazena flags, marcando quais registros
			jah foram lidos e inseridos no arquivo binario
	Retorno: nenhum, pois a funcao manipula o ponteiro do arquivo fp
*/
void seekToRandomPos(FILE* fp, char* used)
{
	char bfr;
	int count = 0;
	int limit;

	//busco o numero aleatorio de um registro que ainda nao foi usado
	do{
		limit = rand() % 100;
		if(count > 100){ //evita loop infinito
			fprintf(stderr,"failed to seek to random pos\n");
			break;
		}
	}while(used[limit]); //busco enquanto nao encontrar uma posicao valida

	used[limit] = 1; //marco que a posicao encontrada foi utilizada

	//me preparo para a leitura
	rewind(fp);
	limit *= 6; /*limit agora tem o numero de '\n's que devo ler
					para chegar na posicao correta de leitura*/


	//leio ateh encontrar minha posicao
	count = 0;
	while(count < limit){
		bfr = fgetc(fp);
		//conto os '\n' ateh chegar no 'RRN' certo
		if(bfr == '\n')count++; 
	}
}

/*Funcao que preenche um arquivo binario de dados com base em um
	arquivo de texto
	O nome do arquivo será "dados.bin"
	Eh gerado tambem um arquivo textual "debug.txt" que 
		exemplifica como os dados sao armazenados no arquivo binario
	Parametros:
		FILE* bin: ponteiro do arquivo binario utilizado
		FILE* txt: ponteiro do arquivo de texto utilizado
	Retorno:
		FILE* bin: ponteiro do arquivo binario gerado 
*/
void buildBinFile(FILE* bin, FILE* txt, FILE* smp)
{
	bTree* bt = newBTree();
	int* randomIds = NULL;
	char* alreadyUsed;
	int readCount;
	Filme* f;
	char endField = FIELD_DELIM;
	char endRegister = REG_DELIM;

	//verifico se os parametros dados sao validos
	if(bin == NULL){
		fprintf(stderr,"Invalid binary filename.\n");
		return;
	}

	if(txt == NULL){
		fprintf(stderr,"Invalid txt file pointer.\n");
		return;
	}

	/* Preparando os arquivos para escrita 
	Verifico se os ponteiros são diferentes de NULL antes de 
		tentar seta-los para o inicio da stream*/
	if(bin)fseek(bin,0L,SEEK_SET);
	if(smp)fseek(smp,0L,SEEK_SET);
	
	/*seto semente de rand como time(NULL) para geracao de id e
		insercao no arquivo binario de forma aleatoria*/
	srand(time(NULL));

	//inicializo a estrutura para leitura
	f = Inicialize_Struct();
	//gero uma lista com ids aleatorios
	randomIds = Rand_Num();
	//gero uma lista marcando quais 'RRN's do arquivo txt ja foram utilizados
	alreadyUsed = (char*)calloc(100,sizeof(char));

	//leio 100 registros do arquivo de texto
	for(readCount = 0; readCount < 100; readCount++){
		printf("==========\n");
		printf("readcount: %d\n",readCount);
		//preciso dar seek para um registro aleatorio do arquivo txt
		//para isso, vou salvar em um vetor quais "RRNs" ja foram colocados
		//depois, dou seek até aquela posicao
		seekToRandomPos(txt,alreadyUsed);

		/*realizo a leitura dos campos no arquivo txt, 
										salvo na struct Filme f*/
		SetId(f,randomIds[readCount]);
		fscanf(txt,"%d",&(f->anoLancamento)); //le ano de lancamento
		fgetc(txt); //le um '\n'
		fscanf(txt,"%d",&(f->duracaoFilme)); //le ano de lancamento
		fgetc(txt); //le um '\n'
		freadline(txt,f->tituloFilme); //le titulo do filme
		freadline(txt,f->generoFilme); //le genero do filme
		freadline(txt,f->descFilme); //le descricao do filme
		freadline(txt,f->producao); //le pais de producao
		// fscanf(txt,"%[^\n]s",f->producao); //le pais de producao

		//escrevo os dados de f no arquivo binario
		int offset = ftell(bin);
		insertBTree(bt,f->idFilme,offset);
		fwrite(&f->idFilme, sizeof(int), 1 , bin); //ID
		fwrite(&f->anoLancamento, sizeof(int), 1 , bin); //ANO
		fwrite(&f->duracaoFilme, sizeof(int), 1 , bin); //DURACAO
		fwrite(f->tituloFilme, sizeof(char), strlen(f->tituloFilme), bin); //TITULO
		fwrite(&endField, sizeof(char), 1 , bin); //DELIMITADOR DE CAMPO
		fwrite(f->generoFilme, sizeof(char), strlen(f->generoFilme), bin); //GENERO
		fwrite(&endField, sizeof(char), 1 , bin); //DELIMITADOR DE CAMPO
		fwrite(f->descFilme, sizeof(char), strlen(f->descFilme), bin); //DESCRICAO
		fwrite(&endField, sizeof(char), 1 , bin); //DELIMITADOR DE CAMPO
		fwrite(f->producao, sizeof(char), strlen(f->producao), bin); //PRODUCAO
		fwrite(&endField, sizeof(char), 1 , bin); //DELIMITADOR DE CAMPO
		fwrite(&endRegister, sizeof(char), 1 , bin); //DELIMITADOR DE REGISTRO

		//caso o arquivo de exemplo tenha sido solicitado, escrevo nele
		if(smp){
			fprintf(smp,"%d",f->idFilme); //ID
			fprintf(smp,"%d",f->anoLancamento); //ANO
			fprintf(smp,"%d",f->duracaoFilme); //DURACAO
			fprintf_str(smp,f->tituloFilme); //TITULO
			fprintf(smp,"%c",endField); //DELIMITADOR DE CAMPO
			fprintf_str(smp,f->generoFilme); //GENERO
			fprintf(smp,"%c",endField); //DELIMITADOR DE CAMPO
			fprintf_str(smp,f->descFilme); //DESCRICAO
			fprintf(smp,"%c",endField); //DELIMITADOR DE CAMPO
			fprintf_str(smp,f->producao); //PRODUCAO
			fprintf(smp,"%c",endField); //DELIMITADOR DE CAMPO
			fprintf(smp,"%c",endRegister); //DELIMITADOR DE REGISTRO	
		}


	}

	//libero memoria utilizada
	free(alreadyUsed);
	free(randomIds);
	Destroy_Struct(&f);
}

/*	lê uma string de um arquivo binario
	A leitura ocorre ateh que um caractere FIELD_DELIM seja encontrado
	Parametros: 
		FILE*  fp: ponteiro do arquivo utilizado
		char* string: endereco onde a string lida sera armazenada
	Retorno: o tamanho da string lida, excluindo o caractere '\0' */
int fread_str(FILE* fp, char* string)
{
	char* buffer = NULL;
	int sizeCounter = 0;
	int curSize = 100;
	char c;

	//realoco o tamanho da buffer passada
	buffer = realloc(buffer,sizeof(char)*curSize);

	//leio do arquivo até encontrar um delimitadores
	do{
		fread(&c,sizeof(char),1,fp);
		//caso seja um caractere valido
		if(c != FIELD_DELIM){
			sizeCounter++;
			//verifico se preciso ampliar o buffer
			if(sizeCounter >= curSize){
				buffer = (char*)realloc(buffer,sizeof(char)*(++curSize));
			}
			//guardo o valor lido
			buffer[sizeCounter-1] = c;
		}
	} while(c != FIELD_DELIM);
	buffer[sizeCounter] = '\0';

	//copio a string lida para o destino
	strcpy(string,buffer);
	//libero o buffer utilizado
	free(buffer);

	//retorno o numero de caracteres lidos e escritos em string
	return sizeCounter;
}

/*	funcao que le um registro do tipo Filme de um arquivo binario
	Parametros: 
		FILE*  fp: ponteiro do arquivo utilizado
		Filme* filme: ponteiro da estrutura onde os dados lidos
						 serao armazenados
	Retorno: nenhum*/
void fread_Filme(FILE* fp, Filme* filme)
{
	char* buffer = malloc(sizeof(char)*500);

	//lendo campos de tamanho fixo
	fread(&(filme->idFilme), sizeof(int), 1, fp );
	fread(&(filme->anoLancamento), sizeof(int), 1, fp );
	fread(&(filme->duracaoFilme), sizeof(int), 1, fp );

	//lendo campos de tamanho variavel
	fread_str(fp,buffer);
	strcpy(filme->tituloFilme,buffer);

	fread_str(fp,buffer);
	strcpy(filme->generoFilme,buffer);

	fread_str(fp,buffer);
	strcpy(filme->descFilme,buffer);

	fread_str(fp,buffer);
	strcpy(filme->producao,buffer);

	//pulando o delimitador de registro
	fseek(fp,1,SEEK_CUR);

	//liberando a memoria utilizada
	free(buffer);
}

/*	Funcao que le um registro do tipo Filme em um arquivo binario
	A leitura eh feita a partir do ponto atual do ponteiro de leitura
		do arquivo.
	Parametros:
		FILE*  fp: ponteiro do arquivo utilizado
	Retorno: Filme* filme: ponteiro da estrutura filme 
							com os dados carregados*/
Filme* getFilme(FILE* fp){

	Filme* filme = Inicialize_Struct();

	fread_Filme(fp,filme);

	return filme;
}

/*	Funcao que busca um registro do tipo Filme em um arquivo binario
	pelo campo ID, retornando os valores do registro encontrado em uma
	estrutura do tipo Filme
	Retorna NULL caso o ID nao seja encontrado no arquivo binario
	Parametros:
		FILE*  fp: ponteiro do arquivo utilizado
		int id: ID a ser buscado no arquivo binario
	Retorno: Filme* filme: ponteiro da estrutura filme encontrada
				NULL caso o ID nao seja encontrado*/
Filme* getFilmeById(FILE* fp, int id){
	Filme* filme = Inicialize_Struct();
	char* buffer = malloc(sizeof(char)*500);
	int searchId;

	while(fgetc(fp) != EOF){ //busco enquanto o arquivo nao acabar

		/*o comando fgetc acima verifica se o arquivo acabou. caso
		nao tenha acabado, preciso voltar uma posicao no ponteiro
		de leitura para ler os dados corretamente*/
		fseek(fp,-1*sizeof(char),SEEK_CUR);

		//carrego o registro completo(isso facilita percorrer o arquivo)
		fread_Filme(fp,filme);
		//verifico se o registro lido possui o ID procurado
		if(filme->idFilme == id) return filme;

	}

	//caso nao encontre, libero a memoria utilizada na busca
	Destroy_Struct(&filme);
	return NULL;
}