all:
	cc sterowany.c -o sterowany ../cJSON/cJSON.o -lcurl 
	cc niesterowany.c -o niesterowany ../cJSON/cJSON.o -lcurl 



