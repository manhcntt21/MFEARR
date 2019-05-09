#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <complex.h>
#include <math.h>
using namespace std;
// declear variable
int dimension_one ;
int dimension_two ; // mac dinh = dimension_one - 20
int dimension ;
int number_individual_population;
int number_current_individual_population;
int **P;
int **Q;
int **Distance;
FILE *filename;
int *factorial_cost_one;
int *factorial_cost_two;
int *factorial_rank_one;
int *factorial_rank_two;
int *skill_factor;
float *divergent; // luu ti le song xot  cua divergent: divergent[i] = number_divergent[i+1] / number_divergent[i]
int *index_divergents;
float *lamda;
float ASRD;  //Accumulated Survival Rate of Divergents,
float epsilon = 0.01;
int number_generation = 500;
int t  = number_generation/10;
float rmp = 0.3; // random assortative mating
float *scalar_fitness;
typedef struct {
    int value;
    int index;
} gene_tsp;

typedef struct {
    int index;
    int x;
    int y;
}Node_Tsp;

Node_Tsp *node_tsp;
// declear prototype functions
void initiation_population_P();
void evaluate_individuals();
void assortative_mating();
void evaluate_individuals_P_coma();
void update_scalar_fitness();
void selection();
void initiation_parameter();
void delete_variable_allocation_dynamic();
void read_data();
void evaluate_individuals_tasks_one();
void evaluate_individuals_tasks_two();
void factorial_cost_one_function() ;
void factorial_cost_two_function() ;
void factorial_rank_one_function() ;
void factorial_rank_two_function() ;
void skill_factor_function();
void crossover(int i, int j);
void mutation(int i);
void swap__task(int *a, int *b) ;
void swap_task(float *a, float *b);
void swap_task_integer(int* &a, int* &b) ;
void sum_index_divergent(int i);
void compute_lamda(int index) ; // lamda[i]  =  divergent [i+1] / divergent[i]
void mfearr();
// function test
void print();
void sort_cost();
int main(int argc, char const *argv[])
{
    srand(time(0));
    initiation_parameter();
    initiation_population_P();
    evaluate_individuals();
//    print() ;
//    print();

    for(int i = 0; i <  number_generation; i ++) {
//         printf("---------------------------------------------------BEFOR------------------------------------------------------------------");
        assortative_mating();
//        printf("%d", number_current_individual_population);
        evaluate_individuals_P_coma();
        update_scalar_fitness();
//        printf("---------------------------------------------------AFTER------------------------------------------------------------------");
        selection();
        evaluate_individuals_tasks_one();
        evaluate_individuals_tasks_two();


//        mfearr();
// //        printf("%d", number_current_individual_population);
//        evaluate_individuals_P_coma();
//        update_scalar_fitness();
// //        printf("---------------------------------------------------AFTER------------------------------------------------------------------");
//        selection();
//        evaluate_individuals_tasks_one();
//        evaluate_individuals_tasks_two();
//        sum_index_divergent(i+1);
//        compute_lamda(i);
//        if( i >= t) {
//            ASRD = 0.0;
//            if( rmp == 0.0 ) continue;
//                for(int j = i - t; j < t ;j++) {
//                    ASRD += lamda[j];
//                    }
// //                    printf("%f ->>>",ASRD);
//                    if (ASRD < epsilon ) rmp = 0.0;
//        }
    }

    print();
//    printf("%.2f ", divergent[1]);
//printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");
//        for(int i = 0 ; i < number_individual_population*2 ;i++) {
//            printf("%-2d", index_divergents[i]);
//        }
//        printf("\n");
//    for(int i = 0 ; i <= number_generation ;i++) {
//            printf("%.2f  ", divergent[i]);
//    }
//    printf("\n");
//        printf("\n");
//    for(int i = 0 ; i <= number_generation ;i++) {
//            printf("%.2f  ", lamda[i]);
//    }
//    printf("\n");
    delete_variable_allocation_dynamic();
    return 0;
}

