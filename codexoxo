#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#define MAX_AIRPORTS 50
#define MAX_WAIT 100
#define INF 1e9

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
        if (size >= MAX_WAIT)
            return;
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

    SeatRequest pop() {
        SeatRequest top = arr[0];
        arr[0] = arr[--size];
        heapifyDown(0);
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
                arr[i] = arr[--size];
                heapifyUp(i);
                heapifyDown(i);
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

class Flight
{
    int id;
    string origin, dest;
    double price;
    int capacity, booked;
    SeatHeap waitlist;
    Flight *left;
    Flight *right;

public:
    Flight(int fid, string o, string d, double p, int c, int b = 0)
    {
        id = fid;
        origin = o;
        dest = d;
        price = p;
        capacity = c;
        booked = b;
        left = right = nullptr;
    }

    Flight *getLeft() { return left; }
    Flight *getRight() { return right; }
    void setLeft(Flight *l) { left = l; }
    void setRight(Flight *r) { right = r; }

    int getID() { return id; }
    string getOrigin() { return origin; }
    string getDest() { return dest; }
    double getPrice() { return price; }
    int getCapacity() { return capacity; }
    int getBooked() { return booked; }
    void bookSeat() { booked++; }
    void cancelSeat()
    {
        if (booked > 0)
            booked--;
    }
    SeatHeap &getWaitlist() { return waitlist; }

    void display()
    {
        cout << "Flight " << id << " | " << origin << " -> " << dest << " | $" << price
             << " | Seats: " << booked << "/" << capacity << endl;
    }
};

class BST {
    Flight *root;
    Flight *insert(Flight *node, Flight *f) {
        if (!node)
            return f;
        if (f->getID() < node->getID())
            node->setLeft(insert(node->getLeft(), f));
        else if (f->getID() > node->getID())
            node->setRight(insert(node->getRight(), f));
        return node;
    }
    Flight *findMin(Flight *node) {
        while (node->getLeft())
            node = node->getLeft();
        return node;
    }
    Flight *removeNode(Flight *node, int id) {
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
                node->setRight(removeNode(node->getRight(), temp->getID()));
                *node = *temp;
            }
        }
        return node;
    }

public:
    BST() { 
        root = nullptr; 
    }
    Flight *getRoot() { 
        return root; 
    }

    void insertFlight(int id, string o, string d, double p, int c, int b = 0) {
        Flight *f = new Flight(id, o, d, p, c, b);
        root = insert(root, f);
    }

    Flight *find(Flight *node, int id) {
        if (!node)
            return nullptr;
        if (id == node->getID())
            return node;
        if (id < node->getID())
            return find(node->getLeft(), id);
        return find(node->getRight(), id);
    }

    void inorder(Flight *node) {
        if (!node)
            return;
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
        if (!node)
            return;
        sortByPrice(node->getLeft(), arr, index);
        arr[index++] = node;
        sortByPrice(node->getRight(), arr, index);
    }

    void deleteFlight(int id) { root = removeNode(root, id); }
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
        while (used[h] && table[h].name != name)
        {
            h = (h + 1) % MAX_AIRPORTS;
            if (h == start)
                return -1;
        }
        if (!used[h])
        {
            table[h].name = name;
            table[h].index = count++;
            used[h] = 1;
        }
        return table[h].index;
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
    void addEdge(int src, int dest, double cost) { adj[src] = new EdgeNode(dest, cost, adj[src]); }
};

class FileManager
{
public:
    static void saveFlights(Flight *node)
    {
        if (!node)
            return;
        saveFlights(node->getLeft());
        ofstream fout("flights.txt", ios::app);
        fout << node->getID() << " " << node->getOrigin() << " " << node->getDest()
             << " " << node->getPrice() << " " << node->getCapacity() << " "
             << node->getBooked() << "\n";
        fout.close();
        ofstream wf("waitlists.txt", ios::app);
        SeatHeap &w = node->getWaitlist();
        for (int i = 0; i < w.getSize(); i++)
            wf << node->getID() << " " << w.get(i).name << " " << w.get(i).priority << "\n";
        wf.close();
        saveFlights(node->getRight());
    }
    static void loadFlights(BST &flights, Graph &g, AirportTable &airports, int &airportCount)
    {
        ifstream fin("flights.txt");
        if (fin)
        {
            int id, cap, booked;
            double price;
            string o, d;
            while (fin >> id >> o >> d >> price >> cap >> booked)
            {
                flights.insertFlight(id, o, d, price, cap, booked);
                int oi = airports.getAirportIndex(o, airportCount);
                int di = airports.getAirportIndex(d, airportCount);
                g.addEdge(oi, di, price);
            }
            g.airportCount = airportCount;
            fin.close();
        }
        ifstream win("waitlists.txt"); {
            int fid, pr, ID;
            string name;
            while (win >> fid >> name >> pr)
            {
                Flight *f = flights.find(flights.getRoot(), fid);
                if (f)
                    f->getWaitlist().push(name, ID, pr);
            }
            win.close();
        }
    }
    static void logPassenger(const string &action, int fid, const string &name) {
        ofstream fout("passenger_history.txt", ios::app);
        fout << action << " FlightID:" << fid << " Passenger:" << name << "\n";
        fout.close();
    }
};

