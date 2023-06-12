```markdown
# Custom Memory Allocator

This is a custom memory allocator written in C that provides functionality similar to `malloc`, `free`, and `realloc`. The allocator allows dynamic allocation and deallocation of memory blocks and provides the ability to resize allocated memory.

## Features

- Allocation of memory blocks using `customMalloc`
- Deallocation of memory blocks using `customFree`
- Resizing of memory blocks using `customRealloc`
- Memory blocks are split and merged to optimize memory utilization
- Detailed information about memory blocks and their state can be printed

## Usage

1. Include the `custom_memory_allocator.h` header file in your C program:

   ```c
   #include "custom_memory_allocator.h"
   ```

2. Allocate memory using `customMalloc`:

   ```c
   int* numbers = (int*)customMalloc(5 * sizeof(int));
   if (numbers == NULL) {
       // Handle memory allocation failure
   }
   ```

3. Free memory using `customFree`:

   ```c
   customFree(numbers);
   ```

4. Resize memory using `customRealloc`:

   ```c
   int* resizedNumbers = (int*)customRealloc(numbers, 10 * sizeof(int));
   if (resizedNumbers == NULL) {
       // Handle memory reallocation failure
   }
   ```

5. Print memory block information:

   ```c
   printMemoryBlocks();
   ```

6. Get the total allocated memory:

   ```c
   size_t totalAllocated = getTotalAllocatedMemory();
   ```

## Example

Here's a simple example demonstrating the usage of the custom memory allocator:

```c
#include <stdio.h>
#include "custom_memory_allocator.h"

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

    customFree(resizedNumbers);

    printMemoryBlocks();

    size_t totalAllocated = getTotalAllocatedMemory();
    printf("Total Allocated Memory: %zu bytes\n", totalAllocated);

    return 0;
}
```

## License

This project is licensed under the [MIT License](LICENSE).

Feel free to customize and integrate the custom memory allocator code into your projects as per your requirements.

## Contributions

Contributions to the project are welcome! If you find any issues or have suggestions for improvements, please create an issue or submit a pull request.

## Acknowledgments

This custom memory allocator is inspired by the concepts of dynamic memory allocation and memory management in C.

## References

- [C Programming Language](https://en.wikipedia.org/wiki/C_(programming_language))
- [Memory Allocation in C](https://en.wikipedia.org/wiki/C_dynamic_memory_allocation)
```
