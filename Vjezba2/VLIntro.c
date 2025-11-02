/*2. Definirati strukturu osoba (ime, prezime, godina rođenja) i napisati program koji:
A. dinamički dodaje novi element na početak liste,
B. ispisuje listu,
C. dinamički dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LENGTH 30

//pokazivac na strukturu Person
typedef struct Person* Position;

typedef struct Person {
	char firstName[MAX_LENGTH], lastName[MAX_LENGTH];
	int Byear;

	//pokazuje na iduci element
	Position Next;

}_person;

//pomocne funkcije
int GetPerson(char *, char *, int *);
int GetLastName(char *);
int UserInput(Position);
int upper_strcmp(const char *, const char*);

//prototipovi zad 2.
int InsertFirst(const char *, const char *, int, Position);
int InsertLast(const char *, const char *, int, Position);
int PrintList(Position);
Position Find(const char *, Position);
Position FindPrev(const char *, Position);
int DeleteElement(const char *, Position);
int FreeList(Position);

int main() {

	//head nema podatke, sluzi samo za implementaciju liste
	_person head = { .firstName =  "", .lastName = "", .Byear = 0, NULL };
	UserInput(&head);
	PrintList(head.Next);
	//na kraju treba osloboditi alociranu memoriju
	FreeList(&head);

	return 0;
}
int GetPerson(char * name, char * last_name, int * year){
    char buffer[128];
    if(!fgets(buffer, sizeof(buffer), stdin)) return 0;
    return sscanf(buffer, "%29s %29s %d", name, last_name, year) == 3;
}
int GetLastName(char *last_name){
    char buffer[128];
     if(!fgets(buffer, sizeof(buffer), stdin)) return 0;
    return sscanf(buffer, "%29s", last_name) == 1;
}
int UserInput(Position P) {
	int choice = -1;
	char buffer[128];
	while (1) {
		char tmp_name[MAX_LENGTH] = "", tmp_last_name[MAX_LENGTH] = "", lookup_last_name[MAX_LENGTH] = "";
		int tmp_year = 0;
		printf("\nIzaberite akciju nad listom:\n");
		printf("1 - Unos na pocetak\n2 - Unos na kraj\n3 - Pronadi element\n4 - Izbrisi element\n");
		printf("0 - prekini unos elemenata\n");
		printf("Odabir: ");
		if (!fgets(buffer, sizeof(buffer), stdin) || sscanf(buffer, "%d", &choice) != 1) {
			printf("greska pri unosu, pokusajte opet\n");
			continue;
		}
		switch(choice){
			case 0:
				printf("kraj unosa elemenata\n");
				return EXIT_SUCCESS;
			case 1:
				printf("Unesite redom ime, prezime i godinu rodenja osobe, odvojiti razmakom: ");
				if (GetPerson(tmp_name, tmp_last_name, &tmp_year)) {
						InsertFirst(tmp_name, tmp_last_name, tmp_year, P);
					break;
				}
				printf("uneseni krivi podatci, pokusajte ponovo\n");
				break;
			case 2:
				printf("Unesite redom ime, prezime i godinu rodenja osobe, odvojiti razmakom: ");
				if (GetPerson(tmp_name, tmp_last_name, &tmp_year)) {
						InsertLast(tmp_name, tmp_last_name, tmp_year, P);
					break;
				}
				printf("uneseni krivi podatci, pokusajte ponovo\n");
				break;
			case 3:
				printf("Unesite prezime osobe koju zelite pronaci: ");
				if (GetLastName(lookup_last_name)) {
						Find(lookup_last_name, P->Next);
					break;
				}
				printf("uneseni krivi podatci, pokusajte ponovo\n");
				break;
			case 4:
				printf("Unesite prezime osobe koju zelite izbrisati: ");
				if (GetLastName(lookup_last_name)) {
						DeleteElement(lookup_last_name, P);
					break;
				}
				printf("uneseni krivi podatci, pokusajte ponovo\n");
				break;
			default:
				printf("\nNeispravna opcija, pokusajte ponovo.\n");
				break;
			}
	}
}

int upper_strcmp(const char* string1, const char* string2) {
	int i = 0;
	char str1[MAX_LENGTH] = "", str2[MAX_LENGTH] = "";
	strcpy(str1, string1);
	strcpy(str2, string2);
	for (i = 0; str1[i]; i++) {
		str1[i] = toupper((unsigned char)str1[i]);
	}
	for (i = 0; str2[i]; i++) {
		str2[i] = toupper((unsigned char)str2[i]);
	}
	return strcmp(str1, str2);
}

int InsertFirst(const char* name, const char* last_name, int year, Position P){
	Position temp;
	temp = (Position)malloc(sizeof(_person));
	if (temp == NULL) {
		printf("\ngreska pri alokaciji memorije u InsertFront!\n");
		exit(EXIT_FAILURE);
	}
	temp->Next = NULL;

	strcpy(temp->firstName, name);
	strcpy(temp->lastName, last_name);
	temp->Byear = year;

	temp->Next = P->Next;
	P->Next = temp;

	return EXIT_SUCCESS;
}

int InsertLast(const char* name, const char* last_name, int year, Position P){
	Position temp;
	while (P->Next != NULL)
		P = P->Next;
	temp = (Position)malloc(sizeof(_person));
	if (temp == NULL){
		printf("\ngreska pri alokaciji memorije u InsertBack!\n");
		exit(EXIT_FAILURE);
	}
	temp->Next = NULL;

	strcpy(temp->firstName, name);
	strcpy(temp->lastName, last_name);
	temp->Byear = year;

	P->Next = temp;

	return EXIT_SUCCESS;
}

int PrintList(Position P){
	if (P == NULL) {
		printf("\nlista je prazna\n");
		return EXIT_SUCCESS;
	}
	printf("\nElementi liste\n");
	while (P != NULL) {
		printf("%s %s %d\n", P->firstName, P->lastName, P->Byear);
		P = P->Next;
	}
	return EXIT_SUCCESS;
}

Position Find(const char* last_name, Position P){
	while (P != NULL && (upper_strcmp(P->lastName, last_name) != 0))
		P = P->Next;
	if (P != NULL)
		printf("\nOsoba %s %s pronadena u listi\n", P->firstName, P->lastName);
	else printf("\nOsoba s prezimenom %s ne postoji u listi\n", last_name);

	return P;
}

Position FindPrev(const char* last_name, Position P){
	while (P->Next != NULL && (upper_strcmp(P->Next->lastName, last_name) != 0))
		P = P->Next;

	return P;
}

int DeleteElement(const char* last_name, Position P){

	Position Prev = FindPrev(last_name, P);
	if (Prev->Next == NULL) {
		printf("\nosoba s prezimenom %s nije u listi\n", last_name);
		return 0;
	}

	Position temp = Prev->Next;
	//element odspojiti iz liste prije brisanja
	Prev->Next = temp->Next;
	temp->Next = NULL;
	//oslobadanje memorije
	free(temp);

	return EXIT_SUCCESS;
}

int FreeList(Position head) {
	
	while (head->Next != NULL) {
		Position temp = head->Next;
		head->Next = temp->Next;
		temp->Next = NULL;
		free(temp);
	}
	
	return EXIT_SUCCESS;
}