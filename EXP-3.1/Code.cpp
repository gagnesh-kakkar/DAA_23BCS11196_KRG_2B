#include <iostream>
using namespace std;

int minDistance(int dist[], bool sptSet[], int n) {
    int min = 999999, index;
    for(int i=0;i<n;i++)
        if(!sptSet[i] && dist[i] <= min) {
            min = dist[i];
            index = i;
        }
    return index;
}

void dijkstra(int graph[20][20], int src, int n) {
    int dist[20];
    bool sptSet[20];
    
    for(int i=0;i<n;i++){
        dist[i] = 999999;
        sptSet[i] = false;
    }

    dist[src] = 0;

    for(int count=0;count<n-1;count++){
        int u = minDistance(dist, sptSet, n);
        sptSet[u] = true;

        for(int v=0;v<n;v++)
            if(!sptSet[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v])
                dist[v] = dist[u] + graph[u][v];
    }

    cout << "Vertex\tDistance from Source\n";
    for(int i=0;i<n;i++)
        cout << i << "\t" << dist[i] << "\n";
}

int main() {
    int n, src;
    int graph[20][20];
    
    cout << "Enter number of vertices: ";
    cin >> n;
    cout << "Enter adjacency matrix:\n";
    for(int i=0;i<n;i++)
        for(int j=0;j<n;j++)
            cin >> graph[i][j];

    cout << "Enter source vertex: ";
    cin >> src;

    dijkstra(graph, src, n);
}