void initiation_parameter() {
    read_data();
    number_individual_population = 100;
    number_current_individual_population = number_individual_population;
    // compute distance
    for(int i = 0; i < dimension ; i++) {
        for(int j = 0 ; j < dimension ; j++) {
            if( i == j) Distance[i][j] = 0;
            else Distance[i][j] = Distance[j][i] = (int)sqrt(((node_tsp[i].x-node_tsp[j].x)*(node_tsp[i].x-node_tsp[j].x)) +((node_tsp[i].y-node_tsp[j].y)*(node_tsp[i].y-node_tsp[j].y))) ;
        }
    }
    factorial_cost_one = new int[number_individual_population*2];
    factorial_cost_two = new int[number_individual_population*2];
    factorial_rank_one = new int[number_individual_population*2];
    factorial_rank_two = new int[number_individual_population*2];
    skill_factor = new int[number_individual_population*2];
    scalar_fitness = new float[number_individual_population*2];
    divergent = new float[number_generation+1];
    index_divergents = new int[number_individual_population*2];
    for(int i = 0 ; i < number_individual_population*2; i++) {
        index_divergents[i] = 0;
    }
    divergent[0] = 0 ;
    lamda = new float[number_generation+1];
}

void initiation_population_P() {
    P = new int*[number_individual_population*2];
    for(int i = 0 ; i < number_individual_population*2;i++) {
        P[i] = new int[dimension];
    }

    Q = new int*[number_individual_population*2];
    for(int i = 0 ; i < number_individual_population*2;i++) {
        Q[i] = new int[dimension_two];
    }

    int array_temporation[dimension];
    for(int i = 0 ; i < dimension ; i++) {
        array_temporation[i] = i;
//        printf("%d ", array_temporation[i]);
    }
    for(int i = 0 ; i < number_individual_population ; i++) {
        random_shuffle(&array_temporation[0], &array_temporation[dimension]);
        for(int j = 0 ; j < dimension ; j++) {
            P[i][j] = array_temporation[j];
        }
    }
}



void delete_variable_allocation_dynamic() {
    for(int i = 0 ; i < number_individual_population*2;i++) {
        delete[] P[i];
    }
    delete[] P;
    for(int i = 0 ; i < number_individual_population*2;i++) {
        delete[] Q[i];
    }
    delete[] Q;

    for(int i = 0 ; i < dimension ; i++) {
        delete[] Distance[i];
    }
    delete[] Distance;
    delete[] node_tsp;
    delete[] factorial_cost_one;
    delete[] factorial_cost_two;
    delete[] factorial_rank_one;
    delete[] factorial_rank_two;
    delete[] skill_factor;
    delete[] scalar_fitness;
    delete[] divergent;
    delete[] index_divergents;
    delete[] lamda;
}
void evaluate_individuals() {
    evaluate_individuals_tasks_one();
    evaluate_individuals_tasks_two();
    skill_factor_function();
}

void sort_cost(int *array_sort, int dimension_tmp) {
    int tmp = 0;
    for(int j = 0 ; j < dimension_tmp -1 ; j++) {
        for(int k = j + 1 ; k < dimension_tmp ; k++) {
            if( array_sort[j] > array_sort[k]) {
                tmp  = array_sort[j];
                array_sort[j]= array_sort[k];
                array_sort[k] =  tmp;
            }
        }
    }
}

void evaluate_individuals_tasks_one()  {
    factorial_cost_one_function();
    factorial_rank_one_function();
}

void factorial_cost_one_function() {
    int cost;
    int c;
    int d;
    for(int i = 0; i < number_current_individual_population; i++)  {
        cost = 0;
        c = 0;
        d = 0;
        if(i < number_individual_population) {
            for(int j = 0; j < dimension - 1; j++) {
                    c = P[i][j] ;
                    d = P[i][j+1];
                    cost += Distance[c][d];
            }
            cost += Distance[P[i][dimension-1]][P[i][0]];
            factorial_cost_one[i] = cost;
        }
        else if (skill_factor[i] == 2) factorial_cost_one[i] = 10e6;
        else if (skill_factor[i] == 1) {
            for(int j = 0; j < dimension - 1; j++) {
                    c = P[i][j] ;
                    d = P[i][j+1];
                    cost += Distance[c][d];
            }
            cost += Distance[P[i][dimension-1]][P[i][0]];
            factorial_cost_one[i] = cost;
        }

    }
}

