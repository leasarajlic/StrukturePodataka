#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/structures.h" //definicije struktura za racun i artikal
#include "include/list_core.h" //funkcije za stvaranje sortiranih vezanih lista racuna i artikala iz datoteka, "zadatak za 2"
#include "include/list_utils.h" //dodatne funkcije nad artiklima i racunima: ona koja je bila zadana + moje dodatne funkcije

/*
za objasnjenje kako je zadatak rjesen procitati navedene header datoteke i readme u Vjezba6.
najbolje je procitati ovim redom: readme-> main.c -> structures.h -> list_core.h, list_core.c -> list_utils.h, list_utils.c
*/
int main() {
	_bill bill_head = { .date = "", .NextBill = NULL, .MyArt = NULL };
	GetAllBills("data/racuni.txt", &bill_head);
	PrintArticles(&bill_head);
	UserInput(&bill_head);
	FreeLists(&bill_head);
	return 0;
}