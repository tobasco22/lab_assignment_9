#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType *next; // create a next pointer (for linkedlist structure)
};

// Fill out this structure
struct HashType
{
    struct RecordType *record; // create a pointer to a RecordType
};

// Compute the hash function
int hash(int x, int tableSize)
{
    return x % tableSize; // hash function is x % tableSize
}

// parses input file to an integer array
int parseData(char *inputFileName, struct RecordType **ppData)
{
    FILE *inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType *)malloc(sizeof(struct RecordType) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// insert record into the hash table
void insertRecord(struct HashType *hashTable, struct RecordType *record, int tableSize)
{
    // call the hash function to get the index
    int index = hash(record->id, tableSize);

    // create a new node for the record
    struct RecordType *newNode = (struct RecordType*)malloc(sizeof(struct RecordType));
    newNode->id = record->id;
    newNode->name = record->name;
    newNode->order = record->order;
    newNode->next = NULL;

    // insert the new node at the head of the linked list at the corresponding index
    if (hashTable[index].record == NULL)
    {
        hashTable[index].record = newNode;
    }
    else
    {
        newNode->next = hashTable[index].record;
        hashTable[index].record = newNode;
    }
}


// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *hashTable, int tableSize)
{
    int i;
    struct RecordType *record;

    printf("\nHash Table Contents:\n");
    for (i = 0; i < tableSize; ++i)
    {
        printf("Index %d -> ", i);

        if (hashTable[i].record == NULL)
        {
            printf("NULL\n");
        }
        else
        {
            record = hashTable[i].record;

            while (record != NULL)
            {
                printf("%d, %c, %d", record->id, record->name, record->order);

                if (record->next != NULL)
                {
                    printf(" -> ");
                }

                record = record->next;
            }

            printf("\n");
        }
    }

    printf("\n\n");
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;
    int i;
    int hashTableSize = 10;
    struct HashType *hashTable = calloc(hashTableSize, sizeof(struct HashType));

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize the hash table
    for (i = 0; i < recordSz; ++i)
    {
        insertRecord(hashTable, &pRecords[i], hashTableSize);
    }

    displayRecordsInHash(hashTable, hashTableSize);

    // Free all the allocated memory
    free(pRecords);

    for (i = 0; i < hashTableSize; ++i)
    {
        struct RecordType *record = hashTable[i].record;

        while (record != NULL)
        {
            struct RecordType *next = record->next;
            free(record);
            record = next;
        }
    }

    free(hashTable);

    return 0;
}
