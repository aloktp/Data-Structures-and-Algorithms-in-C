//
// Created by Alok T P on 10/11/2023.
//

/**
Time complexity is O((m*s)^2). Weighted Graph and queue are used.

m -> the number of timetables
s -> the maximum number of stops on any one timetable

Breadth First Search is used to find the path.
For stage 2, the time complexity is O((m*s)^2). For all the rest of the functions, its O(m*s). Hence, overall worst-case time complexity is O((m*s)^2).

I used a double pointer for the multiple timetables and its derefernced single pointer for a single timetable/train.
**/

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<string.h>  //Necessary for string compare function later on in main function for the user inputing to be stopped when user enters "done"
#include"WGraph.h"
#include"queue.h"

#define SIZE 200 //arbitrary value for number of valid destination
#define NUM 200 //arbitrary number

int visited[NUM];  // array to store visiting order
// indexed by vertex 0..nV-1


/* In C, the size of an array must be known at compile time. i.e. you need to use the symbolic constant #define ARRAYSIZENUMBER etc e.g. #define NUM_OF_STATIONS and when you define an array, you type Stations [NUM_OF_STATIONS]

Since, the number of Stops or Stations or number of Timetables and size of network
all are dynamic values i.e different values for different test cases depending on the values the user inputs, we cant use FIXED SIZE arrays.
Hence, it makes no sense to use symbolic constants.

Hence, we use flexible arrays i.e. arrays without size mentioned eg. stop Stops[] .. another example , int a[]
AND also WE use dynamic data structures ONLY i.e. pointers + memory allocation.

I can use flexible arrays only for the last member in a structure. For the heirarchical structures above, its compulsory to use memory allocation.
 i.e. I cant use the flexible array feature for time tables too
 i.e the following not allowed
 typedef struct {
   int numStops // THIS IS IMPORTANT..or atleast put some arbitrary statement.    a flexible array member must be part of a struct that contains at least one other member. If you want to create an array of structures where each structure contains a flexible array of stops, you need to include at least one regular member in the struct. Otherwise, Error : Error : Flexible array member 'stops' not allowed in otherwise empty struct
   Stop stops[];  // flexible array member .. allowed if last structure in the heirarchical strucutre. Here, the structure is timetables->stops. So, last structure
  } Timetable;

  Timetable timetables[];  // array of Timetable structures //ARRAY is given here as FLEXIBLE. NOT ALLOWED.
   // Either give value inside square brackets or dynamically allocate memory using pointers and malloc function.

   // This declares an array of Timetable structures where each Timetable structure contains a flexible array member stops[]. However, you still need to allocate memory for each Timetable structure separately. You can't avoid dynamic memory allocation if you want to determine the size of the array at runtime.
   // i.e. int main() {
   //   int numStops = 3;  // for example
   //
   //    // Allocate memory for the Timetable structure
   //    Timetable* timetable = malloc(sizeof(Timetable) + numStops * sizeof(Stop));
   //
   //    if (timetable == NULL) {
   //        fprintf(stderr, "Memory allocation failed\n");
   //        return 1;
   //    }
*/

//BASICALLY USE FLEXIBLE ARRAYS AT THE LOWEST ELEMENT ARRAY and use dynamic array for everything else.


int stat_no = -1;   //Stop number to be incremented by 1 each time a new stop is entered

//Also, TOTAL NUMBER OF STOPS to be given as number of vertices of graph = stat_no + 1

typedef struct network
{
    char name[17];  //Station-name
    int transfer_time;   //Transfer time to another train in the station
}network;    //Creating a structure is a no-brainer. You have to create a structure because there are two members. But how to access the structure.. whether with a pointer (i.e. allocating memory using malloc) OR create an Array of Structures is upto you.

network *netp; //network pointer


typedef struct timetable
{
    int numStops;
    stop stops[];
}timetable;

timetable *timep; //Timetable pointer
//Dont declare static
//If I declare 'STATIC' outside functions, it is only available among all functions in this C file
//BUT NOT AVAILABLE among other C files.. i.e. even if I link it with Graph.c etc.

