#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int start;
    int size;
    int allocated;
} MemoryBlock;

MemoryBlock* memoryBlocks;
int numBlocks;

void initializeMemory(int size) {
    numBlocks = 1;
    memoryBlocks = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    memoryBlocks[0].start = 0;
    memoryBlocks[0].size = size;
    memoryBlocks[0].allocated = 0;
}

void allocateFirstFit(int size) {
    int i;
    for (i = 0; i < numBlocks; i++) {
        if (!memoryBlocks[i].allocated && memoryBlocks[i].size >= size) {
            memoryBlocks[i].allocated = 1;
            memoryBlocks[i].size -= size;
            memoryBlocks[numBlocks].start = memoryBlocks[i].start + size;
            memoryBlocks[numBlocks].size = memoryBlocks[i].size;
            memoryBlocks[numBlocks].allocated = 0;
            numBlocks++;
            return;
        }
    }
    printf("Error: No available memory block to allocate.\n");
}

void allocateBestFit(int size) {
    MemoryBlock* bestBlock = NULL;
    int minSize = INT_MAX;
    int i;

    for (i = 0; i < numBlocks; i++) {
        if (!memoryBlocks[i].allocated && memoryBlocks[i].size >= size &&
            memoryBlocks[i].size < minSize) {
            minSize = memoryBlocks[i].size;
            bestBlock = &memoryBlocks[i];
        }
    }

    if (bestBlock != NULL) {
        bestBlock->allocated = 1;
        bestBlock->size -= size;
        memoryBlocks[numBlocks].start = bestBlock->start + size;
        memoryBlocks[numBlocks].size = bestBlock->size;
        memoryBlocks[numBlocks].allocated = 0;
        numBlocks++;
    } else {
        printf("Error: No available memory block to allocate.\n");
    }
}

void allocateWorstFit(int size) {
    MemoryBlock* worstBlock = NULL;
    int maxSize = INT_MIN;
    int i;

    for (i = 0; i < numBlocks; i++) {
        if (!memoryBlocks[i].allocated && memoryBlocks[i].size >= size &&
            memoryBlocks[i].size > maxSize) {
            maxSize = memoryBlocks[i].size;
            worstBlock = &memoryBlocks[i];
        }
    }

    if (worstBlock != NULL) {
        worstBlock->allocated = 1;
        worstBlock->size -= size;
        memoryBlocks[numBlocks].start = worstBlock->start + size;
        memoryBlocks[numBlocks].size = worstBlock->size;
        memoryBlocks[numBlocks].allocated = 0;
        numBlocks++;
    } else {
        printf("Error: No available memory block to allocate.\n");
    }
}

void deallocate(int start) {
    int i;
    for (i = 0; i < numBlocks; i++) {
        if (memoryBlocks[i].start == start && memoryBlocks[i].allocated) {
            memoryBlocks[i].allocated = 0;
            return;
        }
    }
    printf("Error: Memory block at address %d not found or not allocated.\n", start);
}

void printMemoryStatus() {
    int i;
    printf("Memory Status:\n");
    for (i = 0; i < numBlocks; i++) {
        printf("Block: Start=%d, Size=%d, Allocated=%s\n",
               memoryBlocks[i].start, memoryBlocks[i].size,
               memoryBlocks[i].allocated ? "Yes" : "No");
    }
    printf("-----------------------\n");
}

int main() {
    int size;
    int choice;
    int blockSize;

    printf("Enter the size of memory: ");
    scanf("%d", &size);

    initializeMemory(size);

    while (1) {
        printf("1. Allocate (First Fit)\n");
        printf("2. Allocate (Best Fit)\n");
        printf("3. Allocate (Worst Fit)\n");
        printf("4. Deallocate\n");
        printf("5. Print Memory Status\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter the size of memory block to allocate: ");
                scanf("%d", &blockSize);
                allocateFirstFit(blockSize);
                printMemoryStatus();
                break;
            case 2:
                printf("Enter the size of memory block to allocate: ");
                scanf("%d", &blockSize);
                allocateBestFit(blockSize);
                printMemoryStatus();
                break;
            case 3:
                printf("Enter the size of memory block to allocate: ");
                scanf("%d", &blockSize);
                allocateWorstFit(blockSize);
                printMemoryStatus();
                break;
            case 4:
                printf("Enter the start address of memory block to deallocate: ");
                scanf("%d", &blockSize);
                deallocate(blockSize);
                printMemoryStatus();
                break;
            case 5:
                printMemoryStatus();
                break;
            case 6:
                free(memoryBlocks);
                exit(0);
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    return 0;
}
