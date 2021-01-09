#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "../cJSON/cJSON.h"
typedef struct test{
    int wartosc_x;
    int wartosc_y;
    char *podloze;
    
} TEST;

int parse_json(const char * const answer, TEST*dane)  // chan reques za answer
{
    const cJSON *payload = NULL;
    int status = 0;
    cJSON *game_json = cJSON_Parse(answer);
    //printf("parse\n");
    if (game_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        status = 0;
        goto end;
    }
    printf("after null\n");
    //x = cJSON_GetObjectItemCaseSensitive(game_json, "status");
    //printf("after get\n");
    //printf("inside_parse_json: %s\n",x->valuestring);
   /* if (cJSON_IsString(name) && (name->valuestring != NULL))
    {
        printf("Checking monitor \"%s\"\n", name->valuestring);
    }
*/
    payload = cJSON_GetObjectItemCaseSensitive(game_json, "payload");
   // cJSON_ArrayForEach(payload, payload)
    {
        cJSON *x = cJSON_GetObjectItemCaseSensitive(payload, "current_x");
        cJSON *y = cJSON_GetObjectItemCaseSensitive(payload, "current_y");
        cJSON *field_type =  cJSON_GetObjectItemCaseSensitive(payload, "field_type");
        printf("Payload:current_x: %d\n",x->valueint);
        printf("Payload:current_y: %d\n",y->valueint);
        printf("Payload:field_type: %s\n",field_type->valuestring);
        dane->wartosc_x=x->valueint;
        dane->wartosc_y=y->valueint;
        dane->podloze =field_type->valuestring;

        /*if (!cJSON_IsNumber(width) || !cJSON_IsNumber(height))
        {
            status = 0;
            goto end;
        }

        if ((width->valuedouble == 1920) && (height->valuedouble == 1080))
        {
            status = 1;
            goto end;
        }*/
    }

end:
    cJSON_Delete(game_json);
    return status;
}

typedef struct _Memory
{
    char *response;
    size_t size;
} Memory;
 
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
   // printf("Response: %s\n",mem->response);cd s   
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
        //curl_easy_setopt(curl, CURLOPT_HEADER, 1L);
 
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
            //printf("TUTAJ:%s", chunk.response);
            TEST t;
            parse_json(chunk.response, &t);
            //my_function(t);
            printf("t.x= %d\n",t.wartosc_x);
        }
 
        /* zawsze po sobie sprzątaj */
        free(chunk.response);
        curl_easy_cleanup(curl);
    }
}
int info(char *token) {
 
    // http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info/token
    char *url = (char*)malloc(sizeof(char*));
    strcpy(url,"http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/info");
    strcat(url,"/");
    strcat(url,token);
    make_request(url);
    free(url);
    return 0;
}
 
int move(char *token) {
    // http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/move/token
    char *url = (char*)malloc(sizeof(char*));
    strcpy(url,"http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/move");
    strcat(url,"/");
    strcat(url,token);
    make_request(url);
    free(url);
    return 0;
}
 
int rotate(char *token, char *direction)
{
    // http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate/token/direction
    char *url = (char*)malloc(sizeof(char*));
    strcpy(url,"http://edi.iem.pw.edu.pl:30000/worlds/api/v1/worlds/rotate");
    strcat(url,"/");
    strcat(url,token);
    strcat(url,"/");
    strcat(url,direction);
    make_request(url);
    free(url);
    return 0;
}
 
int main(int argc, char **argv)
{
   printf("witaj w misji rozpoznania terenu wroga :\n");
    printf("zasiądź za sterem czołgu i ruszaj na misję:\n");
    printf("sterowanie czołgiem:\n");
    printf("w -do przodu\n");
    printf("s -do tyłu\n");
    printf("a -obrót w lewo\n");
    printf("d -obrót w prawo\n");
    printf("i -info\n");
     printf("powodzenia\n");
    char znak;
    char *swiat=(char*)malloc(sizeof(char*));
    strcpy(swiat,"qwerty_20");
    znak = getchar();
    if (znak == 'w')
    move(swiat);
    else if (znak == 's')
   {
        rotate(swiat,"right");
        rotate(swiat,"right");
         move(swiat);
   }
    else if (znak == 'd')
    rotate(swiat,"right");
    else if (znak =='a')
    rotate(swiat,"left");
    else if (znak =='i')
    info(swiat);

    return 0;
}