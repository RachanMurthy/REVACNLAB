#include<stdio.h>

struct rtable {
    int dist[20], nextnode[20];
} table[20];

int cost[10][10], n;

void distvector() {
    int i, j, k, count = 0;
    
    // Initialize the distance and nextnode table
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            table[i].dist[j] = cost[i][j];
            table[i].nextnode[j] = j;
        }
    }
    
    // Distance vector algorithm
    do {
        count = 0;
        
        for (i = 0; i < n; i++) {
            for (j = 0; j < n; j++) {
                for (k = 0; k < n; k++) {
                    if (table[i].dist[j] > cost[i][k] + table[k].dist[j]) {
                        table[i].dist[j] = table[i].dist[k] + table[k].dist[j];
                        table[i].nextnode[j] = k;
                        count++;
                    }
                }
            }
        }
    } while (count != 0);
}

int main() {
    int i, j;
    
    // Input the number of vertices
    printf("\nEnter the number of vertices: ");
    scanf("%d", &n);
    
    // Input the cost matrix
    printf("\nEnter the cost matrix:\n");
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            scanf("%d", &cost[i][j]);
        }
    }
    
    // Perform distance vector calculation
    distvector();
    
    // Output the distance vector table for each router
    for (i = 0; i < n; i++) {
        printf("\nState value for router %c\n", i + 65);
        printf("\nDestNode\tNextNode\tDistance\n");
        for (j = 0; j < n; j++) {
            if (table[i].dist[j] == 99) {
                printf("%c\t\t-\t\tInfinite\n", j + 65);
            } else {
                printf("%c\t\t%c\t\t%d\n", j + 65, table[i].nextnode[j] + 65, table[i].dist[j]);
            }
        }
    }
    
    return 0;
}

