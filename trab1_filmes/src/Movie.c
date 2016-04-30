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

/*Funcao que gera as IDs aleatorias sem numeros repetidos*/
int *Rand_Num()
 {
	 /*Tres variaveis que serao usadas, dois arrays de inteiros e uma variavel int*/
	 int *aux, *vet, i = 0;

	 /*Alocacao dos dois vetores de inteiros*/
		aux = (int*)malloc(100*sizeof(int));
		vet = (int*)malloc(100*sizeof(int));

		/*Inicializa ambos os vetores, todas as posicoes sao zero*/
		for(i=0; i<100; i++)
		 {
			vet[i] = 0;
			aux[i] = 0;
		 }

		 /*Zera a variavel i, pois sera usada no laco seguinte*/
		i=0;

		/*srand eh necessaria para gerar numeros aleatorios distintos toda vez que o computador seja inicializado*/
		srand(time(NULL));
		
		//srand(time(NULL));
		while(i != 100)
		 {
				/*Essa flag eh necessaria para que evite repeticoes, coloca-se 1 na posicao "aux[i]" no vetor vet,
				caso essa posicao nao seja 1, ele entra na flag e incrementa o i, caso contrario, ele segue no laco*/
				if(vet[aux[i]] != 1)
				 {
						aux[i]  = 1 + ( rand() % 100);
						vet[aux[i]] = 1;
						i++;
				}
		}
		/*Retorna o vetor de inteiros com os numeros gerados na funcao*/
	return aux;
 }

 /*Funcao que insere em dois arquivos, um de texto e um outro binario.
 O arquivo de texto eh usado meramente para debug, para que possamos saber
 se os arquivos estao sendo gravados de fato.
 Ela recebe como parametros dois aquivos, um vetor para armazenar os tamanhos
 das strings e o tamanho total de cada registro. Os delimitadores dos campos sao
 o seguinte caracter "|" e dos registros "#"*/
void Insert_in_File(FILE *fp, FILE *Text, int *size)
 {
	/*Inicia-se a variavel f, do tipo Filme*/
	Filme *f = Inicialize_Struct();
	/*Caracteres de fim de campos e registros*/
	/*Delimitadores e char para uso no loop*/
	char endRegister, endField;
	/*i serve para indice do array e o tam tem o tamanho total do registo*/
	int i = 0, tam = 0;

 /*Enquanto nao estive no final do arquivo, continua no loop*/
	while(fscanf(Text, "%d %c", &f->idFilme, &endRegister)>0)
	{
		/*Recupera o campo do arquivo txtm, assim como os delimitadores!*/
		fscanf(Text, "%d %c", &f->anoLancamento, &endRegister);
		fscanf(Text, "%d %c\n", &f->duracaoFilme,&endRegister);

		/*Recupera o campo do arquivo txtm, assim como os delimitadores! Alem de colocar no array de tamanhos o tamanho da string*/
		fgets(f->tituloFilme, STRING_SIZE,  Text);
		size[i] = strlen(f->tituloFilme);
		fscanf(Text, "%c", &endField);
		i++;

		/*Recupera o campo do arquivo txtm, assim como os delimitadores! Alem de colocar no array de tamanhos o tamanho da string*/
		fgets(f->generoFilme, STRING_SIZE, Text);
		size[i] = strlen(f->generoFilme);
		fscanf(Text, "%c", &endField);
		i++;

		/*Recupera o campo do arquivo txtm, assim como os delimitadores! Alem de colocar no array de tamanhos o tamanho da string*/
		fgets(f->descFilme, STRING_SIZE, Text);
		size[i] = strlen(f->descFilme);
		fscanf(Text, "%c", &endField);
		i++;

		/*Recupera o campo do arquivo txtm, assim como os delimitadores! Alem de colocar no array de tamanhos o tamanho da string*/
		fgets(f->producao, STRING_SIZE, Text);
		size[i] = strlen(f->producao);
		fscanf(Text, "%c %c", &endField, &endRegister);
		i++;

		/*Array com o tamanho total do registro no array, soma dos inteiros, tamanhos de string e delimitadores*/
		tam = (strlen(f->producao)+ strlen(f->descFilme) + strlen(f->generoFilme) + strlen(f->tituloFilme) + 3*sizeof(int) + 7*sizeof(char));
		size[i] = tam ;
		i++;

		/*Escreve registros no arquivo binario!!!*/
		fwrite(&f->idFilme, sizeof(int), 1 , fp);
		fwrite(&endField, sizeof(char), 1 , fp);

		fwrite(&f->anoLancamento, sizeof(int), 1 , fp);
		fwrite(&endField, sizeof(char), 1 , fp);

		fwrite(&f->duracaoFilme, sizeof(int), 1 , fp);
		fwrite(&endField, sizeof(char), 1 , fp);

		fwrite(f->tituloFilme, strlen(f->tituloFilme), 1 , fp);
		fwrite(&endField, sizeof(char), 1 , fp);

		fwrite(f->generoFilme, strlen(f->generoFilme), 1 , fp);
		fwrite(&endField, sizeof(char), 1 , fp);

		fwrite(f->descFilme, strlen(f->descFilme), 1 , fp);
		fwrite(&endField, sizeof(char), 1 , fp);

		fwrite(f->producao, strlen(f->producao), 1 , fp);
		fwrite(&endField, sizeof(char), 1 , fp);

		fwrite(&endRegister, sizeof(char), 1 , fp);
	}
	printf("Dados recuperados com sucesso!\n");
 }

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
//num eh o numero de registros no arquivo de texto
//used eh um vetor que marca qual valor ja foi utilizado
//fp eh o ponteiro do arquivo txt com os filmes
void seekToRandomPos(FILE* fp, char* used, int num)
{
	char bfr;
	int count = 0;
	int limit;

	//seto semente do numero aletorio para time(NULL)
	srand(time(NULL));

	//busco o numero aleatorio de um registro que ainda nao foi usado
	do{
		limit = rand() % num;
		if(count > num){ //evita loop infinito, resultado indesejado
			fprintf(stderr,"failed to seek to random pos\n");
			break;
		}
	}while(used[limit]);

	used[limit] = 1;

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

	//inicializo a estrutura para leitura
	f = Inicialize_Struct();
	//gero uma lista com ids aleatorios
	randomIds = Rand_Num();
	//gero uma lista marcando quais vetores ja foram utilizados
	alreadyUsed = (char*)calloc(num,sizeof(char));
	//leio 100 registros do arquivo de texto
	for(readCount = 0; readCount < num; readCount++){

		//preciso dar seek para uma posicao aleatoria
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