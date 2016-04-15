#ifndef _MOVIE_DATABASE_H_
#define _MOVIE_DATABASE_H_

typedef struct movie_register REGISTER;

// Constructor & destructor
REGISTER *CreateRegister(int id, int production, int year, int duration,
				int genre, char *title, char *description);
void DestroyRegister(REGISTER **reg);


// Setters
void SetId(int id);
void SetProduction(int production);
void SetYear(int year);
void SetDuration(int duration);
void SetGenre(int genre);
void SetTitle(char *title);
void SetDescription(char *description);

// Getters
int GetId(REGISTER *reg);
int GetProduction(REGISTER *reg);
int GetYear(REGISTER *reg);
int GetDuration(REGISTER *reg);
int GetGenre(REGISTER *reg);
char *GetTitle(REGISTER *reg);
char *GetDescription(REGISTER *reg);

#endif