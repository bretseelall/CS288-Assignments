
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define N 4
#define NxN (N*N)
#define TRUE 1
#define FALSE 0

struct node {
	int tiles[N][N]; // 4x4
	int f, g, h;
    // heuristic function parts
    // f - total cost
    // g - c_node(test);urrent path cost (start -> current)
    // h - estimated shortest path cost (current -> goal)
	short zero_row, zero_column;	/* location (row and column) of the empty tile */
	struct node *next;              /* linked list part */
	struct node *parent;			/* used to trace back the solution */
};

int goal_rows[NxN];
int g1 = 0;
int goal_columns[NxN];
struct node *start,*goal;
struct node *open = NULL, *closed = NULL; // open - fringe
struct node *succ_nodes[4]; // 4 adjacent nodes (at most, so this should not include the current node)
// if there are less than 2 successors, some successors must be made NULL
// bc null pointers...

void print_a_node(struct node *pnode) {
	int i,j;
	for (i=0;i<N;i++) {
		for (j=0;j<N;j++) 
			printf("%2d ", pnode->tiles[i][j]);
		printf("\n");
	}
	printf("\n");
}

struct node *initialize(char **argv){ // starts the puzzle
	int i,j,k,index, tile; // think you can remove i's declaration here
	struct node *pnode;

	pnode=(struct node *) malloc(sizeof(struct node));
	index = 1;
	for (j=0;j<N;j++)
		for (k=0;k<N;k++) {
			tile=atoi(argv[index++]);
			pnode->tiles[j][k]=tile;
			if(tile==0) {
				pnode->zero_row=j;
				pnode->zero_column=k;
			}
		}
	pnode->f=0;
	pnode->g=0;
	pnode->h=0;
	pnode->next=NULL;
	pnode->parent=NULL;
	start=pnode;
	printf("initial state\n");
	print_a_node(start);

	pnode=(struct node *) malloc(sizeof(struct node));
	goal_rows[0]=3;
	goal_columns[0]=3;

	for(index=1; index<NxN; index++){
		j=(index-1)/N;
		k=(index-1)%N;
		goal_rows[index]=j;
		goal_columns[index]=k;
		pnode->tiles[j][k]=index;
	}
	pnode->tiles[N-1][N-1]=0;	      /* empty tile=0 */
	pnode->f=0;
	pnode->g=0;
	pnode->h=0;
	pnode->next=NULL;
	goal=pnode; 
	printf("goal state\n");
	print_a_node(goal);

	return start;
}

/* merge the remaining nodes pointed by succ_nodes[] into open list. 
 * Insert nodes based on their f values --- keep f values sorted. */
void merge_to_open() {

	struct node *head = NULL;
	struct node *stPtr;
	stPtr = (struct node *) malloc(sizeof(struct node));
	head = (struct node *) malloc(sizeof(struct node));
	for(int d = 0; d < N; d++)
	{
		if(succ_nodes[d] == NULL)
		{
			continue;
		}
		else if(open == NULL)
		{
			head = succ_nodes[d];
			head->next = NULL;
			open = head;
			
		}
		else if(succ_nodes[d]->f <= head->f)
		{
			struct node *tmp;
			tmp = head;
			head = succ_nodes[d];
			head->next = tmp;
			head->zero_row = succ_nodes[d]->zero_row;
                        head->zero_column = succ_nodes[d]->zero_column;
		}
		else if(succ_nodes[d]->f > head->f && head->next == NULL)
		{
			head->next = succ_nodes[d];
		}
		else
		{
			struct node *previous = head;
			stPtr = head->next;
			while(succ_nodes[d]->f > stPtr->f && stPtr != NULL)
			{
				previous = stPtr;
				stPtr = stPtr->next;
			}
			previous->next = succ_nodes[d];
			succ_nodes[d]->next = stPtr;
		}
	}
	open = head;
	open->next = NULL;
}

/*swap two tiles in a node*/
void swap(int row1,int column1,int row2,int column2, struct node * pnode){
	int temp = pnode->tiles[row1][column1];
	pnode->tiles[row1][column1] = pnode->tiles[row2][column2];
	pnode->tiles[row2][column2] = temp;
}