void factorial_rank_one_function() {

    int *tmp = new int[number_current_individual_population];
    int mark[number_current_individual_population] = {0};
    for( int i = 0; i < number_current_individual_population;i++)
        tmp[i] = factorial_cost_one[i];
    sort_cost(tmp,number_current_individual_population);
//    printf("\n");
    for(int i = 0; i < number_current_individual_population; i++) {
        for(int j = 0; j < number_current_individual_population; j++) {
            if((factorial_cost_one[i] == tmp[j])&&(mark[j]==0)) {
                factorial_rank_one[i] = j + 1;
                mark[j] = 1;
                break;
            }
            else if((tmp[i]==factorial_cost_one[j])&&(mark[j]!=0))
            {
               continue;
            }
        }
    }
    free(tmp);
}
void evaluate_individuals_tasks_two() {

    for(int i = 0 ; i < number_current_individual_population ; i++) {
        int k = 0;
        for(int j = 0 ; j < dimension ; j ++)  {
            if( P[i][j] < dimension_two && k < dimension_two )  Q[i][k++] = P[i][j];
        }
    }
    factorial_cost_two_function();
    factorial_rank_two_function();
}


void factorial_cost_two_function() {
    int cost;
    int c;
    int d;
    for(int i = 0; i < number_current_individual_population; i++)  {
        cost = 0;
        c = 0;
        d = 0;
        if( i < number_individual_population) {
            for(int j = 0; j < dimension_two - 1; j++) {
                    c = Q[i][j] ;
                    d = Q[i][j+1];
                    cost += Distance[c][d];
            }
            cost += Distance[Q[i][dimension_two-1]][Q[i][0]];
            factorial_cost_two[i] = cost;
        }
        else if (skill_factor[i] == 1) factorial_cost_two[i] = 10e6;
        else if (skill_factor[i] == 2) {
            for(int j = 0; j < dimension_two - 1; j++) {
                    c = Q[i][j] ;
                    d = Q[i][j+1];
                    cost += Distance[c][d];
            }
            cost += Distance[Q[i][dimension_two-1]][Q[i][0]];
            factorial_cost_two[i] = cost;
        }
    }
}

void factorial_rank_two_function() {
    int *tmp = new int[number_current_individual_population];
    int mark[number_current_individual_population] = {0};
    for( int i = 0; i < number_current_individual_population;i++)
        tmp[i] = factorial_cost_two[i];
    sort_cost(tmp,number_current_individual_population);
//    printf("\n");
    for(int i = 0; i < number_current_individual_population; i++) {
        for(int j = 0; j < number_current_individual_population; j++) {
            if((factorial_cost_two[i] == tmp[j])&&(mark[j]==0)) {
                factorial_rank_two[i] = j + 1;
                mark[j] = 1;
                break;
            }
            else if((tmp[i]==factorial_cost_two[j])&&(mark[j]!=0))
            {
               continue;
            }
        }
    }
    free(tmp);
}

void assortative_mating() {
    float _rand = (rand()%100)*1.0/100;
    int __rand;
    int tmp1;
    int tmp2;
    int array_tmp[number_individual_population];
    for(int i = 0 ; i < number_individual_population ; i++) {
        array_tmp[i] = i;
    }

    for(int i = 0 ; i < number_individual_population ; i = i + 2) {
        std::random_shuffle(&array_tmp[0],&array_tmp[number_individual_population]);
        tmp1 = array_tmp[0];
        tmp2 = array_tmp[1];
//        printf("(%d %d )\n",tmp1,tmp2);
        if (skill_factor[tmp1] == skill_factor[tmp2] || _rand < rmp) {
            crossover(tmp1,tmp2);
            if( skill_factor[tmp1] != skill_factor[tmp2]) {
                // xet con 1
                if(_rand < 0.5) {
                     skill_factor[number_individual_population+i] = 1;
                }
                else{
                     skill_factor[number_individual_population+i] = 2;
                }
                // xet con 2
                _rand = (rand()%100)*1.0/100;
                if(_rand < 0.5) {
                     skill_factor[number_individual_population+i+1] = 1;
                }
                else{
                     skill_factor[number_individual_population+i+1] = 2;
                }
            } else {
                // truong hop, con cai sinh ra tu bo me co cung skill factor, thi tmp1, hay tmp2 cung nhu nhau 
                 skill_factor[number_individual_population+i] = skill_factor[number_individual_population+i+1] = skill_factor[tmp1];
            }
        } else{
             mutation(tmp1);
             skill_factor[number_individual_population+i] = skill_factor[tmp1];
             mutation(tmp2);
             skill_factor[number_individual_population+i+1] = skill_factor[tmp2];
        }
    }
}

