#ifndef COMPARTIMENTO_HASH_H
#define COMPARTIMENTO_HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HASH_SIZE 7

typedef struct Client {
    int clientCode;
    char name[100];
    int status; // 1 used, 0 free
    int pointer;
} Client;

int hashFunction(int clientKey, int hashSize){
    return clientKey % hashSize;
}

int clientSize() {
    return sizeof(int)  // clientCode
    + sizeof(char) * 100 //clientName
    + sizeof(int) // status
    + sizeof(int); //pointer
}

int readClient(Client * cli, FILE *fileName){
    return (fread(&cli->clientCode, sizeof(int), 1, fileName) &&
    fread(cli->name, sizeof(char), sizeof(cli->name), fileName) &&
    fread(&cli->status, sizeof(int), 1, fileName) &&
    fread(&cli->pointer, sizeof(int), 1, fileName));

}

void writeClient(Client *cli, FILE *fileName, int filePos, int size){
    fseek(fileName, filePos * size, SEEK_SET);
    fwrite(&cli->clientCode, sizeof(int), 1, fileName);
    fwrite(cli->name, sizeof(char), sizeof(cli->name), fileName);
    fwrite(&cli->status, sizeof(int), 1, fileName);
    fwrite(&cli->pointer, sizeof(int), 1, fileName);
}


void findClient(int clientKey, FILE *fileName, int *filePos, int *controlVar){// clientkey= x, controlvar = a filepos = end
    Client * client = (Client*) malloc(sizeof(Client));
    *controlVar = 0;
    int j = -1; // J is the first free position of the client chain

    while (*controlVar == 0) {
        fseek(fileName, *filePos * clientSize() , SEEK_SET);
        readClient(client, fileName);

        if(client->status == 0 ){//if position is free
            j = *filePos;
        }
        if(client->clientCode == clientKey && client->status == 1){// if is the key I'm looking for and if the position is occupied
            *controlVar = 1;//found key
        }
        else {
            if(client->pointer == -1){ // key not found
                *controlVar = 2;
                *filePos = j;
            }else{
                *filePos = client->pointer;
            }
        }

    }
}

void insertClient(Client *cli, FILE *fileName){
    int filePos, controlVar = 0,j,i, mappedPos, temp; 
    filePos = hashFunction(cli->clientCode, HASH_SIZE);
    mappedPos = filePos;
    findClient(cli->clientCode, fileName, &filePos, &controlVar);
    Client * tempCli = (Client*) malloc(sizeof(Client));

    if(controlVar != 1){//key not found
        if(filePos != -1){
            j = filePos;
        }else{
            i=1;
            j = mappedPos;
            while(i <= HASH_SIZE){
                fseek(fileName, j * clientSize() , SEEK_SET);
                readClient(tempCli, fileName);

                if(tempCli->status == 1){//if position is occupied
                    j = (j+1) % HASH_SIZE;
                    i++;
                }else{
                    i = HASH_SIZE + 2;
                }
            }

            if(i == (HASH_SIZE + 1)){
                printf("Overflow!!!\n");
                exit(0);
            }

            //temp = T[h(x)].pointer
            fseek(fileName, mappedPos * clientSize(), SEEK_SET);
            readClient(tempCli, fileName);
            temp = tempCli->pointer;

            //T[h(x)].pointer = j
            tempCli->pointer = j;
            writeClient(tempCli, fileName, mappedPos, clientSize());

            //T[j].pointer = temp
            fseek(fileName, j * clientSize(), SEEK_SET);
            readClient(tempCli, fileName);
            tempCli->pointer = temp;
            writeClient(tempCli, fileName, j, clientSize());

        }

        //T[j].key = clientCode
        //T[j].status = occupied
        cli->status = 1;
        fseek(fileName, j*clientSize(),SEEK_SET);
        fwrite(&cli->clientCode, sizeof(int), 1, fileName);
        fwrite(cli->name, sizeof(char), sizeof(cli->name), fileName);
        fwrite(&cli->status, sizeof(int), 1, fileName);

    }else{
        printf("Not allowed: key already exists!\n");
    }

}

void updateClient(int key, char *name, FILE *fileName){
    int mappedPos = hashFunction(key, HASH_SIZE);
    int controlVar = 0;

    findClient(key, fileName, &mappedPos, &controlVar);

    if(controlVar == 2){
        printf("Dumb user! this client does not exist!!\n");
    }else{
        if(mappedPos != -1){
            fseek(fileName, (mappedPos * clientSize()) + sizeof(int), SEEK_SET);
            fwrite(name, sizeof(char), sizeof(name), fileName);
        }
    }
}

// Imprime funcionario
void createHash(int size, FILE *hashfile){
    Client *cli = (Client *) malloc(sizeof(Client));
    if (cli) memset(cli, 0, sizeof(Client));

    cli->clientCode = -1;
    cli->status = 0;
    cli->pointer = -1;


    for(int i=0; i< HASH_SIZE;i++){
        writeClient(cli,hashfile, i, clientSize());
    }
    fclose(hashfile);
}



// int checkPosition(FILE * fileName, int hashPos){
//     fseek(fileName, hashPos * sizeof(int), SEEK_SET);
//     int readItem;
//     fread(&readItem, sizeof(int), 1, fileName);

//     return readItem;
// }

// void insertPointer(FILE *hashFile, int pointer, int bucket){
//     fseek(hashFile, bucket, SEEK_SET);
//     fwrite(&pointer, sizeof(int), 1, hashFile);
// }

// void printHash(FILE * hashFile){
//     int pointer, i = 0;
//     rewind(hashFile);
//     while(fread(&pointer, sizeof(int), 1, hashFile) > 0){
//         printf("\n-----------------\n");
//         printf("Bucket %d: pointer: %d\n",i, pointer);
//         i++;
//     }
// }

void printClients(FILE *clientsFile){
    Client *c = (Client *) malloc(sizeof(Client));
    rewind(clientsFile);
    while(readClient(c, clientsFile)){
        printf("\n-----------------\n");
        printf("Code: %d\n",c->clientCode);
        printf("Name: %s\n", c->name);
        printf("Status: %d\n", c->status);
        printf("Pointer: %d\n", c->pointer );
    }
    free(c);
}


#endif
