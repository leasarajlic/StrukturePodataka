#pragma once
#include "structures.h"
//u ovom header file-u su dodatne funkcije nad racunima i artiklima

/*funkcija zadana u zadatku 6.
za odredeni artikal provjerava koliko je komada prodano i koliko je potroseno na njega u odredenom periodu*/
int InspectArticle(PositionB B, const char* article, const char* start_date, const char* end_date);

//dodatne funkcije koje nisu u zadatku:
int ArtPopByPeriod(PositionB B, const char* start_date, const char* end_date); //koji je najprodavaniji artikal u odredenom periodu
int MonthsBySpending(PositionB B); //prosjecna potrosnja po mjesecima
int MyArticleMonth(PositionB B, const char* article); //u kojem mjesecu se odredeni artikal najvise prodaje

//korisnicki izbornik
int UserInput(PositionB Bill_Head);

//pomocne fun. za zastitu od krivog korisnickog unosa
int InputArticle(char* art);
int InputPeriod(char* start, char* end);

//pomocne funkcije
int ArticleCount(PositionB B, const char* start, const char* end, const char* article); //koliko se puta artikal prodao u razdoblju
double ArticlePrice(PositionB B, const char* article); //vraca cijenu artikla

//pomocni niz za mjesece, definicija u utils.c
extern const char* Months[12];
