#include <iostream>
#include <fstream>
#include <ctime>
#include <limits>
#include <string>
#include <algorithm>
using namespace std;

#define MAX_AIRPORTS 50
#define MAX_WAIT 100
#define INF 1e9

class Flight;

class SeatRequest {
public:
    string name;
    int id;
    int priority;
    int timestamp;
    SeatRequest() {}
    SeatRequest(string n, int id, int p, int t) : name(n), id(id), priority(p), timestamp(t) {}
};

class SeatHeap {
    SeatRequest arr[MAX_WAIT];
    int size;
    int timeCount;

public:
    SeatHeap()
    {
        size = 0;
        timeCount = 0;
    }
    int getSize() { 
        return size;
    }

    int parent(int i) { 
        return (i - 1) / 2; 
    }
    int left(int i) { 
        return 2 * i + 1; 
    }
    int right(int i) { 
        return 2 * i + 2; 
    }


    SeatRequest get(int i) { 
        return arr[i]; 
    }
    void swap(SeatRequest &a, SeatRequest &b){
        SeatRequest t = a;
        a = b;
        b = t;
    }
    
    void heapifyUp(int i){
        while (i > 0){
            int p = parent(i);

            bool childBetter =
                (arr[i].priority > arr[p].priority) ||
                (arr[i].priority == arr[p].priority &&
                arr[i].timestamp > arr[p].timestamp);

            if (!childBetter)
                break;

            swap(arr[i], arr[p]);
            i = p;
        }
    }

    void push(string name, int ID,  int priority){
        if (size >= MAX_WAIT){
        cout << "Error: Waitlist is full!" << endl;
            return;
        }
        arr[size] = SeatRequest(name, ID, priority, ++timeCount);
        heapifyUp(size);
        size++;
    }

    void heapifyDown(int i) {
        while (true) {
            int l = 2 * i + 1;
            int r = 2 * i + 2;
            int best = i;

            if (l < size && 
                (arr[l].priority > arr[best].priority ||
                (arr[l].priority == arr[best].priority &&
                arr[l].timestamp > arr[best].timestamp))) {
                best = l;
            }
            if (r < size &&
                (arr[r].priority > arr[best].priority ||
                (arr[r].priority == arr[best].priority &&
                arr[r].timestamp > arr[best].timestamp))) {
                best = r;
            }

            if (best == i){
                break;
            } 
            swap(arr[i], arr[best]);
            i = best;
        }

        
    }

    bool hasPassengerID(int ID) {
        for (int i = 0; i < size; i++) {
            if (arr[i].id == ID) {
                return true;
            }
        }
        return false;
    }

    SeatRequest pop() {
        if (size==0){
            return SeatRequest();
        }

        SeatRequest top = arr[0];
        size--;
        if (size > 0){
            arr[0] = arr[size];
            heapifyDown(0);
        }
        return top;
    }
    
    bool empty() { 
        return size == 0; 
    }

    void displayWaitlist()
    {
        if (size == 0)
        {
            cout << "Waitlist empty.\n";
            return;
        }
        cout << "Waitlist:\n";
        for (int i = 0; i < size; i++)
            cout << i + 1 << ". " << arr[i].name << " (Priority " << arr[i].priority << ")\n";
    }

    void removePassenger(int ID) {
        for (int i = 0; i < size; i++) {
            if (arr[i].id == ID) {
                size--;
                if (i < size){
                    arr[i] = arr[size];
                    heapifyUp(i);
                    heapifyDown(i);
                }
                cout << "Passenger " << ID << " removed from list found in waitlist.\n";
                return;
            }
        }
        cout << "Passenger with ID " << ID << " not found in waitlist.\n";
    }

    void modifyPriority(const string &name, int ID,  int newPriority) {
        for (int i = 0; i < size; i++) {
            if (arr[i].id == ID) {
                arr[i].priority = newPriority;
                heapifyUp(i);
                heapifyDown(i);
                cout << "Priority updated for " << name << "\n";
                return;
            }
        }
        cout << "Passenger not found in waitlist.\n";
    }
};