void crossover(int i, int j) {
//    Partially-mapped crossover (PMX)
    int start_crossover_point ; // mien ra tri tu 0 den number_vertex - 1
    int end_croosover_point;
    int *tmp1=(int*)malloc(dimension*sizeof(int));
    int *tmp2=(int*)malloc(dimension*sizeof(int));
    int array_tmp[dimension];
    for(int k = 0 ; k < dimension ; k++) {
        array_tmp[k] = k;
    }
    std::random_shuffle(&array_tmp[0],&array_tmp[dimension]);
    // chắc chắn không thể tạo ra hai số bằng nhau được.
    // chọn 2 số ngẫu nhiên để tiến hành lai ghép
    start_crossover_point = (array_tmp[0] > array_tmp[1]) ? array_tmp[1] : array_tmp[0];
    end_croosover_point = (array_tmp[0] < array_tmp[1]) ? array_tmp[1] : array_tmp[0];
    // printf("start_crossover_point = %d, end_croosover_point = %d\n",start_crossover_point,end_croosover_point);

    for(int k = start_crossover_point ; k <= end_croosover_point ; k++) {
        // thu tu i la cha, j la me
        tmp1[k] = P[j][k]; // tmp1 luu bien cua me
        tmp2[k] = P[i][k]; // tmp2 luu bien cua cha
    }
    int tmp;
    int counter ;
    // xử lý phần trước điểm lát ghép thứ nhất
    for(int k = 0 ; k < start_crossover_point ; k++) {
        counter = 1;
        tmp1[k] = 0;
        tmp = P[i][k];
        while( counter <= (end_croosover_point - start_crossover_point + 1)){
            for(int k1 = start_crossover_point; k1 <= end_croosover_point; k1++) {
                if( tmp == tmp1[k1]) {
                    tmp = P[i][k1];
                    counter = 1;
                    break;
                }
                counter++;
            }
        }
        tmp1[k] = tmp;
    }
//    xử lý phần sau điểm lát ghép thứ hai
    for(int k = end_croosover_point + 1 ; k < dimension ; k++) {
        counter = 1;
        tmp = P[i][k];
        tmp1[k] = 0;
        while(counter <= (end_croosover_point - start_crossover_point + 1)) {
            for(int k1 = start_crossover_point; k1 <= end_croosover_point; k1++) {
                if( tmp == tmp1[k1]) {
                    tmp = P[i][k1];
                    counter = 1;
                    break;
                }
                counter++;
            }
        }
        tmp1[k] = tmp;
    }
// tượng tự với con thư hai
    for(int k = 0 ; k < start_crossover_point ; k++) {
        counter = 1;
        tmp2[k] = 0;
        tmp = P[j][k];
        while( counter <= (end_croosover_point - start_crossover_point + 1)){
            for(int k1 = start_crossover_point; k1 <= end_croosover_point; k1++) {
                if( tmp == tmp2[k1]) {
                    tmp = P[j][k1];
                    counter = 1;
                    break;
                }
                counter++;
            }
        }
        tmp2[k] = tmp;
    }
    for(int k = end_croosover_point + 1 ; k < dimension ; k++) {
        counter = 1;
        tmp = P[j][k];
        tmp2[k] = 0;
        while(counter <= (end_croosover_point - start_crossover_point + 1)) {
            for(int k1 = start_crossover_point; k1 <= end_croosover_point; k1++) {
                if( tmp == tmp2[k1]) {
                    tmp = P[j][k1];
                    counter = 1;
                    break;
                }
                counter++;
            }
        }
        tmp2[k] = tmp;
    }
    for(int k = 0; k < dimension;k++) {
        P[number_current_individual_population][k] = tmp1[k];
    }
    number_current_individual_population = number_current_individual_population + 1;
    for(int k = 0 ; k < dimension ; k++) {
        P[number_current_individual_population][k] = tmp2[k];
    }
    number_current_individual_population = number_current_individual_population + 1;
    free(tmp1);
    free(tmp2);
}
void swap__task(int *a, int *b) {
    int tmp = *a;
        *a = *b;
        *b = tmp;
}
void mutation(int i) {
    // mutation đảo bit
//    int factory_cost_mutation;
    int *tmp2=(int*)malloc(dimension*sizeof(int));
    int first_mutation_point;
    int second_mutation_point;
    int array_tmp[dimension];
    for(int k = 0 ; k < dimension ; k++) {
        array_tmp[k] = k;
    }
    std::random_shuffle(&array_tmp[0],&array_tmp[dimension]);
    first_mutation_point = array_tmp[0];
    second_mutation_point = array_tmp[1];
//    factory_cost_mutation = 0;
    for(int k = 0 ; k < dimension ; k++) {
        tmp2[k] = P[i][k];
    }
    swap__task(tmp2+first_mutation_point,tmp2+second_mutation_point);
    for(int k = 0 ; k < dimension; k++) {
        P[number_current_individual_population][k] = tmp2[k];
    }
    number_current_individual_population = number_current_individual_population + 1;
    free(tmp2);
}
void evaluate_individuals_P_coma() {
    evaluate_individuals();
}

