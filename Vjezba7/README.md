# Kako bi izgledalo sortirano dodavanje direktorija
```c
int AddNewDir(stackPosition ST, const char* newDir_name) {
	if (ST->Next == NULL) {
		printf("greska - nema trenutnog direktorija\n");
		return EXIT_SUCCESS;
	}
	dirPosition ParentDir = ST->Next->dirLevel;
	dirPosition newDir = CreateDir(newDir_name);
	if(ParentDir->subDir == NULL || strcmp(ParentDir->name, newDir->name) > 0){
	//dodaje se na pocetak ako je lista prazna 
	//ili je newDir abecedno prije prvog djeteta
	newDir->Next = ParentDir->subDir;
	ParentDir->subDir = newDir;
	}
	else{
	dirPosition curr = ParentDir->subDir;
	while(curr->Next != NULL && strcmp(curr->Next->name, newDir->name) < 0) {
			curr = curr->Next;
		}
		newDir->Next = curr->Next;
		curr->Next = newDir;
	}
	return EXIT_SUCCESS;
}
//ostatak koda je isti
```