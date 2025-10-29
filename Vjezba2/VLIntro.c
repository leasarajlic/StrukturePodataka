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

int UserInput(Position);
int upper_strcmp(const char*, const char*);

//prototipovi zad 2.
int InsertFront(const char *, const char *, int, Position);
int InsertBack(const char *, const char *, int, Position);
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
int UserInput(Position P) {
	int choice = -1, quit = 0;
	while (choice != quit) {
		char tmp_name[MAX_LENGTH] = "", tmp_last_name[MAX_LENGTH] = "", lookup_last_name[MAX_LENGTH] = "";
		int tmp_year = 0;
		printf("\nIzaberite akciju nad listom:\n");
		printf("1 - Unos na pocetak\n2 - Unos na kraj\n3 - Pronadi element\n4 - Izbrisi element\n");
		printf("0 - prekini unos elemenata\n");
		printf("Odabir: ");
		if (scanf("%d", &choice) == 1) {
			switch (choice) {
			case 0:
				printf("\nkraj unosa elemenata\n");
				return 0;
			case 1:
				printf("\nUnesite redom ime, prezime i godinu rodenja osobe, odvojiti razmakom: ");
				scanf("%29s %29s %d", tmp_name, tmp_last_name, &tmp_year);
				InsertFront(tmp_name, tmp_last_name, tmp_year, P);
				break;
			case 2:
				printf("\nUnesite redom ime, prezime i godinu rodenja osobe, odvojiti razmakom: ");
				scanf("%29s %29s %d", tmp_name, tmp_last_name, &tmp_year);
				InsertBack(tmp_name, tmp_last_name, tmp_year, P);
				break;
			case 3:
				printf("\nUnesite prezime osobe koju zelite pronaci: ");
				scanf("%29s", lookup_last_name);
				Find(lookup_last_name, P);
				break;
			case 4:
				printf("\nUnesite prezime osobe koju zelite izbrisati: ");
				scanf("%29s", lookup_last_name);
				DeleteElement(lookup_last_name, P);
				break;
			default:
				printf("\nUnesena je nedozvoljena vrijednost, pokusajte ponovo.\n");
				break;

			}
		}
		else {
			printf("\nUnesena je nedozvoljena vrijednost, pokusajte ponovo.\n");
			while (getchar() != '\n');
			choice = -1;
			continue;
		}
	}
	return 0;
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

int InsertFront(const char* name, const char* last_name, int year, Position P){
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

	return 0;
}

int InsertBack(const char* name, const char* last_name, int year, Position P){
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

	return 0;
}

int PrintList(Position P){
	if (P == NULL) {
		printf("\nlista je prazna\n");
		return 0;
	}
	printf("\nElementi liste\n");
	while (P != NULL) {
		printf("%s %s %d\n", P->firstName, P->lastName, P->Byear);
		P = P->Next;
	}
	return 0;
}

Position Find(const char* last_name, Position P){
	P = P->Next;
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

	return 0;
}

int FreeList(Position head) {
	
	while (head->Next != NULL) {
		Position temp = head->Next;
		head->Next = temp->Next;
		temp->Next = NULL;
		free(temp);
	}
	
	return 0;
}