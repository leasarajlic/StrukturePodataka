#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 50
#define USER_TABLE_SIZE 11
#define MOVIE_TABLE_SIZE 11
#define MOVIE_INFO 3
#define REVIEW_INFO 3
#define SAME_GENRE_BONUS 2
#define MAX_CANDIDATES 10
#define GOOD_RATING 7
//kolizije u hash tablicama rjesene su pomocu zasebnih redova

typedef enum {
	EXIT, // == 0 ako se ne naglasi drugacije
	LOGIN_MENU, // == 1
	MAIN_MENU // == 2
}State;

typedef struct Review* RevPos;
typedef struct User* UserPos;
typedef struct Movie* MoviePos;

typedef struct Review {
	char user[MAX_LEN];
	char movie[MAX_LEN];
	int rating; //od 1 do 10
	RevPos nextInUser;

	RevPos nextInMovie;
}_rev;

typedef struct User {
	char username[MAX_LEN];
	RevPos myReviews; //lista userovih reviewa
	UserPos Next; //lanac u hash bucketu
}_user;

typedef struct Movie {
	char Title[MAX_LEN], Genre[MAX_LEN];
	int ReleaseYear;
	float avgRating;
	int reviewCount;
	RevPos myReviews; //lista reviewa za ovaj film
	MoviePos Next; //lanac u hash bucketu
}_movie;

typedef struct UserHashTable* TablePosU;
typedef struct UserHashTable {
	int size;
	UserPos* buckets; //tablica, niz pokazivaca na User
}_HashTab_U;

typedef struct MovieHashTable* TablePosM;
typedef struct MovieHashTable {
	int size;
	MoviePos* buckets; //tablica, niz pokazivaca na Movie
}_HashTab_M;

int LoadMovies(const char*, TablePosM);
int LoadUsers(const char*, TablePosU);
int LoadUserReviews(const char*, TablePosU, TablePosM);
int SaveMovies(const char*, TablePosM);
int SaveUsers(const char*, TablePosU);
int SaveUserReviews(const char*, TablePosU);
int freeUserTable(TablePosU, TablePosM);
int freeMovieTable(TablePosM);

unsigned int Myhash(const char*, int);
TablePosU initUserTable(int);
TablePosM initMovieTable(int);

UserPos findUser(TablePosU, const char*);
MoviePos findMovie(TablePosM, const char*);
RevPos HasRated(UserPos, const char*);
int PrintMovie(TablePosM, const char*);
int PrintMyReviews(TablePosU, UserPos);

UserPos addUser(TablePosU, const char*);
MoviePos addMovie(TablePosM, const char*, const char*, int);
RevPos addReview(TablePosU, TablePosM, const char*, const char*, int);
int DeleteReview(TablePosM, UserPos, const char*);
int DeleteUser(TablePosU, TablePosM, const char*);
int UpdateMovieRating(MoviePos, int, int);
int MovieRecommender(TablePosU, TablePosM, UserPos);

State MainMenu(TablePosU, TablePosM, UserPos);
State LoginMenu(TablePosU, UserPos*);
int GetLine(const char*, char*, int);
int readInt(char*, int, int);
int SaveAll(const char*, const char*, const char*, TablePosU, TablePosM);
int LoadAll(const char*, const char*, const char*, TablePosU, TablePosM);
int FreeAll(TablePosU, TablePosM);

int main() {
	TablePosU Users = initUserTable(USER_TABLE_SIZE);
	TablePosM Movies = initMovieTable(MOVIE_TABLE_SIZE);
	LoadAll("Movies.txt", "Users.txt", "Reviews.txt", Users, Movies);
	UserPos current_user = NULL;
	State app_state = LOGIN_MENU;
	while (app_state != EXIT) {
		if (app_state == LOGIN_MENU)
			app_state = LoginMenu(Users, &current_user);
		else if (app_state == MAIN_MENU)
			app_state = MainMenu(Users, Movies, current_user);
	}
	SaveAll("Movies.txt", "Users.txt", "Reviews.txt", Users, Movies);
	FreeAll(Users, Movies);
	return 0;
}

