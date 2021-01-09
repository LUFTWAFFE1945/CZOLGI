#ifndef _BAZA_H
#define _BAZA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _macierz {
int r; //wiersz
int c; //kolumna
int **tab;
}macierz;

void wypisz(macierz *m);
void zapisz(FILE * fin,macierz *m);
macierz* wczytaj(int a, int b);
void realokacja(macierz*m,int a, int b);
void zwolnij(macierz*m);

#endif