#include <stdio.h>
#include <stdlib.h>

typedef int(*int_cmp)(void* lhs, void* rhs);

int find_min_elem(int* arr, int size) {
    int min = 0;
    int minv = arr[0];
    int i = 1;
    for(i = 1; i < size; ++i) {
        if(arr[i] < minv) {
            minv = arr[i];
            min = i;
        }
    }

    return i;

}

void sort(int* arr, int size, int_cmp cmp) {
    for(int i = 0; i < size; ++i) {
        int idx = find_min_elem(arr + i, size - i);
        int tmp = arr[i];
        arr[i] = arr[idx];
        arr[idx] = tmp;
    }

    return;
}

int cmp(void* lhs, void* rhs) {
    return *(int*)lhs - *(int*)rhs;
}

int main() {
    int size; scanf("%d", &size);
    int* arr = (int*)calloc(size, sizeof(int));
    for(int i = 0; i < size; ++i) {
        scanf("%d", arr + i);
    }

    sort(arr, size, cmp);
    for(int i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
}