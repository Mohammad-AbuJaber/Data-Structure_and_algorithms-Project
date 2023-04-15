// Mohammad AbuJaber - 1190298

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <limits.h>
//1:15:46

int numberOfCities = 0;
int numberOfEdges = 0;
int costBetweenTwoEdges = 0;

// A structure to represent a
// node in adjacency list
struct AdjListNode
{
	int dest;
	char name [50];
	int weight;
	struct AdjListNode* next;
};

// A structure to represent
// an adjacency list
struct AdjList
{

// Pointer to head node of list
struct AdjListNode *head;
};

// A structure to represent a graph.
// A graph is an array of adjacency lists.
// Size of array will be V (number of
// vertices in graph)
struct Graph
{
	int V;
	struct AdjList* array;
};

// A utility function to create
// a new adjacency list node
struct AdjListNode* newAdjListNode(int dest,char name [50], int weight)
{
	struct AdjListNode* newNode =(struct AdjListNode*)malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	strcpy(newNode->name,name);
	newNode->weight = weight;
	newNode->next = NULL;
	return newNode;
}

// A utility function that creates
// a graph of V vertices
struct Graph* createGraph(int V)
{
	struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
	graph->V = V;

	// Create an array of adjacency lists.
	// Size of array will be V
	graph->array = (struct AdjList*)
	malloc(V * sizeof(struct AdjList));

	// Initialize each adjacency list
	// as empty by making head as NULL
	for (int i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src,int dest,char name[50], int weight)
{
	// Add an edge from src to dest.
	// A new node is added to the adjacency
	// list of src. The node is
	// added at the beginning
	struct AdjListNode* newNode =newAdjListNode(dest,name,weight);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;

	// Since graph is undirected,
	// add an edge from dest to src also
	newNode = newAdjListNode(src,name, weight);
	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
}

// Structure to represent a min heap node
struct MinHeapNode
{
	int v;
	int dist;
};

// Structure to represent a min heap
struct MinHeap
{

	// Number of heap nodes present currently
	int size;

	// Capacity of min heap
	int capacity;

	// This is needed for decreaseKey()
	int *pos;
	struct MinHeapNode **array;
};

// A utility function to create a
// new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v,int dist)
{
	struct MinHeapNode* minHeapNode =(struct MinHeapNode*)malloc(sizeof(struct MinHeapNode));
	minHeapNode->v = v;
	minHeapNode->dist = dist;
	return minHeapNode;
}

// A utility function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
	struct MinHeap* minHeap =(struct MinHeap*)malloc(sizeof(struct MinHeap));
	minHeap->pos = (int *)malloc(capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array =
		(struct MinHeapNode**)malloc(capacity *sizeof(struct MinHeapNode*));
	return minHeap;
}

// A utility function to swap two
// nodes of min heap.
// Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a,struct MinHeapNode** b)
{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// A standard function to
// heapify at given idx
// This function also updates
// position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap,int idx)
{
	int smallest, left, right;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist )
	smallest = left;

	if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist )
	smallest = right;

	if (smallest != idx)
	{
		// The nodes to be swapped in min heap
		struct MinHeapNode *smallestNode = minHeap->array[smallest];
		struct MinHeapNode *idxNode = minHeap->array[idx];

		// Swap positions
		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] = smallest;

		// Swap nodes
		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

		minHeapify(minHeap, smallest);
	}
}

// A utility function to check if
// the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
	return minHeap->size == 0;
}

// Standard function to extract
// minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
	if (isEmpty(minHeap))
		return NULL;

	// Store the root node
	struct MinHeapNode* root = minHeap->array[0];

	// Replace root node with last node
	struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	// Update position of last node
	minHeap->pos[root->v] = minHeap->size-1;
	minHeap->pos[lastNode->v] = 0;

	// Reduce heap size and heapify root
	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}

