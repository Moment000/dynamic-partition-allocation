#include <stdio.h>
#include <stdlib.h>

// 定义内存块结构体
typedef struct MemoryBlock {
    int start_address;  // 起始地址
    int size;           // 大小
    int is_allocated;   // 是否已分配
    struct MemoryBlock* next;  // 下一个内存块的指针
} MemoryBlock;

// 全局变量，表示内存链表的头节点
MemoryBlock* memory = NULL;

// 函数声明
void initializeMemory(int size);
void allocateMemory(int process_size, int algorithm);
void deallocateMemory(int process_size);
void printMemoryStatus();

int main() {
    int memory_size, choice, process_size;
    int exit_flag = 0;

    while (1) {
        printf("请输入内存大小：");
        if (scanf("%d", &memory_size) == 1 && memory_size > 0) {
            break;
        }
        printf("无效的内存大小，请重新输入\n");
        while (getchar() != '\n') continue; // 清空输入缓冲区
    }

    initializeMemory(memory_size);

    while (!exit_flag) {
        printf("\n1. 分配内存\n2. 释放内存\n3. 显示内存状态\n4. 退出\n");
        printf("请选择操作：");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("请输入进程大小：");
                scanf("%d", &process_size);
                printf("请选择分配算法（1.首次适应 2.最佳适应 3.最坏适应）：");
                scanf("%d", &choice);
                allocateMemory(process_size, choice);
                break;
            case 2:
                printf("请输入要释放的进程大小：");
                scanf("%d", &process_size);
                deallocateMemory(process_size);
                break;
            case 3:
                printMemoryStatus();
                break;
            case 4:
                exit_flag = 1;
                break;
            default:
                printf("无效的选择\n");
        }
    }

    return 0;
}

// 初始化内存
void initializeMemory(int size) {
    memory = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    memory->start_address = 0;
    memory->size = size;
    memory->is_allocated = 0;
    memory->next = NULL;
}

// 首次适应算法分配内存
void firstFit(int process_size) {
    MemoryBlock* current = memory;
    while (current != NULL) {
        if (!current->is_allocated && current->size >= process_size) {
            // 找到一个未分配且大小足够的内存块
            if (current->size == process_size) {
                current->is_allocated = 1;
                printf("分配成功，起始地址：%d\n", current->start_address);
                return;
            } else {
                MemoryBlock* new_block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
                new_block->start_address = current->start_address;
                new_block->size = process_size;
                new_block->is_allocated = 1;
                new_block->next = current->next;

                current->size -= process_size;
                current->start_address += process_size;
                current->next = new_block;

                printf("分配成功，起始地址：%d\n", new_block->start_address);
                return;
            }
        }
        current = current->next;
    }
    printf("无足够的内存空间可供分配\n");
}

// 最佳适应算法分配内存
void bestFit(int process_size) {
    MemoryBlock* current = memory;
    MemoryBlock* best_block = NULL;
    int best_fit = -1;

    while (current != NULL) {
        if (!current->is_allocated && current->size >= process_size) {
            // 找到一个未分配且大小足够的内存块
            if (best_block == NULL || current->size < best_fit) {
                best_block = current;
                best_fit = current->size;
            }
        }
        current = current->next;
    }

    if (best_block != NULL) {
        if (best_block->size == process_size) {
            best_block->is_allocated = 1;
            printf("分配成功，起始地址：%d\n", best_block->start_address);
        } else {
            MemoryBlock* new_block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
            new_block->start_address = best_block->start_address;
            new_block->size = process_size;
            new_block->is_allocated = 1;
            new_block->next = best_block->next;

            best_block->size -= process_size;
            best_block->start_address += process_size;
            best_block->next = new_block;

            printf("分配成功，起始地址：%d\n", new_block->start_address);
        }
    } else {
        printf("无足够的内存空间可供分配\n");
    }
}

// 最坏适应算法分配内存
void worstFit(int process_size) {
    MemoryBlock* current = memory;
    MemoryBlock* worst_block = NULL;
    int worst_fit = -1;

    while (current != NULL) {
        if (!current->is_allocated && current->size >= process_size) {
            // 找到一个未分配且大小足够的内存块
            if (worst_block == NULL || current->size > worst_fit) {
                worst_block = current;
                worst_fit = current->size;
            }
        }
        current = current->next;
    }

    if (worst_block != NULL) {
        if (worst_block->size == process_size) {
            worst_block->is_allocated = 1;
            printf("分配成功，起始地址：%d\n", worst_block->start_address);
        } else {
            MemoryBlock* new_block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
            new_block->start_address = worst_block->start_address;
            new_block->size = process_size;
            new_block->is_allocated = 1;
            new_block->next = worst_block->next;

            worst_block->size -= process_size;
            worst_block->start_address += process_size;
            worst_block->next = new_block;

            printf("分配成功，起始地址：%d\n", new_block->start_address);
        }
    } else {
        printf("无足够的内存空间可供分配\n");
    }
}

// 分配内存
void allocateMemory(int process_size, int algorithm) {
    switch (algorithm) {
        case 1:
            firstFit(process_size);
            break;
        case 2:
            bestFit(process_size);
            break;
        case 3:
            worstFit(process_size);
            break;
        default:
            printf("无效的算法选择\n");
    }
}

// 释放内存
void deallocateMemory(int process_size) {
    MemoryBlock* current = memory;
    MemoryBlock* prev = NULL;

    while (current != NULL) {
        if (current->is_allocated && current->size == process_size) {
            // 找到已分配且大小匹配的内存块
            current->is_allocated = 0;

            // 合并相邻的空闲内存块
            if (current->next != NULL && !current->next->is_allocated) {
                current->size += current->next->size;
                MemoryBlock* temp = current->next;
                current->next = temp->next;
                free(temp);
            }

            if (prev != NULL && !prev->is_allocated) {
                prev->size += current->size;
                prev->next = current->next;
                free(current);
                current = prev;
            }

            printf("释放成功，起始地址：%d\n", current->start_address);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("找不到要释放的内存块\n");
}

// 显示内存状态
void printMemoryStatus() {
    MemoryBlock* current = memory;
    printf("\n空闲分区：\n");
    while (current != NULL) {
        if (!current->is_allocated) {
            printf("起始地址：%d\t大小：%d\n", current->start_address, current->size);
        }
        current = current->next;
    }

    current = memory;
    printf("\n已分配分区：\n");
    while (current != NULL) {
        if (current->is_allocated) {
            printf("起始地址：%d\t大小：%d\n", current->start_address, current->size);
        }
        current = current->next;
    }
}
