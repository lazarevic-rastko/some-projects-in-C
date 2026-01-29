#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50
//POTREBNE STRUKTURE
typedef struct transakcija{
    char racun1[MAX];
    char racun2[MAX];
    float  stanje_racun1,stanje_racun2,iznos_transakcije;
    int status_transakcije;
}Transakcija;
typedef struct element{
    Transakcija sadrzaj_transakcije;
    struct element *next;
}Element;
typedef struct racuni{
    float stanje;
    char id[MAX];
    struct racuni *next;
}Racun;

//FUNKCIJA ZA UČITAVANJE LISTE TRANSAKCIJA
Element* ucitaj_listu(char *ulazna_datoteka, char *pocetno_stanje_racuna){
    Element *lista=NULL,*poslednji=NULL,*novi=NULL;
    Transakcija t;
    FILE *dat=fopen(ulazna_datoteka, "r");
    if(!dat){printf("DAT_GRESKA");return 0;};
    while (fscanf(dat,"%[^ ] -> %[^:]: %f\n",t.racun1,t.racun2,&t.iznos_transakcije)==3){
        novi = malloc(sizeof (Element));
        if(!novi){printf("MEM_GRESKA");return 0;}
        t.stanje_racun1= atof(pocetno_stanje_racuna);
        t.stanje_racun2= atof(pocetno_stanje_racuna);
        t.status_transakcije=(t.stanje_racun1>t.iznos_transakcije?1:0);
        novi->sadrzaj_transakcije=t;
        novi->next=NULL;
        if(poslednji==NULL){
            lista=novi;
        }else{
            poslednji->next=novi;
        }
        poslednji=novi;
    }
    fclose(dat);
    return lista;
}
//FUNKCIJE ZA OBRADU STATUSA TRANSAKCIJE
void azuriraj_status(Element *trenutni){
    Transakcija p=trenutni->sadrzaj_transakcije;
    Transakcija t; int n=0;
    Element *temp;
    for(temp=trenutni; temp; temp=temp->next){
        t=temp->sadrzaj_transakcije;
        t.stanje_racun1=(strcmp(t.racun1,p.racun1)==0)?t.stanje_racun1-p.iznos_transakcije:t.stanje_racun1;
        t.stanje_racun1=(strcmp(t.racun1,p.racun2)==0)?t.stanje_racun1+p.iznos_transakcije:t.stanje_racun1;
        t.stanje_racun2=(strcmp(t.racun2,p.racun1)==0)?t.stanje_racun2-p.iznos_transakcije:t.stanje_racun2;
        t.stanje_racun2=(strcmp(t.racun2,p.racun2)==0)?t.stanje_racun2+p.iznos_transakcije:t.stanje_racun2;
        if(n++)t.status_transakcije=(t.stanje_racun1>t.iznos_transakcije)?1:0;
        temp->sadrzaj_transakcije=t;
    }
}
void obrada_transakcija(Element *lista){
    Element *trenutni;
    Transakcija t;
    if(lista!=NULL) {
        for (trenutni = lista; trenutni; trenutni = trenutni->next) {
            t = trenutni->sadrzaj_transakcije;
            if (t.status_transakcije) {
                azuriraj_status(trenutni);
            }
        }
    }
}
//FUNKCIJA ZA ISPIS ODBIJENIH TRANSAKCIJA
int ispisi_odbijene(Element *lista,char *izlazna_datoteka){
    if(lista!=NULL) {
        FILE *dat= fopen(izlazna_datoteka, "w");
        if(!dat){printf("DAT_GRESKA");return 0;}
        for (Element *trenutni = lista; trenutni; trenutni = trenutni->next) {
            Transakcija t = trenutni->sadrzaj_transakcije;
            if (!t.status_transakcije) {
                fprintf(dat, "T(%s -> %s) = %.2f DENIED\n", t.racun1,
                        t.racun2, t.iznos_transakcije);
            }
        }
        fclose(dat);
    }
}
//FUNKCIJA ZA OBRADU RAČUNA
Racun* izdvoji_racune(Element *lista){
    Element *trenutni;
    Racun *izdvojeni_racuni=NULL, *poslednji=NULL,*tren_2=NULL,*novi;
    Transakcija t;
    int a=1,b=1;
    if(lista!=NULL) {
        for (trenutni = lista; trenutni; trenutni = trenutni->next) {
            t = trenutni->sadrzaj_transakcije;
            for (tren_2 = izdvojeni_racuni; tren_2; tren_2 = tren_2->next) {
                if ((a = strcmp(t.racun1, tren_2->id)) == 0) {
                    tren_2->stanje = t.stanje_racun1;
                    break;
                }
            }
            for (tren_2 = izdvojeni_racuni; tren_2; tren_2 = tren_2->next) {
                if ((b = strcmp(t.racun2, tren_2->id)) == 0) {
                    tren_2->stanje = t.stanje_racun2;
                    break;
                }
            }
            if (a) {
                novi = malloc(sizeof(Racun));
                if (!novi) {
                    printf("MEM_GRESKA");
                    return 0;
                }
                novi->stanje = t.stanje_racun1;
                strcpy(novi->id, t.racun1);
                novi->next = NULL;
                if (poslednji == NULL) {
                    izdvojeni_racuni = novi;
                } else {
                    poslednji->next = novi;
                }
                poslednji = novi;
            }
            if (b && strcmp(t.racun1, t.racun2) != 0) {
                novi = malloc(sizeof(Racun));
                if (!novi) {
                    printf("MEM_GRESKA");
                    return 0;
                }
                novi->stanje = t.stanje_racun2;
                strcpy(novi->id, t.racun2);
                novi->next = NULL;
                if (poslednji == NULL) {
                    izdvojeni_racuni = novi;
                } else {
                    poslednji->next = novi;
                }
                poslednji = novi;
            }
        }
    }
    return izdvojeni_racuni;
}
void ispisi_racune(Racun *lista,char *izlazna_datoteka){
    if(lista!=NULL) {
        FILE *dat = fopen(izlazna_datoteka, "a");
        if (!dat) {
            printf("DAT_GRESKA");
            return 0;
        }
        for (Racun *curr = lista; curr; curr = curr->next) {
            if (curr->next == NULL) { fprintf(dat, "A(%s) = %.2f", curr->id, curr->stanje); }
            else { fprintf(dat, "A(%s) = %.2f\n", curr->id, curr->stanje); }
        }
        fclose(dat);
    }
}
void sortiraj_racune(Racun *lista){
    if(lista!=NULL) {
        for (Racun *i = lista; i; i = i->next) {
            for (Racun *j = i->next; j; j = j->next) {
                if (strcmp(i->id, j->id) > 0) {
                    float temp;
                    char tempime[MAX];
                    strcpy(tempime, i->id), temp = i->stanje;
                    strcpy(i->id, j->id), i->stanje = j->stanje;
                    strcpy(j->id, tempime), j->stanje = temp;
                }
            }
        }
    }
}
//FUNKCIJE ZA OSLOBAĐANJE LISTI I DEALOKACIJU MEMORIJE
void oslobodi_transakcije(Element *lista){
    Element *trenutni;
    for(trenutni=lista; trenutni;){
        Element *temp=trenutni;
        trenutni=trenutni->next;
        free(temp);
    }
}
void oslobodi_racune(Racun *lista){
    Racun *trenutni;
    for(trenutni=lista; trenutni;){
        Racun *temp=trenutni;
        trenutni=trenutni->next;
        free(temp);
    }

}

int main(int argc,char* argv[]) {
    //DEKLARACIJE
    Element *transakcije=NULL;
    Racun *racuni=NULL;
    //UCITAVANJE TRANSAKCIJA
    transakcije=ucitaj_listu(argv[1], argv[3]);
    //OBRADA LISTE TRANSAKCIJA
    obrada_transakcija(transakcije);
    //OBRADA RAČUNA
    racuni=izdvoji_racune(transakcije);
    sortiraj_racune(racuni);
    //ISPIS
    ispisi_odbijene(transakcije, argv[2]);
    ispisi_racune(racuni,argv[2]);
    //OSLOBADJANJE LISTI I DEALOKACIJA
    oslobodi_racune(racuni);
    oslobodi_transakcije(transakcije);
    return 0;
}

