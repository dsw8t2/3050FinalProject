#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct Path {
    int r;
    int c;
    struct Path *next;
} path;

int distance(int,int,int,int);
//path* move(char**,path*,path*,path*,int);
path* move(char**,path*,path*,int,int,int,int);
void print_map(char**,int);
//path* move_inc_dist(char**,path*,path*,int,int);
path* move_inc_dist(char**,path*,path*,int,int,int,int);
path* deadend_filler(char**,path*);
void clear(char**,int);
int activate(path**,char**,path*);
int adjacent(int,int,int,int);
void rewind_path(char**,path*);

path* move_left(char**,path*,char);
path* move_right(char**,path*,char);
path* move_up(char**,path*,char);
path* move_down(char**,path*,char);
path* move_NW(char**,path*,char);
path* move_SW(char**,path*,char);
path* move_NE(char**,path*,char);
path* move_SE(char**,path*,char);

int main (int argc, char** argv)
{

    int i, j, counter=0;
    FILE *fp;
    char** room;
  
    path** list = malloc(sizeof(path*)*2);
    path* head_S = malloc(sizeof(path));
    path* head_F = malloc(sizeof(path));
    path* dest_E = malloc(sizeof(path));
    path* dest_L = malloc(sizeof(path));

    *(list+0) = head_S;
    *(list+1) = head_F;

    room = malloc(sizeof(char *));
    *(room+counter) = (char *)malloc(99);

    if ((fp = fopen(*(argv+1),"r")) == NULL)
    {
        printf("Invalid file.\n");
        return 0;
    }
    else
    {
        while(!feof(fp))
        {
            if (fgets(*(room+counter), 99, fp) == NULL)
                break;
            else
            {
                counter++;
                room = realloc(room,sizeof(char *)*(counter+1));
                *(room+counter) = (char *)malloc(99);
            }
        }
        fclose(fp);
    }

    for (i=0; i<counter; i++)
        printf("%s",*(room+i));
    printf("\n");

    for (i=0; i<counter; i++)
        for (j=0; j<strlen(*(room+i)); j++)
        {
            if ( *(*(room+i)+j) == 'F')
            {
                head_F->r = i;
                head_F->c = j;
                head_F->next = NULL;
            }
            if ( *(*(room+i)+j) == 'L')
            {
                dest_L->r = i;
                dest_L->c = j;
            }
            if ( *(*(room+i)+j) == 'S')
            {
                head_S->r = i;
                head_S->c = j;
                head_S->next = NULL;
            }
            if ( *(*(room+i)+j) == 'E')
            {
                dest_E->r = i;
                dest_E->c = j;
            }

        }


    if (distance(dest_E->r, dest_E->c, dest_L->r, dest_L->c) == 1 || distance(head_S->r, head_S->c, head_F->r, head_F->c) == 1)
    {
        printf("Not possible\n");
        return 0;
    }
    


//    *(list+1) = head_F = move(room, *(list+0), *(list+1), dest_L->r, dest_L->c, distance(head_F->r, head_F->c, dest_L->r, dest_L->c),0);

    *(list+0) = head_S = move(room, *(list+0), *(list+1), dest_E->r, dest_E->c, distance(head_S->r, head_S->c, dest_E->r, dest_E->c),1);

//    *(list+0) = head_S = move(room, *(list+0), *(list+1), dest_E, 1);
//    *(list+1) = head_F = move(room, *(list+0), *(list+1), dest_L, 0);
    clear(room,counter);
    print_map(room, counter);

    free(*(list+0));
    free(*(list+1));
    free(list);
    return 0;
}

int distance (int row_current, int column_current, int row_dest, int column_dest)
{
    return abs(row_current-row_dest)+abs(column_current-column_dest);
}

int adjacent (int Srow, int Scol, int Frow, int Fcol)
{
    if (distance(Srow,Scol,Frow,Fcol) == 1) // side by side
        return 1;
    else if (abs(head->r - dest->r) == 1 && abs(head->c - dest->c) == 1) // diagonal
        return 1;
    else
        return 0;
}

void rewind_path (char** room, path* head)
{
    *(*(room+head->r)+head->c) = ' ';
    head->r = 0;
    head->c = 0;
}



//OLD ALGORITHM, works.