//static timetable **timetables; //Pointer to pointer.... DONOT DECLARE THIS. AS I ALREADY DECLARED later on in main. Maybe thats why Segmentation fault is happening.
//Later when I malloc memory to thie double pointer,.. I can use it as an array variable i.e. timetables[] .. array of pointers which I will point to temporary variable for FOR loop i.e.'timep' addresses.. thereby I can directly access the timetables using this timetable array i.e.timetables[]
//But still timetables is not accessible in all functions without passing as argument. I DON'T KNOW WHY.. because when I remove parameter, .. segmentation fault
//When you declare it static, the value inside cannot be changed, I guess


int netsize, numt; //Global variables... since I need to access it across all function and their values never change, its best to declare them as global variables now.


int gettime(int time, int tt)
{
    int hh = time/100;
    int mm = time%100;

    if (tt>60) {
        hh = hh + tt/60;
        mm = mm + tt % 60;

        if (mm > 60)
        {
            hh = hh + mm/60;
            mm = mm + mm % 60;
        }
        else if (mm == 60)
            hh = hh + 1;
        else
            { //empty
            }
    }
    else if (tt == 60)
        hh = hh + 1;
    else  //tt<60
    {
        mm = mm + tt;
        if (mm > 60) {
            hh = hh + mm/60;
            mm = mm + mm % 60;
        } else if (mm == 60)
            hh = hh + 1;
        else {//empty
        }
    }

    time = (hh*100) + mm;
return time;   //YOU CAN'T RETURN MORE THAN ONE VARIABLE IN C
}

void PrintBFSPath(Graph g, int from_sta_no, int y, timetable **timetables) //print out Path
{
    { 
        int a = y;
        int ct = 1; //just to get the length of the visited vertices from dest to src
        while (a != from_sta_no)
        {
            //printf("%d - ", v);
            a = visited[a];
            ct++;
        }
        //printf("Count %d\n", count);
        //Now to save stop_no from source to destination in an array
        int o = ct;
        int *arr = malloc(ct * sizeof(int)); //Its indices start from 0 to (count-1)
        assert(arr != NULL);
        int b = y;
        ct--;
        arr[ct] = y;
        while (b != from_sta_no)
        {
            //printf("%d - ", v);
            b = visited[b];
            ct--;
            arr[ct] = b;
        }

        // You have to use a forloop to print an integer array as there is no nullterminated \0 string like in string array
        char prev[17]="gh"; //Temporary
        for (int d = 0; d < o; d++)
        {   /*
            int h = arr[d]; //It doesnt work if you directly type arr[d] inside the square brackets of stops
            if (strcmp(timetables[i]->stops[h].name, prev) == 0)
            {
                printf("Change at %s\n", timetables[i]->stops[h].name);
            }*/
            for (int i = 0; i < numt; i++) //Going through all timetables
            {
                for (int j = 0; j < (timetables[i]->numStops); j++)
                {
                    if (timetables[i]->stops[j].stopno == arr[d])
                    {
                        if(strcmp(timetables[i]->stops[j].name, prev)==0)
                        {
                            printf("Change at %s\n", timetables[i]->stops[j].name);
                        }

                        printf("%s %s\n", timetables[i]->stops[j].time, timetables[i]->stops[j].name);


                        memset(prev, '\0', sizeof(prev)); //To clear all elements of prev to '\0' //Unnecessary
                        strcpy(prev, timetables[i]->stops[j].name); //or you can give stops[arr[d]] instead of stops[j] also
                    }
                }
            }
            //printf("Arr %d\n", arr[d + 1]);
            /* This is wrong because both stations can be of different timetables, but "i" here is same.Correct code given before
            if (strcmp((timetables[i]->stops[arr[d]].name), (timetables[i]->stops[arr[d + 1]].name)) == 0)
            {
                printf("Change at %s\n", timetables[i]->stops[arr[d]].name);
            }
            */
        }
        free(arr);
    }
}


