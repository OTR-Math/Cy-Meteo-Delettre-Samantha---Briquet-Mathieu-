/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct PileDyn{
    int nb;
    struct PileDyn* suivant;
}PileDyn;

void affichePile(PileDyn* ppile){
    if(ppile->suivant!=NULL){
        printf("%d -> ", ppile->nb);
        affichePile(ppile->suivant);
    }
    else{
        printf("%d \n", ppile->nb);
    }
}

PileDyn* creationPile(int a){
    PileDyn* c=malloc(sizeof(PileDyn));
    if(c==NULL){
        exit(1);
    }
    c->nb = a;
    c->suivant = NULL;
    return c;
}

PileDyn* empiler(PileDyn* ppile, int nb){
    PileDyn* nouveau=creationPile(nb);
    nouveau->suivant=ppile;
    return nouveau;
}



int main(){
    PileDyn* ppile;
    srand(time(NULL));
    PileDyn* p1=malloc(sizeof(PileDyn));
    ppile=creationPile(rand()%20);
    for(int i=1; i<10; i++){
        p1=empiler(p1,random()%20);
        
    }
    affichePile(p1);
    return 0;
}