class Flight {
    string id;
    string airline, departureTime, arrivalTime;
    string origin, dest;
    double price;
    int capacity, booked;
    SeatHeap waitlist;
    Flight *left;
    Flight *right;
    struct Passengers{
        string name;
        int id;
    };
    Passengers* bookedPassengers;

public:
    Flight(string fid, string a, string o, string d, string dep, string arr, double p, int c, int b = 0) {
        id = fid; 
        airline = a; 
        origin = o; 
        dest = d;
        departureTime = dep; 
        arrivalTime = arr; 
        price = p; 
        capacity = c; 
        booked = b;
        left = right = nullptr;
        bookedPassengers = new Passengers[capacity];
    }
    ~Flight(){
        delete[] bookedPassengers;
    }

    void addPassenger(const string &name, int ID) {
        if (booked < capacity) {
            bookedPassengers[booked].name = name;
            bookedPassengers[booked].id = ID;
        }
    }
    
    void setID(const string& newID){
        id=newID;
    }

    Flight *getLeft() { 
        return left; 
    }
    Flight *getRight() { 
        return right; 
    }
    void setLeft(Flight *l) { 
        left = l; 
    }
    void setRight(Flight *r) { 
        right = r; 
    }

    string getID() { 
        return id; 
    }

    string getOrigin() { 
        return origin; 
    }

    string getAirline() { 
        return airline; 
    }

    string getDepTime() { 
        return departureTime; 
    }
    string getArrTime() { 
        return arrivalTime; 
    }

    string getDest() { 
        return dest; 
    }
    double getPrice() { 
        return price; 
    }
    int getCapacity() { 
        return capacity; 
    }
    int getBooked() { 
        return booked; 
    }
    void bookSeat() { 
        if (booked < capacity){
            booked++; 
        }
    }
    void cancelSeat() {
        if (booked > 0) {
            booked--;
        }
    }

    void copyFrom(Flight* f){
        this->id = f->id;
        this->airline = f->airline;
        this->origin = f->origin;
        this->dest = f->dest;
        this->departureTime = f->departureTime;
        this->arrivalTime = f->arrivalTime;
        this->price = f->price;
        this->capacity = f->capacity;
        this->booked = f->booked;
    }

    SeatHeap &getWaitlist() { 
        return waitlist; 
    }

    bool cancelSeatByPassenger(const string &name, int ID) {
        for (int i = 0; i < booked; i++) {
            if (bookedPassengers[i].id == ID && bookedPassengers[i].name == name) {
                bookedPassengers[i] = bookedPassengers[booked-1];
                booked--;
                return true;
            }
        }
        return false;
    }

    void display() {
    cout << "Flight " << id << " | " << airline << " | " << origin << " -> " << dest
         << " | Departure: " << departureTime << " Arrival: " << arrivalTime
         << " | $" << price << " | Seats: " << booked << "/" << capacity << endl;
    }

};

class BST {
    Flight *root;

    Flight *insert(Flight *node, Flight *f) {
        if (!node) {
            return f;
        }
        if (f->getID() < node->getID()){ 
            node->setLeft(insert(node->getLeft(), f));
        }
        else if (f->getID() > node->getID()) {
            node->setRight(insert(node->getRight(), f));
        }
        return node;
    }

    Flight *findMin(Flight *node) {
        while (node->getLeft()) {
            node = node->getLeft();
        }
        return node;
    }

    Flight *removeNode(Flight *node, string id) {
        if (!node) {
            return nullptr;
        }

        if (id < node->getID()) {
            node->setLeft(removeNode(node->getLeft(), id));
        }  

        else if (id > node->getID()){ 
            node->setRight(removeNode(node->getRight(), id));
        }

        else {
            if (!node->getLeft()) {
                Flight *temp = node->getRight();
                delete node;
                return temp;
            }
            else if (!node->getRight()) {
                Flight *temp = node->getLeft();
                delete node;
                return temp;
            }
            else {
                Flight *temp = findMin(node->getRight());
                node->copyFrom(temp);
                node->setRight(removeNode(node->getRight(), temp->getID()));
            }
        }
        return node;
    }

    void deleteTree(Flight *node) {
        if (!node) {
            return;
        }
        deleteTree(node->getLeft());
        deleteTree(node->getRight());
        delete node; 
    }

public:
    BST() { 
        root = nullptr; 
    }

    Flight *getRoot() { 
        return root; 
    }