State LoginMenu(TablePosU users, UserPos* current_user) {
	int choice, running = 1;
	char line[50];
	while(running){
		printf("\n1 - Log In");
		printf("\n2 - Sign Up");
		printf("\n0 - Exit\n");
		choice = readInt("Choice: ", 0, 2);
		switch (choice) {
		case 1:
			GetLine("Enter your username: ", line, MAX_LEN);
			*current_user = findUser(users, line);
			if (*current_user != NULL) {
				printf("Welcome, %s!\n", (*current_user)->username);
				return MAIN_MENU;
			}
			printf("User not found.\n");
			break;
		case 2:
			GetLine("Create your account username: ", line, MAX_LEN);
			*current_user = findUser(users, line);
			if (*current_user == NULL) {
				*current_user = addUser(users, line);
				printf("Welcome, %s!\n", (*current_user)->username);
				return MAIN_MENU;
			}
			printf("A user with this username already exists.Try a different username.\n");
			break;
		case 0:
			printf("Goodbye!\n");
			return EXIT;
			default:
				printf("Wrong input.\n");
				break;
		}
	}
	return EXIT;
}
State MainMenu(TablePosU users, TablePosM movies, UserPos current_user) {
	int choice, rating, running = 1;
	char line[50];
	MoviePos target_movie = NULL;
	while(running){
		printf("\nYou are currently logged in as: %s\n", current_user->username);
		printf("\n1 - Leave a review\n");
		printf("2 - Delete a review\n");
		printf("3 - Search movies\n");
		printf("4 - My reviews\n");
		printf("5 - Delete my account\n");
		printf("6 - Movie recommendations\n");
		printf("0 - Log out\n");
		choice = readInt("Choice: ", 0, 6);
		switch (choice) {
		case 1:
			GetLine("Enter movie title: ", line, MAX_LEN);
			if (findMovie(movies, line) != NULL && HasRated(current_user, line) == NULL) {
				rating = readInt("Rate this movie: ", 1, 10);
				addReview(users, movies, current_user->username, line, rating);
			}
			else
				printf("You have already rated this movie!\n");
			break;
		case 2:
			GetLine("Enter movie title: ", line, MAX_LEN);
			if (findMovie(movies, line) != NULL && HasRated(current_user, line) != NULL) {
				DeleteReview(movies, current_user, line);
			}
			else
				printf("You have not rated this movie yet!\n");
			break;
		case 3:
			GetLine("Enter movie title: ", line, MAX_LEN);
			PrintMovie(movies, line);
			break;
		case 4:
			PrintMyReviews(users, current_user);
			break;
		case 5:
			if (readInt("Are you sure you want to delete your account?\n1 - Yes, delete\n0 - No, Cancel", 0, 1) == 1) {
				DeleteUser(users, movies, current_user->username);
				current_user = NULL;
				printf("Profile deleted successfully.\n");
				running = 0;
				break;
			}
			break;
		case 6:
			MovieRecommender(users, movies, current_user);
			break;
		case 0:
			current_user = NULL;
			running = 0;
			break;
		default:
			printf("Wrong input, please try again\n");
			break;
		}
	}
	return LOGIN_MENU;
}
int readInt(char* prompt, int min, int max) {
	char buffer[MAX_LEN];
	char* end;
	long value;
	while (1) {
		printf("%s\n", prompt);
		if (!fgets(buffer, sizeof(buffer), stdin))
			continue;
		value = strtol(buffer, &end, 10);
		if (end == buffer || *end != '\n') {
			printf("Wrong input, try again.\n");
			continue;
		}
		if (value < min || value > max) {
			printf("This value has to be between %d and %d, try again.\n", min, max);
			continue;
		}
		return value;
	}
}
int GetLine(const char* prompt, char* buffer, int size) {
	while (1) {
		printf("%s", prompt);
		fflush(stdout);
		if (fgets(buffer, size, stdin) == NULL) {
			printf("Wrong input, please try again\n");
			continue;
		}
		buffer[strcspn(buffer, "\n")] = '\0';
		size_t len = strlen(buffer); //vs je glup

		if (len == 0 || strchr(buffer, '|') != NULL) {
			printf("Character '|' is not allowed.\n");
			continue;
		}
		return EXIT_SUCCESS;
	}
}
int LoadAll(const char* Mfilename, const char* Ufilename, const char* Rfilename, TablePosU users, TablePosM movies) {
	LoadMovies("Movies.txt", movies);
	LoadUsers("Users.txt", users);
	LoadUserReviews("Reviews.txt", users, movies);
	return EXIT_SUCCESS;
}
int SaveAll(const char* Mfilename, const char* Ufilename, const char* Rfilename, TablePosU users, TablePosM movies) {
	SaveMovies("Movies.txt", movies);
	SaveUsers("Users.txt", users);
	SaveUserReviews("Reviews.txt", users);
	return EXIT_SUCCESS;
}
int LoadMovies(const char* filename, TablePosM table) {
	FILE* readm = fopen(filename, "r");
	if (readm == NULL) {
		printf("error! file %s not found\n", filename);
		exit(EXIT_FAILURE);
	}
	char* info[MOVIE_INFO];
	char Line[MAX_LEN], Title[MAX_LEN], Genre[MAX_LEN];
	int Year;
	while (fgets(Line, sizeof(Line), readm)) {
		Line[strcspn(Line, "\n")] = '\0';
		int count = 0;
		char* token = strtok(Line, "|");
		while (token && count < MOVIE_INFO) {
			info[count++] = token;
			token = strtok(NULL, "|");
		}
		if (count == MOVIE_INFO) {
			strcpy(Title, info[0]);
			strcpy(Genre, info[1]);
			Year = atoi(info[2]);
			addMovie(table, Title, Genre, Year);
		}
	}
	fclose(readm);
	return EXIT_SUCCESS;
}
int LoadUsers(const char* filename, TablePosU table){
	FILE* read = fopen(filename, "r");
	if (read == NULL) {
		printf("error! file %s not found\n", filename);
		exit(EXIT_FAILURE);
	}
	char Username[MAX_LEN];
	while (fgets(Username, sizeof(Username), read)) {
		Username[strcspn(Username, "\n")] = '\0';
		addUser(table, Username);
	}
	fclose(read);
	return EXIT_SUCCESS;
}
int LoadUserReviews(const char* filename, TablePosU user_table, TablePosM movie_table) {
	FILE* readr = fopen(filename, "r");
	if (readr == NULL) {
		printf("error! file %s not found\n", filename);
		exit(EXIT_FAILURE);
	}
	char* info[REVIEW_INFO];
	char Line[MAX_LEN], Title[MAX_LEN], Username[MAX_LEN];
	int rating, year;
	while (fgets(Line, sizeof(Line), readr)) {
		Line[strcspn(Line, "\n")] = '\0';
		int count = 0;
		char* token = strtok(Line, "|");
		while (token && count < REVIEW_INFO) {
			info[count++] = token;
			token = strtok(NULL, "|");
		}
		if (count == REVIEW_INFO && findMovie(movie_table, info[1]) != NULL && findUser(user_table, info[0]) != NULL) {
			strcpy(Username, info[0]);
			strcpy(Title, info[1]);
			rating = atoi(info[2]);
			addReview(user_table, movie_table, Username, Title, rating); //dodaj procitani review
		}
	}
	fclose(readr);
	return EXIT_SUCCESS;
}
//djb2 hash
unsigned int Myhash(const char* key, int TABLE_SIZE) {
	unsigned int hash = 5381; //koristi se u djb2
	int c;
	while ((c = *key++)) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash % TABLE_SIZE;
}
TablePosU initUserTable(int size) {
	TablePosU ht = (TablePosU)malloc(sizeof(_HashTab_U));
	if (ht == NULL)
		return NULL;
	ht->size = size;
	ht->buckets = (UserPos*)malloc(size * sizeof(UserPos));
	if (ht->buckets == NULL) {
		free(ht);
		return NULL;
	}
	for (int i = 0; i < size; ++i)
		ht->buckets[i] = NULL;
	return ht;
}
TablePosM initMovieTable(int size) {
	TablePosM ht = (TablePosM)malloc(sizeof(_HashTab_M));
	if (!ht) 
		return NULL;
	ht->size = size;
	ht->buckets = (MoviePos*)malloc(size * sizeof(MoviePos));
	if (ht->buckets == NULL) {
		free(ht);
		return NULL;
	}
	for (int i = 0; i < size; ++i)
		ht->buckets[i] = NULL;
	return ht;
}
UserPos findUser(TablePosU uTable, const char* username) {
	unsigned int ind = Myhash(username, USER_TABLE_SIZE);
	UserPos cur = uTable->buckets[ind];
	while (cur) {
		if (strcmp(cur->username, username) == 0)
			return cur;
		cur = cur->Next;
	}
	return NULL;
}
MoviePos findMovie(TablePosM mTable, const char* name) {
	unsigned int ind = Myhash(name, MOVIE_TABLE_SIZE);
	MoviePos cur = mTable->buckets[ind];
	while (cur) {
		if (strcmp(cur->Title, name) == 0)
			return cur;
		cur = cur->Next;
	}
	return NULL;
}
RevPos HasRated(UserPos curr_user, const char* title) {
	RevPos curr_rev = curr_user->myReviews;
	while (curr_rev != NULL) {
		if (strcmp(curr_rev->movie, title) == 0)
			return curr_rev;
		curr_rev = curr_rev->nextInUser;
	}
	return NULL;
}
int PrintMovie(TablePosM mTable, const char* name) {
	MoviePos movie = findMovie(mTable, name);
	if (movie == NULL)
		return EXIT_FAILURE;
	RevPos curr = movie->myReviews;
	printf("%s|%s|%d\n", movie->Title, movie->Genre, movie->ReleaseYear);
	printf("Rating: %.1f/10\n", movie->avgRating);
	printf("Reviews(%d)\n", movie->reviewCount);
	while (curr != NULL) {
		printf("%s: %d\n", curr->user, curr->rating);
		curr = curr->nextInMovie;
	}
	return EXIT_SUCCESS;
}
int PrintMyReviews(TablePosU uTable, UserPos curr_user) {
	RevPos curr = curr_user->myReviews;
	if (curr == NULL) {
		printf("You haven't reviewed any movies yet.\n");
		return EXIT_SUCCESS;
	}
	printf("My Reviews:\n");
	while (curr != NULL) {
		printf("%s: %d\n", curr->movie, curr->rating);
		curr = curr->nextInUser;
	}
	return EXIT_SUCCESS;
}
UserPos addUser(TablePosU uTable, const char* username) {
	if (findUser(uTable, username))
		return NULL; // vec postoji
	unsigned h = Myhash(username, USER_TABLE_SIZE);
	UserPos u = (UserPos)malloc(sizeof(_user));
	if (u == NULL)
		return NULL;
	strcpy(u->username, username);
	u->myReviews = NULL;
	//dodaj na pocetak
	u->Next = uTable->buckets[h];
	uTable->buckets[h] = u;
	return u;
}
MoviePos addMovie(TablePosM mTable, const char* name, const char* genre, int year) {
	unsigned h = Myhash(name, MOVIE_TABLE_SIZE);
	MoviePos m = (MoviePos)malloc(sizeof(_movie));
	if (!m) return NULL;
	strcpy(m->Title, name);
	strcpy(m->Genre, genre);
	m->ReleaseYear = year;
	m->avgRating = 0.0f;
	m->reviewCount = 0;
	m->myReviews = NULL;
	//dodaj na pocetak
	m->Next = mTable->buckets[h];
	mTable->buckets[h] = m;
	return m;
}
RevPos addReview(TablePosU uTable, TablePosM mTable,
	const char* username, const char* title, int rating) {
	UserPos u = findUser(uTable, username);
	MoviePos m = findMovie(mTable, title);
	if (!u || !m)
		return NULL;
	RevPos r = (RevPos)malloc(sizeof(_rev));
	if (!r)
		return NULL;
	strcpy(r->user, username);
	strcpy(r->movie, title);
	r->rating = rating;
	//ubaci na pocetak liste kod usera
	r->nextInUser = u->myReviews;
	u->myReviews = r;
	//ubaci na pocetak liste kod filma
	r->nextInMovie = m->myReviews;
	m->myReviews = r;
	UpdateMovieRating(m, r->rating, 1);
	return r;
}
int DeleteReview(TablePosM mTable, UserPos curr_user, const char* title) {
	RevPos curr_rev = curr_user->myReviews, prev_rev = NULL;
	while (curr_rev != NULL && strcmp(title, curr_rev->movie) != 0) {
		prev_rev = curr_rev;
		curr_rev = curr_rev->nextInUser;
	}
	if (curr_rev == NULL)
		return EXIT_FAILURE;
	if (prev_rev != NULL)
		prev_rev->nextInUser = curr_rev->nextInUser;
	else
		curr_user->myReviews = curr_rev->nextInUser;
	RevPos ToDelete = curr_rev;
	MoviePos m = findMovie(mTable, title);
	curr_rev = m->myReviews, prev_rev = NULL;
	while (curr_rev != NULL && curr_rev != ToDelete) {
		prev_rev = curr_rev;
		curr_rev = curr_rev->nextInMovie;
	}
	if (curr_rev == NULL) 
		return EXIT_FAILURE;
	if (prev_rev != NULL)
		prev_rev->nextInMovie = curr_rev->nextInMovie;
	else
		m->myReviews = curr_rev->nextInMovie;

	UpdateMovieRating(m, ToDelete->rating, -1); //argument je -1 jer se recenzija brise
	free(ToDelete);
	return EXIT_SUCCESS;
}
int DeleteUser(TablePosU uTable, TablePosM mTable, const char* username) {
	unsigned h = Myhash(username, USER_TABLE_SIZE);
	UserPos prev = NULL, u = uTable->buckets[h];
	while (u != NULL && strcmp(u->username, username) != 0) {
		prev = u;
		u = u->Next;
	}
	if (u == NULL) 
		return EXIT_FAILURE;
	while (u->myReviews != NULL) {
		DeleteReview(mTable, u, u->myReviews->movie);
	}
	if (prev != NULL)
		prev->Next = u->Next;
	else //brisemo prvog usera u bucketu
		uTable->buckets[h] = u->Next;
	free(u);
	return EXIT_SUCCESS;
}
int UpdateMovieRating(MoviePos movie, int rating, int update) {
	//update je +1 ili -1 ovisno o tome je li recenzija dodana ili izbrisana
	int newCount = movie->reviewCount + update;
	if (newCount <= 0) {
		movie->avgRating = 0.0;
		movie->reviewCount = 0;
		return EXIT_SUCCESS;
	}
	float total = movie->avgRating * movie->reviewCount;
	total += rating * update;
	movie->reviewCount = newCount;
	movie->avgRating = total / movie->reviewCount;
	return EXIT_SUCCESS;
}
int SaveMovies(const char* filename, TablePosM table) {
	int i;
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("greska pri otvaranju datoteke %s\n", filename);
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < table->size; i++) {
		MoviePos curr = table->buckets[i];
		while (curr != NULL) {
			fprintf(file, "%s|%s|%d\n", curr->Title, curr->Genre, curr->ReleaseYear);
			curr = curr->Next;
		}
	}
	fclose(file);
	return EXIT_SUCCESS;
}
int SaveUsers(const char* filename, TablePosU table) {
	int i;
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("greska pri otvaranju datoteke %s\n", filename);
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < table->size; i++) {
		UserPos curr = table->buckets[i];
		while (curr != NULL) {
			fprintf(file, "%s\n", curr->username);
			curr = curr->Next;
		}
	}
	fclose(file);
	return EXIT_SUCCESS;
}
int SaveUserReviews(const char* filename, TablePosU table) {
	int i;
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("greska pri otvaranju datoteke %s\n", filename);
		exit(EXIT_FAILURE);
	}
	for (i = 0; i < table->size; i++) {
		UserPos user = table->buckets[i];
		while (user != NULL) {
			RevPos curr_rev = user->myReviews;
			while (curr_rev != NULL) {
				fprintf(file, "%s|%s|%d\n", curr_rev->user, curr_rev->movie, curr_rev->rating);
				curr_rev = curr_rev->nextInUser;
			}
			user = user->Next;
		}
	}
	fclose(file);
	return EXIT_SUCCESS;
}
int FreeAll(TablePosU users, TablePosM movies) {
	freeUserTable(users, movies);
	freeMovieTable(movies);
	return EXIT_SUCCESS;
}
int freeUserTable(TablePosU uTable, TablePosM mTable) {
	if (!uTable) return EXIT_FAILURE;
	for (int i = 0; i < uTable->size; ++i) {
		UserPos curr = uTable->buckets[i];
		while (curr != NULL) {
			UserPos temp = curr;
			curr = curr->Next;
			DeleteUser(uTable, mTable, temp->username);
		}
	}
	free(uTable->buckets);
	free(uTable);
	return EXIT_SUCCESS;
}
int freeMovieTable(TablePosM mTable) {
	//pri pozivu funkcije, reviews su vec izbrisani
	if (!mTable) return EXIT_FAILURE;
	for (int i = 0; i < mTable->size; ++i) {
		MoviePos curr = mTable->buckets[i];
		while (curr != NULL) {
			MoviePos temp = curr;
			curr = curr->Next;
			free(temp);
		}
	}
	free(mTable->buckets);
	free(mTable);
	return EXIT_SUCCESS;
}
int MovieRecommender(TablePosU uTable, TablePosM mTable, UserPos me) {
    char movieTitle[MAX_LEN];
    GetLine("Enter a movie you liked: ", movieTitle, sizeof(movieTitle));
    MoviePos target = findMovie(mTable, movieTitle);
    if (!target) {
        printf("Movie %s not found.\n", movieTitle);
        return EXIT_FAILURE;
    }
    MoviePos candidates[MAX_CANDIDATES];
    int votes[MAX_CANDIDATES] = {0};
    int candidateCount = 0;
    for (RevPos fanRev = target->myReviews; fanRev; fanRev = fanRev->nextInMovie) {
        // preskoci ako nije validan fan
        if (fanRev->rating <= GOOD_RATING || strcmp(fanRev->user, me->username) == 0) continue;
        UserPos fan = findUser(uTable, fanRev->user);
        for (RevPos theirRev = fan->myReviews; theirRev; theirRev = theirRev->nextInUser) {
            // preskoci ako nije dobar kandidat
            if ((theirRev->rating <= GOOD_RATING) || strcmp(theirRev->movie, movieTitle) == 0
            || HasRated(me, theirRev->movie)) continue;
			MoviePos candidate = findMovie(mTable, theirRev->movie);
            int foundIndex = -1;
			int bonus = 0;
            for (int i = 0; i < candidateCount; i++) {
                if (candidates[i] == candidate) {
                    foundIndex = i;
                    break;
                }
            }
            if (foundIndex >= 0) {
                votes[foundIndex]++;
            } else if (candidateCount < MAX_CANDIDATES) {
                candidates[candidateCount] = candidate;
				if (strcmp(candidate->Genre, target->Genre) == 0)
					bonus = SAME_GENRE_BONUS;
                votes[candidateCount] = 1 + bonus;
                candidateCount++;
            }
        }
    }
    if (candidateCount == 0) {
        printf("Sorry, no movies similar to %s were found.\n", movieTitle);
        return EXIT_SUCCESS;
    }
    printf("People who liked '%s', also liked:\n", movieTitle);
    for (int i = 0; i < candidateCount; i++) {
        if (candidates[i] != NULL) {
            printf("%d. %s|%s (%d)\n", i+1, candidates[i]->Title, candidates[i]->Genre, candidates[i]->ReleaseYear);
            printf("Rating: %.1f/10\n", candidates[i]->avgRating);
        }
    }
	return EXIT_SUCCESS;
}