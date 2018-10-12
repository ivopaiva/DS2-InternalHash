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
    teste1->clientCode = 0;//1;
    strcpy(teste1->name,"joaoalbino");
    teste1->pointer = -1;
    teste1->status = 1;

    Client * teste2 = (Client*) malloc(sizeof(Client));
    teste2->clientCode = 21;//1;
    strcpy(teste2->name,"joaoalbi no2");
    teste2->pointer = -1;
    teste2->status = 1;

    Client * teste3 = (Client*) malloc(sizeof(Client));
    teste3->clientCode = 7;//5;
    strcpy(teste3->name,"joaoal bino3");
    teste3->pointer = -1;
    teste3->status = 1;

    Client * teste4 = (Client*) malloc(sizeof(Client));
    teste4->clientCode = 28;//12;
    strcpy(teste4->name,"joaoalbino 4COLISAO");
    teste4->pointer = -1;
    teste4->status = 1;

    Client * teste5 = (Client*) malloc(sizeof(Client));
    teste5->clientCode = 14;//19;
    strcpy(teste5->name,"joaoalbinoRe moving");
    teste5->pointer = -1;
    teste5->status = 1;


    Client * teste6 = (Client*) malloc(sizeof(Client));
    teste6->clientCode = 35;//19;
    strcpy(teste6->name,"joaoalbinoRe2moving");
    teste6->pointer = -1;
    teste6->status = 1;

    Client * teste7 = (Client*) malloc(sizeof(Client));
    teste7->clientCode = 42;//19;
    strcpy(teste7->name,"jao");
    teste7->pointer = -1;
    teste7->status = 1;

    Client * teste8 = (Client*) malloc(sizeof(Client));
    teste8->clientCode = 13;//19;
    strcpy(teste8->name,"zeh");
    teste8->pointer = -1;
    teste8->status = 1;

    insertClient(teste1, hash);
    printf("\n>>>>>>>>>>>>>>>>FICOU ASSIM<<<<<<<<<<<<<<<<<<<\n");
    printClients(hash);
    insertClient(teste2, hash);
    printf("\n>>>>>>>>>>>>>>>>FICOU ASSIM<<<<<<<<<<<<<<<<<<<\n");
    printClients(hash);
    insertClient(teste3, hash);
    printf("\n>>>>>>>>>>>>>>>>FICOU ASSIM<<<<<<<<<<<<<<<<<<<\n");
    printClients(hash);
    insertClient(teste4, hash);
    printf("\n>>>>>>>>>>>>>>>>FICOU ASSIM<<<<<<<<<<<<<<<<<<<\n");
    printClients(hash);
    strcpy(teste4->name,"joaoalbino4COLISA  OATUALIZADO");
    insertClient(teste5, hash);
    printf("\n>>>>>>>>>>>>>>>>FICOU ASSIM<<<<<<<<<<<<<<<<<<<\n");
    printClients(hash);
    insertClient(teste6, hash);
    printf("\n>>>>>>>>>>>>>>>>FICOU ASSIM<<<<<<<<<<<<<<<<<<<\n");
    printClients(hash);
    insertClient(teste7, hash);
    printf("\n>>>>>>>>>>>>>>>>FICOU ASSIM<<<<<<<<<<<<<<<<<<<\n");
    printClients(hash);
    removeClient(hash, 0);
    printf("\n>>>>>>>>>>>>>>>>APOS REMOCAO<<<<<<<<<<<<<<<<<\n");
    printClients(hash);
    insertClient(teste8, hash);
    printf("\n>>>>>>>>>>>>>>>>FICOU ASSIM<<<<<<<<<<<<<<<<<<<\n");
    // printClients(hash);

    updateClient(13, "test", hash);

    printf("Client below\n ---------- \n" );
    printClients(hash);

    // printf("\n***** Removing client 12 *****\n" );

    // removeClient(12, hash, hash);
    // printf("Client below\n ---------- \n" );
    // printClients(hash);


    fclose(hash);

    return 0;
}