    void insertFlight(string id, string airline, string o, string d, string dT, string aT, double p, int c, int b = 0) {
        Flight *f = new Flight(id, airline, o, d, dT, aT, p, c, b);
        root = insert(root, f);
    }

    Flight *find(Flight *node, string id) {
        if (!node) {
            return nullptr;
        } 
        if (id == node->getID()) {
            return node;
        }
        if (id < node->getID()) {
            return find(node->getLeft(), id);
        }
        return find(node->getRight(), id);
    }

    void inorder(Flight *node) {
        if (!node){ 
            return;
        }
        inorder(node->getLeft());
        node->display();
        inorder(node->getRight());
    }

    void displayByOriginDest(Flight *node, string origin, string dest) {
        if (!node)
            return;
        displayByOriginDest(node->getLeft(), origin, dest);
        if ((origin == "" || node->getOrigin() == origin) && (dest == "" || node->getDest() == dest))
            node->display();
        displayByOriginDest(node->getRight(), origin, dest);
    }

    void sortByPrice(Flight *node, Flight **arr, int &index) {
        if (!node){
            return;
        }
        sortByPrice(node->getLeft(), arr, index);
        arr[index++] = node;
        sortByPrice(node->getRight(), arr, index);
    }

    void deleteFlight(string id) { 
        root = removeNode(root, id); 
    }

    ~BST() {
        deleteTree(root);
        root = nullptr; 
    }
};

class Airport {
public:
    string name;
    int index;
    Airport() { index = -1; }
};

class AirportTable {
    Airport table[MAX_AIRPORTS];
    int used[MAX_AIRPORTS];

public:
    AirportTable() {
        for (int i = 0; i < MAX_AIRPORTS; i++)
            used[i] = 0;
    }
    int hashFunc(const string &key) {
        unsigned int h = 0;
        for (int i = 0; i < key.size(); i++)
            h = (h * 31 + key[i]) % MAX_AIRPORTS;
        return h;
    }
    int getAirportIndex(const string &name, int &count) {
        int h = hashFunc(name), start = h;
        while (used[h] && table[h].name != name) {
            h = (h + 1) % MAX_AIRPORTS;
            if (h == start)
                return -1;
        }
        if (!used[h]) {
            table[h].name = name;
            table[h].index = count++;
            used[h] = 1;
        }
        return table[h].index;
    }

    string getAirportName(int index) const {
        for (int i=0; i<MAX_AIRPORTS; i++){
            if(table[i].index == index){
                return table[i].name;
            }
        }
        cout << "ERROR! Airport Index "<< index << " not found!\n";
        return "Unknown";
    }
};

class EdgeNode {
public:
    int dest;
    double cost;
    EdgeNode *next;
    EdgeNode(int d, double c, EdgeNode *n) : dest(d), cost(c), next(n) {}
};

class Graph {
public:
    EdgeNode *adj[MAX_AIRPORTS];
    int airportCount;

    Graph() {
    airportCount = 0;
    for (int i = 0; i < MAX_AIRPORTS; i++)
    adj[i] = nullptr;
    }

    void addEdge(int src, int dest, double cost) { 
        if (src < 0 || src>=airportCount || dest < 0 || dest >= airportCount){
            cout<<"ERROR! Invalid Edge!\n"<< src << "-->" << dest << " is out of range!\n";
            return;
        }
        EdgeNode* newEdge = new EdgeNode(dest, cost, adj[src]);
        adj[src] = newEdge;
    }

