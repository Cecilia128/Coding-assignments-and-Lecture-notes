//
//  AS1_MergeSort_118020046.cpp
//  Demo
//
//  Created by Cecilia on 2020/11/5.
//  Copyright © 2020 Cecilia. All rights reserved.
//

#include <iostream>

using namespace std;

void merge(int aux[], int arr[], int p, int q, int r) {
    for (int m = p; m <=r; m++) aux[m] = arr[m];
    
    int i = p;
    int j = q+1;
    for (int m = p; m <= r; m++) {
        if (i > q) arr[m] = aux[j++];
        else if (j > r) arr[m] = aux[i++];
        else if (aux[i] > aux[j]) arr[m] = aux[j++];
        else arr[m] = aux[i++];
    }
}

void merge_sort(int aux[], int arr[], int p, int r){
    if (p < r) {
        int q = (p+r)/2;
        merge_sort(aux, arr, p,q);
        merge_sort(aux, arr, q+1,r);
        merge(aux, arr, p, q, r);
    }
}


int main()
{
    int N;
    cout << "";
    cin >> N;
    int arr[N];
    for (int i = 0; i < N; i++) {
        cin >> arr[i];
    }
    int aux[N];
    int p = 0;
    int r = N-1;
    merge_sort(aux, arr, p, r);
    
    for (int i = 0; i < N; i++) cout << arr[i] << endl;
}