/*Insertion Sort for Stage 3 to arrange arrival time i.e. destination similar stations in ascending order of arrival times
//MODIFIED INSERTION SORT ALGORITHM .. Sort not by Stopno, but but the arrival times of the Stop numbers.
/* Insertion Sort will make it unnecessarily complicated. Avoid.
void insertionSort(int array[], int n) 
{
    int i;
    for (i = 1; i < n; i++) {
        int element = array[i];                 // for this element ...
        int j = i - 1;
        while (j >= 0 && array[j] > element) {  // ... work down the ordered list
            array[j + 1] = array[j];               // ... moving elements up
            j--;
        }
        array[j + 1] = element;                   // and insert in correct position
    }
}*/


// Breadth-first Search 
bool findPathBFS(Graph g, int src, int dest) {
    int v;
    int nV = numOfVertices(g);
    for (v = 0; v < nV; v++)
        visited[v] = -1;

    queue Q = newQueue();
    QueueEnqueue(Q, src);
    visited[src] = src;
    while (!QueueIsEmpty(Q)) {
        v = QueueDequeue(Q);
        if (v == dest) {
            dropQueue(Q);
            return true;
        }
        else {
            int w;
            for (w = 0; w < nV; w++) {
                if (adjacent(g, v, w) && visited[w] == -1) {
                    QueueEnqueue(Q, w);
                    visited[w] = v;
                }
            }
        }
    }
    dropQueue(Q);
    return false;
}


//Leaves as late as possible.. this only matters for the from station. Intermediate stations dont have to follow this rule.
//SInce there can be utmost one path, there should only be ONE to-station or NO to-station.

