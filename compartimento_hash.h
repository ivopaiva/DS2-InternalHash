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


int findFreePosition(FILE *fileName, int mappedPos){
    Client * client = (Client*) malloc(sizeof(Client));
    int i=0,j = mappedPos;

    while(i < HASH_SIZE){
        fseek(fileName, j*clientSize(), SEEK_SET);
        readClient(client, fileName);

        if(client->status == 0){
            return j;
        }
        j = (j+1) % HASH_SIZE;
        i++;
    }
    printf("Overflow!!!");
    exit(0);
}


int findClient(FILE *fileName, int key){
    int i=0,j;
    Client *tempCli = (Client *) malloc(sizeof(Client));

    j = hashFunction(key, HASH_SIZE);

    while(i < HASH_SIZE){
        fseek(fileName, j*clientSize(), SEEK_SET);
        readClient(tempCli, fileName);

        if(tempCli->clientCode == key){
            return j;//return client position
        }
        j = tempCli->pointer;
        i++;
    }
    return -1;//client not found
}

void insertClient(Client *cli, FILE *fileName){
    int mappedPos, freePos;
    Client *tempCli = (Client *) malloc(sizeof(Client));

    mappedPos = hashFunction(cli->clientCode, HASH_SIZE);
    cli->status = 1;
    

    if(findClient(fileName,cli->clientCode) != -1){
        printf("Dumb user! Client already exists!!!\n");
    }else{
        fseek(fileName, mappedPos*clientSize(), SEEK_SET);
        readClient(tempCli, fileName);

        if(tempCli->status == 0){//Position is available
            fseek(fileName, mappedPos*clientSize(), SEEK_SET);
            fwrite(&cli->clientCode,sizeof(int),1,fileName);
            fwrite(cli->name,sizeof(char),sizeof(cli->name), fileName);
            fwrite(&cli->status, sizeof(int),1,fileName);
        }else{
            int i,j=mappedPos;

            while(i < HASH_SIZE){
                fseek(fileName, j*clientSize(), SEEK_SET);
                readClient(tempCli, fileName);

                if(tempCli->status == 0 || tempCli->pointer == -1){
                    break;
                }
                j = tempCli->pointer;
                i++;
            }

            if(tempCli->status == 0){
                fseek(fileName, j*clientSize(), SEEK_SET);
                fwrite(&cli->clientCode,sizeof(int),1,fileName);
                fwrite(cli->name,sizeof(char),sizeof(cli->name), fileName);
                fwrite(&cli->status, sizeof(int),1,fileName);
            }else if(tempCli->pointer == -1){
                freePos = findFreePosition(fileName, j);
                fseek(fileName, freePos*clientSize(), SEEK_SET);
                fwrite(&cli->clientCode,sizeof(int),1,fileName);
                fwrite(cli->name,sizeof(char),sizeof(cli->name), fileName);
                fwrite(&cli->status, sizeof(int),1,fileName);

                //setting pointer of the last position of the chain. Now its gonna point to the recent added position
                fseek(fileName, j*clientSize()+clientSize()-sizeof(int), SEEK_SET);
                fwrite(&freePos, sizeof(int), 1, fileName);

            }else{
                printf("Overflow!!!\n");
                exit(0);
            }
        }
    }

    
}

void removeClient(FILE *fileName, int key){
    int mappedPos,i=0,pos,j;
    Client * tempCli = (Client*) malloc(sizeof(Client));

    // mappedPos = hashFunction(key, HASH_SIZE);

    
    pos = findClient(fileName, key);

    fseek(fileName, pos*clientSize(), SEEK_SET);
    readClient(tempCli, fileName);

    if(tempCli->clientCode == key){
        int nextInChain = tempCli->pointer;
        j = pos;

        while(i < HASH_SIZE){
            fseek(fileName, j*clientSize(), SEEK_SET);
            readClient(tempCli, fileName);

            if(tempCli->pointer == pos){
                fseek(fileName, j*clientSize()+clientSize()-sizeof(int), SEEK_SET);
                fwrite(&nextInChain, sizeof(int), 1, fileName);
                //i = 0;
            }//else{
                j = (j+1) % HASH_SIZE;
                i++;
            //}
        }
        int t = -1;
        tempCli->status = 0;
        fseek(fileName, pos*clientSize()+clientSize()-sizeof(int)-sizeof(int), SEEK_SET);
        fwrite(&tempCli->status, sizeof(int),1,fileName);
        // fwrite(&t, sizeof(int), 1, fileName);
    }else{
        printf("\nDumb user! Client does not exist!!!\n");
    }

    
}



void updateClient(int key, char *name, FILE *fileName){
    int pos;

    pos = findClient(fileName,key);

    if(pos == -1){
        printf("Dumb user! this client does not exist!!\n");
    }else{
        fseek(fileName, (pos * clientSize()) + sizeof(int), SEEK_SET);
        fwrite(name, sizeof(char), sizeof(name), fileName);
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
