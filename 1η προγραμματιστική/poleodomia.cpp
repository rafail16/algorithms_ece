#include <iostream>
#include <stack>

using namespace std;

int main()
{
    int N;
    cin >> N;
    unsigned long height[N], top;
    unsigned long long left[N], right[N], total[N];
    stack <unsigned long long> s, k;
    for (int i=0; i<N; i++){
        cin >> height[i];
    }
    left[0] = height[0];
    right[N-1] = height[N-1];
    s.push(height[0]);
    k.push(0);
    for (int i=1; i<N; i++){
        while(!s.empty() && s.top()<=height[i]){
            s.pop();
            k.pop();
        }
        s.push(height[i]);
        k.push(i);
        if(s.size()==1) left[i] = (k.top()+1)*height[i];
        else {
            top = k.top();
            k.pop();
            left[i] = left[k.top()] + (i-k.top())*height[i];
            k.push(top);
        }
    }
    while(!s.empty()){
        s.pop();
        k.pop();
    }
    s.push(height[N-1]);
    k.push(N-1);
    for (int i=N-2; i>=0; i--){
        while(!s.empty() && s.top()<=height[i]){
            s.pop();
            k.pop();
        }
        s.push(height[i]);
        k.push(i);
        if(s.size()==1) right[i] = (N-k.top())*height[i];
        else {
            top = k.top();
            k.pop();
            right[i] = right[k.top()] + (k.top()-i)*height[i];
            k.push(top);
        }
    }
    unsigned long long minimum = left[0]+right[0]-height[0];
    for(int i=1; i<N; i++){
        total[i] = left[i]+right[i]-height[i];
        if (total[i] < minimum) minimum = total[i];
    }
    cout<<minimum<<endl;
    return 0;
}
