#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "tokenizer.c"
#include <stdlib.h>
#include <ctype.h>
#include "indexer.h"
#include <unistd.h>

int GetIndex(char *word) {
	char firstLetter;
	if(!word) return -1;
	if(isalpha(word[0])){
		firstLetter = tolower(word[0]);
		if (firstLetter != '\0') {
			return (firstLetter - 'a');
		} else {
			return -1;
		}
	}
	else if(isdigit(word[0])){
		return (word[0]-'0') + 26;
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
}
//CreateNode Needs to be edited 10/16/14 add filename
Node *CreateNode(char *word) {
	word = lowerCase(word);
	Node *new_Node = (Node *)malloc(sizeof(Node));
	printf("Creating node with: %s\n", word);
	new_Node->info = NULL;
	new_Node->value = word;
	new_Node->next = NULL;
	new_Node->info->occ = 1;
	new_Node->info->filename = NULL;//need to change this
	return new_Node;
}

LList *CreateLL() {
	LList *new_LL = malloc(sizeof(LList));
	new_LL->size = 0;
	return new_LL;
}

hashTable *CreateTable() {
	hashTable *new_table = malloc(sizeof(hashTable));
	new_table->buckets = malloc(sizeof(LList *) * 36);
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
	int i,j;
	i = GetIndex(word);
	
	if(!hash){
		return;
	}
	if(i = -1){
		return;
	}
	if(hash->buckets[i] == NULL){
		hash->buckets[i] = CreateLL();
		hash->buckets[i]->root = CreateNode(word);
		hash->buckets[i]->size++;
		hash->buckets[i]->root->info = CreateFileNode(filename);

		//need to add more here
		return;
	}else{
		Node *ptr = hash->buckets[i]->root;

		while(ptr != NULL){
			if(strcmp(word,ptr->value)== 0){
				FileNode *fptr = ptr->info;
				while(fptr != NULL){
					if(strcmp(filename,fptr->filename)==0){
						fptr->occ++;
						return;
					}
					if(fptr->next == NULL){
						FileNode *newf = CreateFileNode(filename);
						fptr->next = newf;
						return;
					}
				fptr=fptr->next;
			}
		}
		if(ptr->next = NULL){
			Node *newOne = CreateNode(word);
			ptr->next = newOne;
			FileNode *fn = CreateFileNode(filename);
			newOne->info = fn;
			return;
		}
		ptr = ptr->next;
		}
	}
}
char *getseparators(char *string){
	char *delims;
	char *copy;
	delims = (char *)malloc(100*sizeof(char));
	int size,i,j;
	j = 0;
	size = 0;
	for(i = 0; string[i] != '\0'; i++){
		if(!isalnum(string[i])){
			if(j == 99){
				delims = realloc(delims, 2*sizeof(delims));
			}
			delims[j] = string[i];
			j++;
		}
	}
	return delims;
}

void Fparse(char *filename, hashTable *ftable){
	FILE *stream;
	char *contents;
	char *token;
	int size = 0;
	if ( stream != NULL ){

    fseek(stream, 0L, SEEK_END);
    long s = ftell(stream);
    rewind(stream);
    contents = malloc(s +1);
    if ( contents != NULL )
    {
      fread(contents, s, 1, stream);
      // we can now close the file

      fclose(stream); stream = NULL;
  	}
  }

	TokenizerT *tk = TKCreate(getseparators(contents),contents);
	token = TKGetNextToken(tk);
	while(token != NULL){
		InsertToTable(ftable, token, filename);
		token = TKGetNextToken(tk);
	}
}
int WriteToFile(char * invfile, hashTable *hash){
	FILE *fp;
	if(fp == NULL){
		return 0;
	}
	if(hash == NULL){
		return 0;
	}
	fp = fopen(invfile, "w");
	int i;
	LList *ll;
	Node *ptr;
	
	for(i = 0; i < 36; i++){
		ll = hash->buckets[i];
		ptr = ll->root;
		if(ll == NULL){
			continue;
		}
		if(ptr == NULL){
			continue;
		}
		while(ptr != NULL){
			fprintf(fp,"<list> %s\n",ptr->value);
			FileNode *fnp = ptr->info;
			while(fnp !=NULL){
				fprintf(fp, "%s %d",fnp->filename,fnp->occ);
				fnp = fnp->next;
			}
			fprintf(fp,"\n</list>\n");
			ptr = ptr->next;
		}
	}
	fclose(fp);
	return 1;
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

int main(int argc, char **argv){

	char *file = argv[2];
	char *inv = argv[1];
	hashTable *ht = CreateTable();
	Fparse(file, ht);
	WriteToFile(inv, ht); 

	

}
