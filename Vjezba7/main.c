#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 50
//zad napravljen tako da je C: root, nema siblings
typedef struct Directory* dirPosition;
typedef struct Directory {
	char name[MAX_LEN];
	dirPosition Next;
	dirPosition subDir;
}_directory;

typedef struct Stack* stackPosition;
typedef struct Stack {
	dirPosition dirLevel;
	stackPosition Next;
}_stack;

//pomocne
int AddRoot(dirPosition, stackPosition, const char* name); //dodaje C: kao root
dirPosition CreateDir(const char*); //alokacija memorije + inicijalizacija
//funkcije za stack
int push(stackPosition, dirPosition);
int pop(stackPosition);
//DOS naredbe
int AddNewDir(stackPosition, const char*); //naredba md
int GoIntoSubdir(stackPosition, const char*); //naredba cd dir
int ReturnToParent(stackPosition); //naredba cd..
int PrintCurrentDir(dirPosition, int); //naredba dir
int UserInput(stackPosition, dirPosition); //korisnik bira koju DOS naredbu ce pozvati
//oslobadanje memorije
int FreeTree(dirPosition); //oslobadanje memorije stabla
int FreeStack(stackPosition); //oslobodanje stacka

int main() {
	_directory Dir_head = { .Next = NULL, .subDir = NULL };
	_stack Stack_head = { .dirLevel = NULL, .Next = NULL };
	AddRoot(&Dir_head, &Stack_head, "C:");
	UserInput(&Stack_head, Dir_head.Next); //saljemo head od stacka i root cvor stabla(C:)
	return 0;
}
int UserInput(stackPosition ST, dirPosition Root) {
	int choice = -1;
	char buffer[128];
	while (1) {
		char tmp_sub[MAX_LEN] = "", tmp_new[MAX_LEN] = "";
		printf("\nIzaberite DOS naredbu\n");
		printf("1 - md\n2 - cd dir \n3 - cd..\n4 - dir\n");
		printf("5 - izlaz\n");
		//ispis da korisnik(ja kad debuggiram) vidi u kojem se direktoriju nalazi
		printf("trenutni dir: %s\n", ST->Next->dirLevel->name);
		printf("Odabir: ");
		if (!fgets(buffer, sizeof(buffer), stdin) || sscanf(buffer, "%d", &choice) != 1) {
			printf("greska pri unosu, pokusajte opet\n");
			continue;
		}
		switch (choice) {
		case 1:
			printf("md ");
			if (!fgets(buffer, sizeof(buffer), stdin) || sscanf(buffer, " %49[^\n]", tmp_new) != 1)
				printf("uneseni krivi podatci, pokusajte ponovo\n");
			else {
				tmp_new[strcspn(tmp_new, "\r\n")] = 0;
				if(AddNewDir(ST, tmp_new) == EXIT_FAILURE)
					printf("stvaranje direktorija nije uspjelo!\n");
			}
			break;
		case 2:
			printf("cd ");
			if (!fgets(buffer, sizeof(buffer), stdin) || sscanf(buffer, " %49[^\n]", tmp_sub) != 1)
				printf("uneseni krivi podatci, pokusajte ponovo\n");
			else {
				tmp_sub[strcspn(tmp_sub, "\r\n")] = 0;
				GoIntoSubdir(ST, tmp_sub);
			}
			break;
		case 3:
			ReturnToParent(ST);
			break;
		case 4:
			PrintCurrentDir(ST->Next->dirLevel, 0);
			break;
		case 5:
			printf("kraj izbornika!\n");
			FreeStack(ST);
			FreeTree(Root);
			return EXIT_SUCCESS;
		default:
			printf("\nNeispravna opcija, pokusajte ponovo.\n");
		}
	}
}
dirPosition CreateDir(const char* newDir_name) {
	dirPosition newDir = (dirPosition)malloc(sizeof(_directory));
	if (!newDir) {
		printf("pogreska s alokacijom memorije za %s\n", newDir_name);
		return NULL;
	}
	strcpy(newDir->name, newDir_name);
	newDir->subDir = NULL;
	newDir->Next = NULL;
	return newDir;
}
int AddRoot(dirPosition dir_head, stackPosition ST, const char* name) {
	dirPosition newDir = CreateDir(name);
	if(newDir ==NULL){
		printf("greska pri stvaranju root direktorija %s!\n", name);
		exit(EXIT_FAILURE);
	}
	newDir->Next = dir_head->Next;
	dir_head->Next = newDir;
	if(push(ST, newDir) == EXIT_FAILURE){
		printf("greska pri inicijaliziranju stoga!\n");
		exit(EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
int push(stackPosition ST, dirPosition Dir) {
	stackPosition temp = (stackPosition)malloc(sizeof(_stack));
	if (temp == NULL) {
		printf("greska pri alokaciji memorije\n");
		return EXIT_FAILURE;
	}
	temp->Next = NULL;
	temp->dirLevel = Dir;
	temp->Next = ST->Next;
	ST->Next = temp;
	return EXIT_SUCCESS;
}
int pop(stackPosition ST){
	if (ST->Next == NULL){
		printf("greska, stog je prazan\n");
		return EXIT_FAILURE;
	}
		stackPosition temp = ST->Next;
		ST->Next = temp->Next;
		temp->Next = NULL;
		temp->dirLevel = NULL;
		free(temp);
		return EXIT_SUCCESS;
}
int AddNewDir(stackPosition ST, const char* newDir_name) {
	if (ST->Next == NULL) {
		printf("greska - nema trenutnog direktorija\n");
		return EXIT_FAILURE;
	}
	dirPosition ParentDir = ST->Next->dirLevel;
	dirPosition newDir = CreateDir(newDir_name);
	if(newDir == NULL) 
		return EXIT_FAILURE;
	newDir->Next = ParentDir->subDir;
	ParentDir->subDir = newDir;
	return EXIT_SUCCESS;
}
int GoIntoSubdir(stackPosition ST, const char* sub_name) {
	if (!ST->Next) {
		printf("greska - nema trenutnog direktorija\n");
		return EXIT_FAILURE;
	}
	dirPosition curr_dir = ST->Next->dirLevel;
	dirPosition curr_sub = curr_dir->subDir;
	//pretrazuj djecu od curr_dir dok ne nades sub_name ili ne dodes do kraja
	while (curr_sub != NULL && strcmp(curr_sub->name, sub_name) != 0) {
		curr_sub = curr_sub->Next;
	}
	//nasli smo sub_name medu djecom
	if (curr_sub != NULL) {
		if(push(ST, curr_sub) == EXIT_FAILURE) { //prebaci se u curr_sub
			printf("nije moguce staviti %s na stog\n", sub_name);
			return EXIT_FAILURE;
		}
		return EXIT_SUCCESS;
	}
	//ako smo prosli svu djecu i nismo nasli sub_name, curr_sub ce biti NULL
	else {
		printf("%s nije pronaden\n", sub_name);
		return EXIT_FAILURE;
	}
}
int ReturnToParent(stackPosition ST) {
	if (ST->Next == NULL) {
		printf("greska - nema trenutnog direktorija\n");
		return EXIT_FAILURE;
	}
	if(strcmp(ST->Next->dirLevel->name, "C:") == 0){
		printf("greska - vec ste u root direktoriju C:\n");
		return EXIT_FAILURE;
	}
	pop(ST); //ako je sve u redu, pop trenutni dir sa stacka
	return EXIT_SUCCESS;
}
int PrintCurrentDir(dirPosition current, int height) {
	if(current == NULL) return EXIT_SUCCESS;
	printf("%*s%s\n", height * 2, "", current->name); //ispis trenutnog direktorija
	if (current->subDir != NULL) {
		PrintCurrentDir(current->subDir, height + 1); //rekurzivno ispisi djecu
	}
	if (current->Next != NULL) {
		PrintCurrentDir(current->Next, height); //rekurzivno ispisi siblings
	}
	return EXIT_SUCCESS;
}
int FreeTree(dirPosition current) {
	if (current == NULL) return EXIT_SUCCESS;
	if (current->subDir != NULL) {
		FreeTree(current->subDir); //rekurzivno oslobodi djecu
	}
	if (current->Next != NULL) {
		FreeTree(current->Next); //rekurzivno oslobodi siblings
	}
	free(current); //oslobodi trenutni
	return EXIT_SUCCESS;
}
int FreeStack(stackPosition ST) {
	stackPosition temp;
	while (ST->Next != NULL) {
		temp = ST->Next;
		ST->Next = temp->Next;
		temp->Next = NULL;
		temp->dirLevel = NULL;
		free(temp);
	}
	return EXIT_SUCCESS;
}