#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>

typedef struct Path {
    int r;
    int c;
    struct Path *next;
} path;

int distance(int,int,int,int);
path* move(char**,path*,path*,int,int,int,int);
void print_map(char**,int);
void print_list(path*);
path* move_inc_dist(char**,path*,path*,int,int,int,int);
path* deadend_filler(char**,path*);

int main (int argc, char** argv)
{

    int i, j, counter=0;
    FILE *fp;
    char** room;
//    char *first, *last, *start, *end;
/*
    first = (char *)malloc(8);
    last = (char *)malloc(8);
    start = (char *)malloc(8);
    end = (char *)malloc(8);
*/    
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
//                row_F = i;
//                column_F = j;
//                first = &*(*(room+head_F->r)+head_F->c);
            }
            if ( *(*(room+i)+j) == 'L')
            {
                dest_L->r = i;
                dest_L->c = j;
//                row_L = i;
  //              column_L = j;
//                last = &*(*(room+dest_L->r)+dest_L->c);
            }
            if ( *(*(room+i)+j) == 'S')
            {
                head_S->r = i;
                head_S->c = j;
                head_S->next = NULL;
//                row_S = i;
//                column_S = j;
//                start = &*(*(room+head_S->r)+head_S->c);
            }
            if ( *(*(room+i)+j) == 'E')
            {
                dest_E->r = i;
                dest_E->c = j;
//                row_E = i;
//                column_E = j;
//                end = &*(*(room+dest_E->r)+dest_E->c);
            }

        }
/*
    printf("FIRST: Row: %d, Column: %d, Value: %c Address: %p\n", head_F->r, head_F->c, *first, first);
    printf("LAST: Row: %d, Column: %d, Value: %c Address: %p\n", row_L, column_L, *last, last);
    printf("START: Row: %d, Column: %d, Value: %c Address: %p\n", head_S->r, head_S->c, *start, start);
    printf("END: Row: %d, Column: %d, Value: %c Address: %p\n", row_E, column_E, *end, end);
*/
//    printf("Distance SE: %d\n", distance(head_S->r, head_S->c, , column_E));

    if (distance(dest_E->r, dest_E->c, dest_L->r, dest_L->c) == 1 || distance(head_S->r, head_S->c, head_F->r, head_F->c) == 1)
    {
        printf("Not possible\n");
        return 0;
    }
    
//    *(list+1) = head_F = move(room, *(list+0), *(list+1), dest_L->r, dest_L->c, distance(head_F->r, head_F->c, dest_L->r, dest_L->c), 0);

/*
    // activate movement
        if ( distance(head_S->r, head_S->c, dest_E->r, dest_E->c) != 1)
            *(list+0) = head_S = move(room, *(list+0), *(list+1), dest_E->r, dest_E->c, distance(head_S->r, head_S->c, dest_E->r, dest_E->c), 1);
       if ( distance(head_F->r, head_F->c, dest_L->r, dest_L->c) != 1)
            *(list+1) = head_F = move(room, *(list+0), *(list+1), dest_L->r, dest_L->c, distance(head_F->r, head_F->c, dest_L->r, dest_L->c), 0);
       if ( distance(head_S->r, head_S->c, dest_E->r, dest_E->c) == 1 && distance(head_F->r, head_F->c, dest_L->r, dest_L->c) == 1)
        {
            printf("Success BITCH\n");
//            break;
        }

*/

//    *(list+1) = head_F = move(room, *(list+0), *(list+1), dest_L->r, dest_L->c, distance(head_F->r, head_F->c, dest_L->r, dest_L->c),0);

    *(list+0) = head_S = move(room, *(list+0), *(list+1), dest_E->r, dest_E->c, distance(head_S->r, head_S->c, dest_E->r, dest_E->c),1);
//    *(list+0) = head_S = move(room, *(list+0), *(list+1),  r, column_E, distance(head_S->r, head_S->c, row_E, column_E), 0);
    print_map(room, counter);
    print_list(*(list+1));

    free(*(list+0));
    free(*(list+1));
    free(list);
    return 0;
}

