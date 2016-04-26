#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Movie.h"  

int main(){

	//Exemplo de uso da funcao getFilme

	//ps: note que temos um \n no final dos campos de tamanho variavel... 
	//Acho que o Bruno esta fazendo assim para facilitar. 


	FILE* fp = fopen("bin.txt","rb");
	if(!fp){
		printf("Problema na abertura do arquivo\n");
		return 1;		
	}else{
		Filme* filme = getFilmeById(fp,43);
		//filme
		if(filme)
			PrintFilme(filme);
		else
			printf("Nao existe registro com tal numero de pesquisa\n");
		Destroy_Struct(&filme);
	}

	fclose(fp);
	return 0;
}


