/*Autores:
	BRUNO BACELAR ABE		9292858
	KAUE LOPES DE MORAIS	9277576
	LUCAS ALEXANDRE SOARES	9293265
	RAFAEL AUGUSTO MONTEIRO	9293095
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Movie.h"  

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