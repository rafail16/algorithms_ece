#include <iostream>
#include <tuple>
#include <list>
using namespace std;

typedef long long ll;
typedef tuple<int,int> int2;

int main(){
    int n, m, k, x;
    cin>>n>>m>>k>>x;
    int isCross[n][m], isVisited[n][m]; //0 if no start of cross, 1 if start of cross
    int2 beginCross[n][m]; //<-1,-1> if no cross from them, <x,y> if cross from them 
    ll grid[n][m][x+1];
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            isCross[i][j] = 0;
            beginCross[i][j] = make_tuple(-1,-1);
            isVisited[i][j] = 0;
        }
    }
    int a, b;
    for(int i=0; i<k; i++){
        cin>>a>>b;
        beginCross[a/m][a%m] = make_tuple(b/m, b%m);
        isCross[a/m][a%m] = 1;
    }
    for (int k=0; k<x+1; k++){
       grid[0][0][k] = 0;
    }
    grid[0][0][0]=1;
    list<int2> queue;
    int2 temp;
    queue.push_back(make_tuple(0,1));
    queue.push_back(make_tuple(1,0));
    isVisited[1][0] = 1;
    isVisited[0][1] = 1;
    while(!queue.empty()){
        temp = queue.front();
        int w = get<0>(temp), y = get<1>(temp);
        queue.pop_front();
        if (isCross[w][y]==1) {
            grid[w][y][0] = 0;
            for(int k=1; k<x+1; k++) grid[w][y][k] = grid[get<0>(beginCross[w][y])][get<1>(beginCross[w][y])][k-1];
            if (w+1<n && isVisited[w+1][y]==0){
                queue.push_back(make_tuple(w+1,y));
                isVisited[w+1][y] = 1;
            }
            if (y+1<m && isVisited[w][y+1]==0){
                queue.push_back(make_tuple(w,y+1));
                isVisited[w][y+1] = 1;
            }
        }
        else {
            if (w==0){
                for(int k=0; k<x+1; k++) grid[w][y][k] = grid[w][y-1][k];
                if (w+1<n && isVisited[w+1][y]==0){
                    queue.push_back(make_tuple(w+1,y));
                    isVisited[w+1][y] = 1;
                }
                if (y+1<m && isVisited[w][y+1]==0){
                    queue.push_back(make_tuple(w,y+1));
                    isVisited[w][y+1] = 1;
                }
            }
            else if (y==0) {
                for(int k=0; k<x+1; k++) grid[w][y][k] = grid[w-1][y][k];
                if (w+1<n && isVisited[w+1][y]==0){
                    queue.push_back(make_tuple(w+1,y));
                    isVisited[w+1][y] = 1;
                }
                if (y+1<m && isVisited[w][y+1]==0){
                    queue.push_back(make_tuple(w,y+1));
                    isVisited[w][y+1] = 1;
                }
            }
            else {
                for(int k=0; k<x+1; k++) grid[w][y][k] = (grid[w-1][y][k] + grid[w][y-1][k])%1000000103;
                if (w+1<n && isVisited[w+1][y]==0){
                    queue.push_back(make_tuple(w+1,y));
                    isVisited[w+1][y] = 1;
                }
                if (y+1<m && isVisited[w][y+1]==0){
                    queue.push_back(make_tuple(w,y+1));
                    isVisited[w][y+1] = 1;
                }
            }
        }
    }
    int sum = 0;
    for(int k=0; k<x+1; k++) {
        sum = (sum+grid[n-1][m-1][k])%1000000103;
    }
    cout<<sum<<endl;
    return 0;
}