void update_scalar_fitness() {
    for(int i = 0 ; i < number_current_individual_population;i++) {
        if( (skill_factor[i] == 2 ) && (factorial_rank_one[i] != factorial_rank_two[i])) scalar_fitness[i] = 1.0/factorial_rank_two[i] ;
        else if ((skill_factor[i] == 1) && (factorial_rank_one[i] != factorial_rank_two[i])) scalar_fitness[i] = 1.0/factorial_rank_one[i];
        else {
            float a = (rand()%100*1.0)/100;
            if (a < 0.5)  scalar_fitness[i] = 1.0/factorial_rank_one[i];
            else scalar_fitness[i] = 1.0/factorial_rank_two[i] ;
        }
    }
}
void swap_task_integer(int *&a, int *&b) {
   int *c = a;
          a = b;
          b = c;
}
void swap_task(float *a, float *b) {
    float c = *a;
    *a = *b;
    *b = c;
}
void selection() {
//    for(int i = 0 ; i < number_current_individual_population ;i++) {
//        printf("%d ", skill_factor[i]);
//    }
//    printf("\n");
//    for(int i = 0 ; i < number_current_individual_population ;i++) {
//        printf("%f ", scalar_fitness[i]);
//    }
//    printf("\n");
    for(int i = 0 ; i < number_current_individual_population - 1 ; i++) {
        for(int j = i + 1 ; j  < number_current_individual_population ; j++) {
            if(scalar_fitness[i] < scalar_fitness[j]) {
//                printf("%d %d\n",i,j);
                swap_task(&scalar_fitness[i],&scalar_fitness[j]);
                swap__task(&skill_factor[i], &skill_factor[j]);
                swap_task_integer(P[i],P[j]);
                swap_task_integer(Q[i],Q[j]);
                swap__task(&index_divergents[i], &index_divergents[j]);
            }
        }
    }
//    for(int i = 0 ; i < number_current_individual_population ;i++) {
//        printf("%d ", skill_factor[i]);
//    }
//    printf("\n");
//    for(int i = 0 ; i < number_current_individual_population ;i++) {
//        printf("%f ", scalar_fitness[i]);
//    }
    number_current_individual_population = number_individual_population;
}

