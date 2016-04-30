#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Movie.h"  


/*lê uma string de um arquivo binario
a leitura é realizada até que um caractere REG_DELIM seja encontrado
retorna o tamanho da string lida excluindo o caractere \0
(equivalente à strlen(string))
autor: Rafael*/
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
				buffer = (char*)realloc(buffer,sizeof(char)*(curSize++));
			}
			//guardo o valor lido
			buffer[sizeCounter-1] = c;
		}
	} while(c != FIELD_DELIM);
	buffer[sizeCounter] = '\0';

	strcpy(string,buffer);

	free(buffer);

	//retorno o numero de caracteres lidos e escritos em string
	return sizeCounter;
}

void seek_delim(FILE* fp, char delim)
{
	char c;

	//leio do arquivo até encontrar um delimitadores
	do{
		fread(&c,sizeof(char),1,fp);
	} while(c != delim);
}

/* funcao que le um registro do tipo Filme de um arquivo binario dado
	por fp.*/
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


//Metodo retorna o struct do filme de numero n na lista de filme que se encontra no arquivo binario fp
//caso o numero nao exista o retorno eh NULL! Logo exige teste de existencia sempre que usada para evitar seg fault 
//n vai ate o (numero de filmes -1)
//author: Kaue, Rafael
//na vdd soh leh o proximo filme do arquivo bin
Filme* getFilme(FILE* fp){

	Filme* filme = Inicialize_Struct();

	fread_Filme(fp,filme);

	return filme;
}



//Metodo realiza busca sequencias para achar o ID na lista de filme que se encontra no arquivo binario fp
//caso o id nao for achado o retorno eh NULL! Logo exige teste de existencia sempre que usada para evitar seg fault 
//author: Kaue
Filme* getFilmeById(FILE* fp, int id){

	Filme* filme = Inicialize_Struct();;
	char* buffer = malloc(sizeof(char)*500);
	int searchId;

	while(fgetc(fp) != EOF){
		fseek(fp,-1*sizeof(char),SEEK_CUR);
		//leio o delimitador
		fread(&searchId, sizeof(int), 1, fp );
		fseek(fp,-1*sizeof(int),SEEK_CUR);
		fread_Filme(fp,filme);
		//verifico se encontrei
		if(filme->idFilme == id) return filme;

	}

	return NULL;
}

