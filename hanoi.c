#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<dos.h>
#include<windows.h>

#define NUMDISCOS 10
#define NUMTORRES 5
typedef struct Nodo{
    int tamanho;
    struct Nodo *prox;
}Disco ;
typedef struct Torre{
    int numDiscos;
    struct Nodo *topo;
} Torre;

Torre *torres[NUMTORRES];
int movimentos;

int push(Torre **p, Disco *nodo){
    if((*p)->topo != NULL){
        if(((*p)->topo->tamanho) < (nodo->tamanho)){
            return 0;
        }
    }
    nodo->prox = (*p)->topo;
    (*p)->topo = nodo;
    (*p)->numDiscos++;
    return 1;
}

Disco *pop(Torre **p){
    Disco *aux = (*p)->topo;
    if(aux == NULL){
        return NULL;
    }
    (*p)->topo = aux->prox;
    (*p)->numDiscos--;
    return aux;

}
void gotoxy(int x, int y){
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){x,y});
}
Disco* getElemento(Torre *torre,int n){
    Disco *aux;
    if(n > torre->numDiscos){
        return NULL;
    }
    else if (torre->topo == NULL){
        torre->numDiscos = 0;
        return NULL;
    }
    aux = torre->topo;
    for (n = n; n> 1 ; n--){
        aux = aux->prox;
    }
    return aux;

}
void inicializaTorre(Torre **torre1){
    *torre1 = (Torre*) malloc(sizeof(Torre));
    (*torre1)->numDiscos = 0;
    (*torre1)->topo = NULL;
}
void imprimeTorre(Torre *torre,int x,int y){
    int i,j,aux2;
    Disco *aux;
    aux2 = NUMDISCOS- torre->numDiscos;
    for(i = 0; i <= aux2; i++){
        gotoxy(x,y);
        for(j = 0; j < NUMDISCOS; j++){
            printf(" ");
        }
        printf("|");
        for(j = 0; j < NUMDISCOS; j++){
            printf(" ");
        }
        y++;
    }
    for (i = 1; i <= torre->numDiscos; i++){
        aux = getElemento(torre,i);
        gotoxy(x,y);
        for(j = 0; j < NUMDISCOS - aux->tamanho; j++){
            printf(" ");
        }
        for(j = 0; j < aux->tamanho; j++){
            printf("/");
        }
        printf("|");
        for(j = 0; j < aux->tamanho; j++){
            printf("\\");
        }
        for(j = 0; j < NUMDISCOS-aux->tamanho; j++){
            printf(" ");
        }
        y++;
    }
    gotoxy(x,y);
    for(j = 0; j <= NUMDISCOS*2; j++){
        printf("-");
    }

}
void imprimeHanoi(){
    int x = 0;
    int y = 2;
    int i;
    gotoxy(0,0);
    printf("Movimentos : %d",movimentos);
    for(i = 0; i< NUMTORRES; i++){
        imprimeTorre(torres[i],x,y);
        x+=(NUMDISCOS * 2) + 1;
    }

}
void criaTorre(){
    Torre *aux;
    int i;
    for(i = 0; i< NUMTORRES; i++){
        inicializaTorre(&aux);
        torres[i]=aux;
        aux = NULL;
    }
}

Disco *criaDisco(int tamanho){
    Disco *aux = malloc(sizeof(Disco));
    aux->tamanho = tamanho;
    aux->prox = NULL;
    return aux;
}
int moveDisco(int torreOrg, int torreDst){
    Disco *aux;
    aux = pop(&torres[torreOrg]);
    if(aux == NULL){
        return 0;
    }
    if(push(&torres[torreDst],aux) != 1){
        push(&torres[torreOrg],aux);
        return 0;
    }
    system("cls");
    movimentos++;
    imprimeHanoi();
    Sleep(500);
    return 1;

}
int getTamanhoTopo(int NTorre){
    if(torres[NTorre]->numDiscos == 0){
        return NUMDISCOS+1;
    }
    return torres[NTorre]->topo->tamanho;
}
void empilhaTorre(int torreOrg,int torreDst,int numDiscos){
    if(numDiscos >=2){
        int numAux,i,j,tamanhoTopo;
        numAux=0;
        tamanhoTopo = getTamanhoTopo(torreOrg);
        for(i = 0;i < NUMTORRES ; i++){//verifica quantas auxiliares tem para fazer a transferencia das torres
            if(i != torreOrg && i != torreDst){
                if(tamanhoTopo < getTamanhoTopo(i)){
                    numAux++;
                }
            }
        }
        while(numDiscos-1 <numAux){
            numAux--;
        }
        int numDiscosPorAux = (numDiscos-1)/numAux;
        int sobra = numDiscos-1-(numDiscosPorAux * numAux);
        j = 0;
        for(i = 0; i< numAux;i++){
            while(j==torreOrg || j == torreDst || tamanhoTopo > getTamanhoTopo(j)){
                j++;
            }

            if(sobra > 0){
                if((sobra+numDiscosPorAux)-1<numAux){

                    empilhaTorre(torreOrg,j,numDiscosPorAux+sobra);
                    sobra = 0;

                }
                else{

                    empilhaTorre(torreOrg,j,numDiscosPorAux+1);

                    sobra--;
                }
            }
            else{

                empilhaTorre(torreOrg,j,numDiscosPorAux);

            }
            j++;
        }
        int tamanhoTorreDest = torres[torreDst]->numDiscos;

        moveDisco(torreOrg,torreDst);
        tamanhoTopo = getTamanhoTopo(torreDst);
        int maiorTopoDeAux,qtdElementosMenores;
        while(torres[torreDst]->numDiscos != numDiscos + tamanhoTorreDest){
            j = 0;
            maiorTopoDeAux = 0;
            for(i = 0;i < NUMTORRES ; i++){//verifica quantas auxiliares tem para fazer a transferencia das torres
                if(i != torreOrg && i != torreDst && getTamanhoTopo(i) < tamanhoTopo && maiorTopoDeAux < getTamanhoTopo(i)){
                    maiorTopoDeAux = getTamanhoTopo(i);
                    j = i;
                }
            }
            qtdElementosMenores = 0;
            for(i = 0; i <torres[j]->numDiscos;i++){
                if(getElemento(torres[j],i+1)->tamanho >tamanhoTopo || qtdElementosMenores+1 >numDiscos){
                    break;
                }
                qtdElementosMenores++;
            }
            empilhaTorre(j,torreDst,qtdElementosMenores);


        }

    }
    else if(numDiscos == 1){
        moveDisco(torreOrg,torreDst);
    }


}

void main(){
    criaTorre();
    int i;
    for(i = NUMDISCOS; i >= 1; i--){
        push(&(torres[0]),criaDisco(i));
    }
    movimentos = 0;
    imprimeHanoi();
    empilhaTorre(0,NUMTORRES-1,NUMDISCOS);

}
