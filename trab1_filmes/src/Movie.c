#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


void Insert_in_Text(FILE *Text, int ID)
 {
	/*Inicia-se a variavel f*/
	Filme *f = Inicialize_Struct();
	/*Caracteres de fim de campos e registros*/
	char End_Register = REG_DELIM;
	char End_Field = FIELD_DELIM;

		/*Escreve-se a ID do registro nos arquivos*/
		f->idFilme = ID;
						fprintf(Text, "%d", f->idFilme);
								fprintf(Text, "%c\n", End_Field);

		/*Escreve-se o ano de lancamento no arquivo*/
		printf("Digite o ano de lancamento: ");
				fscanf(stdin, "%d", &f->anoLancamento);
								fprintf(Text, "%d", f->anoLancamento);
										fprintf(Text, "%c\n", End_Field);

		/*Escreve-se a duracao do filme no arquivo*/
		printf("Digite a duracao: ");
		fscanf(stdin, "%d", &f->duracaoFilme);
				fprintf(Text, "%d", f->duracaoFilme);
						fprintf(Text, "%c\n", End_Field);

	/*Limpa o buffer, para que nao se pegue lixo ao usar a fgets para ler do teclado*/
	fflush(stdin);

		/*Coloca-se no arquivo o titulo do filme*/
		printf("Digite o nome: ");
				fgets(f->tituloFilme, STRING_SIZE, stdin);
						fprintf(Text, "%s", f->tituloFilme);
								fprintf(Text, "%c", End_Field);

		/*Coloca-se no arquivo o genero do filme*/
		printf("Digite o genero: ");
				fgets(f->generoFilme, STRING_SIZE, stdin);
						fprintf(Text, "%s", f->generoFilme);
								fprintf(Text, "%c", End_Field);

		/*Coloca-se no arquivo a sinopse do filme*/
		printf("Digite a sinopse: ");
				fgets(f->descFilme, STRING_SIZE,  stdin);
						fprintf(Text, "%s", f->descFilme);
								fprintf(Text, "%c", End_Field);

		/*Coloca-se no arquivo o pais de producao do filme*/
		printf("Digite o pais de producao: ");
				fgets(f->producao, STRING_SIZE, stdin);
						fprintf(Text, "%s", f->producao);
								fprintf(Text, "%c", End_Field);

		/*Coloca-se no arquivo o indicador de fim do registro*/
		fprintf(Text, "%c\n", End_Register);


	printf("\n");
	/*Desaloca a struct*/
	Destroy_Struct(&f);
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
	char End_Register, End_Field;
	/*i serve para indice do array e o tam tem o tamanho total do registo*/
	int i = 0, tam = 0;

 /*Enquanto nao estive no final do arquivo, continua no loop*/
	while(fscanf(Text, "%d %c", &f->idFilme, &End_Register)>0)
	{
					/*Recupera o campo do arquivo txtm, assim como os delimitadores!*/
		fscanf(Text, "%d %c", &f->anoLancamento, &End_Register);
		fscanf(Text, "%d %c\n", &f->duracaoFilme,&End_Register);

				/*Recupera o campo do arquivo txtm, assim como os delimitadores! Alem de colocar no array de tamanhos o tamanho da string*/
		fgets(f->tituloFilme, STRING_SIZE,  Text);
				size[i] = strlen(f->tituloFilme);
						fscanf(Text, "%c", &End_Field);
		i++;

				/*Recupera o campo do arquivo txtm, assim como os delimitadores! Alem de colocar no array de tamanhos o tamanho da string*/
		fgets(f->generoFilme, STRING_SIZE, Text);
				size[i] = strlen(f->generoFilme);
						fscanf(Text, "%c", &End_Field);
		i++;

			/*Recupera o campo do arquivo txtm, assim como os delimitadores! Alem de colocar no array de tamanhos o tamanho da string*/
		fgets(f->descFilme, STRING_SIZE, Text);
				size[i] = strlen(f->descFilme);
						fscanf(Text, "%c", &End_Field);
		i++;

		/*Recupera o campo do arquivo txtm, assim como os delimitadores! Alem de colocar no array de tamanhos o tamanho da string*/
		fgets(f->producao, STRING_SIZE, Text);
				size[i] = strlen(f->producao);
						fscanf(Text, "%c %c", &End_Field, &End_Register);
		i++;

		/*Array com o tamanho total do registro no array, soma dos inteiros, tamanhos de string e delimitadores*/
		 tam = (strlen(f->producao)+ strlen(f->descFilme) + strlen(f->generoFilme) + strlen(f->tituloFilme) + 3*sizeof(int) + 7*sizeof(char));
		 size[i] = tam ;
		 i++;

		 /*Escreve registros no arquivo binario!!!*/
	 fwrite(&f->idFilme, sizeof(int), 1 , fp);
	 fwrite(&End_Field, sizeof(char), 1 , fp);

	 fwrite(&f->anoLancamento, sizeof(int), 1 , fp);
	 fwrite(&End_Field, sizeof(char), 1 , fp);

	 fwrite(&f->duracaoFilme, sizeof(int), 1 , fp);
	 fwrite(&End_Field, sizeof(char), 1 , fp);

	 fwrite(f->tituloFilme, strlen(f->tituloFilme), 1 , fp);
	 fwrite(&End_Field, sizeof(char), 1 , fp);

	 fwrite(f->generoFilme, strlen(f->generoFilme), 1 , fp);
	 fwrite(&End_Field, sizeof(char), 1 , fp);

	 fwrite(f->descFilme, strlen(f->descFilme), 1 , fp);
	 fwrite(&End_Field, sizeof(char), 1 , fp);

	 fwrite(f->producao, strlen(f->producao), 1 , fp);
	 fwrite(&End_Field, sizeof(char), 1 , fp);

	 fwrite(&End_Register, sizeof(char), 1 , fp);
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