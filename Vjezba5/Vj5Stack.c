/*5. Napisati program koji iz datoteke cita postfiks izraz i zatim koristenjem stoga racuna
rezultat. Stog je potrebno realizirati preko vezane liste.*/
/*
!!Program je napravljen tako da je u postfiksu svaki clan odvojen razmakom 
kako bi program mogao tocno citati viseznamenkaste brojeve.
npr. kad bi pisalo samo 341+ bez razmaka
nema nacina da znamo radi li se o infiksu 3 + 41 ili 34 + 1 ili mozda krivo napisanom 3 + 4 + 1 i sl.
Ako u datoteci ne bude razmak izmedu svakog clana ili izraz nije u jednom redu program nece raditi.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_LEN 256
#define MAX_DIG 15

typedef struct postfix* Position;
typedef struct postfix {
	double number;
	Position Next;
}_postfix;

int pop(Position, char);
int push(double, Position);
int GetPostfix(char *, int);
int CalculatePostfix(Position P);
double Operation(double, char, double);

int main() {
	_postfix head = { .number = 0, .Next = NULL };
	CalculatePostfix(&head);
	return 0;
}
int CalculatePostfix(Position H) {
	char post_buffer[MAX_LEN] = "";
	char num_buffer[MAX_DIG] = ""; //omogucuje pohranu viseznamenkastih brojeva
	int i = 0, j = 0, k = 0;
	//cita postfiks iz datoteke
	GetPostfix(post_buffer, sizeof(post_buffer));
	printf("iz datoteke je procitano: %s\n", post_buffer);
	//postfiks ne smije zavrsavati sa brojem
	if (isdigit(post_buffer[strlen(post_buffer) - 1])) {
		printf("krivo napisan postfix u datoteci!\n");
		exit(EXIT_FAILURE);
	}
	for (i = 0; post_buffer[i] != '\0'; i++) {
		char c = post_buffer[i];
		double num = 0;
		//ako je procitana znamenka dodaje se na num_buffer sve dok se ne procita razmak tj. kraj broja
		if (isdigit(c)) {
			num_buffer[j] = c;
			j++;
		}
		else if (isspace(c)) {
			//dosli smo do razmaka i u num_bufferu ima znamenki => procitali smo broj i treba ga pushat na stack
			if (strlen(num_buffer) != 0) {
				if (sscanf(num_buffer, "%lf", &num) != 1) {
					printf("greska u citanju buffera za brojeve\n");
					exit(EXIT_FAILURE);
				}
				push(num, H);

				/* stare znamenke mogu ostati u memoriji iza prvog '\0'. Npr u num_buffer = {'\0', '3'} 
				'3' visi u nizu i moze pokvarit kasnije unose, a to ne vide ni printf ni strlen jer staju na prvom '\0'.
				zato se num_buffer cisti rucno */
				for (k = 0; k < sizeof(num_buffer); k++)
					num_buffer[k] = '\0';

				//resetiraj inkrement za num_buffer
				j = 0;
			}
			else continue;
		}
		//ako c nije ni whitespace ni broj, onda mora biti operator
		else {
			pop(H, c);
		}
	}
	if (H->Next->Next != NULL) {
		printf("Ostalo je viska elemenata na stacku!\n");
		exit(EXIT_FAILURE);
	}
	printf("Uspjesno izracunat postfix izraz: %s\n", post_buffer);
	printf("Rezultat je: %lf\n", H->Next->number);
	return EXIT_SUCCESS;
}
int GetPostfix(char* post, int buff_size) {
	FILE* readFile = fopen("Postfix.txt", "r");
	if (readFile == NULL) {
		printf("greska! datoteka nije u direktoriju.\n");
		exit(EXIT_FAILURE);
	}
	if (fgets(post, buff_size, readFile) == 0) {
		printf("greska u citanju postfixa iz datoteke.\n");
		exit(EXIT_FAILURE);
	}
	fclose(readFile);
	return EXIT_SUCCESS;
}
int push(double NewEl, Position H) {
	Position temp = (Position)malloc(sizeof(_postfix));
	if (temp == NULL) {
		printf("greska pri alokaciji memorije\n");
		exit(EXIT_FAILURE);
	}
	temp->Next = NULL;
	temp->number = NewEl;
	temp->Next = H->Next;
	H->Next = temp;
	return EXIT_SUCCESS;
}
int pop(Position H, char op) {
	if (H->Next == NULL || H->Next->Next == NULL) {
		printf("Na stacku nema dovoljno operanada za %c. Popravite izraz i ponovo pokrenite program\n", op);
		exit(EXIT_FAILURE);
	}
	Position First = H->Next;
	Position Second = First->Next;
	//rezultat operacije se pohranjuje u First, a Second se brise
	First->number = Operation(Second->number, op, First->number);
	First->Next = Second->Next;
	Second->Next = NULL;
	free(Second);
	return EXIT_SUCCESS;
}
double Operation(double x, char op, double y) {
	switch(op){
		case '+':
			return x + y;
		case '-':
			return x - y;
		case '*':
			return x * y;
		case '/':
			if (y == 0) {
				printf("Greska u %lf/%lf : Dijeljenje s 0 nije dopusteno!\n", x, y);
				exit(EXIT_FAILURE);
			}
			return x / y;
		default:
			printf("Nepoznati operator %c u postfixu.\n", op);
			exit(EXIT_FAILURE);
		}
}