/*3. Prethodnom zadatku dodati funkcije:
A. dinamički dodaje novi element iza određenog elementa,
B. dinamički dodaje novi element ispred određenog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. čita listu iz datoteke.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_LENGTH 30

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
int ReadFromDocument(Position);
int WriteToDocument(Position);

int main() {
	_person head = { .firstName =  "", .lastName = "", .Byear = 0, NULL };
	UserInput(&head);
	PrintList(head.Next);
	printf("\nbrisemo staru listu\n");
	ReadFromDocument(&head);
	printf("\nnesortirana lista ucitana iz dokuemnta\n");
	PrintList(head.Next);
	printf("\nsortirano po prezimenu\n");
	SortAll(&head);
	PrintList(head.Next);

	WriteToDocument(&head);
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
		printf("5 - Unos prije elementa\n6 - Unos poslije elementa\n");
		printf("0 - prekini unos elemenata\n");
		printf("Odabir: ");
		if (!fgets(buffer, sizeof(buffer), stdin) || sscanf(buffer, "%d", &choice) != 1) {
			printf("greska pri unosu, pokusajte opet\n");
			continue;
		}
		switch(choice){
			case 0:
				printf("kraj unosa elemenata\n");
				return 0;
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
						Find(lookup_last_name, P);
					break;
				}
				printf("uneseni krivi podatci, pokusajte ponovo\n");
				break;
			case 4:
				printf("Unesite prezime osobe koju zelite pronaci: ");
				if (GetLastName(lookup_last_name)) {
						DeleteElement(lookup_last_name, P);
					break;
				}
				printf("uneseni krivi podatci, pokusajte ponovo\n");
				break;
			case 5:
				printf("Unesite prezime osobe prije koje unosite novi element: ");
				if (!GetLastName(lookup_last_name)) {
					printf("uneseni krivi podatci, pokusajte ponovo\n");
					break;
				}
				printf("Unesite redom ime, prezime i godinu rodenja nove osobe, odvojiti razmakom: ");
				if (!GetPerson(tmp_name, tmp_last_name, &tmp_year)) {
					printf("uneseni krivi podatci, pokusajte ponovo\n");
					break;
				}
				InsertBefore(lookup_last_name, tmp_name, tmp_last_name, tmp_year, P);
				break;
			case 6:
				printf("Unesite prezime osobe prije koje unosite novi element: ");
				if (!GetLastName(lookup_last_name)) {
					printf("uneseni krivi podatci, pokusajte ponovo\n");
					break;
				}
				printf("Unesite redom ime, prezime i godinu rodenja nove osobe, odvojiti razmakom: ");
				if (!GetPerson(tmp_name, tmp_last_name, &tmp_year)) {
					printf("uneseni krivi podatci, pokusajte ponovo\n");
					break;
				}
				InsertAfter(lookup_last_name, tmp_name, tmp_last_name, tmp_year, P);
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

//funkcije poredane abecedno i po zadatcima

//zad 2. funk:

Position Find(const char* last_name, Position P) {
	P = P->Next;
	while (P != NULL && (upper_strcmp(P->lastName, last_name) != 0))
		P = P->Next;
	if (P != NULL)
		printf("\nOsoba %s %s pronadena u listi\n", P->firstName, P->lastName);
	else printf("\nOsoba s prezimenom %s ne postoji u listi\n", last_name);

	return P;
}

Position FindPrev(const char* last_name, Position P) {
	while (P->Next != NULL && (upper_strcmp(P->Next->lastName, last_name) != 0))
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
	temp->Next =NULL;
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

	strcpy(temp->firstName, name);
	strcpy(temp->lastName, last_name);
	temp->Byear = year;

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

	strcpy(temp->firstName, name);
	strcpy(temp->lastName, last_name);
	temp->Byear = year;

	temp->Next = prev->Next;
	prev->Next = temp;
	return 0;
}
int InsertBefore(const char* last_name_el , const char* name , const char* last_name, int year, Position P){
	Position prev = FindPrev(last_name_el, P);
	if (prev->Next == NULL) {
		printf("Umetanje nije moguce. Osoba %s nije u listi\n", last_name_el);
		return 0;
	}
	Position temp = (Position)malloc(sizeof(_person));
	if (temp == NULL) {
		printf("greska pri alokaciji memorije!\n");
		exit(EXIT_FAILURE);
	}
	temp->Next = NULL;

	strcpy(temp->firstName, name);
	strcpy(temp->lastName, last_name);
	temp->Byear = year;

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

		//p i q sluze za usporedbu dva susjedna elementa
		Position w = H;
		Position p = w->Next;
		Position q = p->Next;

		while (p->Next != Sorted) {
			//ako je prezime na p vece od q - zapocni zamjenu
			if (upper_strcmp(p->lastName, q->lastName) > 0) {
				//zamjeni cvorove
				p->Next = q->Next;
				q->Next = p;
				w->Next = q;
				//podesi - uvijek mora ici redom w, p, q
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
	while (P != NULL) {
		fprintf(write, "%s %s %d\n", P->firstName, P->lastName, P->Byear);
		P = P->Next;
	}
	fclose(write);
	return 0;
}
