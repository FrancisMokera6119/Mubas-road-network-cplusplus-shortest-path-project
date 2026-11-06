//MUBAS CAMPUS ROAD NETWORK SHORTEST PATH PROJECT
#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cctype>
#include <queue>
#include <functional>
using namespace std;

const int N = 20;
const int INF = INT_MAX;

// Represents one road between two vertices
struct Edge {
    char u, v;
    int w;

    Edge(char a, char b, int weight) {
        u = a;
        v = b;
        w = weight;
    }
};

vector<Edge> edges;
vector< pair<int, int> > graph[N];

// Convert letter A-T into index 0-19
int idx(char c) {
    return toupper(c) - 'A';
}

char label(int i) {
    return 'A' + i;
}

// Compare edges by weight for Kruskal
bool compareEdges(Edge a, Edge b) {
    return a.w < b.w;
}

// Add the graph's roads
void addEdges() {
    edges.push_back(Edge('A','B',120));
    edges.push_back(Edge('A','R',200));
    edges.push_back(Edge('A','Q',220));
    edges.push_back(Edge('B','S',500));
    edges.push_back(Edge('B','C',40));
    edges.push_back(Edge('C','D',60));
    edges.push_back(Edge('C','G',100));
    edges.push_back(Edge('C','N',30));
    edges.push_back(Edge('N','O',150));
    edges.push_back(Edge('N','L',60));
    edges.push_back(Edge('L','M',80));
    edges.push_back(Edge('L','K',30));
    edges.push_back(Edge('L','H',200));
    edges.push_back(Edge('K','M',70));
    edges.push_back(Edge('M','P',50));
    edges.push_back(Edge('P','T',30));
    edges.push_back(Edge('T','O',100));
    edges.push_back(Edge('T','Q',90));
    edges.push_back(Edge('Q','R',290));
    edges.push_back(Edge('O','I',180));
    edges.push_back(Edge('I','J',60));
    edges.push_back(Edge('I','G',160));
    edges.push_back(Edge('J','H',140));
    edges.push_back(Edge('J','S',480));
    edges.push_back(Edge('H','G',200));
    edges.push_back(Edge('H','F',160));
    edges.push_back(Edge('F','D',40));
    edges.push_back(Edge('F','G',60));
    edges.push_back(Edge('G','E',50));
    edges.push_back(Edge('E','D',20));

    // Build the undirected graph
    int i;
    for (i = 0; i < (int)edges.size(); i++) {
        int u = idx(edges[i].u);
        int v = idx(edges[i].v);
        int w = edges[i].w;

        graph[u].push_back(make_pair(v, w));
        graph[v].push_back(make_pair(u, w));
    }
}

// Dijkstra's algorithm
void dijkstra(int source, int destination) {
    int dist[N], parent[N];
    bool visited[N];
    int i;

    // Initialise all distances
    for (i = 0; i < N; i++) {
        dist[i] = INF;
        parent[i] = -1;
        visited[i] = false;
    }

    dist[source] = 0;

    // Select the closest unvisited vertex each time
    for (int step = 0; step < N; step++) {
        int u = -1;
        int smallest = INF;

        for (i = 0; i < N; i++) {
            if (!visited[i] && dist[i] < smallest) {
                smallest = dist[i];
                u = i;
            }
        }

        // STOP IF NO REACHABLE NODE REMAINS 
        if (u == -1) {
            break;
        }

        visited[u] = true;

        cout << "\nITERATION : " << label(u)
             << ", DISTANCE: " << dist[u] << "\n";

        // CHECK ALL NEIGHBOURS OF THE SELECTED NODE 
        for (i = 0; i < (int)graph[u].size(); i++) {
            int v = graph[u][i].first;
            int w = graph[u][i].second;

            if (!visited[v] && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;

                cout << "UPDATED " << label(v)
                     << " TO DISTANCE " << dist[v] << "\n";
            }
        }
    }

    cout << "\nFINAL DISTANCE FROM " << label(source) << ":\n";

    for (i = 0; i < N; i++) {
        cout << label(i) << ": ";

        if (dist[i] == INF)
            cout << "INF";
        else
            cout << dist[i];

        cout << "  PREDECESSOR: ";

        if (parent[i] == -1)
            cout << "-";
        else
            cout << label(parent[i]);

        cout << "\n";
    }

    if (dist[destination] == INF) {
        cout << "NO PATH EXISTS.\n";
        return;
    }

    // RECONSTRUCT THE SHORTEST PATH 
    int path[N];
    int count = 0;
    int current = destination;

    while (current != -1) {
        path[count] = current;
        count++;
        current = parent[current];
    }

    cout << "\nSHORTEST PATH : ";

    for (i = count - 1; i >= 0; i--) {
        cout << label(path[i]);

        if (i > 0)
            cout << " -> ";
    }

    cout << "\nTOTAL DISTANCE: "
         << dist[destination] << "\n";
}

// FIND THE GROUP OF A NODE
int findRoot(int parent[], int x) {
    while (parent[x] != x) {
        x = parent[x];
    }
    return x;
}

// KRUSKAL'S MINIMUM SPANNING TREE 
void kruskal() {
    sort(edges.begin(), edges.end(), compareEdges);

    int parent[N];
    int i;

    for (i = 0; i < N; i++) {
        parent[i] = i;
    }

    int total = 0;
    int count = 0;

    cout << "\nMINIMUM SPANNING TREE:\n";

    for (i = 0; i < (int)edges.size(); i++) {
        int u = idx(edges[i].u);
        int v = idx(edges[i].v);

        int rootU = findRoot(parent, u);
        int rootV = findRoot(parent, v);

        // THE THE EDGE ONLY IF IT DOESN'T NOT CREATE A CYCLE
        if (rootU != rootV) {
            parent[rootU] = rootV;

            cout << edges[i].u << " - "
                 << edges[i].v << " : "
                 << edges[i].w << "\n";

            total += edges[i].w;
            count++;
        }
    }

    cout << "NUMBER OF SELECTED EDGES: " << count << "\n";
    cout << "TOTAL MST WEIGHT: " << total << "\n";

    if (count != N - 1) {
        cout << "GRAPH IS DISCONNECTED.\n";
    }
}

int main() {
    addEdges();

    char source, destination;

    cout << "ENTER SOURCE NODE (A-T): ";
    cin >> source;

    cout << "ENTER DESTINATION NODE (A-T): ";
    cin >> destination;

    source = toupper(source);
    destination = toupper(destination);

    if (source < 'A' || source > 'T' ||
        destination < 'A' || destination > 'T') {
        cout << "YOU ENTER INVALID VERTEXI.ENTER LETTERS A TO T WITH SPACE BETWEEN LETTERS A to T.\n";
        return 1;
    }

    dijkstra(idx(source), idx(destination));
    kruskal();

    return 0;
}