/*update the f,g,h function values for a node */
void update_fgh(int i) {
	struct node *pnode = succ_nodes[i];
	int val;
	if(pnode != NULL)
	{
		for(int x = 0; x<N; x++)
		{
			for(int y = 0; y<N; y++)
			{
				val = pnode->tiles[x][y];
				if(val == 0)
				{
					
					continue;
				}
				else if(val == 1 && (x != 0 || y != 0))
				{
					pnode->h += abs(0 - x) + abs(0-y);
				}
				else if(val == 2 && (x != 0 || y != 1))
       	                	{
       	        	                pnode->h += abs(0 - x) + abs(1-y);
       		                }
				else if(val == 3 && (x != 0 || y != 2))
                        	{
                                	pnode->h += abs(0 - x) + abs(2-y);
                        	}
				else if(val == 4 && (x != 0 || y != 3))
                        	{
                                	pnode->h += abs(0 - x) + abs(3-y);
                        	}
				else if(val == 5 && (x != 1 || y != 0))
                      		{
                                	pnode->h += abs(1 - x) + abs(0-y);
                        	}
				else if(val == 6 && (x != 1 || y != 1))
                        	{
                                	pnode->h += abs(1 - x) + abs(1-y);
                        	}
				else if(val == 7 && (x != 1 || y != 2))
                	        {
                       	 	        pnode->h += abs(1 - x) + abs(2-y);
                        	}
				else if(val == 8 && (x != 1 || y != 3))
	                        {
	                                pnode->h += abs(1 - x) + abs(3-y);
	                        }
				else if(val == 9 && (x != 2 || y != 0))
	                        {
	                                pnode->h += abs(2 - x) + abs(0-y);
	                        }
				else if(val == 10 && (x != 2 || y != 1))
	                        {
	                                pnode->h += abs(2 - x) + abs(1-y);
	                        }
				else if(val == 11 && (x != 2 || y != 2))
	                        {
	                                pnode->h += abs(2 - x) + abs(2-y);
	                        }
				else if(val == 12 && (x != 2 || y != 3))
	                        {
	                                pnode->h += abs(2 - x) + abs(3-y);
	                        }
				else if(val == 13 && (x != 3 || y != 0))
	                        {
	                                pnode->h += abs(3 - x) + abs(0-y);
	                        }
				else if(val == 14 && (x != 3 || y != 1))
        	                {
	                                pnode->h += abs(3 - x) + abs(1-y);
	                        }
				else if(val == 15 && (x != 3 || y != 2))
	                        {
	                                pnode->h += abs(3 - x) + abs(2-y);
	                        }
				else
				{
					continue;
				}

			}
	
		}
	pnode->g = g1;
	pnode->f = pnode->g + pnode->h;
	}
}

/* 0 goes down by a row */
void move_down(struct node * pnode){
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row++, pnode->zero_column, pnode);
}

/* 0 goes right by a column */
void move_right(struct node * pnode){
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column++, pnode);
}

/* 0 goes up by a row */
void move_up(struct node * pnode){
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row--, pnode->zero_column, pnode);
}

/* 0 goes left by a column */
void move_left(struct node * pnode){
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column--, pnode);
}

/* expand a node, get its children nodes, and organize the children nodes using
 * array succ_nodes.
 */
