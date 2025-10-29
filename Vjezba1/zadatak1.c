#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

/*1. Napisati program koji prvo pročita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamički alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i učitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova/max_br_bodova*100*/

//struktura za spremanje podataka o studentima
typedef struct Student
{
	char name[20], last_name[20];
	int apsolute_points; //bodovi ostvareni na ispitu(iz datoteke)
	float relative_points; //bodovi studenta u odnosu na max broj bodova
}_stud;

//protipovi funkcija
int GetLines(); 
_stud* GetFileData(int);
int GetRelativePts(_stud*, int);
int Print(_stud*, int);

int main()
{
	_stud* s;
	int br_redova;
	

	//racuna broj redova(studenata) u datoteci
	br_redova = GetLines();

	//cita podatke iz datoteke
	s = GetFileData(br_redova);

	GetRelativePts(s, br_redova);
	Print(s, br_redova);

	//oslobadanje memorije
	free(s);

	return 0;
}

int GetLines() {
	FILE* readFile;
	//otvaranje datoteke
	readFile = fopen("StudentiPrimjer.TXT", "r");
	if (readFile == NULL) {
		printf("Greska pri otvaranju datoteke!\n");
		exit(EXIT_FAILURE);
	}
	int n = 0;
	char c;
	while ((c = fgetc(readFile)) != EOF) {
		if(c == '\n')
		n++;
	}
	rewind(readFile);  //pokazivac se vraca na pocetak datoteke
	return n + 1; //dodaje se 1 jer zadnji red obicno nema '\n'
}

_stud* GetFileData(int lines){
	FILE* readFile;

	//otvaranje datoteke
	readFile = fopen("StudentiPrimjer.TXT", "r");
	if (readFile == NULL) {
		printf("Greska pri otvaranju datoteke!\n");
		exit(EXIT_FAILURE);
	}
	//dinamicka alokacija memorije 
	_stud* s = (_stud*)malloc(lines * sizeof(_stud));
	if (s == NULL) {
		printf("Greska pri alokaciji memorije!\n");
		return 0;
	}
	for (int i = 0; i < lines; i++) {
		fscanf(readFile, "%s %s %d", s[i].name, s[i].last_name, &s[i].apsolute_points);

	}
	fclose(readFile);
	return s;
}

int GetRelativePts(_stud* s, int lines)
{
	int max_bodovi = s[0].apsolute_points;

	//trazimo studenta s najvise bodova
	for (int i = 0; i < lines; i++) {
		if (s[i].apsolute_points > max_bodovi)
			max_bodovi = s[i].apsolute_points;
	}

	//racunamo relativne bodove za sve studente
	for (int i = 0; i < lines; i++) {
		s[i].relative_points = ((float)s[i].apsolute_points / max_bodovi) * 100;
	}
	return 0;
}

int Print(_stud* s, int redovi)
{
	printf("Ime        Prezime      Apsolutni bodovi      Relativni bodovi(%%)\n\n");
	for (int i = 0; i < redovi; i++) {
		printf("%-10s %-20s %-15d %.2f%%\n", s[i].name, s[i].last_name, s[i].apsolute_points, s[i].relative_points);
	}
	return 0;
}
