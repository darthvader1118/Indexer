#include <stdio.h>
#include <stdlib.h> /* malloc */
#include <string.h>

typedef struct _hashTable {
	struct _LList **buckets;
	int size;
} hashTable;

typedef struct _LList {
	struct _Node *root;
	int size;
} LList;

typedef struct _Node {
	char *value;
	struct _Node *next;
	struct _FileNode *info;
} Node;

typedef struct _FileNode{
	char *filename;
	int occ;
	struct _FileNode *next;
}FileNode;

Node *CreateNode(char *word);
LList *CreateLL();
hashTable *CreateTable();
void InsertToTable(hashTable *hash, char *word,char *filename);
int GetIndex(char *word);
void printResult(hashTable *hash);
char *lowerCase(char *word);
int compareNodes(Node *p1, Node *p2);
FileNode *CreateFileNode(char *file);
void Fparse(char *filename, hashTable *ftable);
int WriteToFile(char * invfile, hashTable *hash);
char *FNToString(FileNode *fn);
static void dir_traversal(char *path, hashTable *table);
void freeHashTable(hashTable *hash);