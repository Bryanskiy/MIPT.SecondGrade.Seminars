#include <stdio.h>
#include <stdlib.h>

typedef int (*cmp_int)(void*, void*);

int cmp(void* lhs, void* rhs) {
    return *(int*)lhs - *(int*)rhs;
}

void sort(int* arr, int n, cmp_int cmp) {
    for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j) {
			int out = cmp(arr + i, arr + j);
			if (out < 0) {
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
			}
		}
}

int main() {
    int n; scanf("%d", &n);
    int* arr = (int*)calloc(n, sizeof(int));
    for(int i = 0; i < n; ++i) {
        scanf("%d", arr + i);
    }

    sort(arr, n, cmp);

    for(int i = 0; i < n; ++i) {
        printf("%d ", arr[i]);
    }
}