int distance (int row_current, int column_current, int row_dest, int column_dest)
{
    return abs(row_current-row_dest)+abs(column_current-column_dest);
}



path* move (char** room, path* head_S, path* head_F, int row_dest, int column_dest, int dist, int code)
{
    path* temp;
    path* head = malloc(sizeof(path));
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
//    for (i=0; i<40; i++)
    while (dist != 1)
    {
         if ( column_dest < head->c && *(*(room+head->r)+head->c-1) == ' ' && distance(head->r, head->c-1, row_dest, column_dest) < dist) // left
        {
            *(*(room+head->r)+head->c-1) = c;
            temp = malloc(sizeof(path));
            temp->r = head->r;
            temp->c = head->c-1;
            temp->next = head;
            head = temp;

            dist--;
        }
        else if ( row_dest < head->r && *(*(room+head->r-1)+head->c) == ' ' && distance(head->r-1, head->c, row_dest, column_dest) < dist) // up
        {
            *(*(room+head->r-1)+head->c) = c;
            temp = malloc(sizeof(path));
            temp->r = head->r-1;
            temp->c = head->c;
            temp->next = head;
            head = temp;

            dist--;
        }
        else if ( row_dest > head->r && *(*(room+head->r+1)+head->c) == ' ' && distance(head->r+1, head->c, row_dest, column_dest) < dist) // right
        {
            *(*(room+head->r+1)+head->c) = c;
            temp = malloc(sizeof(path));
            temp->r = head->r+1;
            temp->c = head->c;
            temp->next = head;
            head = temp;

            dist--;
        }
        else if ( column_dest > head->c  && *(*(room+head->r)+head->c+1) == ' ' && distance(head->r, head->c+1, row_dest, column_dest) < dist) // down
        {
            *(*(room+head->r)+head->c+1) = c;
            temp = malloc(sizeof(path));
            temp->r = head->r;
            temp->c = head->c+1;
            temp->next = head;
            head = temp;

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

    return head;
}

path* move_inc_dist (char** room, path* head_S, path* head_F, int row_dest, int column_dest, int dist, int code)
{
    path* temp;
    path* head = malloc(sizeof(path));
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
         if ( *(*(room+head->r)+head->c-1) == ' ') // left
        {
            *(*(room+head->r)+head->c-1) = c;
            temp = malloc(sizeof(path));
            temp->r = head->r;
            temp->c = head->c-1;
            temp->next = head;
            head = temp;

            if (distance(head->r, head->c, row_dest, column_dest) < dist)
                return head;
            dist++;
        }
        else if ( *(*(room+head->r-1)+head->c) == ' ') // up
        {
            *(*(room+head->r-1)+head->c) = c;
            temp = malloc(sizeof(path));
            temp->r = head->r-1;
            temp->c = head->c;
            temp->next = head;
            head = temp;

            if (distance(head->r, head->c, row_dest, column_dest) < dist)
                return head;
            dist++;
        }
        else if ( *(*(room+head->r+1)+head->c) == ' ') // right
        {
            *(*(room+head->r+1)+head->c) = c;
            temp = malloc(sizeof(path));
            temp->r = head->r+1;
            temp->c = head->c;
            temp->next = head;
            head = temp;

            if (distance(head->r, head->c, row_dest, column_dest) < dist)
                return head;
            dist++;
        }
        else if ( *(*(room+head->r)+head->c+1) == ' ') // down
        {
            *(*(room+head->r)+head->c+1) = c;
            temp = malloc(sizeof(path));
            temp->r = head->r;
            temp->c = head->c+1;
            temp->next = head;
            head = temp;

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
    if ( *(*(room+head->r)+head->c) == '1' || *(*(room+head->r)+head->c) == '2')
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

void print_list (path* head)
{
    while(head!=NULL)
    {
        printf("\nRow: %d\t Column: %d", head->r, head->c);
        head = head->next;
    }
}