// Function to decreasy dist value
// of a given vertex v. This function
// uses pos[] of min heap to get the
// current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int dist)
{
	// Get the index of v in heap array
	int i = minHeap->pos[v];

	// Get the node and update its dist value
	minHeap->array[i]->dist = dist;

	// Travel up while the complete
	// tree is not hepified.
	// This is a O(Logn) loop
	while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
	{
		// Swap this node with its parent
		minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
		minHeap->pos[minHeap->array[ (i-1)/2]->v] = i;
		swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

		// move to parent index
		i = (i - 1) / 2;
	}
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
int isInMinHeap(struct MinHeap *minHeap, int v)
{
if (minHeap->pos[v] < minHeap->size)
	return 1;
return -1;
}

// A utility function used to print the solution
void printArr(int dist[], int n,int dest)
{
	printf("Vertex Distance from Source\n");
	for (int i = 0; i < n; ++i){
            if (i==dest)
            printf("%d \t\t %d\n", i, dist[i]);
	}
}

// The main function that calulates
// distances of shortest paths from src to all
// vertices. It is a O(ELogV) function
void dijkstra(struct Graph* graph, int src)
{

	// Get the number of vertices in graph
	int V = graph->V;

	// dist values used to pick
	// minimum weight edge in cut
	int dist[V];

	// minHeap represents set E
	struct MinHeap* minHeap = createMinHeap(V);

	// Initialize min heap with all
	// vertices. dist value of all vertices
	for (int v = 0; v < V; ++v)
	{
		dist[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, dist[v]);
		minHeap->pos[v] = v;
	}

	// Make dist value of src vertex
	// as 0 so that it is extracted first
	minHeap->array[src] = newMinHeapNode(src, dist[src]);
	minHeap->pos[src] = src;
	dist[src] = 0;
	decreaseKey(minHeap, src, dist[src]);

	// Initially size of min heap is equal to V
	minHeap->size = V;

	// In the followin loop,
	// min heap contains all nodes
	// whose shortest distance
	// is not yet finalized.
	while (!isEmpty(minHeap))
	{
		// Extract the vertex with
		// minimum distance value
		struct MinHeapNode* minHeapNode = extractMin(minHeap);

		// Store the extracted vertex number
		int u = minHeapNode->v;

		// Traverse through all adjacent
		// vertices of u (the extracted
		// vertex) and update
		// their distance values
		struct AdjListNode* pCrawl = graph->array[u].head;
		while (pCrawl != NULL)
		{
			int v = pCrawl->dest;

			// If shortest distance to v is
			// not finalized yet, and distance to v
			// through u is less than its
			// previously calculated distance
			if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v])
			{
				dist[v] = dist[u] + pCrawl->weight;

				// update distance
				// value in min heap also
				decreaseKey(minHeap, v, dist[v]);
			}
			pCrawl = pCrawl->next;
		}
	}

	// print the calculated shortest distances
	printArr(dist, V,293);
}


//struct Vertex
//{
//    struct Vertex *next;
//    int id;
//    char nameOfCities [50];
//    int distanceBetweenVertices;
//};
//
//typedef struct Vertex *V,**graph;
//graph map = NULL;
//
//int isEmpty(V v)
//{
//    return (v==NULL);
//}

int isInteger (char *distance)
{

    int number;
    number = atoi( distance );

    if (number == 0 && distance[0] != '0')
        return -1;
    else
        return 1;
}

int calculateNumberOfCities ()
{
    int numberOfCities = 0;
    FILE *in = fopen("segments.txt","r");

    int *pointer = " ";
    char *divide;
    char line [50];

    int flag;

    while (fgets(line,50,in))
    {
        divide = strtok (line, pointer);
        flag = 0;

        while (divide != NULL)
        {

            if (flag == 0)
            {

                if (atoi(divide)>numberOfCities)
                    numberOfCities=(int) strtol(divide,NULL,10);

            }
            else if (flag == 1)
            {

                if (atoi(divide)>numberOfCities)
                    numberOfCities=(int) strtol(divide,NULL,10);
                break;
            }

            flag++;
            divide = strtok (NULL,pointer);
        }
    }
    fclose(in);
    return numberOfCities;
}


//void createMap(int numberOfCities)
//{
//    map = (graph) malloc (sizeof(V) * (numberOfCities*2));
//    for (int i = 0; i<numberOfCities; i++)
//        map[i] = NULL;
//}
//
//V getVertex (int id)
//{
//    V vertex = (V) malloc (sizeof (struct Vertex));
//    vertex->id = id;
//    strcpy(vertex->nameOfCities,"NULL");
//    vertex->distanceBetweenVertices=0;
//    vertex->next = NULL;
//    return vertex;
//}
//
//V getEdge (int destination,char name [50], int distance)
//{
//    V vertex = (V) malloc (sizeof (struct Vertex));
//    vertex->id = destination;
//    strcpy(vertex->nameOfCities,name);
//    vertex->distanceBetweenVertices=distance;
//    vertex->next = NULL;
//    return vertex;
//}
//
//void addVertex (int id)
//{
//    if (isEmpty(map[id]))
//        map[id] = getVertex(id);
//    else
//        printf("The city was already added to the map\n");
//}
//
//void addEdge (int source, int destination, char name [50], int distance)
//{
//
//    if (isEmpty(map[source]))
//        addVertex(source);
//
//    if (isEmpty(map[destination]))
//        addVertex(destination);
//
//    V temp = getEdge(destination,name,distance);
//    temp->next = map[source]->next;
//    map[source]->next = temp;
//}

