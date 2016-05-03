/*Autores:
	BRUNO BACELAR ABE		9292858
	KAUE LOPES DE MORAIS	9277576
	LUCAS ALEXANDRE SOARES	9293265
	RAFAEL AUGUSTO MONTEIRO	9293095
*/
#ifndef _MOVIE_H_
#define _MOVIE_H_

#include <stdio.h>
#include <stdlib.h>

// caractere utilizado como delimitador de registros
#define REG_DELIM '#'
// caractere utilizado como delimitador de campos
#define FIELD_DELIM '|'

// Struct que representa o registro de um filme
typedef struct filme Filme;

// Construtor e Destrutor da estrutura Filme
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

// Funcao de geracao de arquivo binario
void buildBinFile(FILE* bin, FILE* txt);

// Funcoes de leitura de arquivo
Filme* getFilme(FILE* fp);
Filme* getFilmeById(FILE* fp, int id);

// Funcao de impressao de filme
void PrintFilme(Filme *filme);

#endif