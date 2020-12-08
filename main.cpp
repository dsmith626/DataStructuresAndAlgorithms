#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

//get rid of outliers ~ 33% of data from top and bottom of dataset
//reads only middle third of data entries for accuracy

//initialize heap for each hour
void heapify(vector<double> heap, int i){
    int largest = i;
    int left = 2*i + 1;
    int right = 2*i + 2;

    if(left < heap.size() && heap.at(left) > heap.at(largest)){
        largest = left;
    }
    if(right < heap.size() && heap.at(right) > heap.at(largest)){
        largest = right;
    }
    if(largest != i){
        int temp = heap.at(largest);
        heap.at(largest) = heap.at(i);
        heap.at(i) = temp;
        heapify(heap, largest);
    }
}

vector<double> insertHeap(double val, vector<double> heap){
    heap.push_back(val);
    heapify(heap, (int)heap.size()-1);
    return heap;
}

double getMinInHeap(vector<double> heap){
    return heap.at(0);
}

double getMaxInHeap(vector<double> heap){
    return heap.at(heap.size()-1);
}

//initialize bst for each hour
class Node{
public:
    double pricePerHour;
    Node* left;
    Node* right;

    //constructors
    Node(){
        pricePerHour = 0.0;
        left = nullptr;
        right = nullptr;
    }
    Node(double val, Node* l, Node* r){
        pricePerHour = val;
        left = l;
        right = r;
    }
};

Node* insert(Node* head, double val){

    //insert a new node
    if(head == nullptr){
        head = new Node(val, nullptr, nullptr);
        return head;
    }

    //insert
    if(val > head->pricePerHour){
        head->right = insert(head->right, val);
    }
    if(val < head->pricePerHour){
        head->left = insert(head->left, val);
    }
    return head;
}

Node* findMax(Node* head){
    if(head->right == nullptr)
        return head;
    else{
        return findMax(head->right);
    }
}
Node* findMin(Node* head){
    if(head->left == nullptr)
        return head;
    else{
        return findMin(head->left);
    }
}


int main() {

    double distance;
    string time;

    cout << "       UBER PRICE PREDICTOR" << endl;
    cout << "------------------------------------------" << endl;
    cout << "Enter ride distance (miles) : ";
    cin >> distance;
    cout << "Enter current time (military time ex: 13:00) : ";
    cin >> time;
    cout << "------------------------------------------" << endl;

    time = time.substr(0, time.find(":"));
    string file = time + ".csv";

    string val;
    vector<double> heap;
    Node* head = nullptr;

    ifstream ip("/Users/davidsmith/CLionProjects/Project3/" + file);
    if(!ip.is_open())
        cout << "ERROR" << endl;
    while(ip.good()){

        getline(ip, val);
        double v = stod(val);

        heap = insertHeap(v, heap);
        head = insert(head, v);

    }
    ip.close();

    auto heapStartTime = high_resolution_clock::now();
    double heapAvg = (getMaxInHeap(heap) + getMinInHeap(heap)) / 2.0;
    auto heapEndTime = high_resolution_clock::now();
    auto heapTime = duration_cast<microseconds>(heapEndTime - heapStartTime);

    auto treeStartTime = high_resolution_clock::now();
    double treeAvg = (findMax(head)->pricePerHour + findMin(head)->pricePerHour) / 2.0;
    auto treeEndTime = high_resolution_clock::now();
    auto treeTime = duration_cast<microseconds>(treeEndTime - treeStartTime);

    cout << "estimated price using heap: " << heapAvg * distance << endl;
    cout << "heap compute time: " << heapTime.count() << " microseconds" << endl << endl;
    cout << "estimated price using tree: " << treeAvg * distance << endl;
    cout << "tree compute time: " << treeTime.count() << " microseconds" <<endl;

    return 0;
}