    double findShortestPath(int src, int dest, const AirportTable &airports) {
        if (src == -1 || dest == -1) {
            cout<< "ERROR: Invalid Airport Selection!\n";
            return INF; 
        }
        if (airportCount < 2) {
            cout << "Not enough airports to compute routes!\n";
            return INF;
        }

        double dist[MAX_AIRPORTS];
        int parent[MAX_AIRPORTS];
        bool vis[MAX_AIRPORTS];
        
        for (int i = 0; i < airportCount; i++) {
            dist[i] = INF;
            vis[i] = false;
            parent[i] = -1;
        }
        dist[src] = 0;
        
        for (int count = 0; count < airportCount - 1; count++) {
            double minv = INF;
            int u = -1;
            for (int i = 0; i < airportCount; i++) {
                if (!vis[i] && dist[i] <= minv) {
                    minv = dist[i];
                    u = i;
                }
            }

            if (u == -1) break; 
            vis[u] = true;
            
            for (EdgeNode* e = adj[u]; e; e = e->next)
                if (!vis[e->dest] && dist[u] + e->cost < dist[e->dest]) {
                    dist[e->dest] = dist[u] + e->cost;
                    parent[e->dest] = u;
                }
        }
        if (dist[dest] == INF) {
            cout<< "No flight exists between the selected airports!\n";
            return INF;
        }    
            
        cout << "Cheapest cost: $" << dist[dest] << "\nPath: ";
        int path[MAX_AIRPORTS], len = 0;
        for (int v = dest; v != -1; v = parent[v]){
            path[len++] = v;
        }

        for (int i = len - 1; i >= 0; i--) {
            cout << airports.getAirportName(path[i]);
            if (i) {
                cout << " -> ";
            }
            else {
                cout << "\n";
            }
        }

        return dist[dest];
    }

    ~Graph() {
        for (int i = 0; i < MAX_AIRPORTS; i++) {
            EdgeNode* current = adj[i];
            while (current) {
                EdgeNode* next = current->next;
                delete current;
                current = next;
            }
            adj[i] = nullptr;
        }
    }
};

class FileManager {
public:
    static void saveFlights(Flight *node, ofstream &fout, ofstream &wf) {
        if (!node)
            return;
        saveFlights(node->getLeft(), fout, wf);

        fout << node->getID() << " " << node->getAirline() << " " 
             << node->getOrigin() << " " << node->getDest() << " "
             << node->getDepTime() << " " << node->getArrTime() << " "
             << node->getPrice() << " " << node->getCapacity() << " " 
             << node->getBooked() << "\n";

        SeatHeap &w = node->getWaitlist();
        for (int i = 0; i < w.getSize(); i++)
            wf << node->getID() << " " << w.get(i).name << " "  << w.get(i).id << " " << w.get(i).priority << "\n";

        saveFlights(node->getRight(), fout, wf);
    }

    static void saveAllFlights(Flight *root) {
        ofstream fout("flights.txt");      
        ofstream wf("waitlists.txt");    
        saveFlights(root, fout, wf);
        fout.close();
        wf.close();
    }

    static void loadFlights(BST &flights, Graph &g, AirportTable &airports, int &airportCount)
{
    ifstream fin("flights.txt");
    if (fin) {
        int cap, booked;
        string id, airline, aT, dT;
        double price;
        string o, d;
        while (fin >> id >> airline >> o >> d >> dT >> aT >> price >> cap >> booked) {
            flights.insertFlight(id, airline, o, d, dT, aT, price, cap, booked);
            int oi = airports.getAirportIndex(o, airportCount);
            int di = airports.getAirportIndex(d, airportCount);
            g.airportCount = airportCount;  // Update airportCount continuously
            g.addEdge(oi, di, price);
        }
        fin.close();
    }
    
    ifstream win("waitlists.txt");  
    if (win) {
        int pr, ID;
        string fid, name;
        while (win >> fid >> name >> ID >> pr)
        {
            Flight *f = flights.find(flights.getRoot(), fid);
            if (f)
                f->getWaitlist().push(name, ID, pr);
        }
        win.close();
    }
}

    static void logPassenger(const string &action, string fid, const string &name) {
        ofstream fout("passenger_history.txt", ios::app);
        fout << action << " FlightID:" << fid << " Passenger:" << name << "\n";
        fout.close();
    }
};

void collectFlights(Flight* node, const string &origin, const string &dest, Flight* arr[], int &idx) {
    if (!node) return;

    collectFlights(node->getLeft(), origin, dest, arr, idx);

    if ((origin.empty() || node->getOrigin() == origin) &&
        (dest.empty() || node->getDest() == dest)) {
        arr[idx++] = node;
    }

    collectFlights(node->getRight(), origin, dest, arr, idx);
}

void swap(Flight* &a, Flight* &b) {
    Flight* t = a;
    a = b;
    b = t;
}

