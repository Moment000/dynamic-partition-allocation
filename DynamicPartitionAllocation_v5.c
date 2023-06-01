#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <locale.h>

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

void firstFitAllocation(int size) {
    int i;
    for (i = 0; i < numBlocks; i++) {
        if (!memoryBlocks[i].allocated && memoryBlocks[i].size >= size) {
            memoryBlocks[i].allocated = 1;
            memoryBlocks[i].size -= size;
            memoryBlocks[numBlocks].start = memoryBlocks[i].start + size;
            memoryBlocks[numBlocks].size = memoryBlocks[i].size;
            memoryBlocks[numBlocks].allocated = 0;
            numBlocks++;
            printf("成功：分配了 %d 字节的内存块。\n", size);
            return;
        }
    }
    printf("错误：没有可用的内存块进行分配。\n");
}

void bestFitAllocation(int size) {
    MemoryBlock* bestBlock = NULL;
    int smallestSize = INT_MAX;
    int i;

    for (i = 0; i < numBlocks; i++) {
        if (!memoryBlocks[i].allocated && memoryBlocks[i].size >= size &&
            memoryBlocks[i].size < smallestSize) {
            smallestSize = memoryBlocks[i].size;
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
        printf("成功：分配了 %d 字节的内存块。\n", size);
    } else {
        printf("错误：没有可用的内存块进行分配。\n");
    }
}

void deallocateMemoryBlock(int startAddress) {
    int i;
    for (i = 0; i < numBlocks; i++) {
        if (memoryBlocks[i].start == startAddress && memoryBlocks[i].allocated) {
            memoryBlocks[i].allocated = 0;
            printf("成功：回收了起始地址为 %d 的内存块。\n", startAddress);
            return;
        }
    }
    printf("错误：未找到或未分配起始地址为 %d 的内存块。\n", startAddress);
}

void printMemoryStatus() {
    int i;
    printf("当前内存状态：\n");
    printf("-----------------------\n");
    for (i = 0; i < numBlocks; i++) {
        printf("块 %d：起始地址=%d, 大小=%d, 分配状态=%s\n",
               i + 1, memoryBlocks[i].start, memoryBlocks[i].size,
               memoryBlocks[i].allocated ? "已分配" : "未分配");
    }
    printf("-----------------------\n");
}

int main() {

    setlocale(LC_ALL, "");  // 设置输出编码为终端默认编码


    int size;
    int option;
    int blockSize;

    printf("请输入内存大小（字节）：");
    scanf("%d", &size);

    initializeMemory(size);

    while (1) {
        printf("\n");
        printf("1. 分配内存块（首次适应）\n");
        printf("2. 分配内存块（最佳适应）\n");
        printf("3. 回收内存块\n");
        printf("4. 打印内存状态\n");
        printf("5. 退出\n");
        printf("请选择操作：");
        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("请输入要分配的内存块大小（字节）：");
                scanf("%d", &blockSize);
                firstFitAllocation(blockSize);
                // printMemoryStatus();
                break;
            case 2:
                printf("请输入要分配的内存块大小（字节）：");
                scanf("%d", &blockSize);
                bestFitAllocation(blockSize);
                // printMemoryStatus();
                break;
            case 3:
                printf("请输入要回收的内存块起始地址：");
                scanf("%d", &blockSize);
                deallocateMemoryBlock(blockSize);
                // printMemoryStatus();
                break;
            case 4:
                printMemoryStatus();
                break;
            case 5:
                free(memoryBlocks);
                printf("程序已退出。\n");
                exit(0);
                break;
            default:
                printf("错误：无效的选项。\n");
                break;
        }
    }

    return 0;
}
