#ifndef _MOVIE_H_
#define _MOVIE_H_

#include <stdio.h>
#include <stdlib.h>

#define REG_DELIM '#'
#define FIELD_DELIM '|'
#define STRING_SIZE 1000

/*Essa struct seria o registro, o qual tem campos de tamanho fixo e de tamanho variado*/
typedef struct filme
{  	/*Chave primária para busca, eh o codigo do filme. Eh um campo de tamanho  fixo,
	pois trata-se de um codigo numerico que dificilmente ira estourar o tamanho dos inteiros,
	tornando o seu uso variavel desnecessario.*/
	int idFilme;
	/*Ano de lançamento do filme. Campo de tamanho fixo, pois nesse campo o que será tratado sao anos, campo numerico,
	nao ha necessidade de uso de um campo variavel - visto que um filme tem, no maximo, 100 anos.*/
	int anoLancamento;
	/*Duracao do filme, em minutos.Campo de tamanho fixo, pois  estamos adotando a duracao do filme em minutos, nao ha
	"numeros com ponto flutuante", além do fato de que um filme normalmente possui por volta de 120 minutos*/
	int duracaoFilme;
	/*genero do filme. Esse campo eh necessario o uso de tamanho variavel. Como um filme pode possuir mesclas de generos,
	-terror, drama , por exemplo- tornou-se necessario o uso do tamanho variavel.*/
	char *generoFilme;
	/*Titulo do filme. Nesse campo tornou-se necessario o uso de tamanho variavel. Titulos de filmes sao extremamente volateis
	em questao de tamanho, podendo ter apenas um caracter para sua representacao, quanto muitas palavras, para evitar truncamento
	de dados e uso desnecessario da memoria, optou-se por esse campo*/
	char *tituloFilme;
	/*Sinopse do filme. A Sinopse, alem de ser um pequeno texto tem seu tamanho extremamente variado. Para evitar desperdicios e truncamento
	dos dados, usa-se o tamanho variado.*/
	char *descFilme;
	/*Pais de producao do filme. Trata-se de um campo ao qual refere-se aos pais(es) que produziram o filme. Como esse campo tambem pode
	possuir um ou mais paises, foi-se optado pelo uso de um campo variavel*/
	char *producao;
}Filme;

Filme *Inicialize_Struct();
void Destroy_Struct(Filme **filme);

// Setters
void SetId(Filme *f, int id);
void SetYear(Filme *f, int year);
void SetDuration(Filme *f, int duration);
void SetProduction(Filme *f, char *production);
void SetGenre(Filme *f, char *genre);
void SetTitle(Filme *f, char *title);
void SetDescription(Filme *f, char *description);

// Getters
int GetId(Filme *f);
int GetYear(Filme *f);
int GetDuration(Filme *f);
char *GetProduction(Filme *f);
char *GetGenre(Filme *f);
char *GetTitle(Filme *f);
char *GetDescription(Filme *f);

void Insert_in_File(FILE *fp, FILE *Text, int *size);
//void Insert_in_Text(FILE *Text, int ID);

Filme* getFilme(FILE* fp, int n);
Filme* getFilmeById(FILE* fp, int id);

void PrintFilme(Filme *filme);

void buildBinFile(FILE* bin, FILE* txt);

#endif