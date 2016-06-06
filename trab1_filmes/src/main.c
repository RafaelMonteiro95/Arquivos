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
#include "btree.h"

/*	Funcao que exibe um menu para o usuario e retorna um inteiro com a
		opcao escolhida
	Parametros: nenhum
	Retorno: 1, 2 ou 3, caso o input do usuario sejam os caracteres
					1, 2 ou 3, respectivamente
			 0, caso o input do usuario seja qualquer outro valor*/
int checkInput()
{
	char input;
	printf("Escolha uma das opções abaixo:\n");
	printf("1 - Gerar 100 registros aleatórios de filmes\n");
	printf("2 - Exibir todos os dados, um a um\n");
	printf("3 - Recuperar um registro pelo ID(Chave Primária)\n");
	printf("0 - Sair do programa\n");
	scanf("%c",&input);
	//leio como um char, retorno o valor em int desse char
	if(input >= 48 && input <= 51) return input - 48;
	//caso nao seja 1,2 ou 3, retorno 0
	else {
		printf("Entrada inválida\n\n");
		return -1;
	}
}

int main(int argc, char* argv[])
{
	FILE* txt = NULL;
	FILE* bin = NULL;
	FILE* smp = NULL;
	Filme* temp;
	int op;
	int id;
	int i;

	printf("Armazenamento de registros de filmes\n");
    
	printf("Abrindo os arquivos necessarios...");

	//arquivo com a database de filme em txt
	txt = fopen("moviedb.txt","r");
	//arquivo com os registros guardados em binario
	bin = fopen("dados.bin","wb+");
	//arquivo com os registros guardados como texto, para exemplo
	if(argc == 2){
		if(!strcmp(argv[1],"exemplo")){
			/*caso seja utilizado o parametro "exemplo" na execucao do 
			programa, abro o arquivo exemplo.txt*/
			smp = fopen("exemplo.txt","w");
		}
	}


	//verifico se os FILE* são nulos(ou seja, deu erro na leitura do arquivo)
	if(!bin){

		printf("Erro!\nProblema na abertura do arquivo binario\n");
		return 1;

	} else if(!txt) {
		
		printf("Erro!\nProblema na abertura do arquivo \"moviedb.txt\"\n");
		return 1;

	} else {

		printf("Done!\n");
		//aviso caso um arquivo exemplo seja gerado
		if(smp) printf("Um arquivo \"exemplo.txt\" sera gerado nessa execucao\n");

		/*as opcoes 2 e 3 do menu nao fazem sentindo sem um arquivo dados.bin
					logo, inicializo um arquivo inicial.*/
		printf("Inserindo os registros de moviedb.txt para dados.bin...");
		buildBinFile(bin,txt,smp);
		printf("Done!\n\n");

		do{
			//leio a opcao do usuario
			op = checkInput();
			switch(op){

				case 1: //gerar 100 registros

					printf("\nInserindo os registros de moviedb.txt para dados.bin...");
					buildBinFile(bin,txt,smp);
					printf("Done!\n\n");

					getchar(); //limpando buffer da stdin
					break;

				case 2: //exibir todos os registros
					getchar(); //limpando buffer da stdin
					rewind(bin); //preparo bin para leitura

					//exibo 100 registros, um a um
					for(i = 0; i < 100; i++){
						//leio um filme
						temp = getFilme(bin);
						//imprimo
						PrintFilme(temp);
						Destroy_Struct(&temp);
						//imprimo instrucao para usuario
						if(i != 99)printf("\nPressione enter para o proximo filme\n");
						else printf("\nPressione enter para voltar ao menu\n");
						//leio o enter do usuario
						getchar();
					}
					break;

				case 3: //busca de registro por campo chave primaria
					rewind(bin); //preparo bin para busca

					//leio o ID da busca
					printf("\nInsira o ID do filme\n");
					scanf("%d",&id);

					//realizo a busca
					temp = getFilmeById(bin,id);

					//interpreto o resultado
					if(temp){ //achei
						printf("\nRegistro encontrado!");
						PrintFilme(temp);
						printf("\n");
						Destroy_Struct(&temp);
					} //nao achei
					else printf("\nRegistro nao encontrado.\n\n");

					getchar(); //limpando buffer da stdin
					break;

				default: //sair do programa
					break;
			}

		}while(op);
	}

	//fecho os arquivos usados
	fclose(bin);
	fclose(txt);
	if(argc == 2){
		if(!strcmp(argv[1],"exemplo")){
			fclose(smp);
		}
	}
	return 0;
}