int main() {
    BST flights;
    Graph g;
    AirportTable airports;
    int airportCount = 0;
    FileManager::loadFlights(flights, g, airports, airportCount);

    while (true) {
        cout << " 1.Add Flight\n 2.List Flights\n 3.Reserve Seat\n 4.Cancel Seat\n 5.Find Cheapest Route\n "
                "6.Display Waitlist\n 7.Search Flights\n 8.Sort Flights by Price\n 9.Delete Flight\n "
                "10.Manage Waitlist\n 11.Round-trip Booking\n 12.Exit\nChoice: ";
        int ch;
        cin >> ch;
        if (ch == 1)
        {
            int id, cap;
            double price;
            string o, d;
            cout << "Flight ID: ";
            cin >> id;
            cout << "Origin: ";
            cin >> o;
            cout << "Destination: ";
            cin >> d;
            cout << "Price: ";
            cin >> price;
            cout << "Capacity: ";
            cin >> cap;
            flights.insertFlight(id, o, d, price, cap);
            int oi = airports.getAirportIndex(o, airportCount);
            int di = airports.getAirportIndex(d, airportCount);
            g.addEdge(oi, di, price);
            g.airportCount = airportCount;
        }
        else if (ch == 2)
            flights.inorder(flights.getRoot());
        else if (ch == 3)
        {
            int id, pr;
            string name;
            cout << "Flight ID: ";
            cin >> id;
            Flight *f = flights.find(flights.getRoot(), id);
            if (!f)
            {
                cout << "Not found!\n";
                continue;
            }
            cout << "Passenger name: ";
            cin >> name;
            cout << "Priority (1-3): ";
            cin >> pr;
            if (f->getBooked() < f->getCapacity())
            {
                f->bookSeat();
                cout << "Seat confirmed!\n";
                FileManager::logPassenger("Booked", id, name);
            }
            else
            {
                f->getWaitlist().push(name, id, pr);
                cout << "Added to waitlist.\n";
            }
        }
        else if (ch == 4) {
            int id;
            cout << "Flight ID: ";
            cin >> id;
            Flight *f = flights.find(flights.getRoot(), id);
            if (!f)
            {
                cout << "Not found!\n";
                continue;
            }
            if (f->getBooked() > 0){
                f->cancelSeat();
                if (!f->getWaitlist().empty()) {
                    SeatRequest next = f->getWaitlist().pop();
                    f->bookSeat();
                    cout << "Seat assigned to " << next.name << " (priority " << next.priority << ")\n";
                    FileManager::logPassenger("Booked from Waitlist", id, next.name);
                }
                else {
                    cout << "Seat cancelled.\n";
                }
            }
            else {
                cout << "No booked seats.\n";
            }
        }
        else if (ch == 5) {
            string o, d;
            cout << "From: ";
            cin >> o;
            cout << "To: ";
            cin >> d;
            int oi = airports.getAirportIndex(o, airportCount);
            int di = airports.getAirportIndex(d, airportCount);
            double dist[MAX_AIRPORTS];
            int parent[MAX_AIRPORTS];
            bool vis[MAX_AIRPORTS];
            for (int i = 0; i < g.airportCount; i++)
            {
                dist[i] = INF;
                vis[i] = false;
                parent[i] = -1;
            }
            dist[oi] = 0;
            for (int count = 0; count < g.airportCount - 1; count++)
            {
                double minv = INF;
                int u = -1;
                for (int i = 0; i < g.airportCount; i++)
                    if (!vis[i] && dist[i] <= minv)
                    {
                        minv = dist[i];
                        u = i;
                    }
                if (u == -1)
                    break;
                vis[u] = true;
                for (EdgeNode *e = g.adj[u]; e; e = e->next)
                    if (!vis[e->dest] && dist[u] + e->cost < dist[e->dest])
                    {
                        dist[e->dest] = dist[u] + e->cost;
                        parent[e->dest] = u;
                    }
            }
            if (dist[di] >= INF) {
                cout << "No route found.\n";
            }
            else {
                cout << "Cheapest cost: $" << dist[di] << "\nPath: ";
                int path[MAX_AIRPORTS], len = 0;
                for (int v = di; v != -1; v = parent[v])
                    path[len++] = v;
                for (int i = len - 1; i >= 0; i--)
                    cout << path[i] << (i ? " -> " : "\n");
            }
        }
        else if (ch == 6) {
            int id;
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
            Flight *arr[1000];
            int idx = 0;
            flights.sortByPrice(flights.getRoot(), arr, idx);
            for (int i = 0; i < idx; i++)
                arr[i]->display();
        }
        else if (ch == 9) {
            int id;
            cout << "Flight ID to delete: ";
            cin >> id;
            flights.deleteFlight(id);
            cout << "Flight deleted.\n";
        }
        else if (ch == 10) {
            int id;
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
                    string name;
                    cout << "Passenger name: ";
                    cin >> name;
                    f->getWaitlist().removePassenger(id);
                }
                else if (wc == 3) {
                    int id;
                    int p;
                    string name;
                    cout<< "Passenger name: ";
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
            int id1, id2, ID;
            string name;
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
        else if (ch == 12) {
            remove("flights.txt");
            remove("waitlists.txt");
            FileManager::saveFlights(flights.getRoot());
            cout << "Data saved. Exiting.\n";
            break;
        }
    }
    return 0;
}
