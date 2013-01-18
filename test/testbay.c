#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../src/jnxhash.h"


int main(int argc, char** argv)
{
    char* countries[] = {"UK", "France", "Sweden", "Ireland", "U.S", "San francisco", "Israel" };
    char* capitals[] = {"London", "Paris", "Stockholm", "Dublin", "Washington DC", "California", "Jeruseleum"};
    int count = 7;
    int i;

    jnx_hashmap* mymap = jnx_hash_init(5);

    for (i = 0; i < count; ++i) {
        jnx_hash_put(mymap, countries[i], capitals[i]);
    }
    
    
	printf("Country %s with Capital %s\n", countries[0], jnx_hash_get(mymap, countries[0]));
	printf("Country %s with Capital %s\n", countries[3], jnx_hash_get(mymap, countries[3]));
	printf("Country %s with Capital %s\n", countries[5], jnx_hash_get(mymap, countries[5]));
	printf("Country %s with Capital %s\n", countries[6], jnx_hash_get(mymap, countries[6]));

	jnx_hash_delete(mymap);

    
    return 0;
}
