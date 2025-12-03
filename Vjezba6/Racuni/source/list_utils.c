#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/structures.h"
#include "../include/list_core.h"
#include "../include/list_utils.h"
#define MAX_LENGTH 50

const char* Months[12] = { "Sijecanj", "Veljaca", "Ozujak", "Travanj", "Svibanj", "Lipanj", "Srpanj",
"Kolovoz", "Rujan", "Listopad", "Studeni", "Prosinac" };

int UserInput(PositionB Bill_Head) {
	int choice = -1;
	char buffer[128];
	while (1) {
		char tmp_art[MAX_LENGTH] = "", tmp_start[MAX_LENGTH] = "", tmp_end[MAX_LENGTH] = "";
		printf("\nIzaberite akciju nad racunima i artiklima:\n");
		//fun. iz zadatka
		printf("1 - Upit: Koliko je puta odredeni artikal prodan i koliko je na njega potroseno unutar odredenog razdoblja\n"); //funk. iz zadatka
		//dodatne fun.
		printf("2 - Najapopularnji artikal u odredenom razdoblju\n3 - Ukupna potrosnja po mjesecima\n");
		printf("4 - U kojem mjesecu se odredeni artikal najvise kupuje\n");
		//izlaz iz ove fun.
		printf("0 - izlaz iz izbornika\n");
		printf("Odabir: ");
		if (!fgets(buffer, sizeof(buffer), stdin) || sscanf(buffer, "%d", &choice) != 1) {
			printf("greska pri unosu, pokusajte opet\n");
			continue;
		}
		switch (choice) {
		case 0:
			printf("kraj izbornika!\n");
			return EXIT_SUCCESS;
		case 1:
			if (!InputArticle(tmp_art) && !InputPeriod(tmp_start, tmp_end)) {
				InspectArticle(Bill_Head, tmp_art, tmp_start, tmp_end);
				break;
			}
			printf("uneseni krivi podatci, pokusajte ponovo\n");
			break;
		case 2:
			if (!InputPeriod(tmp_start, tmp_end)) {
				ArtPopByPeriod(Bill_Head, tmp_start, tmp_end);
				break;
			}
			printf("uneseni krivi podatci, pokusajte ponovo\n");
			break;
		case 3:
			MonthsBySpending(Bill_Head);
			break;
		case 4:
			if (!InputArticle(tmp_art)) {
				MyArticleMonth(Bill_Head, tmp_art);
				break;
			}
			printf("uneseni krivi podatci, pokusajte ponovo\n");
			break;
		default:
			printf("\nNeispravna opcija, pokusajte ponovo.\n");
		}
	}
}
int InputArticle(char* art) {
	char buffer[128];
	printf("\nunesite artikal: ");
	if (!fgets(buffer, sizeof(buffer), stdin) || sscanf(buffer, "%49s", art) != 1) return EXIT_FAILURE;
	art[strcspn(art, "\r\n")] = 0; //ukloni newline
	return EXIT_SUCCESS; //vraca true ako je sscnaf uspjesan, a false ako nije
}
int InputPeriod(char* start, char* end) {
	char buffer[128];
	printf("\nunesite pocetni datum u YYYY-MM-DD formatu: ");
	if (!fgets(buffer, sizeof(buffer), stdin) || sscanf(buffer, "%49s", start) != 1) return EXIT_FAILURE;
	start[strcspn(start, "\r\n")] = 0; //ukloni newline

	printf("\nunesite zavrsni datum u YYYY-MM-DD formatu: ");
	if (!fgets(buffer, sizeof(buffer), stdin) || sscanf(buffer, "%49s", end) != 1) return EXIT_FAILURE;
	end[strcspn(end, "\r\n")] = 0; //ukloni newline

	if (strcmp(start, end) > 0) return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
int InspectArticle(PositionB B, const char* article, const char* start_date, const char* end_date) {
	int count = ArticleCount(B, start_date, end_date, article);
	double price = ArticlePrice(B, article);
	double total = price * count;
	printf("Artikal %s je u periodu od %s do %s:\n", article, start_date, end_date);
	printf("prodan %d puta i na njega je ukupno potroseno %.2lf eura\n", count, total);
	return EXIT_SUCCESS;
}
int ArticleCount(PositionB B, const char* start, const char* end, const char* article){
	int count = 0;
	PositionB curr_bill = B->NextBill;
	while (curr_bill != NULL && strcmp(curr_bill->date, start) < 0) {
		curr_bill = curr_bill->NextBill;
	}
	while (curr_bill != NULL && strcmp(curr_bill->date, end) <= 0) {
		PositionA curr_art = curr_bill->MyArt;
		while (curr_art != NULL) {
			if (strcmp(curr_art->product, article) == 0)
				count += curr_art->ammount;
			curr_art = curr_art->NextArt;
		}
		curr_bill = curr_bill->NextBill;
	}
	return count;
}
double ArticlePrice(PositionB B, const char* article) {
	double price = 0;
	PositionB curr_bill = B->NextBill;
	while (curr_bill != NULL) {
		PositionA curr_art = curr_bill->MyArt;
		while (curr_art != NULL) {
			if (strcmp(curr_art->product, article) == 0) {
				price = curr_art->price;
				return price;
			}
			curr_art = curr_art->NextArt;
		}
		curr_bill = curr_bill->NextBill;
	}
	return price;
}
int ArtPopByPeriod(PositionB B, const char* start_date, const char* end_date) {
	int art_count;
	PositionA* articles = UniqueArticles(B, &art_count);
	int i, max_count = ArticleCount(B, start_date, end_date, articles[0]->product), this_count;
	PositionA max_art = articles[0];
	for (i = 1; i < art_count; i++) {
		this_count = ArticleCount(B, start_date, end_date, articles[i]->product);
		if (this_count > max_count) {
			max_count = this_count;
			max_art = articles[i];
		}
	}
	if (max_count == 0)
		printf("u ovom periodu nije kupljen nijedan artikal iz baze racuna\n");
	else {
		printf("najpopularniji artikal od %s do %s je: %s\n", start_date, end_date, max_art->product);
		printf("prodan je %d puta\n", max_count);
	}
	FreeUniques(&articles, art_count);
	return EXIT_SUCCESS;
}
int MonthsBySpending(PositionB B) {
	int i = 0, j = 0, year, month;
	double month_expenses[12] = { 0 }, avg;
	int month_bill_count[12] = { 0 };
	PositionB curr_bill = B->NextBill;
	while (curr_bill != NULL) {
		(void)sscanf(curr_bill->date, "%d-%d", &year, &month);
		PositionA art = curr_bill->MyArt;
		while (art != NULL) {
			month_expenses[month - 1] += art->ammount *art->price;
			month_bill_count[month - 1]++;
			art = art->NextArt;
		}
		curr_bill = curr_bill->NextBill;
	}
	printf("potrosnja po mjesecima:\n");
	for (i = 0; i < 12; i++) {
		if (month_bill_count[i] > 0) {
			avg = month_expenses[i] / month_bill_count[i];
			printf("%s: %.2lf eur\n", Months[i], avg);
		}
	}
	return EXIT_SUCCESS;
}
int MyArticleMonth(PositionB B, const char* article) {
	int i = 0, j = 0, year, month, max_ammount, max_ind;
	int month_appear[12] = { 0 };
	PositionB curr_bill = B->NextBill;
	while (curr_bill != NULL) {
		(void)sscanf(curr_bill->date, "%d-%d", &year, &month);
		PositionA art = curr_bill->MyArt;
		while (art != NULL) {
			if (strcmp(art->product, article) == 0) {
				month_appear[month - 1] += art->ammount;
				break;
			}
			art = art->NextArt;
		}
		curr_bill = curr_bill->NextBill;
	}
	max_ammount = month_appear[0];
	max_ind = 0;
	for (i = 0; i < 12; i++) {
		if (month_appear[i] > max_ammount) {
			max_ammount = month_appear[i];
			max_ind = i;
		}
	}
	printf("artikal %s se najvise prodaje u mjesecu %s (ukupno %d puta)\n", article, Months[max_ind], max_ammount);
	return EXIT_SUCCESS;
}
