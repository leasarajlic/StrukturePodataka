/*4. Napisati program za zbrajanje i mnozenje polinoma. Koeficijenti i eksponenti se
citaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nuzno sortirani.*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#define MAX_LEN 256

typedef struct poly* Position;
typedef struct poly {
	double coef, power;
	Position Next;
}_poly;

int GetPolyData(const char *, char *, int); //cita polinome iz datoteka i sprema ih u buffere. svaki polinom je u zasebnoj datoteci
int CreatePoly(char*, Position); //cita clanove polinoma iz buffera i sprema ih u listu
int SortedInput(double, double, Position); //unosi clanove u listu sortirano prema potenciji
Position CreateTerm(double, double); //stvara element liste
int MultiplyPolys(Position, Position, Position); //zbraja polinome
int AddPolys(Position, Position, Position); //mnozi polinome
int PrintPoly(Position);
int FreePoly(Position);

int main() {
	_poly head = {.coef = 0, .power = 0, .Next = NULL };
	_poly poly1 = head;
	_poly poly2 = head;
	char poly1_buffer[MAX_LEN] = "", poly2_buffer[MAX_LEN] = "";
	
	GetPolyData("poly1.txt", poly1_buffer, sizeof(poly1_buffer));
	GetPolyData("poly2.txt", poly2_buffer, sizeof(poly2_buffer));
	
	//stvara vezanu listu za poly1 i ispisuje ju na ekran
	CreatePoly(poly1_buffer, &poly1);
	printf("prvi polinom: ");
	PrintPoly(poly1.Next);
	//stvara vezanu listu za poly2 i ispisuje ju na ekran
	CreatePoly(poly2_buffer, &poly2);
	printf("drugi polinom: ");
	PrintPoly(poly2.Next);

	AddPolys(&poly1, &poly2, &head);
	FreePoly(&head); //brisemo rez. zbrajanja da bismo mogli pohraniti rezultat mnozenja
	MultiplyPolys(&poly1, &poly2, &head);

	FreePoly(&head);
	FreePoly(&poly1);
	FreePoly(&poly2);
	return 0;
}
int GetPolyData(const char *filename, char *buffer, int buffer_size) {
	FILE* readF = fopen(filename, "r");
	if (readF == NULL) {
		printf("greska, datoteka %s nije u direktoriju!\n", filename);
		exit(EXIT_FAILURE);
	}
	if (fgets(buffer, buffer_size, readF) == NULL) {
		printf("greska u citanju polinoma iz datoteke %s\n", filename);
		exit(EXIT_FAILURE);
	}
	fclose(readF);
	return EXIT_SUCCESS;
}
int CreatePoly(char* buffer, Position H) {
	char* token;
	const char* separator = " \t\n"; 
	double tmp_coef, tmp_pow;
	token = strtok(buffer, separator); //string se dijeli na tokene
	while (token != NULL) {
		tmp_coef = strtod(token, NULL);
		token = strtok(NULL, separator);
		if (token == NULL) break; //sprijecava gresku ako je broj tokena neparan
		tmp_pow = strtod(token, NULL);
		token = strtok(NULL, separator);
		SortedInput(tmp_coef, tmp_pow, H);
	}
	return EXIT_SUCCESS;
}
int SortedInput(double cf, double pow, Position P) {
	//lista sortirana po opadajucim potencijama(od vece potencije prema manjoj)
	Position current = P;
	while (current->Next != NULL && pow < current->Next->power) 
		current = current->Next;
		//ako elementi imaju istu potenciju, zbrajaju se i rezultat se sprema u trenutni cvor
		if (current->Next != NULL && current->Next->power == pow) {
			current->Next->coef += cf; //kako smo novi element pohranili zbrajanjem, nema potrebe nastaviti s funkcijom
			return EXIT_SUCCESS; 
		}
		Position temp = CreateTerm(cf, pow);
		temp->Next = current->Next;
		current->Next = temp;
	
	return EXIT_SUCCESS;
}
Position CreateTerm(double cf, double pow) {
	Position temp = (Position)malloc(sizeof(_poly));
	if (temp == NULL) {
		printf("greska pri alokaciji emorije\n");
		exit(EXIT_FAILURE);
	}
	temp->Next = NULL;
	temp->coef = cf;
	temp->power = pow;
	return temp;
}
int MultiplyPolys(Position p1, Position p2, Position main_poly){
	Position current_p1 = p1, current_p2 = p2;
	double tmp_coef, tmp_pow;
	for (current_p1 = p1->Next; current_p1 != NULL; current_p1 = current_p1->Next) {
		for (current_p2 = p2->Next; current_p2 != NULL; current_p2 = current_p2->Next) {
			tmp_coef = current_p1->coef * current_p2->coef;
			tmp_pow = current_p1->power + current_p2->power;
			SortedInput(tmp_coef, tmp_pow, main_poly);
		}
	}
	printf("pomnozeni polinomi: ");
	PrintPoly(main_poly->Next);
	return EXIT_SUCCESS;
}
int AddPolys(Position p1, Position p2, Position main_poly) {
	Position current = p2->Next;
	//2.polinom se kopira u prazni main polinom
	while (current != NULL) {
		SortedInput(current->coef, current->power, main_poly);
		current = current->Next;
	}
	//u main polinom se kopira 1. polinom
	//u SortedInput osigurano je da ce se elementi s istom potencijom zbrojiti
	current = p1->Next;
	while (current != NULL) {
		SortedInput(current->coef, current->power, main_poly);
		current = current->Next;
	}
	printf("zbrojeni polinomi: ");
	PrintPoly(main_poly->Next);
	return EXIT_SUCCESS;
}
int PrintPoly(Position P) {
	//P je adresa prvog stvarnog elementa liste
	Position H = P;
	while (H != NULL) {
		printf("%+.2lfx^%.0lf ", H->coef, H->power);
		H = H->Next;
	}
	return EXIT_SUCCESS;
}
int FreePoly(Position H) {
	Position H = P;
	while (H->Next != NULL) {
		Position temp = H->Next;
		H->Next = temp->Next;
		temp->Next = NULL;
		free(temp);
	}
	return EXIT_SUCCESS;
}