path* move (char** room, path* head_S, path* head_F, int row_dest, int column_dest, int dist, int code)
{
    path* head;
    char c;
    if (code == 1)
    {
        head = head_S;
        c = '1';
    }
    else
    {
        head = head_F;
        c = '0';
    }
//    int i;
//    for (i=0; i<60; i++)
    while (dist != 1)
    {


        if ( (column_dest < head->c || row_dest < head->r) && (*(*(room+head->r)+head->c-1) == ' ' || *(*(room+head->r-1)+head->c) == ' '))
        { // comment out these diagonals.

        // left = c-1, up = r-1
         if ( row_dest < head->r && *(*(room+head->r-1)+head->c) == ' ' && distance(head->r-1, head->c, row_dest, column_dest) < dist) // up
            {
                head = move_up(room,head,c);
    
                dist--;
            }
            else if ( column_dest < head->c && *(*(room+head->r)+head->c-1) == ' ' && distance(head->r, head->c-1, row_dest, column_dest) < dist) // left
            {
                head = move_left(room,head,c);

                dist--;
            }           
            else if ( row_dest > head->r && *(*(room+head->r)+head->c+1) == ' ' && distance(head->r, head->c+1, row_dest, column_dest) < dist) // down
            {
                head = move_down(room,head,c);

                dist--;
            }
            else
            {
                if (code)
                    head_S = head;
                else
                    head_F = head;
                head = move_inc_dist (room, head_S, head_F, row_dest, column_dest, dist,code);
                dist = distance(head->r, head->c, row_dest, column_dest);
            }
        }
        else if ((column_dest > head->c || row_dest > head->r) && (*(*(room+head->r+1)+head->c) == ' ' || *(*(room+head->r)+head->c+1) == ' '))
        { 
          if ( column_dest < head->c && *(*(room+head->r-1)+head->c) == ' ' && distance(head->r-1, head->c, row_dest, column_dest) < dist) // up
            {
                head = move_up(room,head,c);
    
                dist--;
            }
            else if ( row_dest < head->r && *(*(room+head->r)+head->c+1) == ' ' && distance(head->r, head->c+1, row_dest, column_dest) < dist) // right
            {
                head = move_right(room,head,c);

                dist--;
            }
            else if ( row_dest > head->r && *(*(room+head->r+1)+head->c) == ' ' && distance(head->r+1, head->c, row_dest, column_dest) < dist) // down
            {
                head = move_down(room,head,c);

                dist--;
            }
            else
            {
                if (code)
                    head_S = head;
                else
                    head_F = head;
                head = move_inc_dist (room, head_S, head_F, row_dest, column_dest, dist,code);
                dist = distance(head->r, head->c, row_dest, column_dest);
            }
        }
        else
        {
            if (code)
                head_S = head;
            else
                head_F = head;
            head = move_inc_dist (room, head_S, head_F, row_dest, column_dest, dist, code);
            dist = distance (head->r, head->c, row_dest, column_dest);
        }

        if (abs(column_dest-head->c) == 1 && abs(row_dest-head->r) == 1)
            return head;
    }
    return head;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///  New algorithm, broken
/*

path* move (char** room, path* head_S, path* head_F, path* dest, int code)
{
    char c;
    path* temp;
    path* head;
    path* head2;

    if (code)
    {
        head = head_S;
        head2 = head_F;
        c = '1';
    }
    else
    {
        head = head_F;
        head2 = head_S;
        c = '0';
    }

    
    

    int i;
    for (i=0; i<20; i++)
    {
        if (adjacent(head->r, head->c, head2->r, head2->c))
        {
            rewind_path(room,head);
            return head;
        }


        if ( dest->r < head->r && dest->c < head->c) // Quadrant 1, top left
        {
            if (*(*(room+head->r-1)+head->c) == ' ' || *(*(room+head->r)+head->c-1) == ' ' || abs((int)*(*(room+head->r-1)+head->c)-code) || abs((int)*(*(room+head->r)+head->c-1)-code))
            {
                if (*(*(room+head->r-1)+head->c-1) == ' ' || abs((int)*(*(room+head->r-1)+head->c-1)-code)) // NW. -2 dist.
                {
                    head = move_NW(room,head,c);                  
                }
                else if (*(*(room+head->r-1)+head->c) == ' ' || abs((int)*(*(room+head->r-1)+head->c)-code)) // Up, -1 dist.
                {
                    head = move_up(room,head,c);
                }
                else if (*(*(room+head->r)+head->c-1) == ' ' || abs((int)*(*(room+head->r)+head->c-1)-code)) // Left, -1 dist.
                {
                    head = move_left(room,head,c);
                }
            }
            else
            {
                head = move_inc_dist(room,head,dest,code,1);
            }    
        }
        else if (dest->r > head->r && dest->c < head->c) // Quadrant 2, bottom left
        {
            if (*(*(room+head->r+1)+head->c) == ' ' || *(*(room+head->r)+head->c-1) == ' ' || abs((int)*(*(room+head->r+1)+head->c)-code) || abs((int)*(*(room+head->r)+head->c-1)-code))
            {            
                if (*(*(room+head->r+1)+head->c-1) == ' ' || abs((int)*(*(room+head->r+1)+head->c-1)-code)) // SW. -2 dist.
                {
                    head = move_SW(room,head,c);                   
                }
                else if (*(*(room+head->r+1)+head->c) == ' ' || abs((int)*(*(room+head->r+1)+head->c)-code)) // Down, -1 dist.
                {
                    head = move_down(room,head,c);
                }
                else if (*(*(room+head->r)+head->c-1) == ' ' || abs((int)*(*(room+head->r)+head->c-1)-code)) // Left, -1 dist.
                {
                    head = move_left(room,head,c);
                }
            }
            else
            {
                head = move_inc_dist(room,head,dest,code,2);
            }            
        }
        else if (dest->r < head->r && dest->c > head->c) // Qaudrant 3, top right
        {
            if (*(*(room+head->r-1)+head->c) == ' ' || *(*(room+head->r)+head->c+1) == ' ' || abs((int)*(*(room+head->r-1)+head->c)-code) || abs((int)*(*(room+head->r)+head->c+1)-code))
            {
                if (*(*(room+head->r-1)+head->c+1) == ' ' || abs((int)*(*(room+head->r-1)+head->c+1)-code)) // NE. -2 dist.
                {
                    head = move_NE(room,head,c);                  
                }
                else if (*(*(room+head->r-1)+head->c) == ' ' || abs((int)*(*(room+head->r-1)+head->c)-code)) // Up, -1 dist.
                {
                    head = move_up(room,head,c);
                }
                else if (*(*(room+head->r)+head->c+1) == ' ' || abs((int)*(*(room+head->r)+head->c+1)-code)) // Right, -1 dist.
                {
                    head = move_right(room,head,c);
                }
            else
            {
                head = move_inc_dist(room,head,dest,code,3);
            }
        }
        else if (dest->r > head->r && dest->c > head->c)  // Quadrant 4, bottom right
        {
            if (*(*(room+head->r+1)+head->c) == ' ' || *(*(room+head->r)+head->c+1) == ' ' || abs((int)*(*(room+head->r+1)+head->c)-code) || abs((int)*(*(room+head->r)+head->c+1)-code))
            {
                if (*(*(room+head->r+1)+head->c+1) == ' ' || abs((int)*(*(room+head->r+1)+head->c+1)-code)) // SE. -2 dist.
                {
                    head = move_SE(room,head,c);                   
                }
                else if (*(*(room+head->r+1)+head->c) == ' ' || abs((int)*(*(room+head->r+1)+head->c)-code)) // Down, -1 dist.
                {
                    head = move_down(room,head,c);
                }
                else if (*(*(room+head->r)+head->c+1) == ' ' || abs((int)*(*(room+head->r)+head->c+1)-code)) // Right, -1 dist.
                {
                    head = move_right(room,head,c);
                }
            }
            else
            {
                head = move_inc_dist(room,head,dest,code,4);
            }
        }
        else if (dest->c == head->c)
        {
            if (dest->r < head->r)
            {
                if (*(*(room+head->r-1)+head->c) == ' ' || abs((int)*(*(room+head->r-1)+head->c)-code))
                {
                    head = move_up(room,head,c);
                }
                else
                    head = move_inc_dist(room,head,dest,code,5);
            }
            else
            {
                if (*(*(room+head->r+1)+head->c) == ' ' || abs((int)*(*(room+head->r+1)+head->c)-code))
                {
                    head = move_down(room,head,c);
                }
                else
                    head = move_inc_dist(room,head,dest,code,5);
            }
        }
        else if (dest->r == head->r)
        {
            if (dest->c < head->c)
            {
                if (*(*(room+head->r)+head->c-1) == ' ' || abs((int)*(*(room+head->r)+head->c-1)-code))
                {
                    head = move_left(room,head,c);
                }
                else
                    head = move_inc_dist(room,head,dest,code,5);
            }
            else
            {
                if (*(*(room+head->r)+head->c+1) == ' ' || abs((int)*(*(room+head->r)+head->c+1)-code))
                {
                    head = move_right(room,head,c);
                }
                else
                    head = move_inc_dist(room,head,dest,code,5);
            }
        }

        if ((abs(dest->r - head->r)+abs(dest->c - head->c)) == 1)
            return head;
    }
    return head;
}




path* move_inc_dist(char** room, path* head, path* dest, int code, int quadrant)
{
    char c;
    path* temp;

    if (code)
        c = '1';
    else
        c = '0';

    switch(quadrant)
    {
        case 1: 
        {
            if (*(*(room+head->r+1)+head->c) == ' ' || *(*(room+head->r)+head->c+1) == ' ' || abs((int)*(*(room+head->r+1)+head->c)-code) == 1 || abs((int)*(*(room+head->r)+head->c+1)-code) == 1)
            {
                if (*(*(room+head->r+1)+head->c-1) == ' ' || abs((int)*(*(room+head->r+1)+head->c-1)-code)) // SW, no change in dist.
                {
                    head = move_SW(room,head,c);
                }
                else if (*(*(room+head->r-1)+head->c+1) == ' ' || abs((int)*(*(room+head->r-1)+head->c+1)-code)) // NE, no change in dist.
                {
                    head = move_NE(room,head,c);
                }                
                else if (*(*(room+head->r)+head->c+1) == ' ' || abs((int)*(*(room+head->r)+head->c+1)-code)) // Right, +1 dist.
                {
                    head = move_right(room,head,c);
                }
                else if (*(*(room+head->r+1)+head->c) == ' ' || abs((int)*(*(room+head->r+1)+head->c)-code)) // Down, +1 dist.
                {
                    head = move_down(room,head,c);
                }
                else if (*(*(room+head->r+1)+head->c+1) == ' ' || abs((int)*(*(room+head->r+1)+head->c+1)-code)) // SE, +2 dist.
                {
                    head = move_SE(room,head,c);
                }
            }
            else
            {
                head = deadend_filler(room,head);
            }
            return head;
        }
        case 2:
        {
            if (*(*(room+head->r-1)+head->c) == ' ' || *(*(room+head->r)+head->c+1) == ' ' || abs((int)*(*(room+head->r-1)+head->c)-code) == 1 || abs((int)*(*(room+head->r)+head->c+1)-code) == 1)
            {
                if (*(*(room+head->r-1)+head->c-1) == ' ' || abs((int)*(*(room+head->r-1)+head->c-1)-code)) // NW, no change in dist.
                {
                    head = move_NW(room,head,c);
                }
                else if (*(*(room+head->r+1)+head->c+1) == ' ' || abs((int)*(*(room+head->r+1)+head->c+1)-code)) // SE, no change in dist.
                {
                    head = move_SE(room,head,c);
                }                
                else if (*(*(room+head->r)+head->c+1) == ' ' || abs((int)*(*(room+head->r)+head->c+1)-code)) // Right, +1 dist.
                {
                    head = move_right(room,head,c);
                }
                else if (*(*(room+head->r-1)+head->c) == ' ' || abs((int)*(*(room+head->r-1)+head->c)-code)) // Up, +1 dist.
                {
                    head = move_up(room,head,c);
                }
                else if (*(*(room+head->r-1)+head->c+1) == ' ' || abs((int)*(*(room+head->r-1)+head->c+1)-code)) // NE, +2 dist.
                {
                    head = move_NE(room,head,c);
                }
            }
            else
            {
                head = deadend_filler(room,head);
            }
            return head;
        }
        case 3:
        {
            if (*(*(room+head->r+1)+head->c) == ' ' || *(*(room+head->r)+head->c-1) == ' ' || abs((int)*(*(room+head->r+1)+head->c)-code) == 1 || abs((int)*(*(room+head->r)+head->c-1)-code) == 1)
            {
                if (*(*(room+head->r-1)+head->c-1) == ' ' || abs((int)*(*(room+head->r-1)+head->c-1)-code)) // NW, no change in dist.
                {
                    head = move_NW(room,head,c);
                }
                else if (*(*(room+head->r+1)+head->c+1) == ' ' || abs((int)*(*(room+head->r+1)+head->c+1)-code)) // SE, no change in dist.
                {
                    head = move_SE(room,head,c);
                }                
                else if (*(*(room+head->r)+head->c-1) == ' ' || abs((int)*(*(room+head->r)+head->c-1)-code)) // Left, +1 dist.
                {
                    head = move_left(room,head,c);
                }
                else if (*(*(room+head->r+1)+head->c) == ' ' || abs((int)*(*(room+head->r+1)+head->c)-code)) // Down, +1 dist.
                {
                    head = move_down(room,head,c);
                }
                else if (*(*(room+head->r+1)+head->c-1) == ' ' || abs((int)*(*(room+head->r+1)+head->c-1)-code)) // SW, +2 dist.
                {
                    head = move_SW(room,head,c);
                }
            }
            else
            {
                head = deadend_filler(room,head);
            }
            return head;
        }
        case 4: 
        {
            if (*(*(room+head->r-1)+head->c) == ' ' || *(*(room+head->r)+head->c-1) == ' ' || abs((int)*(*(room+head->r-1)+head->c)-code) == 1 || abs((int)*(*(room+head->r)+head->c-1)-code) == 1)
            {
                if (*(*(room+head->r+1)+head->c-1) == ' ' || abs((int)*(*(room+head->r+1)+head->c-1)-code)) // SW, no change in dist.
                {
                    head = move_SW(room,head,c);
                }
                else if (*(*(room+head->r-1)+head->c+1) == ' ' || abs((int)*(*(room+head->r-1)+head->c+1)-code)) // NE, no change in dist.
                {
                    head = move_NE(room,head,c);
                }                
                else if (*(*(room+head->r)+head->c-1) == ' ' || abs((int)*(*(room+head->r)+head->c-1)-code)) // Left, +1 dist.
                {
                    head = move_left(room,head,c);
                }
                else if (*(*(room+head->r-1)+head->c) == ' ' || abs((int)*(*(room+head->r-1)+head->c)-code)) // Up, +1 dist.
                {
                    head = move_up(room,head,c);
                }
                else if (*(*(room+head->r-1)+head->c-1) == ' ' || abs((int)*(*(room+head->r-1)+head->c-1)-code)) // NW, +2 dist.
                {
                    head = move_NW(room,head,c);
                }
            }
            else
            {
                head = deadend_filler(room,head);
            }
            return head;
        }
        default:
        {
            if (dest->c == head->c)
            {
                if (*(*(room+head->r)+head->c-1) == ' ' || abs((int)*(*(room+head->r)+head->c-1)-code)) // left, +1 to dist.
                {
                    head = move_left(room,head,c);
                }
                else if (*(*(room+head->r)+head->c+1) == ' ' || abs((int)*(*(room+head->r)+head->c+1)-code)) // right, +1 to dist.
                {
                    head = move_right(room,head,c);
                }
                else
                {
                    head = deadend_filler(room,head);
                }
            }
            else if (dest->r == head->r)
            {
                if (*(*(room+head->r-1)+head->c) == ' ' || abs((int)*(*(room+head->r-1)+head->c)-code)) // up, +1 to dist.
                {
                    head = move_up(room,head,c);
                }
                else if (*(*(room+head->r+1)+head->c) == ' ' || abs((int)*(*(room+head->r+1)+head->c)-code)) // down, +1 to dist.
                {
                    head = move_down(room,head,c);
                }
                else
                {
                    head = deadend_filler(room,head);
                }
            }
            else
            {
                head = deadend_filler(room,head);
            }
            return head;
        }
    }
    }
    return head;
}

*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// old algorithm, works.
//


path* move_inc_dist (char** room, path* head_S, path* head_F, int row_dest, int column_dest, int dist, int code)
{
    path* head;
    char c;

    if (code)
    {
        head = head_S;
        c = '1';
    }
    else
    {
        head = head_F;
        c = '0';
    }

    while (1)
    {
        if ( *(*(room+head->r-1)+head->c) == ' ') // up
        {
            head = move_up(room,head,c);

            if (distance(head->r, head->c, row_dest, column_dest) < dist)
                return head;
            dist++;
        }
        else if ( *(*(room+head->r)+head->c-1) == ' ') // left
        {
            head = move_left(room,head,c);

            if (distance(head->r, head->c, row_dest, column_dest) < dist)
                return head;
            dist++;
        }
        else if ( *(*(room+head->r+1)+head->c) == ' ') // down
        {
            head = move_down(room,head,c);

            if (distance(head->r, head->c, row_dest, column_dest) < dist)
                return head;
            dist++;
        }
        else if ( *(*(room+head->r)+head->c+1) == ' ') // right
        {
            head = move_right(room,head,c);

            if (distance(head->r, head->c, row_dest, column_dest) < dist)
                return head;
            dist++;
        }
        else
            head = deadend_filler (room,head);
    }

    return head;
}

path* deadend_filler (char** room, path* head)
{
    path* temp = malloc(sizeof(path));
    if ( *(*(room+head->r)+head->c) == '1' || *(*(room+head->r)+head->c) == '0')
    {
        *(*(room+head->r)+head->c) = 'x';
        temp = head;
        head = head->next;
        free(temp);
    }
    return head;
}

void print_map (char** room, int counter)
{
    int i;
    for (i=0; i<counter; i++)
        printf("%s", *(room+i));
}

void clear (char** room, int counter)
{
    int row, col;
    for (row=0; row<counter; row++)
        for (col=0; col<strlen(*(room+row)); col++)
            if (*(*(room+row)+col) == 'x')
                *(*(room+row)+col) = ' ';
}


path* move_left (char** room, path* head, char c)
{
    path* temp;

    *(*(room+head->r)+head->c-1) = c;
    temp = malloc(sizeof(path));
    temp->r = head->r;
    temp->c = head->c-1;
    temp->next = head;
    head = temp;

    return head;
}

path* move_right (char** room, path* head, char c)
{
    path* temp;

    *(*(room+head->r)+head->c+1) = c;
    temp = malloc(sizeof(path));
    temp->r = head->r;
    temp->c = head->c+1;
    temp->next = head;
    head = temp;

    return head;
}

path* move_up (char** room, path* head, char c)
{
    path* temp;

    *(*(room+head->r-1)+head->c) = c;
    temp = malloc(sizeof(path));
    temp->r = head->r-1;
    temp->c = head->c;
    temp->next = head;
    head = temp;

    return head;
}

path* move_down (char** room, path* head, char c)
{
    path* temp;

    *(*(room+head->r+1)+head->c) = c;
    temp = malloc(sizeof(path));
    temp->r = head->r+1;
    temp->c = head->c;
    temp->next = head;
    head = temp;

    return head;
}

path* move_NW (char** room, path* head, char c)
{
    path* temp;

    *(*(room+head->r-1)+head->c-1) = c;
    temp = malloc(sizeof(path));
    temp->r = head->r-1;
    temp->c = head->c-1;
    temp->next = head;
    head = temp;

    return head;
}

path* move_SW (char** room, path* head, char c)
{
    path* temp;

    *(*(room+head->r+1)+head->c-1) = c;
    temp = malloc(sizeof(path));
    temp->r = head->r+1;
    temp->c = head->c-1;
    temp->next = head;
    head = temp;

    return head;
}

path* move_NE (char** room, path* head, char c)
{
    path* temp;

    *(*(room+head->r-1)+head->c+1) = c;
    temp = malloc(sizeof(path));
    temp->r = head->r-1;
    temp->c = head->c+1;
    temp->next = head;
    head = temp;

    return head;
}

path* move_SE (char** room, path* head, char c)
{
    path* temp;

    *(*(room+head->r+1)+head->c+1) = c;
    temp = malloc(sizeof(path));
    temp->r = head->r+1;
    temp->c = head->c+1;
    temp->next = head;
    head = temp;

    return head;
}




/*
int activate (path** list, char** room, path* dest_E, path* dest_L)
{
//    char* address_S = &*(list+0);
//    char* address_F = &*(list+1);

    path* head_S = *(list+0);
    path* head_F = *(list+1);
    
    int S=0, F=0;

    while (S != 1)
    {
        if (S == 0)
        {
            *(list+0) = move(room, head_S, *(list+1), dest->r, dest->c, distance(head_S->r, head_S->c, dest->r, dest->c), 0);
            if (distance(head_S->r, head_S->c, dest->r, dest->c) == 1 || (abs(head_S->r - dest_E->r)*abs(head_S->c - dest_E->c)) == 1)
                S = 1;
        }
        if (F == 0)

    }
    return 1;
}
*/

