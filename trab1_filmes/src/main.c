#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Movie.h"

int checkInput()
{
	char input;
	printf("Escolha uma das opções abaixo:\n");
	printf("1 - Gerar 100 registros aleatórios de filmes\n");
	printf("2 - Exibir todos os dados, um a um\n");
	printf("3 - Recuperar um registro pelo ID(Chave Primária)\n");
	printf("Ou digite qualquer outro valor para sair\n");
	scanf("%c",&input);
	//leio como um char, retorno o valor em int desse char
	if(input >= 49 && input <= 51) return input - 48;
	//caso nao seja 1,2 ou 3, retorno 0
	else return 0;
}

int main()
{
	FILE* txt = NULL;
	FILE* bin = NULL;
	Filme* temp;
	int num = 19;
	int op;
	int id;
	int i;

	printf("Armazenamento de registros de filmes\n");
    
	printf("Abrindo os arquivos necessarios...");

	//arquivo com a database de filme em txt
	txt = fopen("moviedb.txt","r");
	//arquivo com os registros guardados em binario
	bin = fopen("dados.bin","wb+");

	//verifico se fp é nulo(ou seja, deu erro na leitura do arquivo)
	if(!bin || !txt){

		printf("Erro!\nProblema na abertura do arquivo\n");
		return 1;

	} else {

		printf("Done!\n");

		/*as opcoes 2 e 3 do menu nao fazem sentindo sem um arquivo dados.bin
					logo, inicializo um arquivo inicial.*/
		printf("Inserindo os registros de moviedb.txt para dados.bin...");
		buildBinFile(bin,txt,num);
		printf("Done!\n\n");

		do{
			//leio a opcao do usuario
			op = checkInput();
			switch(op){
				case 1: //gerar 100 registros
					fclose(bin);
					bin = fopen("dados.bin","wb+");
					printf("\nInserindo os registros de moviedb.txt para dados.bin...");
					buildBinFile(bin,txt,num);
					printf("Done!\n\n");
					getchar();
					break;
				case 2: //exibir todos os registros
					getchar();
					rewind(bin);
					for(i = 0; i < num; i++){
						temp = getFilme(bin);
						PrintFilme(temp);
						if(i != num-1)printf("\nPressione enter para o proximo filme\n");
						else printf("\nPressione enter para voltar ao menu\n");
						getchar();
					}
					break;
				case 3: //busca de registro por campo chave primaria
					rewind(bin);
					printf("\nInsira o ID do filme\n");
					scanf("%d",&id);
					temp = getFilmeById(bin,id);
					if(temp){
						printf("\nRegistro encontrado!");
						PrintFilme(temp);
						printf("\n");
					}
					else printf("\nRegistro nao encontrado.\n\n");
					getchar();
					break;
				default: //sair do programaa
					break;
			}

		}while(op);
	}

	fclose(bin);
	fclose(txt);
	return 0;
}


