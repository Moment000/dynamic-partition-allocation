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
    while (current != NULL) {
        if (current->block.allocated == 0 && current->block.size >= size) {
            current->block.allocated = 1;
            if (current->block.size > size) {
                Node* newBlock = (Node*)malloc(sizeof(Node));
                newBlock->block.start = current->block.start + size;
                newBlock->block.size = current->block.size - size;
                newBlock->block.allocated = 0;
                newBlock->next = current->next;
                current->block.size = size;
                current->next = newBlock;
            }
            break;
        }
        current = current->next;
    }
}

// 分配内存块（最佳适应算法）
void allocateBestFit(int size) {
    Node* bestBlock = NULL;
    Node* current = head;
    while (current != NULL) {
        if (current->block.allocated == 0 && current->block.size >= size) {
            if (bestBlock == NULL || current->block.size < bestBlock->block.size) {
                bestBlock = current;
            }
        }
        current = current->next;
    }
    if (bestBlock != NULL) {
        bestBlock->block.allocated = 1;
        if (bestBlock->block.size > size) {
            Node* newBlock = (Node*)malloc(sizeof(Node));
            newBlock->block.start = bestBlock->block.start + size;
            newBlock->block.size = bestBlock->block.size - size;
            newBlock->block.allocated = 0;
            newBlock->next = bestBlock->next;
            bestBlock->block.size = size;
            bestBlock->next = newBlock;
        }
    }
}

// 分配内存块（最坏适应算法）
void allocateWorstFit(int size) {
    Node* worstBlock = NULL;
    Node* current = head;
    while (current != NULL) {
        if (current->block.allocated == 0 && current->block.size >= size) {
            if (worstBlock == NULL || current->block.size > worstBlock->block.size) {
                worstBlock = current;
            }
        }
        current = current->next;
    }
    if (worstBlock != NULL) {
        worstBlock->block.allocated = 1;
        if (worstBlock->block.size > size) {
            Node* newBlock = (Node*)malloc(sizeof(Node));
            newBlock->block.start = worstBlock->block.start + size;
            newBlock->block.size = worstBlock->block.size - size;
            newBlock->block.allocated = 0;
            newBlock->next = worstBlock->next;
            worstBlock->block.size = size;
            worstBlock->next = newBlock;
        }
    }
}

// 回收内存块
void deallocate(int start) {
    Node* current = head;
    while (current != NULL) {
        if (current->block.start == start && current->block.allocated == 1) {
            current->block.allocated = 0;
            // 合并相邻的空闲分区
            if (current->next != NULL && current->next->block.allocated == 0) {
                current->block.size += current->next->block.size;
                Node* temp = current->next;
                current->next = current->next->next;
                free(temp);
            }
            if (current != head && current->block.start - current->block.size == current->block.start) {
                Node* temp = head;
                while (temp->next != current) {
                    temp = temp->next;
                }
                temp->block.size += current->block.size;
                temp->next = current->next;
                free(current);
                current = temp;
            }
            break;
        }
        current = current->next;
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
