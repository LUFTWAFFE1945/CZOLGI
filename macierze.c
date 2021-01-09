#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macierze.h"


void wypisz(macierz *m){
{
printf("To jest wynik\n[");
for(int i = (m->r)-1; i >= 0;  i--)
    {
        for(int j = 0; j < m->c; j++)
        {
            printf("%d\t", m->tab[i][j]); 
            if(j+1 == m->c)
            printf("\b\b]\n");
        } 
    if(i+1 != m->r)   
    printf("[");
    }
}
}

void zapisz(FILE * fin,macierz *m){
fprintf(fin,"%d\n%d\n",m->r,m->c);
for(int i = (m->r)-1; i >= 0;  i--)
        for(int j = 0; j <m->c; j++)
        {
            fprintf(fin,"%d\t",m->tab[i][j]);
            if(j+1 == m->c)
            fprintf(fin,"\n");
        }  
}

macierz* wczytaj(int a, int b){
    macierz *m=(macierz*) malloc(sizeof(macierz));
    m->r=a;
    m->c=b;
    m->tab = (int**) malloc(sizeof(int*) * m->r);
    for (int i=0;i<m->r;i++)
        m->tab[i] = (int*) malloc(sizeof(int) * m->c);
 
    return m;
}

void realokacja(macierz*m,int a, int b){
    m->r=a;
    m->c=b;
    m->tab = (int**) realloc(m,sizeof(int*) * m->r);
    for (int i=0;i<m->r;i++)
        m->tab[i] = (int*) realloc(m,sizeof(int) * m->c);
}

void zwolnij( macierz *m){
    for (int i=0;i<m->r;i++) 
    free(m->tab[i]);
    free(m->tab);
    free(m);
}
