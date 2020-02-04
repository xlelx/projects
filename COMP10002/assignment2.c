/* Solution to comp10002 Assignment 2, 2019 semester 2.

   Authorship Declaration:

   (1) I certify that the program contained in this submission is completely
   my own individual work, except where explicitly noted by comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students,
   or by non-students as a result of request, solicitation, or payment,
   may not be submitted for assessment in this subject.  I understand that
   submitting for assessment work developed by or in collaboration with
   other students or non-students constitutes Academic Misconduct, and
   may be penalized by mark deductions, or by other penalties determined
   via the University of Melbourne Academic Honesty Policy, as described
   at https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will
   I do so until after the marks are released. I understand that providing
   my work to other students, regardless of my intention or any undertakings
   made to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring
   service, or drawing the attention of others to such services and code
   that may have been made available via such a service, may be regarded
   as Student General Misconduct (interfering with the teaching activities
   of the University and/or inciting others to commit Academic Misconduct).
   I understand that an allegation of Student General Misconduct may arise
   regardless of whether or not I personally make use of such solutions
   or sought benefit from such actions.

   Signed by: Khant Thurein Han 1028138
   Dated:     9th October 2019

*/


/* Libraries that are used in the assigment*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/* Constants*/
#define STATUS1 "Initial cell in the route is wrong!\n"
#define STATUS2 "Goal cell in the route is wrong!\n"
#define STATUS3 "There is an illegal move in this route!\n"
#define STATUS4 "There is a block on this route!\n"
#define STATUS5 "The route is valid!\n"
#define STATUS6 "The route cannot be repaired!\n"
#define STAGE "==STAGE %1d=======================================\n"
#define DIVIDER "------------------------------------------------\n"
#define ENDING "================================================"
#define MAXDIGITS 5
#define MAXCHAR 999
#define INITIAL 10
#define MAXCELLPERLINE 5
#define TRUE 1
#define FALSE 0
#define DEFAULTCOUNTER 0
#define ABOVE 1
#define BELOW 2
#define LEFT 3
#define RIGHT 4
#define DIRECTIONS {ABOVE, BELOW, LEFT, RIGHT}
#define NOOFDIRECTIONS 4

/*DEFINED STRUCTURES*/

/*coordinate block*/
typedef struct{
	int r;
	int c;
}cell_t;

/*node block that store cell data and counter(stage2)*/
typedef struct node node_t;

struct node{
	cell_t cell;
	int counter;
	node_t *next;
};
/*linked list data structure*/
typedef struct {
	node_t *head;
	node_t *foot;
} list_t;


/*DECLARED FUNCTIONS*/
list_t* make_empty_list(void);
int samecell(cell_t *c1, cell_t *c2);
void free_list(list_t *list);
void free_blocks(cell_t** blocks,int blockn);
list_t* insert_at_head(list_t *list, cell_t value, int counter);
list_t* insert_at_foot(list_t *list, cell_t value, int counter);
int mygetchar();
int inblocks(cell_t **blocks, cell_t *new, int blockn);
int readline(char line[]);
int getcell(char line[], char cell[], int *linei);
cell_t readcell(char cell[]);
cell_t** readblocks(int *blockn, int *moreblocks);
list_t* readroute(int *moreblocks);
void printroute(list_t *route);
node_t* checkstatus(list_t *route, cell_t **blocks, cell_t initcell,
	cell_t goalcell, int blockn, int fixattempted,int toprint);
char** creategrid(list_t *route, cell_t** blocks, cell_t initcell,
	cell_t goalcell, int blockn, int rows, int cols);
void printgrid(char** grid, int rows, int cols);
cell_t* createpair(cell_t **blocks, node_t *pair, int blockn, int rows, int cols,
	int dir);
node_t* inlist(list_t *list, cell_t *new);
list_t* findendofbroken(list_t* route, cell_t **blocks, node_t *issue, 
	int blockn);
list_t* findstartofbroken(list_t* route, cell_t **blocks, node_t *issue, 
	int blockn);
