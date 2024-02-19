// Weighted Graph ADT interface ... COMP9024 23T2

#include<stdbool.h>

typedef struct GraphRep *Graph;


typedef struct stop
{
    int stopno;   //Stop number starting from index 0 and incrementing every time a new stop is entered by user. Even if two stops in two different train routes are the same i.e. same name, they ahve different arrival time. so i will as of now decide to create seperate stop IDs for them.
    char name[17];  //Station-name
    char time[5]; //in format hhmm //.Arrival or departure time ..User can only input a char data type using scanf ..
    //ALSO, WHEN YOU GET THE NEW TIME FROM GETTIME FUNCTION i.e.after adding transfer time.. you cant return two variables. C program DOESNT ALLOW YOU TO "RETURN" MORE THAN 1 VARIABLE.
    //HENCE, ATLEAST THERE I HAVE TO COMBINE HH AND MM TO STRING AND RETURN IT. SO, HERE TO TO BE UNIFORM, LET IT BE STRING INSTEAD OF int hh,mm
    //VERY IMPORTANT - 5 characters including \0 and not 4 characters. //I encountered bufferoverflow when I kept it as 4. And what happened was the first character of another variable got overwritten.
    int transfer_time;
}stop;          //Better to use character data type rather than int. Atleast for Stage 1 where I dont have to add transfer_time, having character i.e. string format readily available as parameter .. i can easily print it.
//For Stage 2 onwards, where I have to add transfer_time, I can convert the character time into an integer using atoi function.


typedef stop Vertex;  //Stops are Vertex for the graph

// edges are pairs of vertices (end-points) with a weight
typedef struct Edge {
   Vertex v;
   Vertex w;
   //int weight; //NOT USED   //Transfer time for same stop but at different trains.
} Edge;

Graph newGraph(int);
int   numOfVertices(Graph);
void  insertEdge(Graph, Edge);
void  removeEdge(Graph, Edge);
int   adjacent(Graph, int, int);  //I Changed // returns weight, or 0 if not adjacent
void  showGraph(Graph);
void  freeGraph(Graph);
bool  findPathBFS(Graph, int, int);  //Function I added extra

//Always use Guards at the bottom and top of header file. It cleared the error I was having in Vlab Univ PC that there were multiple definitions of visited array.
