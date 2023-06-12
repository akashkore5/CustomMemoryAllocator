#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

// Structure to represent a memory block
typedef struct MemoryBlock {
    size_t size;                  // Size of the memory block
    bool isFree;                   // Flag to indicate if the block is free
    struct MemoryBlock* next;     // Pointer to the next memory block
} MemoryBlock;

// Head of the memory block list
MemoryBlock* memoryList = NULL;

// Function to split a memory block into two blocks (one allocated and one free)
void splitMemoryBlock(MemoryBlock* block, size_t newSize) {
    MemoryBlock* newBlock = (MemoryBlock*)((char*)block + sizeof(MemoryBlock) + newSize);
    newBlock->size = block->size - sizeof(MemoryBlock) - newSize;
    newBlock->isFree = true;         // Mark the new block as free
    newBlock->next = block->next;
    block->size = newSize;
    block->isFree = false;            // Mark the original block as allocated
    block->next = newBlock;
}

// Function to merge adjacent free memory blocks
void mergeMemoryBlocks() {
    MemoryBlock* block = memoryList;

    while (block != NULL && block->next != NULL) {
        if (block->isFree && block->next->isFree) {
            block->size += sizeof(MemoryBlock) + block->next->size;
            block->next = block->next->next;
        } else {
            block = block->next;
        }
    }
}

// Function to allocate memory similar to 'malloc'
void* customMalloc(size_t size) {
    MemoryBlock* block = memoryList;
    MemoryBlock* prevBlock = NULL;

    while (block != NULL) {
        if (block->isFree && block->size >= size) {
            if (block->size >= size + sizeof(MemoryBlock) + 1) {
                // Split the block if there's enough space for a new free block
                splitMemoryBlock(block, size);
            }
            block->isFree = false;      // Mark the block as allocated
            return (void*)(block + 1);  // Return the address of the block's data
        }
        prevBlock = block;
        block = block->next;
    }

    // No suitable block found, allocate a new block
    size_t totalSize = sizeof(MemoryBlock) + size;
    block = (MemoryBlock*)malloc(totalSize);

    if (block == NULL) {
        return NULL;  // Memory allocation failed, return NULL
    }

    block->size = size;
    block->isFree = false;            // Mark the block as allocated

    if (prevBlock != NULL) {
        block->next = prevBlock->next;
        prevBlock->next = block;
    } else {
        block->next = memoryList;
        memoryList = block;
    }

    return (void*)(block + 1);    // Return the address of the block's data
}

// Function to deallocate memory similar to 'free'
void customFree(void* ptr) {
    if (ptr == NULL) {
        return;
    }

    MemoryBlock* block = (MemoryBlock*)ptr - 1;
    block->isFree = true;            // Mark the block as free
    mergeMemoryBlocks();
}

// Function to reallocate memory similar to 'realloc'
void* customRealloc(void* ptr, size_t newSize) {
    if (ptr == NULL) {
        // If the provided pointer is NULL, equivalent to malloc
        return customMalloc(newSize);
    }

    MemoryBlock* block = (MemoryBlock*)ptr - 1;
    size_t oldSize = block->size;

    if (newSize <= oldSize) {
        // If the requested size is smaller than or equal to the current size,
        // no need to reallocate, return the same pointer
        return ptr;
    }

    // Allocate a new block with the requested size
    void* newPtr = customMalloc(newSize);
    if (newPtr != NULL) {
        // Copy the contents from the old block to the new block
        memcpy(newPtr, ptr, oldSize);
        // Free the old block
        customFree(ptr);
    }

    return newPtr;
}

// Function to print the current state of memory blocks
void printMemoryBlocks() {
    printf("Memory Blocks:\n");
    MemoryBlock* block = memoryList;
    int index = 1;

    while (block != NULL) {
        printf("Block %d: Address: %p, Size: %zu, Free: %s\n", index, block, block->size, block->isFree ? "true" : "false");
        block = block->next;
        index++;
    }
}

// Function to get the total allocated memory
size_t getTotalAllocatedMemory() {
    size_t totalAllocated = 0;
    MemoryBlock* block = memoryList;

    while (block != NULL) {
        if (!block->isFree) {
            totalAllocated += block->size;
        }
        block = block->next;
    }

    return totalAllocated;
}

int main() {
    int* numbers = (int*)customMalloc(5 * sizeof(int));

    if (numbers == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;  // Error occurred, exit with failure
    }

    for (int i = 0; i < 5; i++) {
        numbers[i] = i + 1;
        printf("%d ", numbers[i]);
    }
    printf("\n");

    printf("Memory Blocks after initial allocation:\n");
    printMemoryBlocks();

    int* resizedNumbers = (int*)customRealloc(numbers, 10 * sizeof(int));

    if (resizedNumbers == NULL) {
        fprintf(stderr, "Memory reallocation failed.\n");
        return 1;  // Error occurred, exit with failure
    }

    for (int i = 0; i < 10; i++) {
        resizedNumbers[i] = i + 1;
        printf("%d ", resizedNumbers[i]);
    }
    printf("\n");

    printf("Memory Blocks after reallocation:\n");
    printMemoryBlocks();

    customFree(resizedNumbers);

    printf("Memory Blocks after deallocation:\n");
    printMemoryBlocks();

    // Get total allocated memory
    size_t totalAllocated = getTotalAllocatedMemory();
    printf("Total Allocated Memory: %zu bytes\n", totalAllocated);

    return 0;
}