int stage2(char fromname[17],char toname[17], char arrivaltime[5],timetable **timetables)
{
    int i,j,k,l;
    //char *k; //k is a pointer - UNUSED
    //printf("netsize %d numt %d",netsize,numt); //Just a check to see if I can access the global variables inside this function
    //ONLY UTMOST ONE POSSIBLE CONNECTION. THINK LIKE A MAZE. GO BACKWARDS FROM DESTINATION TO SOURCE.
    //From station time late or early is not important(Late "as possible"). The 2nd LAST station being late matters and so on for all stations when traced back. The tostation time being late or not doesnt matter ("LEAVES" as late as possible - train never leaves to-station as it is last stop) BUT it must be before arrivaltime.
    
    int count = 0; //There can be to-station in any of the train routes, but what if all of them are after "arrivaltime" ? . To check that -> "no connection"
    Edge e;
    Graph g = newGraph(stat_no + 1);  //Netsize is NOT the number of vertices of the graph. Remember same station can be in many trains. We have to represent them by different vertex because they have different arrival/departure time and the transfer time between these same stations also matters
    for(i=0;i<numt;i++)
    {
        for(j=0;j<timetables[i]->numStops;j++) 
            {
                if (strcmp(timetables[i]->stops[j].name, toname) == 0)  //To-station is in this trains route
                    {
                        if(atoi(timetables[i]->stops[j].time)<=atoi(arrivaltime)) //Also if true, a starting point to trace back
                            {
                                count++;   
                             }       
                     }
            }
    }
    if (count==0)
    {
        printf("No connection.\n"); //No connection
    }
    
    else  //Have to create Graph for here on
    {   //First try to connect all stations on a single route
        for (i = 0; i < numt; i++) //Going through all timetables
        {
            //for(x=&(timetables[i]->stops[0]);x=&(timetables[i]->stops[(timetables[i]->numStops)-2]);x++)  //k is a pointer // Aborted
            for (j = 0; j < (timetables[i]->numStops) - 1; j++) //Till the 2nd last stop of a train
            {
                e.v = timetables[i]->stops[j];
                e.w = timetables[i]->stops[j + 1];
                insertEdge(g, e);
            }
        }

        //Now for same station name connection .. I have to implement it as a seperate for loop. Cant nest it in the same train stations linking forloop because it only iterates to the 2nd last station.
        for (i = 0; i < numt; i++) //Going through all timetables
        {
            for (j = 0; j < (timetables[i]->numStops); j++) //No -1 here. It should go till the last station.
            {
                for (k = 0; k < numt; k++) //Going through all timetables
                {
                    if (k != i)
                    {
                        for (l = 0; l < (timetables[k]->numStops); l++)  //DONOT GIVE -1 here. It should check even the last stop of a train route
                        {
                            if (strcmp((timetables[i]->stops[j].name), (timetables[k]->stops[l].name)) == 0)
                            {
                                int a = atoi(timetables[i]->stops[j].time);
                                //printf("%d\n", a);
                                int b = timetables[i]->stops[j].transfer_time;  //Already an integer //NO NEED TO DO ATOI FUNCTION
                                //printf("%d\n", b);
                                int c = gettime(a, b); //New time is stored in c
                                //printf("%d\n", c);
                                if (c <= atoi(timetables[k]->stops[l].time))
                                {
                                    e.v = timetables[i]->stops[j];
                                    e.w = timetables[k]->stops[l];
                                    insertEdge(g, e);
                                }
                            }
                        }
                    }
                }
            }
        }
        // Find station number of from station (if multiple from stations of same name) which has latest departure time
        int dep_time = 0; //temporary variable to get station number of latest departure time
        int from_sta_no; // temporary variable to store source station_no

        int arr_sta_no[SIZE]; //temporary variable to get store valid destination station number 
        // I can create an array of size "numt" using malloc but I too tired to do so and having to create pointers to manipulate

        int c = -1; //if number of valid destination staion number is more than 1 , then stage 3
        for (i = 0; i < numt; i++)
        {
            for (j = 0; j < (timetables[i]->numStops); j++)
            {
                if (strcmp((timetables[i]->stops[j].name), fromname) == 0)
                {
                    if (atoi(timetables[i]->stops[j].time) > dep_time)
                    {
                        dep_time = atoi(timetables[i]->stops[j].time);
                        from_sta_no = timetables[i]->stops[j].stopno;  //Now we got departure station number with latest departure time
                    }
                }
                // For destination station no.
                if (strcmp((timetables[i]->stops[j].name), toname) == 0)
                {
                    if (atoi(timetables[i]->stops[j].time) <= atoi(arrivaltime))
                    {
                        c++;
                        arr_sta_no[c] = timetables[i]->stops[j].stopno;
                    }
                }
            }
        }
        //For departure time, in Stage 2 and Stage 3, we are only taking latest departure time.. so no need to worry about multiple departure time cases.
        //No need to take difference between times of to and from station as departure station is same. So, only need to get lowest to-station.
        if (c > 0) //STAGE 3 //More than one valid destination (ie. arrival) station
        {
            //stage 3
            //insertionSort(arr_sta_no, (c + 1)); //Since we are passing arrays, its equivalent to call by reference. So, original arr_sta_no will be modified whenever a change in "array[]" occurs in insertion sort function.
            int u = 2500; //Temporary garbage value
            int x = -1; //Temporary
            for (int r = 0; r <= c; r++)
            {
                for (i = 0; i < numt; i++)
                {
                    for (j = 0; j < timetables[i]->numStops; j++)
                    {
                        if (timetables[i]->stops[j].stopno == arr_sta_no[r])
                        {
                            if (atoi(timetables[i]->stops[j].time) <= u)
                            {
                                if (findPathBFS(g, from_sta_no, arr_sta_no[r])) //there is a path
                                {
                                    u = atoi(timetables[i]->stops[j].time);
                                    x = r;
                                }
                            }
                        }
                    }
                }

            }
            if (x == -1) //No path works i.e. from "Utmost" one connection .. 0 connection
                printf("No connection.\n");
            else //There is a single shortest overall path
            {
                findPathBFS(g, from_sta_no, arr_sta_no[x]);  //visited array will be updated for this specific path so that we can print it
                PrintBFSPath(g, from_sta_no, arr_sta_no[x], timetables);
            }
        }
        else
        {
            if (findPathBFS(g, from_sta_no, arr_sta_no[0]))
            {
                PrintBFSPath(g, from_sta_no, arr_sta_no[0], timetables);
            }
            else
            {
                printf("No connection."); //No connection - No path between latest departure start and single end station.     
            }
        }
    }
    // free(arr); arr MUST be freed at its local scope ONLY i.e. the if -statement only
    //showGraph(g);
    freeGraph(g);
    //free(g);  This lead to double free in Vlab PC. Only freeGraph is necessary i guess.
    return 0;
}