int partition(Flight** arr, int low, int high) {
    double pivotPrice = arr[high]->getPrice(); 
    
    int i = (low - 1); 

    for (int j = low; j <= high - 1; j++) {
        if (arr[j]->getPrice() < pivotPrice) {
            i++; 
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

void quickSort(Flight** arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quickSort(arr, pi + 1, high);
    }
}

void testRuntimes(BST &flights, Graph &g, AirportTable &airports, int airportcount) {
    cout << "\nTESTING RUNTIMES\n\n";
    
    Flight* arr[1000];
    int count = 0;
    flights.sortByPrice(flights.getRoot(), arr, count);
    
    if (count == 0) {
        cout << "No flights to test. Please add flights first.\n";
        return;
    }
    
    cout << "Testing with " << count << " flights\n\n";
    
    cout << "BST Search Test\n";
    clock_t start = clock();
    flights.find(flights.getRoot(), "F250");
    clock_t end = clock();
    double time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
    cout << "BST Search for F250: " << time << " ms\n\n";
    
    cout << "Linear Search Test\n";
    start = clock();
    for (int i = 0; i < count; i++) {
        if (arr[i]->getID() == "F250") break;
    }
    end = clock();
    time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
    cout << "Linear Search for F250: " << time << " ms\n\n";
    
    cout << "Selection Sort Test\n";
    Flight* arr1[1000];
    for (int i = 0; i < count; i++) arr1[i] = arr[i];
    
    start = clock();
    for (int i = 0; i < count - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < count; j++)
            if (arr1[j]->getPrice() < arr1[minIdx]->getPrice())
                minIdx = j;
        if (minIdx != i)
            swap(arr1[i], arr1[minIdx]);
    }
    end = clock();
    time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
    cout << "Selection Sort: " << time << " ms\n\n";
    
    cout << "Quick Sort Test\n";
    Flight* arr2[1000];
    for (int i = 0; i < count; i++) arr2[i] = arr[i];
    
    start = clock();
    quickSort(arr2, 0, count - 1);
    end = clock();
    time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
    cout << "Quick Sort: " << time << " ms\n\n";
    
    cout << "Bubble Sort Test\n";
    Flight* arr3[1000];
    for (int i = 0; i < count; i++) arr3[i] = arr[i];
    
    start = clock();
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (arr3[j]->getPrice() > arr3[j + 1]->getPrice()) {
                swap(arr3[j], arr3[j + 1]);
            }
        }
    }
    end = clock();
    time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
    cout << "Bubble Sort: " << time << " ms\n\n";
    
    cout << "Heap Push Operations Test\n";
    Flight* testFlight = flights.find(flights.getRoot(), "F1");
    if (testFlight) {
        start = clock();
        for (int i = 0; i < 50; i++) {
            testFlight->getWaitlist().push("TestPass" + to_string(i), 9000 + i, 2);
        }
        end = clock();
        time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
        cout << "50 Heap Push operations: " << time << " ms\n\n";
    }

    cout << "Dijkstra's Shortest Path Test\n";
    
    if (g.airportCount < 2) {
        cout << "Not enough airports to test Dijkstra.\n";
    } else {
        int sourceIdx = 0;
        int destIdx = g.airportCount - 1;
        
        start = clock();
        double cost = g.findShortestPath(sourceIdx, destIdx, airports);
        end = clock();
    
        time = ((double)(end - start) / CLOCKS_PER_SEC) * 1000.0;
        
        cout << "Dijkstra on " << g.airportCount << " airports: " << time << " ms\n";
        if (cost >= INF) {
            cout << "No path found.\n\n";
        } else {
            cout << "Shortest path cost: $" << cost << "\n\n";
        }
    }
}