void expand(struct node *selected) {
	int row = selected->zero_row;
	int column = selected->zero_column;
	succ_nodes[0] = NULL;
	succ_nodes[1] = NULL;
	succ_nodes[2] = NULL;
	succ_nodes[3] = NULL;
	if(row != 0)
	{
		struct node *up;
		up = (struct node *) malloc(sizeof(struct node));
		for(int z = 0; z<N; z++)
        	{
                	for(int x = 0; x<N; x++)
                	{
                        	up->tiles[z][x] = selected->tiles[z][x];
                	}
        	}
		up->f = 0;
        	up->g = 0;
        	up->h = 0;
		up->zero_row = row;
		up->zero_column = column;
		up->parent = selected;
		move_up(up);
		succ_nodes[0] = up;
	}
	if(row != 3)
	{
		struct node *down;
                down = (struct node *) malloc(sizeof(struct node));
                for(int z = 0; z<N; z++)
                {
                        for(int x = 0; x<N; x++)
                        {
                                down->tiles[z][x] = selected->tiles[z][x];
                        }
                }
                down->f = 0;
                down->g = 0;
                down->h = 0;
		down->parent = selected;
		down->zero_row = row;
                down->zero_column = column;
                move_down(down);
		succ_nodes[1] = down;
	}
	if(column != 0)
	{
		struct node *left;
                left = (struct node *) malloc(sizeof(struct node));
                for(int z = 0; z<N; z++)
                {
                        for(int x = 0; x<N; x++)
                        {
                                left->tiles[z][x] = selected->tiles[z][x];
                        }
                }
                left->f = 0;
                left->g = 0;
                left->h = 0;
		left->parent = selected;
		left->zero_row = row;
                left->zero_column = column;
                move_left(left);
		succ_nodes[2] = left;
	}
	if(column != 3)
	{
		struct node *right;
                right = (struct node *) malloc(sizeof(struct node));
                for(int z = 0; z<N; z++)
                {
                        for(int x = 0; x<N; x++)
                        {
                                right->tiles[z][x] = selected->tiles[z][x];
                        }
                }
                right->f = 0;
                right->g = 0;
                right->h = 0;
		right->parent = selected;
		right->zero_row = row;
                right->zero_column = column;
                move_right(right);
		succ_nodes[3] = right;
	}
}

int nodes_same(struct node *a,struct node *b) {
	int flg=FALSE;
	if (memcmp(a->tiles, b->tiles, sizeof(int)*NxN) == 0)
		flg=TRUE;
	return flg;
}

/* Filtering. Some states in the nodes pointed by succ_nodes may already be
included in
 * either open or closed list. There is no need to check these states. Release the
 * corresponding nodes and set the corresponding pointers in succ_nodes to NULL.
This is
 * important to reduce execution time.
 * This function checks the (i)th node pointed by succ_nodes array. You must call
this
 * function in a loop to check all the nodes in succ_nodes. Free the successor node
and
 * set the corresponding pointer to NULL in succ_node[] if a matching node is found
in
 * the list.
 */

void filter(int i, struct node *pnode_list){ 
	struct node *pnode = succ_nodes[i];
	if(pnode_list != NULL && pnode != NULL)
	{
		while(pnode_list != NULL)
		{
			if(nodes_same(pnode_list, pnode))
			{
				succ_nodes[i] = NULL;
				break;
			}
			pnode_list = pnode_list->next;
		}
	}
}

int main(int argc,char **argv) {
	int iter,cnt;
	struct node *copen, *cp, *solution_path;
    // copen - node to expand
	int ret, i, pathlen=0, index[N-1];
	
	solution_path=NULL;
	start=initialize(argv);	/* init initial and goal states */
	open=start; 

	iter=0;
	while (open!=NULL) {	/* Termination cond with a solution is tested in expand. */
        // above: step 4 from pseudo-code slides
		copen=open;
		open=open->next;  /* get the first node from open to expand */
        // above removes first node
		if(nodes_same(copen,goal)){ /* goal is found */
			do{ /* trace back and add the nodes on the path to a list */
				copen->next=solution_path;
				solution_path=copen;
				copen=copen->parent;
				pathlen++;
			} while(copen!=NULL);
			printf("Path (lengh=%d):\n", pathlen); 
			copen=solution_path;
			while(copen != NULL)
			{
				print_a_node(copen);
				copen = copen->next;
			}
			 /* print out the nodes on the list */
			break;
		}
		expand(copen);       /* Find new successors */
		g1++;
		for(i=0;i<4;i++){
			filter(i,open);
			filter(i,closed);
			update_fgh(i); // estimate new f, g, h values for the node
		}
		merge_to_open();       /* New open list */
		copen->next=closed;
		closed=copen;		/* New closed */
		/* print out something so that you know your 
		 * program is still making progress 
		 */
		iter++;
		if(iter %1000 == 0)
			printf("iter %d\n", iter);
	}
	return 0;
} /* end of main */
