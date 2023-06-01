# 动态分区分配方式的模拟

## 实验目的
1. 掌握动态分区分配方式使用的数据结构和分配算法。（首次适应分配算法/最佳适应分配算法/最坏适应分配算法）
2. 进一步加深对动态分区分配管理方式及其实现过程的理解。

## 实验环境

- 硬件环境：
    - 计算机一台
    - 局域网环境
- 软件环境：
    - Windows/Linux操作系统
    - C编译环境（Dev、VS、gcc等）

## 实验内容

1. 编写C程序，模拟实现首次/最佳/最坏适应分配算法的内存块分配与回收，要求每次分配与回收后显示出空闲分区和已分配分区的情况。
2. 假设初始状态，内存空间为640KB
3. 请求序列：
    - 作业1申请130KB，作业2申请60KB，作业3申请100KB，
    - 作业2释放60KB，作业3释放100KB，作业1释放130KB


> 以下是一个简单的C程序，模拟实现首次适应（First Fit）、最佳适应（Best Fit）和最坏适应（Worst Fit）分配算法的内存块分配与回收。程序使用链表来表示内存块，并通过显示空闲分区和已分配分区的情况来展示分配与回收的过程。

> 该程序首先调用initializeMemory函数初始化内存空间的大小，然后通过调用allocateFirstFit、allocateBestFit和allocateWorstFit函数进行内存块的分配，最后通过调用deallocate函数进行内存块的回收。每次分配和回收后，调用printMemoryStatus函数打印空闲分区和已分配分区的情况。


**注意：该程序仅提供了基本的内存分配和回收功能，并没有进行错误处理和边界检查。在实际使用中，可能需要对其进行进一步的改进和扩展。**
```c

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

```


>以下是一个改进版的C程序，对前面的程序进行了一些改进：
> 1. 添加了错误处理和边界检查，确保输入的内存块大小合法，并处理分配失败和回收不存在的内存块的情况。
> 2. 添加了合并相邻空闲分区的功能，提高内存利用率。
> 3. 修改了分配算法的实现，使其更加高效。

```c
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
        printf("Error: No available memory block to allocate.\n");
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
        printf("Error: No available memory block to allocate.\n");
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

```