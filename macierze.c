/*program uruchumiony przez valgrind działa dobrze, bez valgrinda zapełnia macierz losowymi liczbami*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "../cJSON/cJSON.h"

typedef struct test{
    int wartosc_x;
    int wartosc_y;
    char podloze[10];
    
} TEST;

typedef struct _macierz {
int r; //wiersz
int c; //kolumna
int przesuniecie_x;
int przesuniecie_y;
int **tab;
}macierz;

typedef struct dane{
    int x[3];
    int y[3];
    char* field[3]; 
}Dane;


void uzupelnienie(Dane*odczyt,macierz*plansza){


    for(int i=0;i<3;i++){
    uzupelnij(plansza,odczyt->x[i],odczyt->y[i],odczyt->field[i]);
    }
    
}

macierz*wczytaj(char*nazwa){
    FILE* plik;
    macierz*m =(macierz*) malloc(sizeof(macierz));
    plik = fopen(nazwa, "r");  
    fscanf(plik, "%d", &m->r);
    fscanf(plik, "%d", &m->c);
    m->tab = (int**) calloc(m->r, sizeof(int*));
    for (int i=0;i<m->r;i++)
    m->tab[i] = (int*) calloc(m->c, sizeof(int)); 
    // wczytanie
    for (int i=(m->r)-1; i >=0; i--) 
    {
        for (int j=0; j < m->c; j++) 
        {
            fscanf(plik, "%d", &m->tab[i][j]);
        }
    }
    fclose(plik);
    return m;
}

macierz*utworz(){
    macierz*m =(macierz*) malloc(sizeof(macierz));
    m->r=1;
    m->c=1;
    m->tab = (int**) calloc(m->r, sizeof(int*));
    for (int i=0;i<m->r;i++)
    m->tab[i] = (int*) calloc(m->c, sizeof(int)); 
    m->tab[0][0]=0;
    return m;
}

int plikIstnieje (char* nazwa){
    FILE* plik;
    plik = fopen(nazwa, "r");  
    if (plik)
    {
        fclose(plik);
        return 1;
    }
    return 0;
}

void zapisz(char*nazwa,macierz*m){  
    FILE*plik=fopen(nazwa,"w");
    fprintf(plik,"%d\n%d\n",m->r,m->c);
    for(int i = (m->r)-1; i>=0; i--)
        for(int j = 0; j <m->c; j++)
        {
            fprintf(plik,"%d\t",m->tab[i][j]);
            if(j+1 == m->c)
            fprintf(plik,"\n");
        }  
    fclose(plik);


}

void zwolnij(macierz*m){
    for (int i=0;i<m->r;i++) 
    free(m->tab[i]);
    free(m->tab);
    free(m); 
}

void uzupelnij(macierz*m,int x,int y, char*pole){ ////to do poprawy
    //warunek realokacji 
    if(m->r < x)
        if(x> 0){
        m->r=x;
        m->tab = (int**) realloc (m->tab, sizeof(int*) * (m->r));   
        }
    if(m->c < y)
        if(y > 0){
        m->c=y;  
        for (int i=0;i<m->r;i++)
        m->tab[i] = (int*) realloc(m->tab[i], sizeof(int) * (m->c));
        }
    printf("%d %d\n",m->r,m->c);
    //uzupelnienie macierzy
    if(pole = "grass")
    m->tab[x-1][y-1]=1;
    if(pole == "sand")
    m->tab[x-1][y-1]=2;
    if(pole == "wall")
    m->tab[x-1][y-1]=3;
    printf("%d %d\n",m->r,m->c);


}

int main(int argc, char **argv)
{
    char*nazwa_pliku="macierztestowa.txt";
    struct macierz *plansza;
     printf("%d\n",plikIstnieje(nazwa_pliku));
    if(plikIstnieje(nazwa_pliku)==1){
        plansza = wczytaj(nazwa_pliku);
    }
    else{
        plansza = utworz();
    }
    zapisz(nazwa_pliku,plansza);
    zwolnij(plansza);
    return 0;
}


//w tankv9 do maciory dodać te kurwy przesuniecie;