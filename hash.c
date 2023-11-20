#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _CLOSED_HASHTABLE_H_
#define _CLOSED_HASHTABLE_H_

typedef unsigned int uint;
typedef char* STRING;
typedef char** STRING_ARRAY_PTR;

typedef struct hash_tag {
    uint size;
    uint tableSize;
    STRING_ARRAY_PTR list;
} HASH_TABLE;

typedef HASH_TABLE* HASH_TABLE_PTR;

// Function to create a hash table
HASH_TABLE_PTR createHashTable(uint tableSize);

// Function to check if the hash table is empty
uint isEmpty(HASH_TABLE_PTR H);

// Function to check if the hash table is full
uint isFull(HASH_TABLE_PTR H);

// Function to insert a key-value pair into the hash table
void put(HASH_TABLE_PTR H, STRING key, STRING data);

// Function to find data associated with a key in the hash table
STRING find(HASH_TABLE_PTR H, STRING key);

// Function to erase a key-value pair from the hash table
STRING erase(HASH_TABLE_PTR H, STRING key);

// Function to destroy the hash table and free memory
void destroy(HASH_TABLE_PTR H);

// Function to print the contents of the hash table
void printTable(HASH_TABLE_PTR H);

#endif

// Function to create a hash table
HASH_TABLE_PTR createHashTable(uint tableSize) {
    HASH_TABLE_PTR newTable = (HASH_TABLE_PTR)malloc(sizeof(HASH_TABLE));
    if (newTable == NULL) {
        fprintf(stderr, "Memory allocation failed for hash table.\n");
        exit(EXIT_FAILURE);
    }

    newTable->size = 0;
    newTable->tableSize = tableSize;
    newTable->list = (STRING_ARRAY_PTR)malloc(tableSize * sizeof(STRING));
    if (newTable->list == NULL) {
        fprintf(stderr, "Memory allocation failed for hash table array.\n");
        exit(EXIT_FAILURE);
    }

    // Initialize each list in the table
    for (uint i = 0; i < tableSize; i++) {
        newTable->list[i] = NULL;
    }

    return newTable;
}

// Function to check if the hash table is empty
uint isEmpty(HASH_TABLE_PTR H) {
    return H->size == 0;
}

// Function to check if the hash table is full
uint isFull(HASH_TABLE_PTR H) {
    return H->size == H->tableSize;
}

// Hash function: simple hash based on the length of the key
uint hashFunction(STRING key, uint tableSize) {
    return strlen(key) % tableSize;
}

// Function to insert a key-value pair into the hash table
void put(HASH_TABLE_PTR H, STRING key, STRING data) {
    if (isFull(H)) {
        fprintf(stderr, "Error: Hash table is full.\n");
        return;
    }

    uint hashValue = hashFunction(key, H->tableSize);

    // Create a new string and copy the data into it
    STRING newData = strdup(data);

    // Check if the list at the calculated hash value is empty
    if (H->list[hashValue] == NULL) {
        H->list[hashValue] = newData;
    } else {
        // Handle collisions by appending to the list
        STRING current = H->list[hashValue];
        while (current != NULL && current[0] != '\0') {
            current++;
        }
        current = newData;
    }

    H->size++;
}

// Function to find data associated with a key in the hash table
STRING find(HASH_TABLE_PTR H, STRING key) {
    uint hashValue = hashFunction(key, H->tableSize);

    STRING current = H->list[hashValue];
    while (current != NULL && strcmp(current, key) != 0) {
        current++;
    }

    return current;
}

// Function to erase a key-value pair from the hash table
STRING erase(HASH_TABLE_PTR H, STRING key) {
    uint hashValue = hashFunction(key, H->tableSize);

    STRING current = H->list[hashValue];
    while (current != NULL && strcmp(current, key) != 0) {
        current++;
    }

    if (current != NULL) {
        free(current);
        H->size--;
    }

    return current;
}

// Function to destroy the hash table and free memory
void destroy(HASH_TABLE_PTR H) {
    for (uint i = 0; i < H->tableSize; i++) {
        free(H->list[i]);
    }
    free(H->list);
    free(H);
}

// Function to print the contents of the hash table
void printTable(HASH_TABLE_PTR H) {
    for (uint i = 0; i < H->tableSize; i++) {
        printf("Index %u: %s\n", i, H->list[i]);
    }
}

int main() {
    HASH_TABLE_PTR hashTable = createHashTable(10);

    put(hashTable, "key1", "data1");
    put(hashTable, "key2", "data2");
    put(hashTable, "key3", "data3");

    printTable(hashTable);

    STRING foundData = find(hashTable, "key2");
    if (foundData != NULL) {
        printf("Found data: %s\n", foundData);
    } else {
        printf("Data not found.\n");
    }

    erase(hashTable, "key2");

    printTable(hashTable);

    destroy(hashTable);

    return 0;
}