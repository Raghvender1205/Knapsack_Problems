/**
 * Branch and Bound can be solved using FIFO, LIFO and LC strategies. 
 * The least cost(LC) is considered the most intelligent as it selects the next node based on a Heuristic Cost Function. 
 * It picks the one with the least cost.


As 0/1 Knapsack is about maximizing the total value, we cannot directly use the LC Branch and Bound technique to solve this. Instead, we convert this into a minimization problem by taking negative of the given values.

Follow the steps below to solve the problem:

. Sort the items based on their value/weight(V/W) ratio.

. Insert a dummy node into the priority queue.

. Repeat the following steps until the priority queue is empty:

. Extract the peek element from the priority queue and assign it to the current node.
. If the upper bound of the current node is less than minLB, the minimum lower bound of all the nodes explored, 
    then there is no point of exploration. So, continue with the next element. The reason for not considering the nodes
    whose upper bound is greater than minLB is that, the upper bound stores the best value that might be achieved. 
    If the best value itself is not optimal than minLB, then exploring that path is of no use.
    Update the path array.

. If the current node’s level is N, then check whether the lower bound of the current node is less than finalLB, minimum lower bound of all the paths that reached the final level. If it is true, update the finalPath and finalLB. Otherwise, continue with the next element.

. Calculate the lower and upper bounds of the right child of the current node.

. If the current item can be inserted into the knapsack, then calculate the lower and upper bound of the left child of the current node.

. Update the minLB and insert the children if their upper bound is less than minLB
 * */

////Using LC Branch and Bound (Heuristic Cost Function)
#include <iostream>
#include <algorithm>
#include <queue>
#include <vector> 

using namespace std;

int size; //Store the number of items

float capacity; //Knapsack Capacity

typedef struct Item{
    float weight;
    int value;
    int idx; //stores the index of items
} Item;

typedef struct Node{
    //Upper Bound: Best Case (Fractional Knapsack)
    float ub;
    //lower Bound: Worst Case (0/1)
    float lb;
    ///Level of the node in the decision tree
    int level;
    ///Flag if the current Item is selected or not
    bool flag;
    ///Total Value
    float tv;
    //Total Weight
    float tw;
} Node;

///Function to calculate Upper_bound(includes Fractional Knapsack parts of the item)
float upper_bound(float tv, float tw, int idx, vector<Item>& arr){
    float value = tv;
    float weight = tw;
    for(int i=idx; i < size; i++){
        if(weight + arr[i].weight <= capacity){
            weight += arr[i].weight;
            value -= arr[i].value;
        }
        else{
            value -= (float)(capacity - weight) /  arr[i].weight * arr[i].value;
            break;
        }
    }
    return value;
}

///Function to cal. lower_bound (doesn't include fractional part)
float lower_bound(float tv, float tw, int idx, vector<Item>& arr){
    float value = tv;
    float weight = tw;
    for(int i=idx; i<size; i++){
        if(weight + arr[i].weight <= capacity){
            weight += arr[i].weight;
            value -= arr[i].value;
        }
        else{
            break;
        }
    }
    return value;
}

class comp{
    public:
        bool operator() (Node a, Node b){
            return a.lb > b.lb;
        }
};

void assign(Node& a, float ub, float lb, int level, bool flag, float tv, float tw){
    a.ub = ub;
    a.lb = lb;
    a.level = level;
    a.flag = flag;
    a.tv = tv;
    a.tw = tw;
}

void knapsack(vector<Item>& arr){
    ///Sort the items based on the profit/weight ratio
    sort(arr.begin(), arr.end(), [&](Item& a, Item& b){
        return a.value / a.weight > b.value / b.weight; 
    });

    ///min_lb --> Minimum Lower bound of all the nodes explored
    
    ///final_lb --> Minimum lower bound of all the paths that reached the final level
    float min_lb = 0, final_lb = INT_MAX;

    ///curr_path -->Bool arr to store at every index if the element is included or not
    ///final_path --> bool arr to store the result of selection arr when it reached the last level
    bool curr_path[size], final_path[size];

    ///priority_queue to store the nodes based on lower_bounds
    priority_queue<Node, vector<Node>, comp> pq;

    Node current, left, right;
    current.lb = current.ub = current.tw = current.tv = current.level = current.flag = 0;

    ///Insert a dummy Node
    pq.push(current);
    for(int i=0; i<size; i++){
        curr_path[i] = final_path[i] = false;
    }

    while(!pq.empty()){
        current = pq.top();
        pq.pop();

        if(current.ub  > min_lb || current.ub >= final_lb){
            // If the current node's best case 
            // value is not optimal than min_lb, 
            // then there is no reason to explore 
            // that path including final_lb 
            // eliminates all those paths whose 
            // best values is equal to final_lb 
            continue;
        }
        ///Update Path
        if(current.level != 0){
            curr_path[current.level - 1] = current.flag;
        }
        if(current.level == size){
            //Reached Last Level
            if(current.lb < final_lb){
                for(int i=0; i<size; i++){
                    final_path[arr[i].idx] = curr_path[i];
                }
            }
            final_lb = min(current.lb, final_lb);
            continue;
        }

        int level = current.level;
        ///rightNode --> Excludes current Item, Hence cp, cw will obtain the value of that parent
        assign(right, upper_bound(current.tv, current.tw, level+1, arr), lower_bound(current.tv, current.tw, level+1, arr), level+1, false, current.tv, current.tw);

        ///Check whether adding the element item will not exceed the knapsack weight
        // Capacity(c) and lb should be calculated including the current item
        if(current.tw + arr[current.level].weight <= capacity){
            left.ub = upper_bound(current.tv - arr[level].value, current.tw - arr[level].weight, level+1, arr);
            left.lb = lower_bound(current.tv - arr[level].value, current.tw - arr[level].weight, level+1, arr);

            assign(left, left.ub, left.lb, level+1, true, current.tv - arr[level].value, current.tw + arr[level].weight);
        }
        ///If left Node can be inserted 
        else{
            ///Stop the left node from getting added to the Priority_Queue.
            left.ub = left.lb = 1;
        }

        //Update the Lower bound
        min_lb = min(min_lb, left.lb);
        min_lb = min(min_lb, right.lb);

        //Exploring nodes whose ub > min_lb will never give the optimal result
        if(min_lb >= left.ub){
            pq.push(left);
        }
        if(min_lb >= right.ub){
            pq.push(right);
        }
    }
    cout << "Items taken into the" << "knapsack are: \n";
    if(final_lb == INT_MAX){
        final_lb = 0;
    }
    for(int i=0; i<size; i++){
        cout << final_path[i] << " ";
    }
    cout << endl;
    cout << "Maximum Profit is: "<< (-final_lb) << endl;
}

int main(){
    size = 4;
    capacity = 15;

    vector<Item> arr;
    arr.push_back({2, 10, 0});
    arr.push_back({4, 10, 1});
    arr.push_back({6, 12, 2});
    arr.push_back({9, 18, 3});

    knapsack(arr);

    return 0;
}