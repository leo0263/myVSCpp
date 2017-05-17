// simple implementation of hash table
// using array for the buckets and linked list for collision handling
// hashing function algorithm used : djb2
// by : leo.0263

#include <iostream>
#include <cassert>
using namespace std;

#define MAX_BUCKET 1000

struct t_HashTable;
struct t_Database;

struct t_HashTable {
	char key[20];
	t_Database *pDatabase;
	t_HashTable *next;
};

struct t_Database {
	char name[20];
	int  securityNumber;
	t_Database *next;
	t_HashTable *pHashTable;
};

t_HashTable *hashTable[MAX_BUCKET];


// --------------------------------------------------- utils functions
unsigned long hashFunction(char *str)
{
	unsigned long hash = 5381;
	int c;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash % MAX_BUCKET;
}

void stringCopy(char* toStr, char* fromStr) {
	int i = 0;
	while ((toStr[i] != '\0') && (fromStr[i] != '\0')) {
		toStr[i] = fromStr[i];
		i++;
	}
	toStr[i] = '\0';
}

int stringCompare(char* str1, char* str2) {
	bool isSame = true;
	bool notEOS1 = true;
	bool notEOS2 = true;
	int i = 0;

	while (notEOS1 && notEOS2 && isSame) {
		if (str1[i] != str2[i])  isSame = false;
		if (str1[i] == '\0') notEOS1 = false;
		if (str2[i] == '\0') notEOS2 = false;
		i++;
	}

	i--;
	if (isSame) return 0;
	else {
		int char1 = str1[i];
		int char2 = str2[i];

		if (char1 >= 97) char1 -= 32;
		if (char2 >= 97) char2 -= 32;

		if (char1 - char2 == 0) {
			if (!notEOS1) return -1;
			else return 1;
		} else 
			return (char1 - char2);
	}
}
// --------------------------------------------------- eof utils functions


void purgeHashTable() {
	t_HashTable *pointer, *toDelete;
	for (int i = 0; i < MAX_BUCKET; i++) {
		pointer = hashTable[i];
		while (pointer != NULL) {
			toDelete = pointer;
			pointer = pointer->next;
			delete toDelete;
		}

		hashTable[i] = NULL;
	}
}

void addToHashTable(t_Database *data) {
	int bucketNum = hashFunction(data->name);

	t_HashTable *newHashData = new t_HashTable();
	newHashData->next = NULL;
	stringCopy(newHashData->key, data->name);
	newHashData->pDatabase = data;

	t_HashTable *pHashPosition = hashTable[bucketNum];
	if (pHashPosition == NULL) {
		hashTable[bucketNum] = newHashData;
	} else {
		while (pHashPosition->next != NULL)
			pHashPosition = pHashPosition->next;
		pHashPosition->next = newHashData;
	}
}



int main()
{
	// test utilities
	char name1[20] = "aaa";
	char name2[20] = "aaa";
	char name3[20] = "aab";
	char name4[20] = "aaab";

	assert(stringCompare(name1, name2) == 0);
	assert(stringCompare(name2, name3) < 0);
	assert(stringCompare(name3, name2) > 0);
	assert(stringCompare(name2, name4) < 0);

	char name5[20];
	stringCopy(name5, name1);
	cout << "string 1 = [" << name1 << "]" << endl;
	cout << "string 5 = [" << name5 << "]" << endl;
	assert(stringCompare(name5, name1) == 0);

	// test hash table
	cout << "hash(" << name1 << ") = " << hashFunction(name1) << endl;
	cout << "hash(" << name2 << ") = " << hashFunction(name2) << endl;
	cout << "hash(" << name3 << ") = " << hashFunction(name3) << endl;
	cout << "hash(" << name4 << ") = " << hashFunction(name4) << endl;
	cout << "hash(" << name5 << ") = " << hashFunction(name5) << endl;

	cout << "if you see this, then all test is OK" << endl;
	cout << "type SOMETHING to end!" << endl;
	cin >> name1;
	return 0;
}

