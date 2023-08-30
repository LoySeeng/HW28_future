#include <iostream>
#include <future>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <ctime>

unsigned int conc = std::thread::hardware_concurrency();
unsigned int num_threads = 0;


void merge(int array[], int const left, int const mid, int const right)
{
    int const subArr_L = mid - left + 1;
    int const subArr_R = right - mid;

    auto* L_Arr = new int[subArr_L];
    auto *R_Arr = new int[subArr_R];

    for (auto i = 0; i < subArr_L; i++)
        L_Arr[i] = array[left + i];
    for (auto j = 0; j < subArr_R; j++)
        R_Arr[j] = array[mid + 1 + j];

    auto index_Arr_L = 0;
    auto index_Arr_R = 0;
    int index_Merge = left;

    while (index_Arr_L < subArr_L && index_Arr_R < subArr_R) {
        if (L_Arr[index_Arr_L] <= R_Arr[index_Arr_R]) {
            array[index_Merge] = L_Arr[index_Arr_L];
            index_Arr_L++;
        }
        else {
            array[index_Merge] = R_Arr[index_Arr_R];
            index_Arr_R++;
        }
        index_Merge++;
    }

    while (index_Arr_L < subArr_L) {
        array[index_Merge] = L_Arr[index_Arr_L];
        index_Arr_L++;
        index_Merge++;
    }

    while (index_Arr_R < subArr_R) {
        array[index_Merge] = R_Arr[index_Arr_R];
        index_Arr_R++;
        index_Merge++;
    }

    delete[] L_Arr;
    delete[] R_Arr;
}


void mergeSort(int array[], int const begin, int const end) {
    
    if (begin >= end)
        return;

    int mid = begin + (end - begin) / 2;
   
    // Количество элементов для включения фьючерс можно изменять в зависимости от размера массива
    if ((end - begin) > 100 && num_threads <= conc) {
        num_threads++;
        std::future<void> backMS1 = std::async(std::launch::async, mergeSort, std::ref(array), begin, mid);
        std::future<void> backMS2 = std::async(std::launch::async, mergeSort, std::ref(array), mid + 1, end);
        
        backMS1.wait();
        backMS2.wait();
        merge(array, begin, mid, end);
    }
    else {
        mergeSort(array, begin, mid);
        mergeSort(array, mid + 1, end);

        merge(array, begin, mid, end);
    }
}

// Не использовала, т.к. массивы достаточно большие
void print(int array[], long n) {
    std::cout << "Array: ";
    for (long i = 0; i < n; i ++) {
        std::cout << array[i] << " ";
    }
}


int main() {

    std::srand(0);


    long N = 10000;
    int* Array = new int[N];
    for (long i = 0; i < N; i++) {
        Array[i] = 1 + std::rand() % 100;
    }

    mergeSort(Array, 0, N - 1);

    delete[] Array;

    return 0;

}