void read_data() {
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char* pch =NULL;
    char* array_[5];
    // filename = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/st70.tsp", "r");
   filename = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/eil51.tsp", "r");
//filename = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/eil101.tsp", "r");
//filename = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/rat99.tsp", "r");
//filename = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/rat783.tsp", "r");
//filename = fopen("/home/manhdo/Documents/TTTH_HHTT/ALL_tsp/a280.tsp", "r");
//filename = fopen("/home/manhdo/Documents/TTTH_HHTT/ALL_tsp/eil76.tsp", "r");
//filename = fopen("/home/manhdo/Documents/TTTH_HHTT/ALL_tsp/rat195.tsp", "r");
//filename = fopen("/home/manhdo/Documents/TTTH_HHTT/ALL_tsp/rat575.tsp", "r");
    if (filename == NULL)
        exit(EXIT_FAILURE);

    for(int i = 0; i < 3 ; i++) {
        if (read = getline(&line, &len, filename) != -1) {
//            printf("Retrieved line of length %zu:\n", read);
//            printf("%s", line);
        }
    }
    if (read = getline(&line, &len, filename) != -1) {
//            printf("Retrieved line of length %zu:\n", read);
//            printf("%s", line);
            pch = strtok(line, " ");
            int j = 0;
            while(pch != NULL) {
                array_[j++] = pch;
                pch = strtok(NULL,":");
            }
            dimension_one = atoi(array_[1]);  // dimension_one
            dimension_two = dimension_one - 20;
            dimension = (dimension_one > dimension_two) ? dimension_one : dimension_two ;
    }
    Distance = new int*[dimension];
// Distance
    for(int i = 0 ; i < dimension ; i++) {
        Distance[i] = new int[dimension];
    }
    for(int i = 0; i < 2 ; i++) {
        if (read = getline(&line, &len, filename) != -1) {

        }
    }
    node_tsp = new Node_Tsp[dimension]; // use to compute distance
    int k = 0;
    while (read = getline(&line, &len, filename) != -1) {
        int j = 0 ;
        if (strcmp(line,"EOF") < 0) {
//            printf("%s", line);
            pch = strtok(line, " ");
            while(pch != NULL) {
                array_[j++] = pch;
                pch = strtok(NULL," ");
            }
            node_tsp[k].index = atoi(array_[0]);
            node_tsp[k].x = atoi(array_[1]);
            node_tsp[k].y = atoi(array_[2]);
            k++;
        }
    }
    fclose(filename);
}

void print() {
printf("-----------------Task1---------------------------------------------------------------------------------------------------------------------------\n");
    for(int i = 0 ; i  < number_current_individual_population ;i ++) {
        for(int j = 0; j  < dimension ; j++) {
            printf("%-2d ",P[i][j]);
        }
        printf("\n");
    }
printf("-----------------Task2---------------------------------------------------------------------------------------------------------------------------\n");
    for(int i = 0 ; i  < number_current_individual_population ;i ++) {
        for(int j = 0; j  < dimension_two ; j++) {
            printf("%-2d ",Q[i][j]);
        }
        printf("\n");
    }
 printf("-----------------Distance---------------------------------------------------------------------------------------------------------------------------\n");
     for(int i = 0 ; i  < dimension ;i ++) {
         for(int j = 0; j  < dimension ; j++) {
             printf("%-2d ",Distance[i][j]);
         }
         printf("\n");
     }
printf("------------------cost task1--------------------------------------------------------------------------------------------------------------------------\n");
    for(int i = 0 ; i < number_current_individual_population ;i++) {
        printf("%-3d ", factorial_cost_one[i]);
    }
    printf("\n");
printf("-------------------cost task2-------------------------------------------------------------------------------------------------------------------------\n");
    for(int i = 0 ; i < number_current_individual_population ;i++) {
        printf("%-3d ", factorial_cost_two[i]);
    }
    printf("\n");


printf("-------------------rank task1-------------------------------------------------------------------------------------------------------------------------\n");
    for(int i = 0 ; i < number_current_individual_population ;i++) {
        printf("%d ", factorial_rank_one[i]);
    }
    printf("\n");
printf("-------------------rank task2-------------------------------------------------------------------------------------------------------------------------\n");
    for(int i = 0 ; i < number_current_individual_population ;i++) {
        printf("%d ", factorial_rank_two[i]);
    }
    printf("\n");
printf("-------------------skill factor-------------------------------------------------------------------------------------------------------------------------\n");
    for(int i = 0 ; i < number_current_individual_population ;i++) {
        printf("%d ", skill_factor[i]);
    }
    printf("\n");
printf("--------------------scalar fitness------------------------------------------------------------------------------------------------------------------------\n");
    for(int i = 0 ; i < number_current_individual_population ;i++) {
        printf("%.2f ", scalar_fitness[i]);
    }
    printf("\n");

    if(rmp == 0.0) {
        printf("-------------------divergent------------------------------------------------------------------------------------------------------------------------\n");
            for(int i = 0 ; i < number_generation ;i++) {
                printf("%.0f ", divergent[i]);
            }
            printf("\n");

        printf("-------------------lamda------------------------------------------------------------------------------------------------------------------------\n");
            for(int i = 0 ; i < number_generation ;i++) {
                printf("%.2f ", lamda[i]);
            }
            printf("\n");

        printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");
            for(int i = 0 ; i < number_individual_population*2 ;i++) {
                printf("%-2d", index_divergents[i]);
            }
            printf("\n");
    }
}

