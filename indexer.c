// Krishna Yellayi and Arnesh Sahay
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <ctype.h>
#include "indexer.h"
#include <unistd.h>
#include <errno.h>



int GetIndex(char *word) {
	char firstLetter = word[0];
	if(!word) return -1;
	if(isalpha(firstLetter)){
		firstLetter = tolower(firstLetter);
		if (firstLetter != '\0') {
			return (firstLetter - 'a');
		} else {
			return -1;
		}
	}
	else if(isdigit(firstLetter)){
		return (firstLetter-'0') + 26;
	}
	else{
		return -1;
	}
}
FileNode *CreateFileNode(char *file){
	FileNode *fn = (FileNode *)malloc(sizeof(FileNode));
	fn->filename = file;
	fn->occ =1;
	fn->next = NULL;
	return fn;
}
//CreateNode Needs to be edited 10/16/14 add filename
Node *CreateNode(char *word) {
	word = lowerCase(word);
	Node *new_Node = (Node *)malloc(sizeof(Node));
	//printf("Creating node with: %s\n", word);
	new_Node->info = NULL;
	new_Node->value = word;
	new_Node->next = NULL;
	new_Node->info = NULL;//need to change this
	return new_Node;
}

LList *CreateLL() {
	LList *new_LL = (LList *)malloc(sizeof(LList));
	new_LL->size = 0;
	new_LL->root = NULL;
	return new_LL;
}

hashTable *CreateTable() {
	hashTable *new_table = (hashTable *)malloc(sizeof(hashTable));
	new_table->buckets = (LList **)malloc(sizeof(LList *) * 36);
	new_table->size = 36;
	return new_table;
}
char *lowerCase(char *word){
	int i;
	i=0;
	for(i = 0; i < strlen(word); i++){
		word[i] = tolower(word[i]);
	}
	return word;
}
void InsertToTable(hashTable *hash, char *word, char *filename){
	int i;
	i = GetIndex(word);
	Node *ptr;
	
	if(!hash){
		return;
	}
	if(i == -1){
		return;
	}
	if(hash->buckets[i] == NULL){
		hash->buckets[i] = CreateLL();
		hash->buckets[i]->root = CreateNode(word);
		hash->buckets[i]->size++;
		hash->buckets[i]->root->info = CreateFileNode(filename);
		//printLL(hash, i);
		//printf("%s\n",hash->buckets[i]->root->value);
		//printTable(hash);

		//need to add more here
		return;
	}else{
		ptr = hash->buckets[i]->root;

		while(ptr != NULL){
			if(strcmp(lowerCase(word),ptr->value)== 0){
				FileNode *fptr = ptr->info;
				while(fptr != NULL){
					if(strcmp(filename,fptr->filename)==0){
						fptr->occ++;
						/*if(isalpha(ptr->value[0])){
							if(ptr->value[0] - 'a' != i){
								puts("error aborting now");
								exit(0);
							}
						}
							else if(isdigit(ptr->value[0])){
								if((ptr->value[0]-'0') + 26 != i){
									puts("error aborting now");
									exit(0);
								}
							}
								else
									return;*/
					return;
					}
					if(fptr->next == NULL){
						FileNode *newf = CreateFileNode(filename);
						fptr->next = newf;
						//puts(newf->filename);
						//printLL(hash, i);
						/*if(isalpha(ptr->value[0])){
							if(ptr->value[0] - 'a' != i){
								puts("error aborting now");
								exit(42);
							}
						}
							else if(isdigit(ptr->value[0])){
								if((ptr->value[0]-'0') + 26 != i){
									puts("error aborting now");
									exit(42);
								}
							}
								else
									return;*/
					return;
					
					}
				fptr=fptr->next;
			}
		}
		if(ptr->next == NULL){
			Node *newOne = CreateNode(word);
			FileNode *fn = CreateFileNode(filename);
			newOne->info = fn;
			ptr->next = newOne;
			hash->buckets[i]->size++;
			//printLL(hash, i);
			//printf("%s\n",ptr->value);
			//printTable(hash);
			/*if(isalpha(ptr->value[0])){
							if(ptr->value[0] - 'a' != i){
								puts("error aborting now");
								exit(42);
							}
						}
							else if(isdigit(ptr->value[0])){
								if((ptr->value[0]-'0') + 26 != i){
									puts("error aborting now");
									exit(42);
								}
							}
								else
									return;*/
		return;
		}
		ptr = ptr->next;
		}
	}
}
char *getseparators(char *string){
	char *delims;
	delims = (char *)malloc(100*sizeof(char));
	int i,j;
	j = 0;
	for(i = 0; string[i] != '\0'; i++){
		if(!isalnum(string[i])){
			if(j == 99){
				delims = realloc(delims, 2*sizeof(delims));
			}
			delims[j] = string[i];
			j++;
		}
	}
	delims = realloc(delims, (j+1)*sizeof(char));
	return delims;
}
char *FormatString(char *input){
	int i;
	for(i = 0; i < strlen(input); i++){
		if(!isalnum(input[i])){
			input[i] = ' ';
		}
	}
	return input;
}

