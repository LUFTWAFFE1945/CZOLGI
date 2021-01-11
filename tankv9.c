#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "../cJSON/cJSON.h"

int moj_globalny_int;
char moj_globalny_char;  //h

typedef struct dane{
    int x[3];
    int y[3];
    char* field[3]; 
}Dane;
typedef struct Maciora {
    int r;
    int c;
    int **tab;
}maciora;
typedef struct _Memory
{
    char *response;
    size_t size;
} Memory;

maciora*utworz(){
    maciora*m =(maciora*) malloc(sizeof(maciora));
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
maciora*wczytaj(char*nazwa){
    FILE* plik;
    maciora*m =(maciora*) malloc(sizeof(maciora));
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

void  uzupelnienie (Dane*d,maciora*m)
{

}
static size_t write_callback(void *data, size_t size, size_t nmemb, void *userp)
{
    /* to jest rzeczywista liczba bajtów przekazanych przez curl */
    size_t realsize = size * nmemb;
 
    /* jawnie konwertujemy (void*) na naszą strukturę, bo wiemy, że będziemy ją tutaj otrzymywać */
    Memory *mem = (Memory *) userp;
 
    char *ptr = NULL;
 
    /* Sprawdzamy czy pierwszy raz wywołujemy funkcję i trzeba zaalokować pamięć po raz pierwszy,
    czy trzeba zrobić reallokację (która kopiuje automatycznie starą zawartość w nowe miejsce) */
    if (mem->response != NULL)
        ptr = realloc(mem->response, mem->size + realsize + 1);
    else
        ptr = malloc(mem->size + realsize + 1);
 
    if (ptr == NULL)
        return 0; /* brak pamięci! */
 
    /* teraz zapamiętujemy nowy wskaźnik i doklejamy na końcu dane przekazane przez curl w 
       obszarze pamięci wskazywanym przez data */
    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0; // to na końcu dodajemy bo to w końcu string, i zawsze powinien się skończyć!
 
    return realsize;
}
char * make_request(char *url)
{
    CURL *curl;
    CURLcode res;
    Memory chunk;
    chunk.size = 0;
    chunk.response = NULL;
 
     curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        // curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
 
        /* to jest funkcja 'callback', która będzie wywołana przez curl gdy odczyta on kawałek danych
       ta funkcja musi mieć wywołanie zgodne z wymaganiami, które możesz sprawdzić tutaj:
       https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
 
        /* to jest adress struktury, który będzie przekazywany do naszej funkcji 'callback',
       do tej struktury nasz funkcja 'callback' będzie dopisywać wynik */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
 
        /* Wykonaj zapytanie 'synchronicznie', to znaczy następna linijka kodu nie wykona się
       dopóki nie nadejdzie odpowiedź z serwera. */
        res = curl_easy_perform(curl);
 
        /* Sprawdzamy czy wystapił jakis błąd? */
        if (res != CURLE_OK)
            fprintf(stderr, "Błąd! curl_easy_perform() niepowodzenie: %s\n", curl_easy_strerror(res));
        else
        {
            printf("%s", chunk.response);
        }
        /* zawsze po sobie sprzątaj */
        // interpret_response(chunk.response);
        // free(chunk.response);
        // curl_easy_cleanup(curl);
    }
    return chunk.response;
}
Dane* interpret_response(const char* const chunk,Dane *dane)
{
    const cJSON *payload = NULL;
    cJSON *game_json = cJSON_Parse(chunk);
    if (game_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
    }
    else {
        payload = cJSON_GetObjectItemCaseSensitive(game_json, "payload");
        cJSON *current_x = cJSON_GetObjectItemCaseSensitive(payload, "current_x");
        cJSON *list =  cJSON_GetObjectItemCaseSensitive(payload, "list");
        if(list!=NULL)
        {
                cJSON *object = NULL;
                int i=0;
                cJSON_ArrayForEach(object,list)
                {
                    cJSON *x = cJSON_GetObjectItemCaseSensitive(object, "x");
                    cJSON *y = cJSON_GetObjectItemCaseSensitive(object, "y");
                    cJSON *type = cJSON_GetObjectItemCaseSensitive(object, "type"); 
                    dane->y[i]=y->valueint;
                    dane->field[i] =(char*)malloc(sizeof(char)*strlen((type->valuestring)+1));
                    strcpy(dane->field[i], type->valuestring);
                    i++;
                }
        }           
 
        else if (current_x != NULL) {
            cJSON *current_y = cJSON_GetObjectItemCaseSensitive(payload, "current_y");
            cJSON *field_type =  cJSON_GetObjectItemCaseSensitive(payload, "field_type");
            dane->x[0]=current_x->valueint;
            dane->y[0]=current_y->valueint;
            dane->field[0] =field_type->valuestring;
        }
 
    }
    return dane;
}
char* info(char *token, maciora*p) {
 
    char*chunk= (char*)malloc(sizeof(char)*1024);
    char *url = (char*)malloc(sizeof(char)*1024);
    strcpy(url,"http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info");
    strcat(url,"/");
    strcat(url,token);
    Dane*z;
    uzupelnienie( interpret_response(make_request(url),z),p);
    free(url);
    return 0;
}
 
char* explore(char *token, maciora*p) {
    char*chunk= (char*)malloc(sizeof(char)*1024);
    char *url = (char*)malloc(sizeof(char)*1024);
    strcpy(url,"http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/explore");
    strcat(url,"/");
    strcat(url,token);
    Dane*z;
    uzupelnienie( interpret_response(make_request(url),z),p);
    free(url);
    return 0;
} 
char* move(char *token) {
    char*chunk= (char*)malloc(sizeof(char)*1024);
    char *url = (char*)malloc(sizeof(char)*1024);
    strcpy(url,"http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/move");
    strcat(url,"/");
    strcat(url,token);
    make_request(url);
    free(url);
    return 0;
}
 
char* rotate(char *token, char *direction)
{
    char*chunk= (char*)malloc(sizeof(char)*1024);
    char*url = (char*)malloc(sizeof(char)*1024);
    strcpy(url,"http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate");
    strcat(url,"/");
    strcat(url,token);
    strcat(url,"/");
    strcat(url,direction);
    make_request(url);
    free(url);
    return 0;
}

int main()
{
    char*chunk=(char*)malloc(sizeof(char*));
    char*nazwa_pliku="macierztestowa.txt";
    struct macierz *plansza;
    printf("%d\n",plikIstnieje(nazwa_pliku));
    if(plikIstnieje(nazwa_pliku)==1){
        plansza = wczytaj(nazwa_pliku);
    }
    else{
        plansza = utworz();
    }



}
 
    //a=wczytaj(fin);
   // algorytm( macierz_ogólne(maciora))
    //zapisz do pliku