#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
// 定义内存块结构体
typedef struct {
    int start;    // 起始地址
    int size;     // 大小
    int allocated; // 是否已分配
} MemoryBlock;

// 定义链表节点结构体
typedef struct Node {
    MemoryBlock block;
    struct Node* next;
} Node;

Node* head; // 链表头节点

// 初始化内存空间
void initializeMemory(int size) {
    head = (Node*)malloc(sizeof(Node));
    head->block.start = 0;
    head->block.size = size;
    head->block.allocated = 0;
    head->next = NULL;
}

// 分配内存块（首次适应算法）
void allocateFirstFit(int size) {
    Node* current = head;
    int found = 0;

    while (current != NULL) {
        if (current->block.allocated == 0 && current->block.size >= size) {
            current->block.allocated = 1;
            found = 1;
            break;
        }
        current = current->next;
    }

    if (!found) {
        printf("Error: 没有可用的内存块可分配。\n");
    }
}

// 分配内存块（最佳适应算法）
void allocateBestFit(int size) {
    Node* bestBlock = NULL;
    Node* current = head;
    int minSize = INT_MAX; //2147483647

    while (current != NULL) {
        if (current->block.allocated == 0 && current->block.size >= size) {
            if (current->block.size < minSize) {
                minSize = current->block.size;
                bestBlock = current;
            }
        }
        current = current->next;
    }

    if (bestBlock != NULL) {
        bestBlock->block.allocated = 1;
    } else {
        printf("Error: 没有可用的内存块可分配。\n");
    }
}

// 分配内存块（最坏适应算法）
void allocateWorstFit(int size) {
    Node* worstBlock = NULL;
    Node* current = head;
    int maxSize = INT_MIN;

    while (current != NULL) {
        if (current->block.allocated == 0 && current->block.size >= size) {
            if (current->block.size > maxSize) {
                maxSize = current->block.size;
                worstBlock = current;
            }
        }
        current = current->next;
    }

    if (worstBlock != NULL) {
        worstBlock->block.allocated = 1;
    } else {
        printf("Error: 没有可用的内存块可分配。\n");
    }
}

// 回收内存块
void deallocate(int start) {
    Node* current = head;

    while (current != NULL) {
        if (current->block.start == start && current->block.allocated == 1) {
            current->block.allocated = 0;
            break;
        }
        current = current->next;
    }

    if (current == NULL) {
        printf("Error: %d 地址处的内存块未找到或未分配.\n", start);
    }
}

// 打印空闲分区和已分配分区的情况
void printMemoryStatus() {
    Node* current = head;
    printf("Memory Status:\n");
    while (current != NULL) {
        printf("Block: Start=%d, Size=%d, Allocated=%s\n",
               current->block.start, current->block.size,
               current->block.allocated ? "Yes" : "No");
        current = current->next;
    }
    printf("-----------------------\n");
}

int main() {
    int size;
    int choice;
    int blocksize;

    printf("输入分配内存大小： ");
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
                printf("输入要分配的内存块大小: ");
                scanf("%d", &blocksize);
                allocateFirstFit(blocksize);
                printMemoryStatus();
                break;
            case 2:
                printf("输入要分配的内存块大小: ");
                scanf("%d", &blocksize);
                allocateBestFit(blocksize);
                printMemoryStatus();
                break;
            case 3:
                printf("输入要分配的内存块大小: ");
                scanf("%d", &blocksize);
                allocateWorstFit(blocksize);
                printMemoryStatus();
                break;
            case 4:
                printf("Enter the start address of memory block to deallocate: ");
                scanf("%d", &blocksize);
                deallocate(blocksize);
                printMemoryStatus();
                break;
            case 5:
                printMemoryStatus();
                break;
            case 6:
                exit(0);
                break;
            default:
                printf("Invalid choice.\n");
                break;
        }
    }

    return 0;
}