void Fparse(char *filename, hashTable *ftable){
	FILE *stream;
	char *contents; //= (char *)malloc(2000*sizeof(char *));
	char *token;
	int j, size;
	stream = fopen(filename, "rb+");
	if ( stream != NULL ){

    fseek(stream, 0L, SEEK_END);
    size = (int)ftell(stream);
    rewind(stream);
    contents = (char *)malloc(size +1);
    for(j = 0; j <(size/sizeof(char)); j++){
    	contents[j] = fgetc(stream);
    }
    fclose(stream);
  //size_t len = 0;
  	
  //use strtok not this shit
  //fgets(contents, 2000, stream);
  //while(contents != NULL){
  	contents = FormatString(contents);
	
	token = strtok(contents, " ");
	while(token != NULL){
		InsertToTable(ftable, token, filename);
		//puts(token);
		token = strtok(NULL, " ");
	}
	//free(contents);
	//char *contents = (char *)malloc(2000*sizeof(char *));
	//contents = fgets(contents, 2000, stream);
	//free(sep);
	}
	
}
int WriteToFile(char * invfile, hashTable *hash){
	FILE *fp;
	fp = fopen(invfile, "wb+");
	if(fp == NULL){
		return 0;
	}
	if(hash == NULL){
		return 0;
	}
	int i;
	LList *ll;
	Node *ptr;
	
	for(i = 0; i < 36; i++){
		ll = hash->buckets[i];
		//fprintf(fp, "hello\n");
		if(ll == NULL){
			continue;
		}
		ptr = ll->root;
		if(ptr == NULL){
			continue;
		}
		while(ptr != NULL){

			fprintf(fp,"<list> %s\n",ptr->value);
			FileNode *fnp = ptr->info;
			while(fnp !=NULL){
				fprintf(fp, "%s %d ",fnp->filename,fnp->occ);
				fnp = fnp->next;
			}
			fprintf(fp,"\n</list>\n");
			ptr = ptr->next;
		}
	}
	fclose(fp);
	return 1;
}

