#include <stdio.h>
#include <stdlib.h>
#include "compartimento_hash.h"
#include "string.h"

//UPDATE N TÁ FUNCIONANDO DIREITO. NA VERDADE, ACHO Q TEM ALGO ERRADO NA INSERÇÃO (TALVEZ O ACERTO DE PONTEIROS).
//TEM DE FAZER A REMOÇÃO
int main(int argc, char const *argv[]) {
    FILE * hash = fopen("tabHash.dat", "w+b");
    createHash(HASH_SIZE, hash);
    hash = fopen("tabHash.dat", "r+b");

    Client * teste1 = (Client*) malloc(sizeof(Client));
    teste1->clientCode = 1;
    strcpy(teste1->name,"joaoalbino");
    teste1->pointer = -1;
    teste1->status = 1;

    Client * teste2 = (Client*) malloc(sizeof(Client));
    teste2->clientCode = 1;
    strcpy(teste2->name,"joaoalbi no2");
    teste2->pointer = -1;
    teste2->status = 1;

    Client * teste3 = (Client*) malloc(sizeof(Client));
    teste3->clientCode = 5;
    strcpy(teste3->name,"joaoal bino3");
    teste3->pointer = -1;
    teste3->status = 1;

    Client * teste4 = (Client*) malloc(sizeof(Client));
    teste4->clientCode = 12;
    strcpy(teste4->name,"joaoalbino 4COLISAO");
    teste4->pointer = -1;
    teste4->status = 1;

    Client * teste5 = (Client*) malloc(sizeof(Client));
    teste5->clientCode = 19;
    strcpy(teste5->name,"joaoalbinoRe moving");
    teste5->pointer = -1;
    teste5->status = 1;

    insertClient(teste1, hash);
    insertClient(teste2, hash);
    insertClient(teste3, hash);
    insertClient(teste4, hash);
    strcpy(teste4->name,"joaoalbino4COLISA  OATUALIZADO");
    insertClient(teste5, hash);

    updateClient(12, "test", hash);

    printf("Client below\n ---------- \n" );
    printClients(hash);

    // printf("\n***** Removing client 12 *****\n" );

    // removeClient(12, hash, hash);
    // printf("Client below\n ---------- \n" );
    // printClients(hash);


    fclose(hash);

    return 0;
}
