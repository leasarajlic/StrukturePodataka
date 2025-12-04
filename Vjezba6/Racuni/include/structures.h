#pragma once
/*imamo 2 vezane liste, jednu za racune i drugu za artikle.
svaki element u listi racuna predstavlja pojedini racun. 
lista racuna je implementirana sa headom, a artikli bez head-a.
*/

//lista artikala je unutar svakog racuna, artikli su sortirani abecedno po nazivu
typedef struct Article* PositionA;
typedef struct Article {
	char product[50]; //ime artikla
	int ammount; //kolicina koja je kupljena
	double price; //cijena po komadu
	PositionA NextArt; //pokazuje na sljedeci artikal
}_artc;

//glavna lista racuna, racuni su sortirani po datumu, svaki racun sadrzi listu svojih artikala(MyArt)
typedef struct Bill* PositionB;
typedef struct Bill {
	char date[50]; //datum izdavanja racuna
	PositionB NextBill; //pokazivac na sljedeci racun
	PositionA MyArt; //pokazivac na listu artikala ovog racuna
}_bill;