int stage1(char fromname[17], char toname[17], char arrivaltime[5],timetable **timetables) //you can declare timetables as just a normal pointer instead of a double pointer if you are not going to modify the original timetables pointer inside this stage1 function.  If you only need to access the data pointed to by timetables, then declaring the parameter as timetable *timetables is sufficient.If you need to modify the original pointer (e.g., you want to allocate memory within the function and update the pointer to point to the newly allocated memory), you should declare the function parameter as a pointer to a pointer (timetable **) to allow modifications to the original pointer.
{
    //DONOT "FREE" TIMETABLES while inside any function. It actually frees the memory.. because pointer after all saves memory address. AND also, if you have a free statement at end of main function , then "double free" error will show.
    int i, j, si = -1, sj = -1, ei = -1, ej = -1, count = 0; //s and e are to save start and end index of from and to stations. Initalized as -1. //count is to check if from/to stations exist in more than one timetable, in which case have to pass to stage 2.
    //char sname[17]; char stime[5]; int statime; int atime; //IMPORTANT FOR DEBUGGIN.. as I dont know how to look into heapspace in VSCODE or anything.
    for (i = 0; i < numt; i++) 
    {
        for (j = 0; j < timetables[i]->numStops; j++)
        {
            //strcpy(sname,timetables[i]->stops[j].name); //For debugging
            //printf("%s\n", sname); //For debugging
            if (strcmp(timetables[i]->stops[j].name, fromname) == 0) //Matching station name in a train timetable
            {
                si = i;    //Store the index number of timetable
                sj = j;    //Store the index number of stop
                count++; //Number of times either fromname or toname is encountered while iterating through all timetables.
            }
            //IDEAL case for Stage 1 is from and to exists only in a single timetable.

            if (strcmp(timetables[i]->stops[j].name, toname) == 0) //Matching station name in a train timetable
            {
                ei = i;    //Store the index number of timetable
                ej = j;    //Store the index number of stop
                count++; //Numb̥er of times either fromname or toname is encountered while iterating through all timetables.
            }
            // DONOT USE == for string comparison i.e. Donot type (timetables[i]->stops[j].name == fromname)||(timetables[i]->stops[j].name == toname)
            //The equality operator == is for comparing single characters or numerical values, not entire strings.
            //Use strcmp
        }
    }
    //si == ei .. what if from station in one timetable and tostation found only once but in another timetable
    if (si==ei && count == 2 && si!=-1 && ei!=-1)
    {
        /*For debugging purpose
        //stage1 ...
        //ALSO YOU DONOT NEED TO CONSIDER TRANSFER_TIME as we are travelling in SAME TRAIN in this case i.e. not Changing train

        //strcpy(stime, timetables[si]->stops[ej].time);
        //printf("%s\n", stime);

        //statime = atoi(timetables[si]->stops[ej].time);

        //printf("%d\n", statime);
        //atime = atoi(arrivaltime);

        //printf("%d\n",atime);

        */

        if ((atoi(timetables[si]->stops[ej].time)) <= (atoi(arrivaltime))) //Its (either si or ei) && ej i.e. to station time. arrivaltime input by user is passed as parameter to this function.
        {
            for(int l=sj; l<=ej; l++)  //Index so <= & not <
                printf("%s %s\n",timetables[si]->stops[l].time,timetables[si]->stops[l].name);
        }
    }
    else if((si!=-1 && ei!=-1 && si!=ei && count == 2)||(count>2)) //1st case is e.g. from in one timetable and to in another timetable but from and to only appear once in all timetables together.  2nd case i.e. count>2 will also prevent case when there is from and to in last timetable because for loop continues till last iteration, but the count is also greater than 2 . i.e. more than one from or more than one two or both ..
    {//There may be better connections or there may be NO connections at all i.e. e.g. from station in one train and to station in another train, but both of them have no other common stations which both trains pass through
        //Stage 2 to 3 .. i.e. need to check for connections
        stage2(fromname,toname,arrivaltime,timetables);
    }
    else if(si==-1 || ei==-1)  //From station OR to station OR both doesnt exist in any timetable
    {
        printf("No connection.\n");
    }
    else //In C , last has to be an else . Last cannot be an else if
    {
        // Empty .. basically equivalent ot pass statement in python. It does nothing
    }  
    return 0;
}


