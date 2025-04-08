#include <iostream>
#include <math.h>
#include <bits/stdc++.h> 

using namespace std; 

void swap(unsigned long long *a, unsigned long long *b) { 
    int temp = *a; 
    *a = *b; 
    *b = temp; 
}

int maxOfArr(unsigned long long a[], int n){
    int max = a[0];
    for (int i=1; i<n; i++) if (a[i]>max) max = a[i];
    return max;
}
void partition(unsigned long long a[], int low, int high, int &i, int &j) { 
    if (high - low <= 1) { 
        if (a[high] < a[low]) swap(&a[high], &a[low]); 
        i = low; 
        j = high; 
        return; 
    } 
  
    int mid = low; 
    int pivot = a[high]; 
    while (mid <= high) { 
        if (a[mid]<pivot) swap(&a[low++], &a[mid++]); 
        else if (a[mid]==pivot) mid++; 
        else if (a[mid]>pivot) swap(&a[mid], &a[high--]); 
    } 
    i = low-1; 
    j = mid;
}

void quicksort(unsigned long long a[], int low, int high) { 
    if (low>=high) //if there is 1 or 0 elements, return 
        return; 
    int i, j; 
    partition(a, low, high, i, j); 
    quicksort(a, low, i); 
    quicksort(a, j, high); 
}

int removeDuplicates(unsigned long long a[], int n) { 
    if (n==0 || n==1) return n; 
    int j = 0; // To store index of next unique element 
    for (int i=0; i < n-1; i++) 
        if (a[i] != a[i+1]) a[j++] = a[i]; 
    a[j++] = a[n-1]; 
    return j; 
}

int main(){ 
    ios_base::sync_with_stdio(false); 
    cin.tie(NULL);
    // faster input output
    int N, K;
    unsigned long long D, T, Ts, Cs, Tf, Cf;
    cin>>N>>K>>D>>T;
    unsigned long long p[N], c[N], distance[N];
    for (int i=0; i<N; i++) cin>>p[i]>>c[i];
    for (int i=0; i<K; i++) cin>>distance[i];
    cin>>Ts>>Cs>>Tf>>Cf;
    quicksort(distance, 0, K-1);
    K = removeDuplicates(distance, K);
    unsigned long long length[K+1];
    length[0] = distance[0];
    length[K] = D-distance[K-1];
    for (int i=1; i<K; i++) length[i] = distance[i] - distance[i-1];
    quicksort(length, 0, K);
    unsigned long long minCap = 1 + (length[K] - 1) / Cs;
    //minCap is the minimum Capacity required to travel the greatest distance
    long double fast[K+1], slow[K+1], sumFast = 0;
    //with the minCap what is the fastest way to get to each stop
    for (int i=0; i<=K; i++){
        fast[i] = 0;
        slow[i] = 0;
        if (length[i]*Cf<=minCap) fast[i] = length[i];
        else{
            slow[i] = (1.0*(length[i]*Cf - minCap)) / (1.0*(Cf - Cs));
            fast[i] = (1.0*(minCap - length[i]*Cs)) / (1.0*(Cf - Cs));
        }
        sumFast+=fast[i];
    }
    long double reqFast = (1.0*(D*Ts - T)) / (1.0*(Ts - Tf)), dif = reqFast - sumFast, curDif;
    //need to do these many km to arrive exactly in time T to destination with less fast time
    //dif how many more fast km he need to do
    //curDif will be how many he can do in each segment
    unsigned long long needCap = 0;
    int divided = K+1;
    if(dif>0){
        for (int i=0; i<=K; i++){
            if (fast[i]==length[i]) divided--;
            else {
                curDif = slow[i];
                long double average = dif/(divided*1.0);
                if(curDif>=average){
                    needCap = ceil(average*(Cf - Cs));
                    break;
                }
                else {
                    dif-=curDif;
                    divided--;
                }
            }
        }
    }
    minCap+=needCap;
    unsigned long long finalP=0;
    for (int i=0; i<N; i++){
        if(finalP==0 && c[i]>=minCap) finalP = p[i];
        else if (c[i]>=minCap && p[i]<finalP) finalP = p[i];
    }
    if(finalP==0) cout<<"-1\n";
    else cout<<finalP<<endl;
    return 0; 
} 