static void dir_traversal(char *path, hashTable *table) {

	//Declare directory & file variables
	DIR *dir; struct dirent *fil;

	//Tells user what directory is currently being accessed
	printf("Current directory: %s\n", path);

	//Error checking
	if(!(dir=opendir(path)))
		puts("Invalid Directory");
	//While the directory can be read, the function continues to traverse
	while((fil = readdir(dir))){
		//Extract filename from fil variable
		char *fil_name = (fil->d_name);
		//Continue to next instance of while loop if file is not an actual file
		if(strcmp(fil_name, ".") == 0 || strcmp(fil_name, "..") == 0)
			continue;
		//Designate size for the filepath variable
		size_t fil_pathsize = (2 * sizeof(char)) + strlen(fil_name) + (strlen(path));
		//Declaration & memory allocation for filepath variable
		char *fil_path = malloc(fil_pathsize);
		//Copy the variable contents of path to the filepath variable
		strcpy(fil_path, path);
		//Add forward slash character to the filepath variable
		strcat(fil_path, "/");
		//Add filename variable contents to the filepath variable
		strcat(fil_path, fil_name);
		//Recursive call to dir_traversal method if fil contains a directory
		if(fil->d_type == DT_DIR){
			dir_traversal(fil_path,table);
		//Calls file parsing function if fil contains a regular file
		}else if(fil->d_type == DT_REG){
			Fparse(fil_path, table);
			continue;
		}else
			puts("error");
		//Frees memory for filepath variable
		//free(fil_path);
	}
	//Closes the current directory after all its directories are traversed & files are parsed  
	closedir(dir);
}
void freeHashTable(hashTable *hash){
	int i;
	LList *ll;
	Node *ptr;
	for(i = 0; i < 36; i++){
		ll = hash->buckets[i];
		//fprintf(fp, "hello\n");
		if(ll == NULL){
			free(ll);
			continue;
		}
		ptr = ll->root;
		if(ptr == NULL){
			free(ptr);
			continue;
		}
		while(ptr != NULL){

			
			FileNode *fnp = ptr->info;
			while(fnp !=NULL){
				free(fnp->filename);
				
				fnp = fnp->next;
			}
			free(ptr->value);
			free(ptr->info);
			free(ptr);
			ptr = ptr->next;
		}
		free(ll);
	}
	free(hash);
}
void printTable(hashTable *hash){
	int i;
	LList *ll;
	Node *ptr;
	for(i = 0; i < hash->size; i++){
		ll = hash->buckets[i];
		//fprintf(fp, "hello\n");
		if(ll == NULL){
			free(ll);
			continue;
		}
		ptr = ll->root;
		if(ptr == NULL){
			//printf("%s\n", ptr->value);
			continue;
		}
		while(ptr != NULL){

			printf("%s\n", ptr->value);
			FileNode *fnp = ptr->info;
			while(fnp !=NULL){
				printf("%s\n", fnp->filename);
				
				fnp = fnp->next;
			}
			//free(ptr->value);
			//free(ptr->info);
			//free(ptr);
			ptr = ptr->next;
		}
		//free(ll);
	}
	//free(hash);
}


/*char *FNToString(FileNode *fn){
	char *fname;
	char *res;
	char *intstring;
	intstring = (char *)malloc(sizeof(char)*8);
	fname = fn->filename;
	sprintf(intstring, "%d", fn->occ);
	res = strcat(res, fname);
	res = strcat(res, " ");
	res = strcat(res, intstring);
	return res;
}
*/
void printLL(hashTable *hash, int i){
	LList *ll = hash->buckets[i];
	Node *ptr = ll->root;
	if(ptr == NULL){
		puts("empty linked list");
		return;
	}
	while(ptr != NULL){
		printf("%s\n", ptr->value);
		ptr = ptr->next;
	}
}
int main(int argc, char **argv){
	char *path = argv[2];
	char *inv = argv[1];
	hashTable *ht = CreateTable();
	struct stat s;

	if( stat(path,&s) == 0 )
	{
    	if( s.st_mode & S_IFDIR )
    	{
        	//it's a directory
        	dir_traversal(path,ht);
    	}
    	else if( s.st_mode & S_IFREG )
    	{
        	//it's a file
        	Fparse(path, ht);
    	}
    	else
    	{
        	//something else
        	puts("Did not type in a valid directory or file");
    	}
	}
	else
	{
    	//error
    	puts("Try again");
	}
	WriteToFile(inv, ht);
	puts("file successfully written");
	//printTable(ht);
	//freeHashTable(ht); 

	
return 0;
}
