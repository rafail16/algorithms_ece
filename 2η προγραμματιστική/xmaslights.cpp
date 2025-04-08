#include <iostream>
#include <vector>
using namespace std;

int min (int x, int y){
    return (x < y)? x: y;
}
  
// A utility function to add an edge in an 
// undirected graph. 
void addEdge(vector<int> child[], vector<int> par[], int u, int v) { 
    child[u].push_back(v); 
    par[v].push_back(u); 
} 
  
int minVertexCover(vector<int> child[], vector<int> par[], int mVC[], int root){
    if(child[root].size()==0) return 0;
    if(mVC[root]!=0) return mVC[root];
	int size_inc = 1;
	for (auto x : child[root]) size_inc += minVertexCover(child, par, mVC, x);
	
	int size_exc = 0;
	for (auto x : child[root]){
	    size_exc += 1;
	    for (auto y : child[x]) {
	        size_exc += minVertexCover(child, par, mVC, y);
	    }
	}
	
	mVC[root] = min(size_inc, size_exc); 
	return mVC[root]; 
} 

int main() { 
    int n, k, root = 0;
    cin>>n>>k;
    int mVC[n+1];
    for (int i=0; i<n+1; i++) mVC[i]=0;
    vector<int> children[n+1], parent[n+1];
    for (int i=0; i<n-1; i++){
        int first, second;
        cin>>first>>second;
        if(i==0) root = first;
        addEdge(children,parent,first,second);
    }
    cout<<minVertexCover(children, parent, mVC, root)<<'\n';
    return 0; 
} 
