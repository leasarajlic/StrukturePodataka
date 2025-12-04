#pragma once
#include "structures.h"
//u ovom header file-u su funkcije za stvaranje listi, ispis i oslobadanje mem.

//stvaranje liste racuna i njihovih artikala
int GetAllBills(const char* filename, PositionB Head); //glavna funk, otvara datoteku s nazivima racuna i stvara sortiranu listu racuna
PositionB GetThisBill(const char* filename); //otvara datoteku pojedinog racuna i dohvaca podatke, vraca adresu novog racuna

//alokacija memorije i inicijalizacija
PositionB CreateBill(); 
PositionA CreateArticle(const char* product_name, int am, double price); 

//sortirani unosi
int SortedInputName(const char* product_name, int am, double price, PositionB my_bill); //unos artikala sortirano po nazivu
int SortedInputDate(PositionB B, PositionB new); //unos racuna sortirano po datumu(najstariji->najnoviji)

//ispis i oslobadanje memorije
int PrintArticles(PositionB B); //ispisuje sve artikle bez duplikata
int FreeLists(PositionB B); //oslobada sve liste(racune i njihove artikle)

//pomocne
PositionA* UniqueArticles(PositionB B, int* str_count); //stvara niz pokazivaca na artikle bez duplih(svaki artikal ima JEDAN pok.)
int IsUnique(PositionA* list, int prod_number,const char* product);//provjera postoji li vec artikal u nizu pokazivaca
int FreeUniques(PositionA** plist, int count);//resetira niz pokazivaca artikala na NULL i oslobada memoriju