void skill_factor_function() {
    for(int i = 0 ; i < number_individual_population ;i++) {
        if( factorial_rank_one[i] < factorial_rank_two[i]) skill_factor[i] = 1;
        else if (factorial_rank_one[i] > factorial_rank_two [i] ) skill_factor[i] = 2;
        else {
            float a = (rand()%100*1.0)/100;
            if (a < 0.5) skill_factor[i] = 1;
            else skill_factor[i] = 2;
        }
    }
}
void sum_index_divergent(int index)  {
    float sum = 0;
    for(int i = 0; i < number_individual_population; i++)  {
        sum+= index_divergents[i];
    }
    divergent[index] = sum;
//    printf("  %d",sum);
 }

void compute_lamda(int index) {
    if ( ( divergent[index+1] - divergent[index] >= 0) && (divergent[index] != 0) && (divergent[index+1] != 0))  {
        lamda[index] = 1.0;
    }
   else if(divergent[index+1] - divergent[index] < 0) {
        lamda[index] = divergent[index+1]*1.0/divergent[index];
    }
    else if ( (divergent[index+1] - divergent[index])< 10e-10 ||  (divergent[index+1] - divergent[index]) > -10e-10 ) {
        lamda[index] = 0.0;
    }
}

void mfearr() {
    float _rand;
    float  __rand ;
    int tmp1;
    int tmp2;
    int array_tmp[number_individual_population];
    for(int i = 0 ; i < number_individual_population ; i++) {
        array_tmp[i] = i;
    }

    for(int i = 0 ; i < number_individual_population ; i = i + 2) {
        _rand = (rand()%100)*1.0/100;
        __rand = (rand()%100)*1.0/100;
        std::random_shuffle(&array_tmp[0],&array_tmp[number_individual_population]);
        tmp1 = array_tmp[0];
        tmp2 = array_tmp[1];
//        printf("(%d %d )\n",tmp1,tmp2);
        if ( _rand < rmp) {
            std::random_shuffle(&array_tmp[0],&array_tmp[number_individual_population]);
            tmp1 = array_tmp[0];
            tmp2 = array_tmp[1];
            // truong hop khac skillfactor
            while ( skill_factor[tmp1]  == skill_factor[tmp2]  ) {
                std::random_shuffle(&array_tmp[0],&array_tmp[number_individual_population]);
                tmp1 = array_tmp[0];
                tmp2 = array_tmp[1];
            }
            crossover(tmp1,tmp2);
            // xet con 1
            if (__rand < 0.5) {
                skill_factor[number_individual_population+i] =  1;
            } else {
                skill_factor[number_individual_population+i] = 2;
            }
            index_divergents[number_individual_population+i] = 1;

            // xet con 2
            __rand = (rand()%100)*1.0/100;
            if (__rand < 0.5) {
                skill_factor[number_individual_population+i+1] = 1;
            } else {
                skill_factor[number_individual_population+i+1] = 2;
            }
            index_divergents[number_individual_population+i+1] = 1;
        } else  {
            // cung skill factor
            std::random_shuffle(&array_tmp[0],&array_tmp[number_individual_population]);
            tmp1 = array_tmp[0];
            tmp2 = array_tmp[1];
            while ( skill_factor[tmp1]  != skill_factor[tmp2]  ) {
                std::random_shuffle(&array_tmp[0],&array_tmp[number_individual_population]);
                tmp1 = array_tmp[0];
                tmp2 = array_tmp[1];
            }
            crossover(tmp1,tmp2);
            skill_factor[number_individual_population+i] = skill_factor[number_individual_population+i+1] = skill_factor[tmp1];
            index_divergents[number_individual_population+i] = 0;
            index_divergents[number_individual_population+i+1] = 0;
        }
    }
}