int main(void) {
    //int netsize; 
    printf("Size of network: ");
    scanf("%d", &netsize); //network size
    netp = malloc(netsize * sizeof(network));
    assert(netp!=NULL);
    int i;
    for(i=0;i<netsize;i++)
    {
        scanf("%s",netp[i].name);    // VERY IMPORTANT. In the code above, netp[i].name is an array,i.e. a string is an array and can be pointed so you don't need to use & with it.
        scanf("%d",&netp[i].transfer_time); // IMPORTANT. However, you do need to use & for netp[i].transfer_time because it's a scalar (integer) variable.
    }

    printf("Number of timetables: ");
    //int numt; 
    scanf("%d",&numt); //number of timetables

    /* Now for timetables, since the number of stops can be different in each time table, we have to have DIFFERENT memory allocations for EACH TIMETABLE.
     * Hence, we have to use double pointer. i.e. timet pointing to an array of timep pointers, each of which point to each timetable ...
     * Hence, there are two ways to implement this
     * In each for loop, timep will be used like a "temporary" pointer which holds the address of the current timetable i.e. of current iteration
     * timet will thus store all the addresses of different addresses moving through timep.
     * timet being a pointer to a pointer is a double pointer or you can directly access it like an array. Since, array is technically a pointer object.
     * The effect of both methods are the same.
     *
     * i.e. timetable **timetables = malloc(numTimetables * sizeof(timetable *));
     *                            VS
     *      timetable *timetables[numTimetables];
     *
     * So, when we do for loop for each timetable iteration, the double pointer will point to a different pointer address pointing to a different timetable.
     * In any case, since there are MULTIPLE timetables, array which is a data structure which can store multiple values OR Equivalently, multiple address which point to different timetable structures ... these multiple addresses can be stored in multiple pointers. i.e. a pointer to a pointer. i.e. double pointer... EVen if you initialize a double pointer, you need to use the [ ] array symbol to store multiple pointer addtesses. Check MEthod 2 in scratch
     * We could have used the double pointers or any pointers WITHOUT the [ ]symbol if we knew the starting and ending addtess. But, since we dont know it , no point .
     * Both ways to do so I have shown as 2 Examples in my struct code scratch.txt
     * For ease, I am not going to use double pointer. I will stick with array.
     * stops is a flexible array member, and it dynamically allocates memory for each timetable and its stops. In the C programming language, flexible array members must always be the last member in a struct,
     *
     *The correct way to allocate memory for a struct with a flexible array member is:
        timetable *timep = malloc(sizeof(timetable) + numStops * sizeof(stop));
      This allocates memory for the fixed-size part of the struct (sizeof(timetable)) plus the space needed for the flexible array (numStops * sizeof(stop)).
     */

    // Allocate memory for an array of timetable pointers
    timetable **timetables = malloc(numt * sizeof(timetable *));  //the timetables double pointer is ultimately i.e. in the end, its pointing to timetable data structure .. hence the syntax timetable **timetables on LHS.
    assert(timetables!=NULL);
    //Read data for each timetable
    
    for(i=0;i<numt;i++)
    {
        printf("Number of stops: ");
        int nums; //number of stops
        scanf("%d", &nums);

        //Allocate memory for all stops in a single timetable
        timep = malloc(sizeof(timetable) + nums * sizeof(stop));  //This allocates memory for the fixed-size part i.e. int numStops... of the struct (sizeof(timetable)) plus the space needed for the flexible array (numStops * sizeof(stop)).
        assert(timep!=NULL);
        //Now timep pointer has the address of a timetable.
        //timep will be used like a temporary address storer of each timetable. Each double pointer in the double pointer array

        //To store the number of stops in a timetable
        timep->numStops = nums;

        //Now to store the stop details i.e. name and arrival/departure time of a stop in the Stop data structure
        int j,k; // USE DIFFERENT VARIABLE NAME .. both for loops inner and outer shouldnt clash .. so dont use i.
        for (j = 0; j < nums; j++)
        {
            stat_no++;
            scanf("%s", timep->stops[j].name);  //name is a string. i.e. array or can be pointed to. SO no need of & operator.
            scanf("%s", timep->stops[j].time);
            // Brackets not necessary i.e. scanf("%d", &(timep->stop[i].time));  // because & operator has has a higher precedence than the -> operator, so the statement without parentheses is already correct and will work as intended:
            //But you can use brackets as a good practice.


            timep->stops[j].stopno = stat_no;  //First stop number becomes 0 for Vertex 0.

            //NEXT is VERY IMPORTANT
            for(k=0;k<netsize;k++)  //Give transfer time to stop
                if(strcmp((timep->stops[j].name),(netp[k].name)) == 0)
                    timep->stops[j].transfer_time = netp[k].transfer_time;  //Update transfer time also
        }   

        // Store the timetable in the array of pointers
        timetables[i] = timep;  //i.e. timetables[0] pointing to the address temporarily stored in timep in the 1st iteration. Next, timetables[1] pointing to the address temporarily stored in timep in the 2nd iteration.
        //Even if timetables is a double pointer, we need to use [ ] array symbol ...
        //timetable's'[i] ... where timetable's' is the double pointer.
    }
    printf("\n");

    //User Give from and to station name
    char fromname[17];
    char toname[17];
    char arrivaltime[5];  //VERY VERY IMPORTANT. Arrivaltime is 5 characters including the '\0'. I encountered bufferoverflow when I kept it as 4. And what happened was the first character of toname got overwritten.

    //strcpy(fromname,"abcdefghijklmnop"); // Prevents errors.. better to give some value to initialized string.
    //strcpy(toname,"abcdefghijklmnop");  //just giving random values .. its always good to put some values in the strings just after you intiialized

    printf("From: ");
    memset(fromname, '\0', sizeof(fromname));  //Ensure all 16 characters are initialized to 0 before scanning a new string. Not exactly necessary for this program, but just for my fear. Also, memset is good for example passwords: Security: When working with buffers that will hold sensitive data (like passwords), it's a good practice to zero out the buffer once you're done with it so that the sensitive data isn't left in memory where it might inadvertently be accessed later.
    scanf("%s", fromname);
    while(strcmp(fromname, "done")!= 0) //never ending loop until break statement //Since its a character array, no need of & symbol
    {
        //printf("From: ");
        //scanf("%s", fromname);

        //if (strcmp(fromname, "done") == 0)  //i.e. if both strings are equal
        //    break;

        printf("To: ");
        memset(toname, '\0', sizeof(toname));
        scanf("%s", toname);

        printf("Arrive at or before: ");
        memset(arrivaltime, '\0', sizeof(arrivaltime));
        scanf("%s", arrivaltime); //& not necessary. Already a string array.

        printf("\n");

        stage1(fromname, toname, arrivaltime,timetables); //No need of & for toname as its already array ie. string. And when string is passed, the address of first element is passed.
        //netsize for stage 2 through stage 1 or after stage 1
        printf("\n");
        printf("From: ");
        memset(fromname, '\0', sizeof(fromname));
        scanf("%s", fromname);
        //printf("\n");
    }
    printf("Bye\n");
    // Don't forget to free the allocated memory when you're done
    for (int t = 0; t <numt; t++)
    {
        free(timetables[t]);  //This frees all the timep values
    }

    free(timetables);  //This frees all the double pointer or the addresses stored in the elements of the array of timetable pointers
    // the statement free(timetables); does not free the memory of all the elements (pointers) in the array of pointers. Instead, it frees the memory that was allocated for the array of pointers itself.
    //free(timep); //DONOT Free this. Because even the last address in time p is freed in the previous for loop of timetables freeing.
    // If you free this, it will become a double free error "Process finished with exit code -1073740940 (0xC0000374)". Windows error
    //timep is The temporary address pointer .. free the memory stored in the last iteration
    free(netp);
    return 0;
}