list_t* combinelists(list_t *list1, list_t *list2);
list_t* fixroute(list_t* route, list_t* newpath, node_t *issue, list_t* endofbroken,
	list_t* startofbroken);
list_t* findpath(cell_t **blocks,list_t *queue, node_t *issue, int blockn,
	int rows, int cols);
list_t* solveonce(list_t* route,cell_t** blocks, node_t *issue, int blockn,int rows,int cols,
	int* nosol);
list_t* solveall(list_t* route,cell_t** blocks, node_t *issue, 
	cell_t initcell, cell_t goalcell, int blockn,int rows,int cols, int* nosol);
void stagetwo(list_t* route, cell_t** blocks, node_t* issue, cell_t initcell,
	cell_t goalcell,int blockn,int rows,int cols, int *mblocks);
void getdimensions(int *rows, int *cols);


/* driver code*/
int main(int argc, char* argv[]) {
	int rows, cols;
	char line[MAXCHAR+1];
	cell_t initcell, goalcell;
	
	printf(STAGE, 0);
	getdimensions(&rows, &cols);
	readline(line);
	initcell = readcell(line);
	readline(line);
	goalcell = readcell(line);
	
	int blockn = 0, moreblocks = 0;
	cell_t** blocks = readblocks(&blockn, &moreblocks);
	printf("The grid has %d block(s).\n",blockn);
	printf("The initial cell in the grid is [%d,%d].\n",initcell.r, initcell.c);
	printf("The goal cell in the grid is [%d,%d].\n",goalcell.r, goalcell.c);
	/*int i = 0;
	for (; i<INITIAL; i++){
		printf("::::::[%d,%d]\n", blocks[i]->r, blocks[i]->c);
	}*/
	
	//printf("cell r: %d\ncell c: %d\n", goalcell.r, goalcell.c);
	//printf("block pointer:%d\n", blocks);
	list_t* route = readroute(&moreblocks);
	printf("The proposed route in the grid is:\n");
	printroute(route);
	
	
	int fixattempted = FALSE, toprint = TRUE, nosol = FALSE;
	node_t *issue = checkstatus(route,blocks,initcell,goalcell,blockn,fixattempted,toprint);
	
	
	printf(STAGE, 1);
	
	char** grid = creategrid(route,blocks,initcell,goalcell,blockn,rows,cols);
	
	printgrid(grid, rows, cols);
	
	if(issue != NULL&&!samecell(&issue->cell, &goalcell)){
		printf(DIVIDER);
		route = solveonce(route,blocks,issue, blockn,rows,cols,&nosol);
		
		grid = creategrid(route,blocks,initcell,goalcell,blockn,rows,cols);
		printgrid(grid,rows,cols);
		printf(DIVIDER);
		toprint = TRUE, fixattempted = FALSE;
		if(!nosol){                    
			printroute(route);
		}
		checkstatus(route,blocks,initcell,goalcell,blockn,fixattempted,toprint);
	}
	if (issue!=NULL){
		stagetwo(route, blocks, issue, initcell,goalcell,blockn,rows,cols,&moreblocks);
	}
	printf(ENDING);
	free_list(route);
	free_blocks(blocks, blockn);
	
	return 0;
}


