/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*************************************exo1**************************************/

#include <stdio.h>
#include <stdlib.h>

typedef struct chainon{
    int nombre;
    struct chainon* suivant;
    int val;
}Chainon;

Chainon* creationChainon(int a){
    Chainon* c=malloc(sizeof(Chainon));
    if(c==NULL){
        exit;
    }
    c->nombre=a;
    c->suivant=NULL;
    return c;
}

Chainon* insertDebut(Chainon* pliste, int a){
    Chainon* nouveau=creationChainon(a);
    nouveau->suivant=pliste;
    pliste=nouveau;
    return pliste;
}

Chainon* insertFin(Chainon* pliste, int a){
    Chainon* nouveau = creationChainon(a);
    if(pliste==NULL){
        pliste=nouveau;
    }
    else{
        Chainon* p=pliste;
        while(p->suivant!=NULL){
            p=p->suivant;
        }
        p->suivant=nouveau;
    }
    return pliste;
}

void afficheListe(Chainon* pliste){
    Chainon* p=pliste;
    while(p!=NULL){
        printf("%d", p->nombre);
        if(p->suivant!=NULL){
            printf("->");
        }
        p=p->suivant;
    }
}


/******************************exo2********************************************/


Chainon* chainonTrie(Chainon* pliste, int a){
    Chainon* nouveau=creationChainon(a);
    Chainon* val;
    if(pliste==NULL){
        pliste=nouveau;
    }
    else{
        if(a<pliste->val){
            insertDebut(pliste,a);
        }
        else{
            Chainon* p2;
            Chainon* p1=pliste;
            while(p1->val<a && p1->suivant!=NULL){
                p2=p1;
                p1=p1->suivant;
            }
            if(p1->suivant==NULL){
                p1->suivant=nouveau;
            }
            else{
                p2->suivant=nouveau;
                nouveau->suivant=p1;
            }
        }
    }
    return pliste;
}


int main(){
    int a;
    Chainon* pliste;
    creationChainon(a);
    insertDebut(pliste, a);
    insertFin(pliste,a);
    afficheListe(pliste);
    chainonTrie(pliste, a);
}

















