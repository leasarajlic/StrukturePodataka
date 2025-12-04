#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/structures.h" //definicije struktura za racun i artikal
#include "include/list_core.h" //funkcije za stvaranje sortiranih vezanih lista racuna i artikala iz datoteka, "zadatak za 2"
#include "include/list_fun.h" //dodatne funkcije s artiklima


int main() {
	_bill bill_head = { .date = "", .NextBill = NULL, .MyArt = NULL };
	GetAllBills("data/racuni.txt", &bill_head);
	PrintArticles(&bill_head);
	UserInput(&bill_head);
	FreeLists(&bill_head);
	return 0;
}