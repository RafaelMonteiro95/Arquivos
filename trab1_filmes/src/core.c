#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Movie.h"  


//Metodo retorna o struct do filme de numero n na lista de filme que se encontra no arquivo binario fp
//caso o numero nao exista o retorno eh NULL! Logo exige teste de existencia sempre que usada para evitar seg fault 
//n vai ate o (numero de filmes -1)
//author: Kaue
Filme* getFilme(FILE* fp, int n){

	Filme* filme = Inicialize_Struct();
	char* buffer = malloc(sizeof(char)*500);
	char marcador;


	//pulando registros 1...(n-1)
	while(n>=0){
		fread(&marcador, sizeof marcador,1,fp);
		while(marcador!=REG_DELIM){
			fread(&marcador, sizeof marcador,1,fp);
		}
		n--;
	}

	//Verifica Se registro existe e esta dentro do arquivo			
	if(fread(&(filme->idFilme),sizeof filme->idFilme,1,fp)<1){
		return NULL;
	}

	//lendo campos de tamanho fixo
	fread(&marcador, sizeof marcador,1,fp);
	fread(&(filme->anoLancamento), sizeof filme->anoLancamento, 1, fp );
	fread(&marcador, sizeof marcador,1,fp);
	fread(&(filme->duracaoFilme), sizeof filme->duracaoFilme, 1, fp );
	fread(&marcador, sizeof marcador,1,fp);	
	fread(&marcador, sizeof marcador,1,fp);


	//A partir daqui comeca a leitura dos campos de tamanho variavel

	//fica para proxima melhoria modularizar a funcao para pegar uma string
	//de um stream antes de um separador(no caso, FIELD_DELIM)

	int contador = 0;
	while(marcador!=FIELD_DELIM){
		buffer[contador]= marcador;
		fread(&marcador, sizeof marcador,1,fp);
		contador++;
	}
	fread(&marcador, sizeof marcador,1,fp);

	buffer[contador] = '\0';
	strcpy(filme->tituloFilme,buffer);


	contador = 0;
	while(marcador!=FIELD_DELIM){
		buffer[contador]= marcador;
		fread(&marcador, sizeof marcador,1,fp);
		contador++;
	}
	fread(&marcador, sizeof marcador,1,fp);

	buffer[contador] = '\0';
	strcpy(filme->generoFilme,buffer);


	contador = 0;
	while(marcador!=FIELD_DELIM){
		buffer[contador]= marcador;
		fread(&marcador, sizeof marcador,1,fp);
		contador++;
	}
	fread(&marcador, sizeof marcador,1,fp);
	buffer[contador] = '\0';
	strcpy(filme->descFilme,buffer);


	contador = 0;
	while(marcador!=FIELD_DELIM){
		buffer[contador]= marcador;
		fread(&marcador, sizeof marcador,1,fp);
		contador++;
	}
	buffer[contador] = '\0';
	strcpy(filme->producao,buffer);



	return filme;
}



//Metodo realiza busca sequencias para achar o ID na lista de filme que se encontra no arquivo binario fp
//caso o id nao for achado o retorno eh NULL! Logo exige teste de existencia sempre que usada para evitar seg fault 
//author: Kaue
Filme* getFilmeById(FILE* fp, int id){

	Filme* filme = Inicialize_Struct();
	char* buffer = malloc(sizeof(char)*500);
	char marcador;




	//Pesquisa do registro de ID N	
	if(fread(&(filme->idFilme),sizeof filme->idFilme,1,fp)<1){
		return NULL;
	}

	while(filme->idFilme != id){
		
		fread(&marcador, sizeof marcador,1,fp);
		while(marcador!=REG_DELIM){
			fread(&marcador, sizeof marcador,1,fp);
		}

		if(fread(&(filme->idFilme),sizeof filme->idFilme,1,fp)<1){
			return NULL;
		}

	}


	//lendo campos de tamanho fixo
	fread(&marcador, sizeof marcador,1,fp);
	fread(&(filme->anoLancamento), sizeof filme->anoLancamento, 1, fp );
	fread(&marcador, sizeof marcador,1,fp);
	fread(&(filme->duracaoFilme), sizeof filme->duracaoFilme, 1, fp );
	fread(&marcador, sizeof marcador,1,fp);	
	fread(&marcador, sizeof marcador,1,fp);


	//A partir daqui comeca a leitura dos campos de tamanho variavel

	//fica para proxima melhoria modularizar a funcao para pegar uma string
	//de um stream antes de um separador(no caso, FIELD_DELIM)

	int contador = 0;
	while(marcador!=FIELD_DELIM){
		buffer[contador]= marcador;
		fread(&marcador, sizeof marcador,1,fp);
		contador++;
	}
	fread(&marcador, sizeof marcador,1,fp);

	buffer[contador] = '\0';
	strcpy(filme->tituloFilme,buffer);


	contador = 0;
	while(marcador!=FIELD_DELIM){
		buffer[contador]= marcador;
		fread(&marcador, sizeof marcador,1,fp);
		contador++;
	}
	fread(&marcador, sizeof marcador,1,fp);

	buffer[contador] = '\0';
	strcpy(filme->generoFilme,buffer);


	contador = 0;
	while(marcador!=FIELD_DELIM){
		buffer[contador]= marcador;
		fread(&marcador, sizeof marcador,1,fp);
		contador++;
	}
	fread(&marcador, sizeof marcador,1,fp);
	buffer[contador] = '\0';
	strcpy(filme->descFilme,buffer);


	contador = 0;
	while(marcador!=FIELD_DELIM){
		buffer[contador]= marcador;
		fread(&marcador, sizeof marcador,1,fp);
		contador++;
	}
	buffer[contador] = '\0';
	strcpy(filme->producao,buffer);

	free(buffer);

	return filme;
}




