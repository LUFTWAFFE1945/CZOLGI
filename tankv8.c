#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "../cJSON/cJSON.h"
 
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
char* info(char *token, macira*p) {
 
    char*chunk= (char*)malloc(sizeof(char)*1024);
    char *url = (char*)malloc(sizeof(char)*1024);
    strcpy(url,"http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info");
    strcat(url,"/");
    strcat(url,token);
    dane_z_odczytu=interpret_response(make_request(url),dane);
    uzupełnienie(dzane_z_odczytu,p);
    free(url);
    return 0;
}
 
char* explore(char *token, macira*p) {
    char*chunk= (char*)malloc(sizeof(char)*1024);
    char *url = (char*)malloc(sizeof(char)*1024);
    strcpy(url,"http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/explore");
    strcat(url,"/");
    strcat(url,token);
    dane_z_odczytu=interpret_response(make_request(url),dane);
    uzupełnienie(dzane_z_odczytu,p);
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
    Dane *dane=malloc(sizeof(Dane));
    char*chunk=(char*)malloc(sizeof(char*));
    FILE *fin=fopen(nazwa_folderu,"r");
    if(fin == NULL)
    {
    
    }
    Macierz *a;
    a=wczytaj(fin);
    algorytm( macierz_ogólne(maciora))
    //zapisz do pliku