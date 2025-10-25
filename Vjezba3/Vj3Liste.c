#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH 30


typedef struct Person* Position;

typedef struct Person {
	char firstName[MAX_LENGTH], lastName[MAX_LENGTH];
	int Byear;

	//pokazuje na iduci element
	Position Next;

}_person;

//zad 2.
int InsertFirst(const char *, const char *, int, Position);
int InsertLast(const char *, const char *, int, Position);
int PrintList(Position);
Position Find(const char *, Position);
Position FindPrev(const char *, Position);
int DeleteElement(const char *, Position);
int FreeList(Position);


//zadatak 3.
int InsertBefore(const char*, const char*, const char*, int, Position);
int InsertAfter(const char*, const char*, const char*, int, Position);
int SortAll(Position);
int WriteToDocument(Position);
int ReadFromDocument(Position);



int main() {

	//head nema podatke, sluzi samo za implementaciju liste
	_person head = { .firstName =  "", .lastName = "", .Byear = 0, NULL };

	InsertFirst("Ante", "Antic", 2000, &head);
	InsertLast("Marta", "Ivancic", 2006, &head);
	InsertBefore("Antic","Lara","Laric", 1998, &head);
	InsertAfter("Laric", "Lovre", "Hrstic", 1993, &head);
	DeleteElement("Hrstic", &head);
	Find("Hrstic", &head);

	printf("\nbrisemo staru listu\n");
	FreeList(&head);
	ReadFromDocument(&head);
	printf("\nnesortirana lista ucitana iz dokuemnta\n");
	PrintList(&head);
	printf("\nsortirano po prezimenu\n");
	SortAll(&head);
	PrintList(&head);

	WriteToDocument(&head);
	FreeList(&head);

	return 0;
}

//funkcije poredane abecedno i po zadatcima

//zad 2. funk:

Position Find(const char* last_name, Position P) {
	P = P->Next;
	while (P != NULL && (strcmp(P->lastName, last_name) != 0))
		P = P->Next;
	if (P != NULL)
		printf("\nOsoba %s %s pronadena u listi\n", P->firstName, P->lastName);
	else printf("\nOsoba s prezimenom %s ne postoji u listi\n", last_name);

	return P;
}

