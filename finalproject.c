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
int adjacent(int,int,path*);

path* move(char**,path*,path*,int,int,int,int);
path* move_inc_dist(char**,path*,path*,int,int,int,int,path*);
path* deadend_filler(char**,path*);

void freeStringArray(char**,int);
void freelist(path**);

void clear(char**,int);
void clear_path(char**,int);
void print_map(char**,int);

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
    

    printf("S Robot Path: \n\n");
    *(list+0) = head_S = move(room, *(list+0), *(list+1), dest_E->r, dest_E->c, distance(head_S->r, head_S->c, dest_E->r, dest_E->c),1);
    if (*(list+0) == NULL)
        printf("Path failed.\n");
    else
    {
        clear(room,counter);
        print_map(room,counter);
        clear_path(room,counter);
    }

    printf("\nF Robot Path: \n\n");
    *(list+1) = head_F = move(room, *(list+0), *(list+1), dest_L->r, dest_L->c, distance(head_F->r, head_F->c, dest_L->r, dest_L->c),0);
    if (*(list+1) == NULL)
        printf("Path failed.\n");
    else
    {
        clear(room,counter);
        print_map(room,counter);
        clear_path(room,counter);
    }

    freeStringArray(room,counter);
    freelist(list);
    free(dest_E);
    free(dest_L);

    return 0;
}

int distance (int row_current, int column_current, int row_dest, int column_dest)
{
    return abs(row_current-row_dest)+abs(column_current-column_dest);
}

int adjacent (int row, int col, path* head2)
{
    if (distance(row,col,head2->r,head2->c) == 1)
        return 1;
    else if (abs(row - head2->r) == 1 && abs(col - head2->c) == 1)
        return 1;
    else
        return 0;
}

void freeStringArray (char** string, int counter)
{
    int i;
    for (i=0; i<counter; i++)
        free(*(string+i));
}

void freelist (path** list)
{
    int i;
    path* head;
    path* temp;

    for (i=0; i<2; i++)
    {
        head = *(list+i);
        while (head != NULL)
        {
            temp = head;
            head = head->next;
            free(temp);
        }
    }
    free(list);
}



//OLD ALGORITHM, works.

path* move (char** room, path* head_S, path* head_F, int row_dest, int column_dest, int dist, int code)
{
    path* head;
    path* head2;
    path* start;

    char c;
    if (code == 1)
    {
        head = start = head_S;
        head2 = head_F;
        c = '1';
    }
    else
    {
        head = start = head_F;
        head2 = head_S;
        c = '0';
    }
    while (dist != 1)
    {


        if ((column_dest < head->c || row_dest < head->r) && (*(*(room+head->r)+head->c-1) == ' ' || *(*(room+head->r-1)+head->c) == ' '))
        { 

            if (( row_dest < head->r && distance(head->r-1, head->c, row_dest, column_dest) < dist) && *(*(room+head->r-1)+head->c) == ' ' && adjacent(head->r-1,head->c, head2) != 1) // up
            {
                head = move_up(room,head,c);
    
                dist--;
            }
            else if ((column_dest < head->c && distance(head->r, head->c-1, row_dest, column_dest) < dist) && *(*(room+head->r)+head->c-1) == ' ' && adjacent(head->r, head->c-1,head2)!= 1) // left
            {
                head = move_left(room,head,c);

                dist--;
            }           
            else if ( row_dest > head->r && *(*(room+head->r)+head->c+1) == ' ' && distance(head->r, head->c+1, row_dest, column_dest) < dist && adjacent(head->r,head->c+1,head2) != 1) // down
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
                head = move_inc_dist (room, head_S, head_F, row_dest, column_dest, dist,code,start);
                if (head == NULL)
                    return NULL;
                dist = distance(head->r, head->c, row_dest, column_dest);
            }
        }
        else if ((column_dest > head->c || row_dest > head->r) && (*(*(room+head->r+1)+head->c) == ' ' || *(*(room+head->r)+head->c+1) == ' '))
        { 
          if ( column_dest < head->c && *(*(room+head->r-1)+head->c) == ' ' && distance(head->r-1, head->c, row_dest, column_dest) < dist && adjacent(head->r-1,head->c,head2)!=1) // up
            {
                head = move_up(room,head,c);
    
                dist--;
            }
            else if ( row_dest < head->r && *(*(room+head->r)+head->c+1) == ' ' && distance(head->r, head->c+1, row_dest, column_dest) < dist && adjacent(head->r,head->c+1,head2) != 1) // right
            {
                head = move_right(room,head,c);

                dist--;
            }
            else if ( row_dest > head->r && *(*(room+head->r+1)+head->c) == ' ' && distance(head->r+1, head->c, row_dest, column_dest) < dist && adjacent(head->r+1,head->c,head2) != 1) // down
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
                head = move_inc_dist (room, head_S, head_F, row_dest, column_dest, dist,code,start);
                if (head == NULL)
                    return NULL;
                dist = distance(head->r, head->c, row_dest, column_dest);
            }
        }
        else
        {
            if (code)
                head_S = head;
            else
                head_F = head;
            head = move_inc_dist (room, head_S, head_F, row_dest, column_dest, dist, code,start);
            if (head == NULL)
                return NULL;
            dist = distance (head->r, head->c, row_dest, column_dest);
        }

        if (abs(column_dest-head->c) == 1 && abs(row_dest-head->r) == 1)
            return head;
    }
    return head;
}


// old algorithm, works.
//


path* move_inc_dist (char** room, path* head_S, path* head_F, int row_dest, int column_dest, int dist, int code, path* start)
{
    path* head;
    path* head2;
    char c;

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

    while (1)
    {
        if ( *(*(room+head->r-1)+head->c) == ' ' && adjacent(head->r-1,head->c,head2) != 1) // up
        {
            head = move_up(room,head,c);

            if (distance(head->r, head->c, row_dest, column_dest) < dist)
                return head;
            dist++;
        }
        else if ( *(*(room+head->r)+head->c-1) == ' ' && adjacent(head->r,head->c-1,head2) != 1) // left
        {
            head = move_left(room,head,c);

            if (distance(head->r, head->c, row_dest, column_dest) < dist)
                return head;
            dist++;
        }
        else if ( *(*(room+head->r+1)+head->c) == ' ' && adjacent(head->r+1,head->c,head2) != 1) // down
        {
            head = move_down(room,head,c);

            if (distance(head->r, head->c, row_dest, column_dest) < dist)
                return head;
            dist++;
        }
        else if ( *(*(room+head->r)+head->c+1) == ' ' && adjacent(head->r, head->c+1, head2) != 1) // right
        {
            head = move_right(room,head,c);

            if (distance(head->r, head->c, row_dest, column_dest) < dist)
                return head;
            dist++;
        }
        else
        {
            if (start->r == head->r && start->c == head->c)
                return NULL;
            else
                head = deadend_filler (room,head);
        }
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

void clear_path (char** room, int counter)
{
    int row, col;
    for (row=0; row<counter; row++)
        for (col=0; col<strlen(*(room+row)); col++)
            if (*(*(room+row)+col) == '0' || *(*(room+row)+col) == '1')
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