int main() {
    BST flights;
    Graph g;
    AirportTable airports;
    int airportCount = 0;
    
    FileManager::loadFlights(flights, g, airports, airportCount);
    while (true) {
        cout << " 1.Add Flight\n 2.List Flights\n 3.Reserve Seat\n 4.Cancel Seat\n 5.Find Cheapest Route\n "
                "6.Display Waitlist\n 7.Search Flights\n 8.Sort Flights by Price\n 9.Delete Flight\n "
                "10.Manage Waitlist\n 11.Round-trip Booking\n 12.Test Runtimes\n 13.Exit\nChoice: ";
        int ch;
        if (!(cin >> ch)) {
            cout << "Invalid input! Please enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        
        if (ch < 1 || ch > 13) {
            cout << "Invalid choice! Please enter a number between 1-12.\n";
            continue;
        }

        if (ch == 1) {
            int cap;
            string id, airline, aT, dT;
            double price;
            string o, d;
            cout << "Flight ID: ";
            cin >> id;
            cout << "Airline: ";
            cin >> airline;
            cout << "Origin: ";
            cin >> o;
            cout << "Destination: ";
            cin >> d;
            cout << "Departure Time";
            cin >> dT;
            cout << "Arrival Time";
            cin >> aT;
            cout << "Price: ";
            cin >> price;
            cout << "Capacity: ";
            cin >> cap;
            if (cap <= 0) {
                cout << "Error: Capacity must be at least 1!\n";
                continue;
            }

            if (flights.find(flights.getRoot(), id)) {
                cout << "Error: Flight ID '" << id << "' already exists!\n";
                continue;
            }
            
            flights.insertFlight(id, airline, o, d, dT, aT, price, cap);
            int oi = airports.getAirportIndex(o, airportCount);
            int di = airports.getAirportIndex(d, airportCount);
            g.addEdge(oi, di, price);
            g.airportCount = airportCount;
        }
        else if (ch == 2){
            if (!flights.getRoot()) {
                cout << "No flights available in the system.\n";
                continue;
            }

            flights.inorder(flights.getRoot());
        }
        else if (ch == 3)
        {
            int pr, passID;
            string id, name;
            cout << "Flight ID: ";
            cin >> id;
            Flight *f = flights.find(flights.getRoot(), id);
           
            if (!f) {
                cout << "Not found!\n";
                continue;
            }

            cout << "Passenger Name: ";
            cin >> name;
            cout << "Priority (1-3): ";
            cin >> pr;
           
            if (pr < 1 || pr > 3) {
                cout << "Error: Priority must be between 1 and 3!\n";
                continue;
            }

            cout << "Passenger ID: ";
            cin >> passID;
           
            if (f->getBooked() < f->getCapacity()) {
                f->addPassenger(name,passID);
                f->bookSeat();
                cout << "Seat confirmed!\n";
                FileManager::logPassenger("Booked", id, name);
            }
            else {
                if (f->getWaitlist().hasPassengerID(passID)) {
                    cout << "Error: Passenger ID " << passID << " is already in waitlist!\n";
                    continue;
                }

                f->getWaitlist().push(name, passID, pr);
                cout << "Added to waitlist.\n";
            }
        }

        else if (ch == 4) {
            string flightID;
            cout << "Flight ID: ";
            cin >> flightID;
            Flight *f = flights.find(flights.getRoot(), flightID);
           
            if (!f) {
            cout << "Flight not found!\n";
            continue;
            }

            string passName;
            int passID;
            cout << "Passenger Name: ";
            cin >> passName;
            cout << "Passenger ID: ";
            cin >> passID;

            if (f->cancelSeatByPassenger(passName, passID)) {
                cout << "Seat for " << passName << " (ID " << passID << ") cancelled.\n";
                FileManager::logPassenger("Cancelled", flightID, passName);

                SeatHeap &waitlist = f->getWaitlist();
                if (!waitlist.empty()) {
                    SeatRequest next = waitlist.pop(); 
                    f->addPassenger(next.name, next.id); 
                    f->bookSeat(); 
                                  
                    cout << "Seat automatically assigned to " << next.name 
                         << " (priority " << next.priority << ")\n";
                    FileManager::logPassenger("Booked from Waitlist", flightID, next.name);
                }
            } 
            else if (f->getWaitlist().hasPassengerID(passID)) {
                 f->getWaitlist().removePassenger(passID);
                 cout << "Passenger " << passID << " found and removed from waitlist.\n";
            }
            else {
                cout << "Passenger not found in booked seats or waitlist for this flight.\n";
            }
        }

        else if (ch == 5) {
            string o, d;
            cout << "From: ";
            cin >> o;
            cout << "To: ";
            cin >> d;

            int tempCount = airportCount;
            int oi = airports.getAirportIndex(o, tempCount);
            int di = airports.getAirportIndex(d, tempCount);
            
            if (oi==-1 || di==-1 || oi >= airportCount || di >= airportCount ){
                cout<<"ERROR! Aiports not found in the current flight network!\n";
                continue;
            }
            else {
                double cost = g.findShortestPath(oi, di, airports);
            }
        }

        else if (ch == 6) {
            string id;
            cout << "Flight ID: ";
            cin >> id;
            Flight *f = flights.find(flights.getRoot(), id);
            if (f)
                f->getWaitlist().displayWaitlist();
            else
                cout << "Flight not found.\n";
        }
        else if (ch == 7) {
            string o, d;
            cout << "Origin (or empty for any): ";
            cin.ignore();
            getline(cin, o);
            cout << "Destination (or empty for any): ";
            getline(cin, d);
            flights.displayByOriginDest(flights.getRoot(), o, d);
        }
        else if (ch == 8) {
            string origin, dest;
            cout << "Origin (or empty for any): ";
            cin.ignore();
            getline(cin, origin);
            cout << "Destination (or empty for any): ";
            getline(cin, dest);

            Flight* arr[1000]; 
            int idx = 0;

            collectFlights(flights.getRoot(), origin, dest, arr, idx);

            if (idx == 0) {
                cout << "No flights found for the given route.\n";
            } 
            
            else {
                quickSort(arr, 0, idx - 1);
                cout << "Flights Sorted by Price:\n";
                for (int i = 0; i < idx; i++)
                    arr[i]->display();
            }
        }

        else if (ch == 9) {
            string id;
            cout << "Flight ID to delete: ";
            cin >> id;
            if (!flights.find(flights.getRoot(), id)) {
                cout << "Error: Flight '" << id << "' not found!\n";
                continue;
            }

            flights.deleteFlight(id);
            cout << "Flight deleted.\n";
        }
        else if (ch == 10) {
            string id;
            cout << "Flight ID: ";
            cin >> id;
            Flight *f = flights.find(flights.getRoot(), id);
            if (!f) {
                cout << "Flight not found.\n";
                continue;
            }
            while (true) {
                cout << "1.Display Waitlist 2.Remove Passenger 3.Modify Priority 4.Back\nChoice: ";
                int wc;
                cin >> wc;
                if (wc == 1)
                    f->getWaitlist().displayWaitlist();
                else if (wc == 2){
                    int id;
                    cout << "Passenger ID: ";
                    cin >> id;
                    f->getWaitlist().removePassenger(id);
                }
                else if (wc == 3) {
                    int id;
                    int p;
                    string name;
                    cout<< "Passenger name: ";
                    cin >> name;
                    cout << "Passenger id: ";
                    cin >> id;
                    cout << "New priority: ";
                    cin >> p;
                    f->getWaitlist().modifyPriority(name, id, p);
                }
                else {
                    break;
                }
            }
        }
        else if (ch == 11) {
            int ID;
            string id1, id2, name;
            cout << "Passenger name: ";
            cin >> name;
            cout << "Passenger ID: ";
            cin >> ID;
            cout << "Outbound Flight ID: ";
            cin >> id1;
            Flight *f1 = flights.find(flights.getRoot(), id1);
            cout << "Return Flight ID: ";
            cin >> id2;
            Flight *f2 = flights.find(flights.getRoot(), id2);
            if (!f1 || !f2) {
                cout << "One of the flights not found.\n";
                continue;
            }
            int pr = 1;
            if (f1->getBooked() < f1->getCapacity()) {
                f1->bookSeat();
                cout << "Outbound booked.\n";
                FileManager::logPassenger("Round-trip Outbound", id1, name);
            }
            else {
                f1->getWaitlist().push(name, ID, pr);
                cout << "Added to outbound waitlist.\n";
            }
            if (f2->getBooked() < f2->getCapacity()) {
                f2->bookSeat();
                cout << "Return booked.\n";
                FileManager::logPassenger("Round-trip Return", id2, name);
            }
            else {
                f2->getWaitlist().push(name, ID, pr);
                cout << "Added to return waitlist.\n";
            }
        }
        else if (ch == 12)
        {
            testRuntimes(flights, g, airports, airportCount);
        }
        
        else if (ch == 13) {
            remove("flights.txt");
            remove("waitlists.txt");
            FileManager::saveAllFlights(flights.getRoot());
            cout << "Data saved. Exiting.\n";
            break;
        }
    }
    return 0;
}
