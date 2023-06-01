#include <stdio.h>
#include <stdlib.h>

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
    Node* previous = NULL;
    while (current != NULL) {
        if (current->block.allocated == 0 && current->block.size >= size) {
            // 判断当前空闲分区是否足够容纳新的分配块
            if (current->block.size > size) {
                // 创建新的空闲分区节点
                Node* newBlock = (Node*)malloc(sizeof(Node));
                newBlock->block.start = current->block.start + size;
                newBlock->block.size = current->block.size - size;
                newBlock->block.allocated = 0;
                newBlock->next = current->next;
                // 调整当前空闲分区的大小
                current->block.size = size;
                current->next = newBlock;
            }
            current->block.allocated = 1;
            break;
        }
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Error: No available memory block to allocate.\n");
    }
}

// 分配内存块（最佳适应算法）
void allocateBestFit(int size) {
    Node* bestBlock = NULL;
    Node* previousBestBlock = NULL;
    Node* current = head;
    Node* previous = NULL;
    int minSize = INT_MAX;

    while (current != NULL) {
        if (current->block.allocated == 0 && current->block.size >= size) {
            if (current->block.size < minSize) {
                minSize = current->block.size;
                bestBlock = current;
                previousBestBlock = previous;
            }
        }
        previous = current;
        current = current->next;
    }

    if (bestBlock != NULL) {
        if (bestBlock->block.size > size) {
            Node* newBlock = (Node*)malloc(sizeof(Node));
            newBlock->block.start = bestBlock->block.start + size;
            newBlock->block.size = bestBlock->block.size - size;
            newBlock->block.allocated = 0;
            newBlock->next = bestBlock->next;
            bestBlock->block.size = size;
            bestBlock->next = newBlock;
        }
        bestBlock->block.allocated = 1;
    } else {
        printf("Error: 没有可用的内存块可分配\n");
    }
}

// 分配内存块（最坏适应算法）
void allocateWorstFit(int size) {
    Node* worstBlock = NULL;
    Node* previousWorstBlock = NULL;
    Node* current = head;
    Node* previous = NULL;
    int maxSize = INT_MIN;

    while (current != NULL) {
        if (current->block.allocated == 0 && current->block.size >= size) {
            if (current->block.size > maxSize) {
                maxSize = current->block.size;
                worstBlock = current;
                previousWorstBlock = previous;
            }
        }
        previous = current;
        current = current->next;
    }

    if (worstBlock != NULL) {
        if (worstBlock->block.size > size) {
            Node* newBlock = (Node*)malloc(sizeof(Node));
            newBlock->block.start = worstBlock->block.start + size;
            newBlock->block.size = worstBlock->block.size - size;
            newBlock->block.allocated = 0;
            newBlock->next = worstBlock->next;
            worstBlock->block.size = size;
            worstBlock->next = newBlock;
        }
        worstBlock->block.allocated = 1;
    } else {
        printf("Error: 没有可用的内存块可分配.\n");
    }
}

// 回收内存块
void deallocate(int start) {
    Node* current = head;
    Node* previous = NULL;
    while (current != NULL) {
        if (current->block.start == start && current->block.allocated == 1) {
            current->block.allocated = 0;

            // 合并相邻的空闲分区（前面的分区）
            if (previous != NULL && previous->block.allocated == 0) {
                previous->block.size += current->block.size;
                previous->next = current->next;
                free(current);
                current = previous;
            }

            // 合并相邻的空闲分区（后面的分区）
            if (current->next != NULL && current->next->block.allocated == 0) {
                current->block.size += current->next->block.size;
                Node* temp = current->next;
                current->next = current->next->next;
                free(temp);
            }
            break;
        }
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        printf("Error: Memory block at address %d not found or not allocated.\n", start);
    }
}

// 打印空闲分区和已分配分区的情况
void printMemoryStatus() {
    Node* current = head;
    printf("Memory Status:\n");
    while (current != NULL) {
        printf("分区: 开始=%d, 大小=%d, 是否分配=%s\n",
               current->block.start, current->block.size,
               current->block.allocated ? "是" : "否");
        current = current->next;
    }
    printf("-----------------------\n");
}

int main() {
    initializeMemory(100); // 初始化内存空间大小为100

    // 进行内存分配与回收演示
    allocateFirstFit(20);
    printMemoryStatus();

    allocateBestFit(10);
    printMemoryStatus();

    allocateWorstFit(30);
    printMemoryStatus();

    deallocate(0);
    printMemoryStatus();

    deallocate(30);
    printMemoryStatus();

    return 0;
}
