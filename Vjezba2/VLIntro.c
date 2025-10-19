#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 30

//pokazivac na strukturu Person
typedef struct Person* Position;

typedef struct Person {
	char firstName[MAX_LENGTH], lastName[MAX_LENGTH];
	int Byear;

	//pokazuje na iduci element
	Position Next;

}_person;

//prototipovi
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

	//dodavanje elemenata u listu
	InsertFront("Ante", "Antic", 2000, &head);
	InsertBack("Dzuro", "Orvelic", 1984, &head);
	InsertBack("Lara","Laric", 1998, &head);
	PrintList(&head);

	//brisanje elementa
	DeleteElement("Antic", &head);
	PrintList(&head);

	//trazenje osobe koja je/nije u listi
	Find("Laric", &head);
	Find("Horvat", &head);

	//na kraju treba osloboditi alociranu memoriju
	FreeList(&head);

	return 0;
}

int InsertFront(const char* name, const char* last_name, int year, Position P){
	Position temp;
	temp = (Position)malloc(sizeof(_person));
	if (temp == NULL) {
		printf("\ngreska pri alokaciji memorije u InsertFront!\}");
		exit(EXIT_FAILURE);
	}
	temp->Next = NULL;

	//unos podataka
	strcpy(temp->firstName, name);
	strcpy(temp->lastName, last_name);
	temp->Byear = year;

	//umetanje iza head-a
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
		printf("\ngreska pri alokaciji memorije u InsertBack!\}");
		exit(EXIT_FAILURE);
	}
	temp->Next = NULL;

	//unos podataka
	strcpy(temp->firstName, name);
	strcpy(temp->lastName, last_name);
	temp->Byear = year;

	//umetanje na kraj
	P->Next = temp;

	return 0;
}

int PrintList(Position P){

	//preskacemo head jer u njemu nema podataka za ispis
	P = P->Next;
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
	while (P != NULL && (strcmp(P->lastName, last_name) != 0))
		P = P->Next;
	if (P != NULL)
		printf("\nOsoba %s %s pronadena u listi\n", P->firstName, P->lastName);
	else printf("\nOsoba s prezimenom %s ne postoji u listi\n", last_name);

	return P;
}

Position FindPrev(const char* last_name, Position P){
	while (P->Next != NULL && (strcmp(P->Next->lastName, last_name) != 0))
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

	//element odspojiti iz liste prije brisanja(nista ne pokazuje na njega)
	Prev->Next = temp->Next;
	//oslobadanje memorije
	free(temp);

	return 0;
}

int FreeList(Position head) {

	//koristimo current za setanje po listi
	//preskacemo head jer nije dinamicki alociran
	Position current = head->Next;

	while (current != NULL) {
		//temp je element za brisanje
		Position temp = current;

		//pomicemo se na iduci element
		current = current->Next;

		//oslobadamo memoriju od temp
		free(temp);
	}

	//na kraju head ne pokazuje na nista
	head->Next = NULL;
	return 0;
}