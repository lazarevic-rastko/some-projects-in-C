#include <stdio.h>
#include <stdlib.h>
#define MRTAV 'X'
#define ZIV 'O'
#define MRTVO_POLJE 4
#define ZIVO_POLJE 3
#define VODA 'W'
#define VODENO_POLJE 5
char**alociraj_matricu(char**mat,int red,int kol) {
    mat = malloc(sizeof(char *) * red);
    if (mat == NULL) {
        printf("MEM_GRESKA");
        return 0;
    }
    for (int i = 0; i < red; i++) {
        mat[i] = malloc(kol * sizeof(char));
        if (mat[i] == NULL) {
            printf("MEM_GRESKA");
            return 0;
        }
    }
    return mat;
}
void ucitaj_matricu(char** mat,int red, int kol){
    getchar();
    for(int i=0;i<red;i++){
        for(int j=0;j<kol;j++){
            mat[i][j]=getchar();}
        getchar();
    }
}
void ispisi_matricu(char** mat,int red,int kol){
    for(int i=0;i<red;i++){
        for(int j=0;j<kol;j++){
            if(i==red-1 && j==kol-1)printf("%c",mat[i][j]);
            else if(j==kol-1)printf("%c\n",mat[i][j]);
            else{printf("%c",mat[i][j]);}
            }
        }
    }
void dealociraj_matricu(char** mat,int red){
    for(int i=red-1;i>=0;i--)free(mat[i]);free(mat);
}

int ispitivanje_susednih_polja(char** mat,int i,int j,int red, int kol){
    int sus_ZIV=0;
    int start_red=(i==0)?0:i-1;      //POCETNI RED
    int end_red=(i==red-1)?red-1:i+1;//KRAJNJI RED
    int start_kol=(j==0)?0:j-1;      //POCETNA KOLONA
    int end_kol=(j==kol-1)?kol-1:j+1;//KRAJNJA KOLONA
    for(int k=start_red;k<=end_red;k++){
        for(int l=start_kol;l<=end_kol;l++){
            if(k==i && l==j)continue;
            if(mat[k][l]==ZIV)sus_ZIV++;
        }
    }
    return sus_ZIV;
}
void vodena_funkcija(char** mat,char** mat_cop,int i,int j,int red, int kol){
    int start_red=(i==0)?0:i-1;      //POCETNI RED
    int end_red=(i==red-1)?red-1:i+1;//KRAJNJI RED
    int start_kol=(j==0)?0:j-1;      //POCETNA KOLONA
    int end_kol=(j==kol-1)?kol-1:j+1;//KRAJNJA KOLONA
    for(int k=start_red;k<=end_red;k++) {
        for (int l = start_kol; l <= end_kol; l++) {
            if (k == i && l == j)mat_cop[k][l]='W';;
            if(mat[k][l]!='W')mat_cop[k][l]='O';
                    else{mat_cop[k][l]='W';}
        }
    }
}

int trenutno_polje(char** mat,int i,int j){
    if (mat[i][j]==MRTAV)return MRTVO_POLJE;
    if(mat[i][j]==ZIV)return ZIVO_POLJE;
    if(mat[i][j]==VODA)return VODENO_POLJE;
}
void obrada_izlazne_matrice(char** mat_org,char** mat_kop, int red, int kol){
    int tren_ziv;
    for(int i=0;i<red;i++){
        for(int j=0;j<kol;j++){
            tren_ziv=ispitivanje_susednih_polja(mat_org,i,j,red,kol);
            if(trenutno_polje(mat_org,i,j)==5){
                vodena_funkcija(mat_org,mat_kop,i,j,red,kol);
            }
            else if(trenutno_polje(mat_org,i,j)==3 && (tren_ziv==2 || tren_ziv==3)){
                mat_kop[i][j]='O';
            }
            else if(trenutno_polje(mat_org,i,j)==4 && tren_ziv==3){
                mat_kop[i][j]='O';
            }else{mat_kop[i][j]='X';}
        }
    }
}


int main() {
    char**matrica,**pomocna_matrica,**pom;
    int M,N,iter,iter1;
    scanf("%d %d",&M,&N);                    //ucitavanje dimenzije matrice, MxN
    if(M<=0 || N<=0)return 0;
    //ALOKACIJA POTREBNE MEMORIJE ZA MATRICE
    matrica=alociraj_matricu(matrica, M, N);
    pomocna_matrica=alociraj_matricu(pomocna_matrica, M, N);
    //UNOS
    ucitaj_matricu(matrica, M, N);
    scanf("%d",&iter1);                      //unos broja iteracija
    //INICIJALNI ISPIS
    printf("INITIAL\n");
    ispisi_matricu(matrica, M, N);
    if(iter1<=0){
        dealociraj_matricu(matrica,M);
        dealociraj_matricu(pomocna_matrica,M);
        return 0;}
    putchar('\n');
    // ITERACIJE //
    iter=iter1;
    while(iter){
        obrada_izlazne_matrice(matrica, pomocna_matrica, M, N);
        printf("ITERATION%d\n",iter1-iter);
        ispisi_matricu(pomocna_matrica, M, N);
        if(iter!=1)putchar('\n');
        pom=matrica;
        matrica=pomocna_matrica;
        pomocna_matrica=pom;
        iter--;
    }
    //OSLOBADJANJE MEMORIJE
    dealociraj_matricu(matrica,M);
    dealociraj_matricu(pomocna_matrica,M);
    return 0;
}

