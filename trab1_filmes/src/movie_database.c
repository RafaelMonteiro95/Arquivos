/* Movie database source file

	Bruno Bacelar Abe		9292858
	Kaue Lopes de Morais		9277576
	Lucas Alexandre Soares		9293265
	Rafael Augusto Monteiro	9293095

*/

#include <stdio.h>
#include <stdlib.h>

#include "movie_database.h"

// Examples
#define SET_GENRE(genre) (1 << genre)			// Bitwise set genre (should make a table genre|bit)
#define GET_GENRE(value, genre) (value & (1 << genre))	// Bitwise set genre (should make a table genre|bit)

// Enum to compare and pass to SET/GET_GENRE macro
enum genre {
	COMEDY,	// 0
	DRAMA		// 1
};
// so i can make something like
// reg->genre = GET_GENRE(COMEDY) + GET_GENRE(DRAMA)
// (reg->genre & GETGENRE(COMEDY) ? is a comedy! : is not a comedy)

struct movie_register {
	
	int idFilme;		// Unique ID (primary key) - 4bytes
	int producao;		// Production country - 4bytes	
	int anoLancamento;	// Year - 4bytes
	int duracaoFilme;	// Duration in minutes - 4bytes
	int generoFilme;	// Genre as bitwise boolean (up to 32 genras) - 4bytes
	char *tituloFilme;	// Title - variable size string
	char *descFilme	string;	// Description - variable size string
};

REGISTER *CreateRegister(int id, int production, int year, int duration,
				int genre, char *title, char *description);
void DestroyRegister(REGISTER **reg);


// Setters
void SetId(REGISTER *reg, int id) { reg->idFilme = id; }
void SetProduction(REGISTER *reg, int production) { reg->producao = production; }
void SetYear(REGISTER *reg, int year) { reg->anoLancamento = year; }
void SetDuration(REGISTER *reg, int duration) { reg->duracaoFilme = duration; }
void SetGenre(REGISTER *reg, int genre) { reg->generoFilme = genre; }
void SetTitle(REGISTER *reg, char *title) { reg->tituloFilme = title; }
void SetDescription(REGISTER *reg, char *description) { reg->descFilme = description; }

// Getters
int GetId(REGISTER *reg) { return reg->idFilme; }
int GetProduction(REGISTER *reg) { return reg->producao; }
int GetYear(REGISTER *reg) { return reg->anoLancamento; }
int GetDuration(REGISTER *reg) { return reg->duracaoFilme; }
int GetGenre(REGISTER *reg) { return reg->generoFilme; }
char *GetTitle(REGISTER *reg) { return reg->tituloFilme; }
char *GetDescription(REGISTER *reg) { return reg->descFilme; }