Position FindPrev(const char* last_name, Position P) {
	while (P->Next != NULL && (strcmp(P->Next->lastName, last_name) != 0))
		P = P->Next;

	return P;
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

int DeleteElement(const char* last_name, Position P) {

	Position Prev = FindPrev(last_name, P);
	if (Prev->Next == NULL) {
		printf("\nosoba s prezimenom %s nije u listi\n", last_name);
		return 0;
	}
	Position temp = Prev->Next;

	//element odspojiti iz liste prije brisanja
	Prev->Next = temp->Next;
	//oslobadanje memorije
	free(temp);

	return 0;
}

int InsertFirst(const char* name, const char* last_name, int year, Position P){
	Position temp;
	temp = (Position)malloc(sizeof(_person));
	if (temp == NULL) {
		printf("\ngreska pri alokaciji memorije u InsertFirst!\n");
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

int InsertLast(const char* name, const char* last_name, int year, Position P){
	Position temp;
	while (P->Next != NULL)
		P = P->Next;
	temp = (Position)malloc(sizeof(_person));
	if (temp == NULL){
		printf("\ngreska pri alokaciji memorije u InsertLast!\n");
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

	//preskacemo head - nema podataka za ispis
	P = P->Next;
	if (P == NULL) {
		printf("\nlista je prazna\n");
		return 0;
	}
	printf("\nElementi liste:\n\n");
	while (P != NULL) {
		printf("%s %s %d\n", P->firstName, P->lastName, P->Byear);
		P = P->Next;
	}
	return 0;
}


 //zad 3. funk:

int InsertAfter(const char* last_name_el, const char* name, const char* last_name, int year, Position P) {
	Position prev = Find(last_name_el, P);
	if (prev == NULL) {
		printf("Umetanje nije moguce.\n");
		return 0;
	}
	Position temp = (Position)malloc(sizeof(_person));
	if (temp == NULL) {
		printf("greska pri alokaciji memorije!\n");
		exit(EXIT_FAILURE);
	}
	temp->Next = NULL;

	//podatci
	strcpy(temp->firstName, name);
	strcpy(temp->lastName, last_name);
	temp->Byear = year;

	//spoji s listom
	temp->Next = prev->Next;
	prev->Next = temp;
	return 0;
}

int InsertBefore(const char* last_name_el , const char* name , const char* last_name, int year, Position P){
	Position prev = FindPrev(last_name_el, P);
	if (prev == NULL) {
		printf("Umetanje nije moguce. Osoba %s nije u listi\n", last_name_el);
		return 0;
	}
	Position temp = (Position)malloc(sizeof(_person));
	if (temp == NULL) {
		printf("greska pri alokaciji memorije!\n");
		exit(EXIT_FAILURE);
	}
	temp->Next = NULL;

	//podatci
	strcpy(temp->firstName, name);
	strcpy(temp->lastName, last_name);
	temp->Byear = year;

	//spoji s listom
	temp->Next = prev->Next;
	prev->Next = temp;

	return 0;
}

int SortAll(Position H){

	//Za sortiranje se koristi bubble sort

	/*Sorted incijalizirati na NULL jer zadnji el. pokazuje na NULL 
	tj. kad prvi put bubble dovede najveci element na kraj to cemo znati jer ce p->next biti Sorted == NULL*/
	Position Sorted = NULL;

	//bubble sort sve dok prvi element iza head-a nije Sorted
	while (Sorted != H->Next) {

		/*inicijaliziraj redom 
		w na head, 
		p na prvi stvarni element 
		q nakon p*/

		//p i q sluze za usporedbu dva susjedna elementa(p - prvi, q - drugi)
		Position w = H;
		Position p = w->Next;
		Position q = p->Next;

		while (p->Next != Sorted) {
			//ako je prezime na p vece od q - zapocni zamjenu
			if (strcmp(p->lastName, q->lastName) > 0) {

				//zamjeni cvorove
				p->Next = q->Next;
				q->Next = p;
				w->Next = q;

				//podesi w, p, q - uvijek mora ici redom w, p, q
				w = q;
				q = p->Next;
			}

			//ako ne treba zamjeniti cvorove, samo inkrementiraj w, p, q
			else {
				w = p;
				p = q;
				q = q->Next;
			}
		}
		//kad je p->Next Sorted, stavi sorted na p
		Sorted = p;
	}
	
	return 0;
}

int ReadFromDocument(Position P) {
	//obrisi staru listu prije ucitavanja nove
	FreeList(P);
	FILE* read;
	read = fopen("People.txt", "r");
	if (read == NULL) {
		printf("\ngreska pri otvaranju datoteke!\n");
		exit(EXIT_FAILURE);
	}
	char tmp_name[MAX_LENGTH] = "", tmp_last_name[MAX_LENGTH] = "";
	int tmp_year = 0;
	while (fscanf(read, "%s %s %d", tmp_name, tmp_last_name, &tmp_year) == 3) {
		InsertLast(tmp_name, tmp_last_name, tmp_year, P);
	}
	fclose(read);
	return 0;
}

int WriteToDocument(Position P){
	FILE* write;
	write = fopen("PeopleFromList.txt", "w");
	if (write == NULL) {
		printf("\ngreska pri otvaranju datoteke!\n");
		exit(EXIT_FAILURE);
	}
	if (P->Next == NULL) {
		fprintf(write, "Lista je prazna.\n");
		return 0;
	}
	P = P->Next;
	fprintf(write, "Osobe procitane iz liste:\n");
	while (P != NULL) {
		fprintf(write, "%s %s %d\n", P->firstName, P->lastName, P->Byear);
		P = P->Next;
	}
	fclose(write);
	return 0;

}