void readFile (struct Graph* graph)
{

    FILE *in = fopen("segments.txt","r");

    int *pointer = " ";
    char *divide;
    char line [50];

    int flag;
    int nameCondition;
    int source, destination, distance;
    int source2, destination2;
    char name[50] = "";
    char tmp[50];
    char prev[50];

    char *ptr = strtok(tmp, " ");

    while (fgets(line,50,in))
    {
        divide = strtok (line, pointer);
        flag = 0;
        nameCondition = 0;
        while (divide != NULL)
        {

            if (flag == 0)
            {

                source = atoi(divide);
                destination2 = atoi(divide);
                flag++;

            }
            else if (flag == 1)
            {

                destination = atoi(divide);
                source2 = atoi(divide);
                flag++;

            }
            else if (flag == 2)
            {

                ptr = divide;
                strcpy(prev, ptr);
                strcpy(name, " ");
                ptr = strtok(NULL, " ");
                while((ptr != NULL))
                {
                    strcat(name, prev);
                    strcat(name, " ");
                    strcpy(prev, ptr);
                    if (isInteger(ptr))
                        distance = atoi(ptr);
                    else
                        distance = 0;
                    ptr = strtok(NULL, " ");
                }

            }

            divide = strtok (NULL,pointer);
        }
        addEdge(graph,source,destination,name,distance);
        addEdge(graph,source2,destination2,name,distance);
    }
    fclose(in);

}

//void printFile (int numberOfCities)
//{
//    FILE *out = fopen("output.txt","w");
//
//    for(int i = 0; i<numberOfCities; ++i)
//    {
//
//        V current = map[i];
//
//        while (current != NULL)
//        {
//
//            fprintf(out,"{id : %d, distance : %d , street name: %s}-->",current->id,current->distanceBetweenVertices,current->nameOfCities);
//            current = current->next;
//        }
//
//        fprintf(out,"NULL\n");
//
//    }
//    fclose(out);
//}

//int shortestRout (int source, int destination){
//
//    if (source == destination)
//                return 0;
//        else if
//
//    int temp1=0,temp2=0;
//    V current = map[0];
//
//    for(int i = 0; i<numberOfCities; ++i)
//    {
//
//        V current = map[i];
//
//        while (current != NULL)
//        {
//            if (current->id == destination)
//                break;
//
//            current->distanceBetweenVertices = current->distanceBetweenVertices + current->next->distanceBetweenVertices;
//
//            current = current->next;
//            temp1 = current->distanceBetweenVertices;
//
//        }
//
//    }
//
//    return temp1;
//}

//
//void cost (int source,int destination){
//
//    V current = map[source];
//    if (getVertex(destination) != NULL){
//          while (current != NULL)
//        {
//            if (current->id == destination){
//                costBetweenTwoEdges = current->distanceBetweenVertices;
//                    printf("\nThe cost is: %d\n",costBetweenTwoEdges);
//            }
//             if ((current->id != destination) && (current->next == NULL))
//                    printf("\nThe edges you chose are not directly connected!!\n");
//
//            current = current->next;
//        }
//    }
//}

int main()
{

    numberOfCities = calculateNumberOfCities();
    printf("The number of cities is : %d\n",numberOfCities);
        struct Graph* graph = createGraph(numberOfCities+1);

//    createMap(numberOfCities);

    readFile(graph);

//    int past=0,k=0,source,destination;
//    V current = map[0];
//    for(int i = 0; i<numberOfCities; ++i)
//    {
//
//        V current = map[i];
//        if (past>numberOfEdges)
//                    numberOfEdges=past;
//
//        past=0;
//        k=0;
//        while (current != NULL)
//        {
//
//            printf("{id<%d> : %d, distance : %d , street name: %s}-->",k,current->id,current->distanceBetweenVertices,current->nameOfCities);
//            current = current->next;
//            k++;
//            past++;
//
//        }
//
//        printf("NULL\n");
//
//    }
//    printFile(numberOfCities);

    printf("\nThe max number of edges is %d\n",(numberOfEdges-1));

//    printf("\nEnter the source city:\n");
//    scanf("%d",&source);
//    printf("\nEnter the destination:\n");
//    scanf("%d",&destination);
//
//    printf("\nThe shortest path is:\n%d",shortestRout(source,destination));

//    cost(1,1);
//    printf("\n%d\n",costBetweenTwoEdges);

    dijkstra(graph,295);

    return 0;
}
