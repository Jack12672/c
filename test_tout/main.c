#include <stdbool.h>

struct Neighbourg
{
    int p;
    struct Neigghbourg *nei[3];
    bool b; 
};


void recus (struct Neighbourg *n[20])
{
    if (n[1]->b==true) return;

    for (int i = 0; i<3; i++)
    {
        
    }
}

int main()
{
    struct Neighbourg *neighbourg[20];

    neighbourg[1]->p=1;
    neighbourg[1]->nei[0]=neighbourg[2];
    neighbourg[1]->nei[1]=3;
    neighbourg[1]->nei[2]=4;

    neighbourg[2]->p=2;
    neighbourg[2]->nei[0]=1;
    neighbourg[2]->nei[1]=3;
    neighbourg[2]->nei[2]=5;

    neighbourg[3]->p=3;
    neighbourg[3]->nei[0]=2;
    neighbourg[3]->nei[1]=6;
    neighbourg[3]->nei[2]=7;

    neighbourg[4]->p=4;
    neighbourg[4]->nei[0]=3;
    neighbourg[4]->nei[1]=5;
    neighbourg[4]->nei[2]=7;


    neighbourg[5]->p=5;
    neighbourg[5]->nei[0]=2;
    neighbourg[5]->nei[1]=4;
    neighbourg[5]->nei[2]=7;

    neighbourg[6]->p=6;
    neighbourg[6]->nei[0]=2;
    neighbourg[6]->nei[1]=5;
    neighbourg[6]->nei[2]=4;


    neighbourg[7]->p=7;
    neighbourg[7]->nei[0]=2;
    neighbourg[7]->nei[1]=6;
    neighbourg[7]->nei[2]=1;

    for (int i=1; i<8; i++)
    {
        neighbourg[i]->b=false;
    }




}