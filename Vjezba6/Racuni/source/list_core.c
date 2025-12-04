#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/structures.h"
#include "../include/list_core.h"
#define UNIQUE 1
#define NOT_UNIQUE 0
#define MAX_ARTCS 1000

int GetAllBills(const char* filename, PositionB Bill_Head){
	char path[128];
	snprintf(path, sizeof(path), "data/%s", filename) //put do racuni.txt
	FILE* readb = fopen(path, "r");
	if (readb == NULL) {
		printf("greska pri otvaranju datoteke %s", filename);
		exit(EXIT_FAILURE);
	}
	char BillName[50] = "";
	while (fscanf(readb, "%49s", BillName) == 1) {
		PositionB new_bill = GetThisBill(BillName);
		if (new_bill != NULL)
			SortedInputDate(Bill_Head, new_bill);
	}
	fclose(readb);
	return EXIT_SUCCESS;
}
PositionB CreateBill() {
	PositionB newBl = (PositionB)malloc(sizeof(_bill));
	if (newBl == NULL) {
		printf("greska pri alokaciji memorije!\n");
		exit(EXIT_FAILURE);
	}
	newBl->NextBill = NULL;
	newBl->MyArt = NULL;
	newBl->date[0] = '\0';
	return newBl;
}
PositionB GetThisBill(const char* filename){
	char path[128];
	snprintf(path, sizeof(path), "data/%s", filename) //put do datoteke
	FILE* tb = fopen(path, "r");
	if (tb == NULL) {
		printf("greska pri otvaranju racuna %s", path);
		exit(EXIT_FAILURE);
	}
	PositionB newB = CreateBill();
	if(fscanf(tb, "%49s", newB->date) != 1) exit(EXIT_FAILURE);
	char buffer[256] = "";
	while (fgets(buffer, sizeof(buffer), tb)) {
		char tmp_prod_name[50];
		int tmp_ammo;
		double tmp_price;
		if (sscanf(buffer, "%49[^,], %d, %lf", tmp_prod_name, &tmp_ammo, &tmp_price) == 3) {
			SortedInputName(tmp_prod_name, tmp_ammo, tmp_price, newB);
		}
	}
	fclose(tb);
	return newB;
}
int SortedInputDate(PositionB B, PositionB new){
	//B je head element liste racuna
	PositionB curr = B;
	while (curr->NextBill != NULL && strcmp(curr->NextBill->date, new->date) < 0) {
		/*strcmp moze za usporedbu samo zbog YYYY - MM - DD formata, 
		u bilo kojem drugom formatu trebalo bi parsirati na godinu, mjesec, dan*/
		curr = curr->NextBill;
	}
	new->NextBill = curr->NextBill;
	curr->NextBill = new;

	return EXIT_SUCCESS;
}
PositionA CreateArticle(const char* product_name, int am, double price) {
	PositionA newAr = (PositionA)malloc(sizeof(_artc));
	if (newAr == NULL) { 
		printf("greska pri alokaciji memorije za %s\n", product_name);
		exit(EXIT_FAILURE); }
	newAr->NextArt = NULL;
	strcpy(newAr->product, product_name);
	newAr->ammount = am;
	newAr->price = price;
	return newAr;
}
int SortedInputName(const char* product_name, int am, double price, PositionB my_bill){
	PositionA current = my_bill->MyArt;
	PositionA newArt = CreateArticle(product_name, am, price);
	if (current == NULL || strcmp(current->product, product_name) > 0) {
		newArt->NextArt = current;
		my_bill->MyArt = newArt;
	}
	else {
		while (current->NextArt != NULL && strcmp(current->NextArt->product, product_name) < 0) {
			current = current->NextArt;
		}
		newArt->NextArt = current->NextArt;
		current->NextArt = newArt;
	}
	return EXIT_SUCCESS;
}
int PrintArticles(PositionB B){
	int count;
	int i = 0;
	PositionA* articles = UniqueArticles(B, &count);
	printf("procitani artikli:\n");
	for (i = 0; i < count; i++) {
		printf("%s ", articles[i]->product);
	}
	FreeUniques(&articles, count);
	return EXIT_SUCCESS;
}
int FreeLists(PositionB B) {
	PositionB curr_bill = B->NextBill, temp_bill;
	PositionA curr_art = NULL, temp_art;
	while (B->NextBill != NULL) {
		temp_bill = curr_bill;
		curr_bill = curr_bill->NextBill;
		curr_art = temp_bill->MyArt;
		//oslobodi listu artikala za ovaj racun
		while (curr_art != NULL) {
			temp_art = curr_art;
			curr_art = curr_art->NextArt;
			temp_art->NextArt = NULL;
			free(temp_art);
		}
		//odspoji racun iz liste pa ga oslobodi
		B->NextBill = curr_bill;
		temp_bill->MyArt = NULL;
		temp_bill->NextBill = NULL;
		free(temp_bill);
	}
	return EXIT_SUCCESS;
}
PositionA* UniqueArticles(PositionB B, int* str_count) {
	int i = 0;
	PositionB curr_bill = B->NextBill;
	PositionA* unique = (PositionA*)calloc(MAX_ARTCS, sizeof(PositionA)); //calloc sve pokazivace inicijalizira na NULL
	if (unique == NULL) {
		*str_count = 0;
		exit(EXIT_FAILURE);
	}
	*str_count = 0;
	while (curr_bill != NULL && *str_count < MAX_ARTCS) {
		PositionA curr_art = curr_bill->MyArt;
		while (curr_art != NULL && *str_count < MAX_ARTCS) {
			if (IsUnique(unique, *str_count, curr_art->product)) {
				unique[(*str_count)++] = curr_art;
			}
			curr_art = curr_art->NextArt;
		}
		curr_bill = curr_bill->NextBill;
	}
	return unique;
}
int IsUnique(PositionA* list, int prod_number, const char* product) {
	int i = 0;
	for (i = 0; i < prod_number; i++) {
		if (strcmp(list[i]->product, product) == 0)
			return NOT_UNIQUE;
	}
	return UNIQUE;
}
int FreeUniques(PositionA** plist, int count) {
	int i = 0;
	PositionA* list = *plist;
	for (i = 0; i < count; i++) {
		list[i] = NULL;
	}
	free(list);
	*plist = NULL;
	return EXIT_SUCCESS;
}