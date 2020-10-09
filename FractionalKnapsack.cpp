#include <iostream>
#include <algorithm>

using namespace std;

struct Item{
    int value, weight;
    
    ///Constructor
    Item(int value, int weight) : value(value), weight(weight){}
};

///Comparison function to sort Item acc. to val/weight rate
bool cmp(struct Item a, struct Item b){
    double r1 = (double)a.value / a.weight;
    double r2 = (double)b.value / b.weight;
    return r1 > r2;
}

//.Main Greedy Function
double fractionalKnapsack(int W, struct Item arr[], int n){
    //Sort on basis of ratio
    sort(arr, arr+n, cmp);

    int curWeight = 0; //Current weight of the Knapsack
    double finalvalue = 0.0; //Result
    
    for(int i=0; i<n; i++){
        ///If adding items won't overflow, add it completely
        if(curWeight + arr[i].weight <= W){
            curWeight += arr[i].weight;
            finalvalue += arr[i].value;
        }
        ///If we can't add the current item add its fractional part
        else{
            int remain = W - curWeight;
            finalvalue += arr[i].value * ((double)remain / arr[i].weight);
            break;
        }
    }
    return finalvalue;
}

int main(){
    int W = 50; ///Weight of Knapsack
    Item arr[] = {{60, 10}, {100, 20}, {120, 30}};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Maximum value we can obtain = " << fractionalKnapsack(W, arr, n);

    return 0;
}