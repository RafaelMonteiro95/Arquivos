#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Movie.h"

/*Funcao que inicializa a Struct para que possamos usar os vetores de tamanho variavel, essa funcao retorna
uma variavel -do tipo Filme -com memoria ja alocada*/
Filme *Inicialize_Struct()
 {
		/*Aloca memoria para o time Filme*/
	 Filme *f = malloc(sizeof(Filme));

		/*Inicializa a variavel inteira com zero*/
		f->idFilme = 0;
		f->anoLancamento = 0;
		f->duracaoFilme = 0;

		/*Aloca memoria para uso das Strings*/
		f->generoFilme = (char*)malloc(STRING_SIZE*sizeof(char));
		f->tituloFilme = (char*)malloc(STRING_SIZE*sizeof(char));
		f->descFilme = (char*)malloc(STRING_SIZE*sizeof(char));
		f->producao = (char*)malloc(STRING_SIZE*sizeof(char));


	return f;
 }

void Destroy_Struct(Filme **f)
 {
 		if(!f || !(*f)) return;

		 /*Funcao contraria a Inicializa_Struct, essa desaloca toda a Struct utilizada*/
		free((*f)->generoFilme);
		free((*f)->tituloFilme);
		free((*f)->descFilme);
		free((*f)->producao);
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

/*Funcao que gera as IDs aleatorias sem numeros repetidos*/
int* Rand_Num()
{
	int i, j, temp;
	int* values = NULL;
	char* used = NULL;

	//aloco vetor com os valores
	values = (int*)malloc(100*sizeof(int));
	//aloco um vetor que marca quais valores foram usados
	used = (char*)calloc(100,sizeof(char));

	for(i = 0; i < 100; i++){
		//gero um numero aleatorio valido
		do{
			temp = rand() % 100;
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

//imprime uma string str em um arquivo fp
void fprintf_str(FILE* fp, char* str)
{
	int i = 0;
	int size = strlen(str);
	while(i < size){
		if(str[i] == '\0') break;
		fprintf(fp,"%c",str[i++]);
	}
}

/*essa funcao da seek no arquivo txt buscando um registro que ainda
nao tenha sido inserido no arquivo binario
parametros:
num eh o numero de registros no arquivo de texto
used eh um vetor que marca qual valor ja foi utilizado
fp eh o ponteiro do arquivo txt com os filmes*/
void seekToRandomPos(FILE* fp, char* used, int num)
{
	char bfr;
	int count = 0;
	int limit;

	//busco o numero aleatorio de um registro que ainda nao foi usado
	do{
		limit = rand() % num;
		if(count > num){ //evita loop infinito, resultado indesejado
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
		if(bfr == '\n')count++;
	}

	/*pronto! ja coloquei o ponteiro do arquivo na posicao correta de
	leitura.*/
}

/*Funcao que preenche um arquivo binario de dados com base em um
	arquivo de texto
	O nome do arquivo será "dados.bin"
	Parametros:
		FILE* bin: ponteiro do arquivo binario utilizado
		FILE* txt: ponteiro do arquivo de texto utilizado
		int num: numero de registros no arquivo txt
	Retorno:
		FILE* bin: ponteiro do arquivo binario gerado 
*/
void buildBinFile(FILE* bin, FILE* txt, int num)
{
	int* randomIds = NULL;
	char* alreadyUsed;
	int readCount;
	Filme* f;
	char endField = FIELD_DELIM;
	char endRegister = REG_DELIM;
	FILE* debug = fopen("debug.txt","w");

	//verifico se os parametros dados sao validos
	if(bin == NULL){
		fprintf(stderr,"Invalid binary filename.\n");
		return;
	}

	if(txt == NULL){
		fprintf(stderr,"Invalid txt file pointer.\n");
		return;
	}
	
	/*seto semente de rand como time(NULL) para geracao de id e
		insercao no arquivo binario de forma aleatoria*/
	srand(time(NULL));

	//inicializo a estrutura para leitura
	f = Inicialize_Struct();
	//gero uma lista com ids aleatorios
	randomIds = Rand_Num();
	//gero uma lista marcando quais 'RRN's do arquivo txt ja foram utilizados
	alreadyUsed = (char*)calloc(num,sizeof(char));
	//leio 'num' registros do arquivo de texto
	for(readCount = 0; readCount < num; readCount++){

		//preciso dar seek para um registro aleatorio do arquivo txt
		//para isso, vou salvar em um vetor quais "RRNs" ja foram colocados
		//depois, dou seek até aquela posicao
		seekToRandomPos(txt,alreadyUsed,num);

		/*realizo a leitura dos campos no arquivo txt, 
										salvo na struct Filme f*/
		SetId(f,randomIds[readCount]);
		fscanf(txt,"%d",&(f->anoLancamento)); //le ano de lancamento
		fgetc(txt); //le um '\n'
		fscanf(txt,"%d",&(f->duracaoFilme)); //le ano de lancamento
		fgetc(txt); //le um '\n'
		fscanf(txt,"%[^\n]s",f->tituloFilme); //le titulo do filme
		fgetc(txt); //le um '\n'
		fscanf(txt,"%[^\n]s",f->generoFilme); //le genero do filme
		fgetc(txt); //le um '\n'
		fscanf(txt,"%[^\n]s",f->descFilme); //le descricao do filme
		fgetc(txt); //le um '\n'
		fscanf(txt,"%[^\n]s",f->producao); //le ano de producao

		//escrevo os dados de f no arquivo binario
		fwrite(&f->idFilme, sizeof(int), 1 , bin);
		fwrite(&f->anoLancamento, sizeof(int), 1 , bin);
		fwrite(&f->duracaoFilme, sizeof(int), 1 , bin);
		fwrite(f->tituloFilme, sizeof(char), strlen(f->tituloFilme), bin);
		fwrite(&endField, sizeof(char), 1 , bin);
		fwrite(f->generoFilme, sizeof(char), strlen(f->generoFilme), bin);
		fwrite(&endField, sizeof(char), 1 , bin);
		fwrite(f->descFilme, sizeof(char), strlen(f->descFilme), bin);
		fwrite(&endField, sizeof(char), 1 , bin);
		fwrite(f->producao, sizeof(char), strlen(f->producao), bin);
		fwrite(&endField, sizeof(char), 1 , bin);
		fwrite(&endRegister, sizeof(char), 1 , bin);

		fprintf(debug,"%d",f->idFilme);
		fprintf(debug,"%d",f->anoLancamento);
		fprintf(debug,"%d",f->duracaoFilme);
		fprintf_str(debug,f->tituloFilme);
		fprintf(debug,"%c",endField);
		fprintf_str(debug,f->generoFilme);
		fprintf(debug,"%c",endField);
		fprintf_str(debug,f->descFilme);
		fprintf(debug,"%c",endField);
		fprintf_str(debug,f->producao);
		fprintf(debug,"%c",endField);
		fprintf(debug,"%c",endRegister);
	}
}