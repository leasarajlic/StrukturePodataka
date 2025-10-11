#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

//struktura za spremanje podataka o studentima
typedef struct Student
{
	char ime[20], prez[20];
	int apsolutni_bodovi; //bodovi ostvareni na ispitu(iz datoteke)
	float relativni_bodovi; //bodovi studenta u odnosu na max broj bodova
}_stud;

//protipovi funkcija
int brojac_redova(FILE*); 
_stud* ucitaj_podatke(FILE*, int);
int relativni_bodovi(_stud*, int);
int ispis(_stud*, int);

int main()
{
	_stud* s;
	int br_redova;
	FILE* readFile;

	//otvaranje datoteke
	readFile = fopen("StudentiPrimjer.TXT", "r");
	if (readFile == NULL) {
		printf("Greska pri otvaranju datoteke!\n");
		return 1;
	}

	//racuna broj redova(studenata) u datoteci
	br_redova = brojac_redova(readFile);

	//cita podatke iz datoteke
	s = ucitaj_podatke(readFile, br_redova);

	relativni_bodovi(s, br_redova);
	ispis(s, br_redova);

	//oslobadanje memorije
	free(s);

	return 0;
}

int brojac_redova(FILE* read) {
	int n = 0;
	char c;
	while ((c = fgetc(read)) != EOF) {
		if(c == '\n')
		n++;
	}
	rewind(read);  //pokazivac se vraca na pocetak datoteke
	return n + 1; //dodaje se 1 jer zadnji red obicno nema '\n'
}

_stud* ucitaj_podatke(FILE* read, int redovi)
{
	//dinamicka alokacija memorije 
	_stud* s = (_stud*)malloc(redovi * sizeof(_stud));
	if (s == NULL) {
		printf("Greska pri alokaciji memorije!\n");
		return 0;
	}
	for (int i = 0; i < redovi; i++) {
		fscanf(read, "%s %s %d", s[i].ime, s[i].prez, &s[i].apsolutni_bodovi);

	}
	fclose(read);
	return s;
}

int relativni_bodovi(_stud* s, int redovi) 
{
	int max_bodovi = s[0].apsolutni_bodovi;

	//trazimo studenta s najvise bodova
	for (int i = 0; i < redovi; i++) {
		if (s[i].apsolutni_bodovi > max_bodovi)
			max_bodovi = s[i].apsolutni_bodovi;
	}

	//racunamo relativne bodove za sve studente
	for (int i = 0; i < redovi; i++) {
		s[i].relativni_bodovi = ((float)s[i].apsolutni_bodovi / max_bodovi) * 100;
	}
	return 0;
}

int ispis(_stud* s, int redovi)
{
	printf("Ime        Prezime      Apsolutni bodovi      Relativni bodovi(%%)\n\n");
	for (int i = 0; i < redovi; i++) {
		printf("%-10s %-20s %-15d %.2f%%\n", s[i].ime, s[i].prez, s[i].apsolutni_bodovi, s[i].relativni_bodovi);
	}
	return 0;
}