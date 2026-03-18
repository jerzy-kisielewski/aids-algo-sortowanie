#include <iostream>
#include <chrono>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;
using namespace std::chrono;

void insertionSort(int array[], int n);
void selectionSort(int array[], int n);
void shellSort(int array[], int n);
void heapSort(int array[], int n);
void quickSortLeft(int array[], int low, int high);
void quickSortRandom(int array[], int low, int high);

void heapify(int array[], int n, int i);
int partitionLeft(int array[], int low, int high);
int partitionRandom(int array[], int low, int high);

void generateRandom(int array[], int n);
void generateAscending(int array[], int n);
void generateDescending(int array[], int n);
void generateConstant(int array[], int n);
void generateAshape(int array[], int n);

int main() {
    srand(time(nullptr));

    cout << "**********************\n";
    cout << "q - QuickSort (left pivot)\n";
    cout << "r - QuickSort (random pivot)\n";
    cout << "s - SelectionSort\n";
    cout << "i - InsertionSort\n";
    cout << "h - HeapSort\n";
    cout << "l - ShellSort\n";
    cout << "**********************\n";
    cout << "Podaj rodzaj algorytmu\n";
    char algo;
    cin >> algo;
    ofstream file;

    switch (algo) {
        case 'q': file.open("quick_left.txt"); break;
        case 'r': file.open("quick_random.txt"); break;
        case 's': file.open("selection.txt"); break;
        case 'i': file.open("insertion.txt"); break;
        case 'h': file.open("heap.txt"); break;
        case 'l': file.open("shell.txt"); break;
        default:
            cout << "Zly wybor\n";
            return 0;
    }

    for(int i = 1; i <= 10; i += 1) {
        cout << "Podaj rozmiar tablicy\n";
        int n;
        cin >> n;
        int* array = new int[n];

        cout << "**********************\n";
        cout << "1 - losowe\n";
        cout << "2 - losowe rosnace\n";
        cout << "3 - losowe malejace\n";
        cout << "4 - stale\n";
        cout << "5 - A-ksztaltne\n";
        cout << "**********************\n";
        cout << "Podaj sposob na uzupelnienie tablicy\n";
        int type;
        cin >> type;
        switch (type) {
            case 1: generateRandom(array, n); break;
            case 2: generateAscending(array, n); break;
            case 3: generateDescending(array, n); break;
            case 4: generateConstant(array, n); break;
            case 5: generateAshape(array, n); break;
            default:
                cout << "Zly typ danych\n";
                continue;
        }
        auto start = high_resolution_clock::now();

        switch (algo) {
            case 'q':
                quickSortLeft(array, 0, n - 1);
                break;
            case 'r':
                quickSortRandom(array, 0, n - 1);
                break;
            case 's':
                selectionSort(array, n);
                break;
            case 'i':
                insertionSort(array, n);
                break;
            case 'h':
                heapSort(array, n);
                break;
            case 'l':
                shellSort(array, n);
                break;
        }

        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);

        file << n << " " << duration.count() << endl;

        delete[] array;
    }

    file.close();
    return 0;
}

void insertionSort(int array[], int n) {
    for (int j = 1; j < n; j++) {
        int key = array[j];
        int i = j - 1;

        while (i >= 0 && array[i] > key) {
            array[i + 1] = array[i];
            i--;
        }
        array[i + 1] = key;
    }
}

void selectionSort(int array[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (array[j] < array[min])
                min = j;
        }
        swap(array[i], array[min]);
    }
}

vector<int> sedgewick(int n) {
    vector<int> gaps;
    int k = 0;

    while (true) {
        int gap;
        if (k % 2 == 0)
            gap = 9 * (1 << k) - 9 * (1 << (k/2)) + 1;
        else
            gap = 8 * (1 << k) - 6 * (1 << ((k+1)/2)) + 1;

        if (gap > n) break;
        gaps.push_back(gap);
        k++;
    }

    reverse(gaps.begin(), gaps.end());
    return gaps;
}

void shellSort(int array[], int n) {
    vector<int> gaps = sedgewick(n);

    for (int gap : gaps) {
        for (int i = gap; i < n; i++) {
            int temp = array[i];
            int j = i;

            while (j >= gap && array[j - gap] > temp) {
                array[j] = array[j - gap];
                j -= gap;
            }
            array[j] = temp;
        }
    }
}

void heapify(int array[], int n, int i) {
    int largest = i;
    int l = 2*i + 1;
    int r = 2*i + 2;

    if (l < n && array[l] > array[largest])
        largest = l;

    if (r < n && array[r] > array[largest])
        largest = r;

    if (largest != i) {
        swap(array[i], array[largest]);
        heapify(array, n, largest);
    }
}

void heapSort(int array[], int n) {
    for (int i = n/2 - 1; i >= 0; i--)
        heapify(array, n, i);

    for (int i = n-1; i > 0; i--) {
        swap(array[0], array[i]);
        heapify(array, i, 0);
    }
}

int partitionLeft(int array[], int low, int high) {
    int pivot = array[low];
    int i = low + 1;
    int j = high;

    while (true) {
        while (i <= j && array[i] <= pivot) i++;
        while (array[j] > pivot) j--;

        if (i >= j) break;
        swap(array[i], array[j]);
    }

    swap(array[low], array[j]);
    return j;
}

void quickSortLeft(int array[], int low, int high) {
    if (low < high) {
        int p = partitionLeft(array, low, high);
        quickSortLeft(array, low, p - 1);
        quickSortLeft(array, p + 1, high);
    }
}

int partitionRandom(int array[], int low, int high) {
    int pivotIndex = low + rand() % (high - low + 1);
    swap(array[low], array[pivotIndex]);
    return partitionLeft(array, low, high);
}

void quickSortRandom(int array[], int low, int high) {
    if (low < high) {
        int p = partitionRandom(array, low, high);
        quickSortRandom(array, low, p - 1);
        quickSortRandom(array, p + 1, high);
    }
}

void generateRandom(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % 100;
}

void generateAscending(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = i;
}

void generateDescending(int arr[], int n) {
    for (int i = 0; i < n; i++)
        arr[i] = n - i;
}

void generateConstant(int arr[], int n) {
    int val = rand() % 100;
    for (int i = 0; i < n; i++)
        arr[i] = val;
}

void generateAshape(int arr[], int n) {
    int mid = n / 2;
    int val = 1;
    for (int i = 0; i <= mid; i++) {
        arr[i] = val;
        val += 2;
    }
    val -= 3;
    for (int i = mid + 1; i < n; i++) {
        val -= 2;
        arr[i] = val;
    }
}