/*function from listops.c from lec07 comp10002
initializes a list that is empty*/
list_t* make_empty_list(void) {
	list_t *list;
	list = (list_t*)malloc(sizeof(*list));
	assert(list!=NULL);
	list->head = list->foot = NULL;
	return list;
}
/*checks whether the cell values are the same*/
int samecell(cell_t *c1, cell_t *c2){
	if (c1->r == c2->r && c1->c == c2->c){
		return TRUE;
	}
	return FALSE;
}
/*function from listops.c from lec07 comp10002
frees allocated memory*/
void free_list(list_t *list) {
	node_t *curr, *prev;
	assert(list!=NULL);
	curr = list->head;
	while (curr) {
		prev = curr;
		curr = curr->next;
		free(prev);
	}
	free(list);
}
/*free allocated memory of blocks*/
void free_blocks(cell_t** blocks,int blockn){
	int i;
	for( i =0; i<blockn;i++){
		free(blocks[i]);
	}
	free(blocks);
}
/*function from listops.c from lec07 comp10002
modified to include counter as an argument
adds a node to the end of a list*/
list_t* insert_at_head(list_t *list, cell_t value, int counter) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));
	assert(list!=NULL && new!=NULL);
	new->cell = value;
	new->counter = counter;
	new->next = list->head;
	list->head = new;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->foot = new;
	}
	return list;
}
/*function from listops.c from lec07 comp10002
modified to include counter as an argument
adds a node to the beginning of a list*/
list_t *insert_at_foot(list_t *list, cell_t value, int counter) {
	node_t *new;
	new = (node_t*)malloc(sizeof(*new));	
	assert(list!=NULL && new!=NULL);
	new->cell = value;
	new->counter = counter;
	new->next = NULL;
	if (list->foot==NULL) {
		/* this is the first insertion into the list */
		list->head = list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}



/* getchar() code obtained from Assigment1 FAQ to avoid potential erros*/
int mygetchar() {
	int c;
	while ((c=getchar())=='\r') {
	}
	return c;
}

/*checks whether a cell is a block*/
int inblocks(cell_t **blocks, cell_t *new, int blockn){
	int i;
	for (i = 0; i < blockn; i++){
		if (new->r == blocks[i]->r && new->c == blocks[i]->c){
			return TRUE;
		}
	}
	return FALSE;
}
/* reads a line from standard input*/
int readline(char line[]){
	char c;
	int i = 0;
	while((c = mygetchar()) != EOF && c != '\n'){
			line[i++] = c;
	}
	line[i] = '\0';
	if(c == EOF){
		return EOF;
	}
	return 0;
}

/*Records the dimensions of the grid and prints it out.*/
void getdimensions(int *rows, int *cols){
	char strrow[MAXDIGITS+1];
	char strcol[MAXDIGITS+1];
	char c;
	int i = 0;	
	while((c = mygetchar()) != 'x'){
		strrow[i++] = c;
	}
	strrow[i] = '\0';
	*rows = atoi(strrow);
	i = 0;
	while((c = mygetchar()) != '\n'){
		strcol[i++] = c;
	}
	strcol[i] = '\0';
	*cols = atoi(strcol);
	printf("The grid has %d rows and %d columns.\n", *rows, *cols);
}

/*gets a string of coordinates from a line
returns TRUE if it fails to do so*/
int getcell(char line[], char cell[], int *linei){
	int i = *linei, celli = 0;

	while(line[i]!= '['){
		if (line[i] == '\0'){
			return TRUE;
		}
		i++;
	}

	while(line[i]!= ']'){
		cell[celli++] = line[i];
		i++;
	}
	cell[celli++] = line[i];
	i++;
	cell[celli] = '\0';

	*linei = i;
	return FALSE;
}
/*Returns a cell type after reading a string form of a coordinate*/
cell_t readcell(char cell[]){
	cell_t result;                         
	int r, c, i = 0, celli = 1;
	char strr[MAXDIGITS+1];
	char strc[MAXDIGITS+1];
	
	
	while(cell[celli] != ','){
		strr[i++] = cell[celli];
		celli++;
	}
	strr[i] = '\0';
	r = atoi(strr);
	
	celli++;
	i = 0;
	while(cell[celli] != ']'){
		strc[i++] = cell[celli]; 
		celli++;
	}
	strc[i] = '\0';
	c = atoi(strc);
	result.r = r;
	result.c = c;
	return result;
}
/*records all the blocks from the standard input
and returns a set of pointers to cells that are blocks*/
cell_t** readblocks(int *blockn, int *moreblocks){	
	char line[MAXCHAR+1];
	cell_t **blocks;
	size_t current_size = INITIAL;
	cell_t *new;
	blocks = (cell_t**)malloc(current_size*sizeof(*blocks));
	int num = *blockn, end;
	do{
		end = readline(line);
		if(strlen(line) == 0){
			break;
		}
		if(line[0] == '$'){
			end = FALSE;
			break;
		}
		
		new = (cell_t*)malloc(current_size*sizeof(cell_t));
		*new = readcell(line);
		if (num == current_size){
			current_size *= 2;
			blocks = realloc(blocks, current_size*sizeof(*blocks));
		}
		blocks[num] = (cell_t*)malloc(sizeof(new));
		blocks[num++] = new;
	} while(end != EOF);
	*moreblocks = end;
	*blockn = num;
	return blocks;
}
/*reads a route and returns a linked list form of the route*/
list_t* readroute(int *moreblocks){
	list_t *route;
	route = make_empty_list();
	int linei = 0, endcell, endline;
	char cell[(MAXDIGITS*2)+4];
	cell_t new;
	char line[MAXCHAR+1];
	
	do {
		endline = readline(line);
		if(line[0] == '$'){
			break;
		}
		linei = 0;
		do {
			endcell = getcell(line, cell, &linei);
			if (endcell != TRUE){
				new = readcell(cell);
				//printf("cell r: %d\ncell c: %d\n", new.r, new.c);
				route = insert_at_foot(route, new, DEFAULTCOUNTER);
			}
		}while(endcell!=TRUE);
	}while(endline!=EOF);
	*moreblocks = endline;
	return route;
}

/*prints out the whole path of the specified list*/
void printroute(list_t *route){
	int count = 0;
	node_t *node = route->head;
	while(node!= NULL){
		printf("[%d,%d]", node->cell.r,node->cell.c);
		if (node->next == NULL){
			printf(".\n");
			break;
		}
		else{
			printf("->");
		}
		count++;
		if (count == MAXCELLPERLINE){
			printf("\n");
			count = 0;
		}
		node = node->next;
	}
}

/*determines the current state of a route
NULL is returned if the route is any of the status 1,2,3.
the pointer to the node that occurs before a block is returned for status 4
the pointer to the goalcell is returned if a route is valid*/
node_t* checkstatus(list_t *route, cell_t **blocks, cell_t initcell,
	cell_t goalcell, int blockn, int fixattempted,int toprint){
	int valid = TRUE;
	if(initcell.r != route->head->cell.r || initcell.c != route->head->cell.c){
		if(toprint){
		printf("%s",STATUS1);
		}
		valid = FALSE;
		return NULL;
	}
	if(goalcell.r != route->foot->cell.r || goalcell.c != route->foot->cell.c){
		if(toprint){
			printf("%s",STATUS2);
		}
		valid = FALSE;
		return NULL;
	}
	if(route != NULL){
		node_t *node, *prevnode;
		int rdiff, cdiff;
		prevnode = route->head;
		node = prevnode->next;
		while (node!=NULL){
			rdiff = abs(prevnode->cell.r - node->cell.r);
			cdiff = abs(prevnode->cell.c - node->cell.c);
			//printf("%d - %d\n",rdiff, cdiff);
			if ((rdiff > 1 && cdiff == 0) || (rdiff == 0 && cdiff > 1)){
				if(toprint){
					printf("%s",STATUS3);
				}
				valid = FALSE;
				return NULL;
			}
			prevnode = node;
			node = prevnode->next;
		}
	}
	if(blocks!=NULL){
		node_t *node, *prevnode;
		prevnode = NULL;
		
		node = route->head;
		//printf("%d - %d\n",node->cell.r,blocks[i]->r);      
		while(node!= NULL){
			if(inblocks(blocks, &(node->cell), blockn)==TRUE){
				
				if(fixattempted){
					if(toprint){
						printf("%s",STATUS6);
					}
				}
				else{
					if(toprint){
						printf("%s",STATUS4);
					}
				}
				
				valid = FALSE;
				return prevnode;
			}
			prevnode = node;
			node = node->next;
		}
	}
	if (valid == TRUE){
		if(toprint){
			printf("%s",STATUS5);
		}
	}
	return route->foot;
}

/*creates a 2 dimensional grid with the initial cell, goalcell, route and
blocks included*/
char** creategrid(list_t *route, cell_t** blocks, cell_t initcell,
	cell_t goalcell, int blockn, int rows, int cols){
	char **grid;
	grid = (char**)malloc((rows+1)*sizeof(char*));
	int i, j;
	for (i =0; i< rows+1; i++){
		grid[i] = (char*)malloc((cols+1)*sizeof(char));
	}
	for (i =0; i< rows+1; i++){
		for (j = 0; j < cols+1; j++){
			grid[i][j] = ' ';
		}
	}
	for (i = 1; i < rows +1;i++){
		grid[i][0] = ((i-1)%10)+ '0';
	}
	for (j = 1; j< cols +1;j++){
		grid[0][j] = ((j-1)%10)+ '0';
	}
	
	node_t *node;
	node = route->head;
	while(node != NULL){
		grid[node->cell.r + 1][node->cell.c + 1] = '*';
		node = node->next;
	}
	
	grid[initcell.r + 1][initcell.c + 1]= 'I';
	grid[goalcell.r + 1][goalcell.c + 1]= 'G';
	for (i = 0; i < blockn; i++){
		grid[blocks[i]->r + 1][blocks[i]->c + 1] = '#';
	}
	return grid;
}
/*prints the specified grid*/
void printgrid(char** grid, int rows, int cols){
	int i, j;
	for (i =0; i< rows+1; i++){
		for (j = 0; j < cols+1; j++){
			printf("%c", grid[i][j]);
		}
		printf("\n");
	}
}
/*returns a valid cell of the 1 block to the specified direction from
the current cell*/
cell_t* createpair(cell_t **blocks, node_t *pair, int blockn, int rows, int cols,
	int dir){
	cell_t* new = (cell_t*)malloc(sizeof(cell_t));
	if (dir == ABOVE){
		new->r = pair->cell.r - 1;
		new->c = pair->cell.c;
	}
	else if (dir == BELOW){
		new->r = pair->cell.r + 1;
		new->c = pair->cell.c;
	}
	else if (dir == LEFT){
		new->r = pair->cell.r;
		new->c = pair->cell.c - 1;
	}
	else{
		new->r = pair->cell.r;
		new->c = pair->cell.c + 1;
	}
	
	if((new->r < 0 || new->r >= rows)||(new->c < 0 || new->c >= cols)){
		return NULL;
	}
	int i;
	for (i = 0; i < blockn; i++){
		if (new->r == blocks[i]->r && new->c == blocks[i]->c){
			return NULL;
		}
	}
	return new;
	
}
/*verifies if a cell exists in the specified list*/
node_t* inlist(list_t *list, cell_t *new){
	node_t *node= list->head;
	while(node!= NULL){
		if(samecell(&node->cell, new)){
			return node;
		}
		node = node->next;
	}
	return NULL;
}
/*returns a list that is the remainder of a route that has been broken off 
by ONE block cell*/
list_t* findendofbroken(list_t* route, cell_t **blocks, node_t *issue, int blockn){
	node_t *node = issue->next->next; 
	while(node!=NULL){
		if(inblocks(blocks,&(node->cell), blockn)==FALSE){
			list_t *newlist;
			newlist = make_empty_list();
			newlist->head = node;
			newlist->foot = route->foot;
			//printf("ENDOFBROKEN:\n");
			//printroute(newlist);
			//printf("\n\n");
			return newlist;
		}
		node = node->next;
	}
	return NULL;
}
/*returns the route up until the cell before it hits a block cell*/
list_t* findstartofbroken(list_t* route, cell_t **blocks, node_t *issue, int blockn){
	node_t *node = route->head;
	node_t *prevnode = NULL;
	while(node!=NULL){
		if(samecell(&node->cell,&issue->next->cell)){
			list_t *newlist;
			newlist = make_empty_list();
			newlist->head = route->head;
			newlist->foot = prevnode;
			//printf("ENDOFBROKEN:\n");
			//printroute(newlist);
			//printf("\n\n");
			return newlist;
		}
		prevnode = node;
		node = node->next;
	}
	return NULL;
}
/*adds two lists together in order of list1 to list2*/
list_t* combinelists(list_t *list1, list_t *list2){
	list_t* newlist;
	newlist = make_empty_list();
	newlist->head = list1->head;
	newlist->foot = list2->foot;
	list1->foot->next = list2->head;
	return newlist;
}
/*returns a repaired version of a route at ONE block by combining a new path 
with the rest of the route*/
list_t* fixroute(list_t* route, list_t* newpath, node_t *issue, list_t* endofbroken,
	list_t* startofbroken){
	list_t* newlist;
	node_t *node, *new;
	newlist = make_empty_list();
	newlist->head = startofbroken->head;
	newlist->foot = startofbroken->foot;
	node = newlist->foot;
	newlist->foot->next = newpath->head->next;
	newlist->foot = newpath->foot;
	
	//printf("::::%d,%d\n\n", endofbroken->cell.r, endofbroken->cell.c);
	//printroute(route);
	//printroute(newpath);
	//printroute(endofbroken);
	
	/*connects the new path to the second half of the broken route*/
	while(node!=NULL){
		if((new = inlist(endofbroken, &node->cell)) != NULL){
			node->next = new->next;
			break;	
		}
		node = node->next;
	}		
	newlist->foot = endofbroken->foot;
	//printroute(newlist);	
	return newlist;
	
	
}
/*returns a list of pairs that the pair finding algorithm produces
which terminates when a route block has been found or all pairs
have been processed*/
list_t* findpair(cell_t **blocks, node_t *issue, int blockn,
	int rows, int cols, list_t *endofbroken){

	list_t *queue;
	queue = make_empty_list();
	queue = insert_at_foot(queue, issue->cell, DEFAULTCOUNTER);
	node_t *pair = queue->head, *node;
	cell_t *new;
	int directions[] = DIRECTIONS;
	int i, exists= FALSE, found = FALSE;
	while(pair != NULL){
		/*create a pair for every direction*/
		for (i = 0; i < NOOFDIRECTIONS; i++){
			new = createpair(blocks, pair, blockn, rows, cols, directions[i]);
			exists = FALSE;
			if(new){
				node = queue->head;
				/*checks if the newly formed pair already exists in queue*/
				while(node != NULL){
					if (samecell(&node->cell, new) == TRUE){
						exists = TRUE;
						break;
					}
					node = node->next;
				}
				
				if(exists == FALSE){
					//printf("%d,%d,  %d\n", new->r, new->c, pair->counter + 1);
					queue = insert_at_foot(queue, *new, pair->counter + 1);
					if(inlist(endofbroken, new)){
						found = TRUE;
						break;
					}
				}
			}
			free(new);
		}
		if (found){
			break;
		}
		pair = pair->next;
	}
	if(found==FALSE){
		return NULL;
	}
	return queue;
	
}
/*creates the path from the cell before the block to any cell on 
the original route*/
list_t* findpath(cell_t **blocks,list_t *queue, node_t *issue, int blockn,
	int rows, int cols){
	list_t* newpath;
	newpath = make_empty_list();
	
	node_t* solpair = queue->foot, *curnode;
	newpath = insert_at_head(newpath, solpair->cell, solpair->counter);
	int directions[] = DIRECTIONS, i;
	node_t *minadjnode;
	minadjnode = solpair;
	cell_t *new;
	
	/*chooses an adjacent cell with the lowest counter value at every point
	unitl it reaches the route*/
	while(!samecell(&issue->cell,&solpair->cell)){
		for (i = 0; i < NOOFDIRECTIONS; i++){
			new = createpair(blocks, solpair, blockn, rows, cols, directions[i]);  
			
			/*finds an adjacent cell with the lowest counter value*/
			if(new!= NULL && (curnode =inlist(queue, new))!=NULL){
				//printf("%d,%d", new->r, new->c);
				if(curnode->counter < minadjnode->counter){
					minadjnode = curnode;
				}
			}
		}
		newpath = insert_at_head(newpath, minadjnode->cell, minadjnode->counter);
		solpair = minadjnode;
	}
	return newpath;

}
/*Solve one blocked segment of a route and returns the route*/
list_t* solveonce(list_t* route,cell_t** blocks, node_t *issue, int blockn,int rows,int cols,
	int* nosol){
	int nosolution = *nosol;
	list_t* oldpath;
	list_t* oldroute;
	list_t* queue;
	queue = make_empty_list();
	list_t *endofbroken = make_empty_list();
	endofbroken = findendofbroken(route, blocks, issue, blockn);
	queue = findpair(blocks, issue, blockn,rows, cols, endofbroken);
	list_t* newpath;
	newpath = make_empty_list();
	list_t* newroute;
	newroute = make_empty_list();
	list_t *startofbroken = make_empty_list();
	startofbroken = findstartofbroken(route, blocks, issue, blockn);
	if(queue!=NULL){
		oldpath = newpath;
		oldroute = newroute;
		newpath = findpath(blocks, queue,issue, blockn,rows, cols);
		newroute = fixroute(route, newpath, issue, endofbroken, startofbroken);
		free_list(oldpath);
		free_list(oldroute);
	}
	else{
		nosolution = TRUE;
		*nosol = nosolution;
		return route;
	}	
	if (newroute!=NULL){
		*nosol = nosolution;
		return newroute;
	}
	else{
		nosolution = TRUE;
		*nosol = nosolution;
		return route;
	}
	
}
/*Solves all blocked segments of the route if possible*/
list_t* solveall(list_t* route,cell_t** blocks, node_t *issue, 
	cell_t initcell, cell_t goalcell, int blockn,int rows,int cols, int* nosol){
	list_t* newlist;
	int nosolution = *nosol;
	newlist = make_empty_list();
	newlist->head = route->head;
	newlist->foot = route->foot;
	
	int toprint = FALSE, fixattempted = TRUE;
	/*loops until the node reaches the goal*/
	while(!samecell(&issue->cell, &goalcell)){
		newlist = solveonce(newlist,blocks,issue, blockn,rows,cols,&nosolution);
		
		if(nosolution){
			/*runs if no solution to a segment*/
			break;
		}
		issue = checkstatus(newlist,blocks,initcell,goalcell,blockn,fixattempted,toprint);
	}
	*nosol = nosolution;
	return newlist;

}
/*handles stage two*/
void stagetwo(list_t* route, cell_t** blocks, node_t* issue, cell_t initcell,
	cell_t goalcell,int blockn,int rows,int cols, int *mblocks){
	int first = TRUE;
	int moreblocks = *mblocks;
	char** grid;
	if (moreblocks != EOF){
		printf(STAGE, 2);
		/*will keep looping if standard input contains more blocks*/
		while(moreblocks!=EOF){
			if(!first){
				printf(ENDING);
				printf("\n");
			}
			
			/*check if the route is valid in the updated grid*/
			first = FALSE;
			int blockn = 0;
			blocks = readblocks(&blockn, &moreblocks);
			grid = creategrid(route,blocks,initcell,goalcell,blockn,rows,cols);	
			printgrid(grid, rows, cols);
			int toprint = FALSE, nosol = FALSE, fixattempted = FALSE;
			issue = checkstatus(route,blocks,initcell,goalcell,blockn,fixattempted,toprint);
			
			/*runs there is a block in the route of the updated grid*/
			if(issue!=NULL&&!samecell(&issue->cell, &goalcell)){
				printf(DIVIDER);
				
				route = solveall(route,blocks,issue, initcell,goalcell,blockn,rows,cols,&nosol);
				grid = creategrid(route,blocks,initcell,goalcell,blockn,rows,cols);
				printgrid(grid,rows,cols);
				printf(DIVIDER);
				toprint = TRUE, fixattempted = TRUE;
				if(!nosol){
					printroute(route);
				}
				checkstatus(route,blocks,initcell,goalcell,blockn,fixattempted,toprint);
			}
		}
	}
}

/*Algorithms are fun!*/
