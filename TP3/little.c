/**
 * Projec : gtsp (voyageur de commerce)
 *
 * Date   : 07/04/2014
 * Author : Olivier Grunder
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define NBR_TOWNS 6

/* Distance matrix */
double dist[NBR_TOWNS][NBR_TOWNS] ;
double nearest_neighbour ;

/* Each edge has a starting and ending node */
int starting_town[NBR_TOWNS] ;
int ending_town[NBR_TOWNS] ;

/* no comment */
int best_solution[NBR_TOWNS] ;
double best_eval=-1.0 ;


/**
 * Berlin52 :
 *  6 towns : Best solution (2315.15): 0 1 2 3 5 4
 * 10 towns : Best solution (2826.50): 0 1 6 2 7 8 9 3 5 4
 */
float coord[NBR_TOWNS][2]=
{
    {565.0,  575.0},
    { 25.0,  185.0},
    {345.0,  750.0},
    {945.0,  685.0},
    {845.0,  655.0},
    {880.0, 660.0},
} ;


// Matrice distance

void matrice_distance(double dist[NBR_TOWNS][NBR_TOWNS],float coord[NBR_TOWNS][2])
{
    for (int i=0;i<NBR_TOWNS;i++)
    {
        for (int j=0;j<NBR_TOWNS;j++)
        {
            if (i==j){
                dist[i][j]=-1;
            }
            else{
                dist[i][j]=sqrt(pow(coord[j][0]-coord[i][0],2)+pow(coord[j][1]-coord[i][1],2));
            }
        }
    }
}

/**
 * print a matrix
 */
void print_matrix(double d[NBR_TOWNS][NBR_TOWNS])
{
    int i, j ;
    for (i=0; i<NBR_TOWNS; i++)
    {
        printf ("%d:", i) ;
        for (j=0; j<NBR_TOWNS; j++)
        {
            printf ("%6.1f ", d[i][j]) ;
        }
        printf ("\n") ;
    }
}

/**
 * print a solution
 */
void print_solution(int* sol, double eval)
{
    int i ;
    printf ("(%.2f): ", eval) ;
    for (i=0; i<NBR_TOWNS; i++)
        printf ("%d ",sol[i]);
    printf("\n") ;
}




/**
 * evaluation of a solution
 */
double evaluation_solution(int* sol)
{
    double eval=0 ;
    int i ;
    for (i=0; i<NBR_TOWNS-1; i++)
    {
        eval += dist[sol[i]][sol[i+1]] ;
    }
    eval += dist[sol[NBR_TOWNS-1]][sol[0]] ;

    return eval ;

}

/**
 * nearest neighbour solution
 */
double build_nearest_neighbour()
{
    /* solution of the nearest neighbour */
    int i,j, sol[NBR_TOWNS];

    int list[10]={0,-1,-1,-1,-1,-1,};
    /* evaluation of the solution */
    double eval = 0 ;
    
    sol[0] = 0 ;
    for(i=1;i<NBR_TOWNS;i++)
    {   
        double min =1000;
        int indice=0;
        for(j=0;j<NBR_TOWNS;j++)
        {
            if(dist[i][j]<min && list[j]==-1 && i!=j)
            {
                min=dist[i][j];
                indice=j;
            }
        }
        list[indice]=1;
        sol[i]=indice;
    }
    eval = evaluation_solution(sol);
    printf("Nearest neighbour ");
    print_solution (sol, eval);
    
    for (i=0;i<NBR_TOWNS;i++) 
        best_solution[i] = sol[i] ;
    best_eval  = eval ;
    return eval ;
}


/**
 *  Build final solution
 */
void build_solution()
{
    int i, solution[NBR_TOWNS] ;

    int indiceCour = 0;
    int villeCour = 0;

    while (indiceCour < NBR_TOWNS)
    {

        solution[indiceCour] = villeCour ;

        // Test si le cycle est hamiltonien
        for (i = 0; i < indiceCour; i++)
        {
            if (solution[i] == villeCour)
            {
                // printf ("cycle non hamiltonien\n") ;
                return;
            }
        }
        // Recherche de la ville suivante
        int trouve = 0;
        int i = 0;
        while ((!trouve) && (i < NBR_TOWNS))
        {
            if (starting_town[i] == villeCour)
            {
                trouve = 1;
                villeCour = ending_town[i];
            }
            i++;
        }
        indiceCour++;
    }

    double eval = evaluation_solution(solution) ;

    if (best_eval<0 || eval < best_eval)
    {
        best_eval = eval ;
        for (i=0; i<NBR_TOWNS; i++)
            best_solution[i] = solution[i] ;
        printf ("New best solution: ") ;
        print_solution (solution, best_eval) ;
    }
    return;
}




/**
 *  Little Algorithm
 */
