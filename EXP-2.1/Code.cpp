#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int partition(int arr[], int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for(int j = low; j < high; j++) {
        if(arr[j] <= pivot) {
            i++;
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if(low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

int main() {
    int n;
    cout << "Enter number of elements (n): ";
    cin >> n;

    int *arr = new int[n];

    srand(time(0));
    for(int i = 0; i < n; i++) {
        arr[i] = rand() % 10000;
    }

    clock_t start = clock();
    quickSort(arr, 0, n - 1);
    clock_t end = clock();

    double timeTaken = double(end - start) / double(CLOCKS_PER_SEC);

    cout << "Time taken to sort " << n << " elements: " << timeTaken << " seconds" << endl;

    delete[] arr;
    return 0;
}
