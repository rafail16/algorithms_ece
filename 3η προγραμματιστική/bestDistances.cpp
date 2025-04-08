#include <iostream>  
#include <vector>  
#include <math.h>  
#include <string>  
#include <fstream>   
#include <string.h>   
#include <sstream>   
using namespace std;  
  
string DecimalToBinary(long long int number);  
  
struct Edge {   
    int src, dest, weight;   
};   
   
struct Graph {   
    int V, E;    
    Edge* edge;   
};   
  
struct subset {   
    int parent;   
    int rank;   
};   
  
// A utility function to find set of an element i (uses path compression technique)   
int find(struct subset subsets[], int i) {  
    if (subsets[i].parent != i) subsets[i].parent = find(subsets, subsets[i].parent);   
    return subsets[i].parent;   
}   
  
// A function that does union of two sets of x and y (uses union by rank)   
void Union(struct subset subsets[], int x, int y) {   
    int xroot = find(subsets, x);   
    int yroot = find(subsets, y);   
    // Attach smaller rank tree under root of high rank tree (Union by Rank)   
    if (subsets[xroot].rank < subsets[yroot].rank) subsets[xroot].parent = yroot;   
    else if (subsets[xroot].rank > subsets[yroot].rank) subsets[yroot].parent = xroot;   
    else {//same ranks one root inc by 1   
        subsets[yroot].parent = xroot;   
        subsets[xroot].rank++;   
    }   
}   
  
int boruvkaMST(struct Graph* graph, int A[], int edges[]) {  
    int max = 0;
    int V = graph->V, E = graph->E;   
    Edge *edge = graph->edge;   
    struct subset *subsets = new subset[V];   
    int *cheapest = new int[V];   
    for (int v = 0; v < V; ++v) {   
        subsets[v].parent = v;   
        subsets[v].rank = 0;   
        cheapest[v] = -1;   
    }   
    int numTrees = V;   
    int MSTweight = 0;   
  
    while (numTrees > 1) {   
        // Everytime initialize cheapest array   
        for (int v = 0; v < V; ++v) cheapest[v] = -1;  
        // Traverse through all edges and update cheapest of every component   
        for (int i=0; i<E; i++) {  
            int set1 = find(subsets, edge[i].src);   
            int set2 = find(subsets, edge[i].dest);   
            // If two corners of current edge belong to same set, ignore current edge   
            if (set1 == set2) continue;   
            // Else check if current edge is closer to previous cheapest edges of set1 and set2   
            else{   
                if (cheapest[set1] == -1 || edge[cheapest[set1]].weight > edge[i].weight) cheapest[set1] = i;   
                if (cheapest[set2] == -1 || edge[cheapest[set2]].weight > edge[i].weight) cheapest[set2] = i;   
            }   
        }   
        // Consider the above picked cheapest edges and add them to MST   
        for (int i=0; i<V; i++) {   
            // Check if cheapest for current set exists   
            if (cheapest[i] != -1) {   
                int set1 = find(subsets, edge[cheapest[i]].src);   
                int set2 = find(subsets, edge[cheapest[i]].dest);   
                if (set1 == set2) continue;   
                //MSTweight += edge[cheapest[i]].weight;   
                A[edge[cheapest[i]].src+V*edge[cheapest[i]].dest] = edges[edge[cheapest[i]].src+V*edge[cheapest[i]].dest];  
                A[edge[cheapest[i]].dest+V*edge[cheapest[i]].src] = edges[edge[cheapest[i]].src+V*edge[cheapest[i]].dest];  
                Union(subsets, set1, set2);   
                numTrees--;   
                if (max < edges[edge[cheapest[i]].src+V*edge[cheapest[i]].dest]) max = edges[edge[cheapest[i]].src+V*edge[cheapest[i]].dest];
            }   
        }   
    }  
    return max;   
}   
  
struct Graph* createGraph(int V, int E) {   
    Graph* graph = new Graph;   
    graph->V = V;   
    graph->E = E;   
    graph->edge = new Edge[E];   
    return graph;   
}   
  
int countNodes(int A[], int r, int V, long long times[], int edges[]) {  
    int nodeCount = 1;  
    int childCount = 0;  
    vector<int> helping;  
    for (int i=0; i<V; i++){  
        if(A[r*V+i]!=-1) helping.push_back(i);  
        A[i*V+r] = -1;  
        A[r*V+i] = -1;  
    }  
    if (helping.size()==0) return nodeCount;  
    for (auto i = helping.begin(); i != helping.end(); ++i){  
        childCount = countNodes(A, *i, V, times, edges);  
        //weight.push_back(edges[r+*i*V]);  
        times[edges[r+*i*V]]= childCount*(V-childCount);  
        //cout<<r+1<<' '<<*i+1<<' '<<childCount*(V-childCount)<<'\n';  
        nodeCount += childCount;  
    }   
    return nodeCount;  
}  
  
int main() {  
    int V, E;  
    cin>>V>>E;  
    struct Graph* graph = createGraph(V, E);  
    int a, b, c;  
    int edges[V*V], A[V*V];  
    long long times[E+1];  
    for (int j=0; j<V*V; j++){  
            A[j]=-1;  
            edges[j]=-1;  
    }  
    times[E] = 0;
    for (int i=0; i<E; i++) {  
        times[i] = 0;  
        cin>>a>>b>>c;  
        if(edges[a-1+V*(b-1)]==-1 || edges[a-1+V*(b-1)]>c){  
            graph->edge[i].src=a-1;  
            graph->edge[i].dest=b-1;  
            graph->edge[i].weight=c;  
            edges[a-1+V*(b-1)]=c;  
            edges[b-1+V*(a-1)]=c;  
        }  
    }  
    int maxW = boruvkaMST(graph, A, edges);  
    string str="";  
    int number = countNodes(A, 0, V, times, edges);  
    long long temp, divi, modi; 
    for (int i=0; i<=maxW; i++) {
        temp = times[i];  
        divi = temp/2;  
        modi = temp%2;  
        if (modi==0) str = '0' + str;  
        else str = '1' + str;
        times[i+1] += divi;  
    }
    while (divi!=0) {  
        temp = divi;  
        divi = temp/2;  
        modi = temp%2;  
        if (modi==0) str = '0' + str;  
        else str = '1' + str;  
    }  
    cout<<str<<endl;  
    return 0;   
}  