void little_algorithm(double d0[NBR_TOWNS][NBR_TOWNS], int iteration, double eval_node_parent)
{

    if (iteration == NBR_TOWNS)
    {
        build_solution ();
        return;
    }

    /* Do the modification on a copy of the distance matrix */
    double d[NBR_TOWNS][NBR_TOWNS] ;
    memcpy (d, d0, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

    int i, j ;

    double eval_node_child = eval_node_parent;

    /* substract the min of the rows and the min of the columns
       and update the evaluation of the current node */


for (i=0; i<NBR_TOWNS; i++)
{   
    double min_row=INFINITY;
    for (j=0; j<NBR_TOWNS; j++)
    {
        if (d[i][j]<min_row && d[i][j]!=-1) 
            min_row=d[i][j];
    }
    for (j=0; j<NBR_TOWNS; j++)
    {
        if(d[i][j]!=-1)
            d[i][j] -= min_row;
    }
    if (min_row!=INFINITY) 
        eval_node_child += min_row;
}

for (j=0; j<NBR_TOWNS; j++)
{
    double min_col=INFINITY;
    for (int i=0; i<NBR_TOWNS; i++)
        {
         if (d[i][j]<min_col && d[i][j]!=-1)
            min_col = d[i][j];
        }
    for (i=0; i<NBR_TOWNS; i++)
        {
        if(d[i][j]!=-1)
            d[i][j] -= min_col;
        }
    if (min_col!=INFINITY) 
        eval_node_child += min_col;
}

    /* Cut : stop the exploration of this node */
    if (best_eval>=0 && eval_node_child >= best_eval)
        return;

    /* Compute the penalities to identify the zero with max penalty
       If no zero in the matrix, then return, solution infeasible */
    double penalty=0.0;
    double max_penalty = 0.0;
    int izero=-1, jzero=-1 ;
    for (i=0;i<NBR_TOWNS;i++)       
    {
        for(j=0;j<NBR_TOWNS;j++)    
        {
            if(d[i][j]==0)
            {
                int k;
                double min_row=INFINITY, min_col=INFINITY;
                for(k=0;k<NBR_TOWNS;k++)
                {
                    if(d[i][k]<min_row && d[i][k]>=0 && k!=j)
                        min_row=d[i][k];
                }
                for(k=0;k<NBR_TOWNS;k++)
                {
                    if(d[k][j]<min_col && d[k][j]>=0 && k!=i)
                        min_col=d[k][j];
                }
                penalty=min_row+min_col;
                if(penalty>max_penalty)
                {
                    max_penalty=penalty;
                    izero=i;
                    jzero=j;
                }
            }
        }
    }
    /* If no zero in the matrix, then return, solution infeasible */
    if (izero == -1)
        return;

    /* Store the row and column of the zero with max penalty in
       starting_town and ending_town */
    starting_town[iteration] = izero;
    ending_town[iteration] = jzero;

    /* Do the modification on a copy of the distance matrix */
    double d2[NBR_TOWNS][NBR_TOWNS] ;
    memcpy (d2, d, NBR_TOWNS*NBR_TOWNS*sizeof(double)) ;

   
/* Modify the matrix d2 according to the choice of the zero with the max penalty */
for(i=0;i<NBR_TOWNS;i++)
    {
        d2[izero][i]=-1;
        d2[i][jzero]=-1;
    }
/* Explore left child node according to given choice */
little_algorithm(d2, iteration + 1, eval_node_child);
/* Do the modification on a copy of the distance matrix */
memcpy(d2, d, NBR_TOWNS*NBR_TOWNS*sizeof(double));

/* Modify the dist matrix to explore the other possibility : the non-choice
   of the zero with the max penalty */
d2[izero][jzero]=-1; 

/* Explore right child node according to non-choice */
double eval_node_right = eval_node_child;
little_algorithm(d2, iteration, eval_node_right);

}

/**
 *
 */
int main (int argc, char* argv[])
{

    best_eval = -1 ;

    /* Print problem informations */
    printf ("Points coordinates:\n") ;
    int i,j ;
    for (i=0; i<NBR_TOWNS; i++)
    {
        printf ("Node %d: x=%f, y=%f\n", i, coord[i][0], coord[i][1]) ;
    }
    printf ("\n") ;


    /* Calcul de la matrice des distances */
    matrice_distance(dist,coord);
    printf ("Distance Matrix:\n") ;
    print_matrix (dist) ;
    printf ("\n") ;
    /** Nearest Neighbour heuristic : uncomment when needed */
    double nearest_neighbour = build_nearest_neighbour() ;

    /** Little : uncomment when needed */
    int iteration = 0 ;
    double lowerbound = 0.0 ;
     
    little_algorithm(dist, iteration, lowerbound) ;
    printf("Best solution:") ;
    print_solution (best_solution, best_eval) ;
    
    printf ("Hit RETURN!\n") ;
    getchar() ;

    return 0 ;
}
