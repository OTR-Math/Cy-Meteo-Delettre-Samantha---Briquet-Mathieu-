/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Chainon{
    int nb;
    struct Chainon* suivant;
}Chainon;

Chainon* creationChainon(int a){
    Chainon* c= malloc(sizeof(Chainon));
    if(c==NULL){
        exit(1);
    }
    c->nb =a;
    c->suivant=NULL;
    return c;
}

Chainon* insertDebut(Chainon* pliste, int a){
    Chainon* nouveau=creationChainon(a);
    nouveau->suivant=pliste;
    return nouveau;
}

Chainon* insertFin(Chainon* pliste, int a){
    Chainon* nouveau=creationChainon(a);
    
    if(pliste==NULL){
        nouveau=pliste;
    }
    else{
        Chainon* p1 =pliste;
        while(p1->suivant != NULL){
            p1=p1->suivant;
        }
        p1->suivant = nouveau;
    }
    return pliste;
}

Chainon* ajoutCroissant(Chainon* pliste, int a){
    Chainon* nouveau = creationChainon(a);
    Chainon* p1=pliste;
    if(pliste==NULL){
        pliste=nouveau;
        
    }
    else if(pliste ->nb > a){
        pliste=insertDebut(pliste, a);
    }
        else{
            while(p1->suivant ->nb < a){
                p1=p1 -> suivant;
            }
            if(p1->suivant==NULL){
                p1->suivant=nouveau;
            }
            else{
                nouveau->suivant=p1->suivant;
                p1->suivant=nouveau;
            }
        }
        
    
    return pliste;
}

Chainon* suppDebut(Chainon* pliste, int a){
    Chainon* p1=pliste
    if(pliste!=NULL){
        pliste=pliste->suivant;
        free(p1);
    }
    return pliste;
}

Chainon* suppFin(Chainon* pliste, int a ){
    Chainon* p1=liste;
    if(pliste==NULL){
        return NULL;
    }
    else if(pliste->suivant == NULL){
        free(p1);
        return NULL;
    }
    else{ 
        while(p1->suivant ->suivant!=NULL){
            p1=p1-> suivant;
        }
        free(p1);
        p1->suivant=NULL;
    }
    return pliste;
}

Chainon* supp1Element(Chainon* pliste, int a){
    Chainon* p1 = pliste;
    Chainon* p2;
    if(pliste==NULL){
        return pliste;
    }
    if(pliste->nb==a){
        pliste=suppDebut(pliste,a);
        return pliste;
    }
    while(p1->suivant !=NULL && p1->suivant->nb !=a){
        p1=p1->suivant;
    }
    if(p1->suivant = NULL){
        return pliste;
    }
    p2=p1->suivant;
    p1->suivant = p2->suivant;
    free(p2);
    return pliste;
}

Chainon* inverse1(Chainon* pliste1, Chainon* pliste2){
    Chainon* p1=pliste1;
    while(p1!=NULL){
        pliste2=insertDebut(pliste2, p1->nb);
        p1=p1->suivant;
    }
    return pliste2;
}

Chainon* inverse2(Chainon* pliste){
    Chainon* p1;
    Chainon *p2;
    if(pliste==NULL){
        return pliste;
    }
    p1=pliste->suivant;
    pliste->suivant=NULL;
    while(p1){
     p2=p1->suivant;
     p1->suivant=pliste;
     pliste=p1;
     p1=p2;
    }
    return pliste;
}

int main()
{
   Chainon* listeChainee = malloc(sizeof(Chainon));
   srand(time(NULL));
   Chainon* pliste=NULL;
   for(int i=0; i<10;i++){
       pliste=insertDebut(pliste, rand()%6);
   }
    return 0;
}
