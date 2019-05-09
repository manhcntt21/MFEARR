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
int dimension_two ;
int dimension ; // duoc gan bang max cua dimension_one, dimension_two trong ham read_data
int number_individual_population;
int number_current_individual_population;
int **P;
int **Q;
int **Distance;
int **Distance_two;
FILE *filename_one;
FILE *filename_two;
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
Node_Tsp *node_tsp_two;
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
    // print();
    for(int i = 0; i <  number_generation; i ++) {
    //     // printf("---------------------------------------------------BEFOR------------------------------------------------------------------\n");
    //     assortative_mating();
    //     evaluate_individuals_P_coma();
    //     update_scalar_fitness();
    // //     // printf("---------------------------------------------------AFTER------------------------------------------------------------------\n");
    //     selection();
    //     evaluate_individuals_tasks_one();
    //     evaluate_individuals_tasks_two();
//        bat dau mfearr 
       mfearr();
//        printf("%d", number_current_individual_population);
       evaluate_individuals_P_coma();
       update_scalar_fitness();
//        printf("---------------------------------------------------AFTER------------------------------------------------------------------");
       selection();
       evaluate_individuals_tasks_one();
       evaluate_individuals_tasks_two();
       sum_index_divergent(i+1);
       compute_lamda(i);
       if( i >= t) {
           ASRD = 0.0;
           if( rmp == 0.0 ) continue;
               for(int j = i - t; j < t ;j++) {
                   ASRD += lamda[j];
                   }
//                    printf("%f ->>>",ASRD);
                   if (ASRD < epsilon ) rmp = 0.0;
       }
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
    /*
    1 doc du lieu tu file
    2 khoi tao so luong ca the trong quan the
    3 tinh distance dua vao du lieu tu file
    voi mfearr 
    4 khoi tao gia tri index_divergent (danh dau mot ca the co la divergents)
    */
    read_data();
    number_individual_population = 100;
    number_current_individual_population = number_individual_population;
    // compute distance_one
    for(int i = 0; i < dimension_one ; i++) {
        for(int j = 0 ; j < dimension_one ; j++) {
            if( i == j) Distance[i][j] = 0;
            else Distance[i][j] = Distance[j][i] = (int)sqrt(((node_tsp[i].x-node_tsp[j].x)*(node_tsp[i].x-node_tsp[j].x)) +((node_tsp[i].y-node_tsp[j].y)*(node_tsp[i].y-node_tsp[j].y))) ;
        }
    }
    // compute distance two

    for(int i = 0; i < dimension_two ; i++) {
        for(int j = 0 ; j < dimension_two ; j++) {
            if( i == j) Distance_two[i][j] = 0;
            else Distance_two[i][j] = Distance_two[j][i] = (int)sqrt(((node_tsp_two[i].x-node_tsp_two[j].x)*(node_tsp_two[i].x-node_tsp_two[j].x)) +((node_tsp_two[i].y-node_tsp_two[j].y)*(node_tsp_two[i].y-node_tsp_two[j].y))) ;
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
    /*
    1 cap phat bo nho cho mang P, Q (loi giai cho task1, task2)
    2 khoi tao loi giai cho task1, task2
    */
    P = new int*[number_individual_population*2];
    for(int i = 0 ; i < number_individual_population*2;i++) {
        P[i] = new int[dimension_one];
    }

    Q = new int*[number_individual_population*2];
    for(int i = 0 ; i < number_individual_population*2;i++) {
        Q[i] = new int[dimension_two];
    }

    // khoi tao gia tri cho cac ca the P
    int array_temporation[dimension_one];
    for(int i = 0 ; i < dimension_one ; i++) {
        array_temporation[i] = i;
    //        printf("%d ", array_temporation[i]);
    }
    for(int i = 0 ; i < number_individual_population ; i++) {
        random_shuffle(&array_temporation[0], &array_temporation[dimension_one]);
        for(int j = 0 ; j < dimension_one ; j++) {
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

    for(int i = 0 ; i < dimension_one ; i++) {
        delete[] Distance[i];
    }
    for(int i = 0 ; i < dimension_two ; i++) {
        delete[] Distance_two[i];
    }
    delete[] Distance_two;
    delete[] Distance;
    delete[] node_tsp;
    delete[] node_tsp_two;
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
    /*
    1 danh gia ca the P theo task 1
    2 danh gia ca the P theo task 2 (truoc khi danh gia P theo task 2, phai decode P ra Q)
    3 tinh skillfactor - xem P giai task 1 hay task 2 tot hon
    */
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
    /*
    1 tinh cost cua cac ca the trong P theo Distance tuong ung cua no
    2 xep hang rank cua no (nguyen tac xep hang: di tu trai sang phai, neu co cung cost thi gia tri cost phat hien truoc, rank cao hon)
    */
    factorial_cost_one_function();
    factorial_rank_one_function();
}

void factorial_cost_one_function() {
    /*
    1 cac ca the trong quan the ban dau ( = so luong number_individual_population ) duoc tinh nhu binh thuong
    2 cac ca the duoc tao ra trong qua trinh assortative, thi duoc tinh theo skillfactor
    3 gia tri cost doi voi task cua ca the ma skillfactor khac, duoc gan mac dinh la 10e6 
    */
    int cost;
    int c;
    int d;
    for(int i = 0; i < number_current_individual_population; i++)  {
        cost = 0;
        c = 0;
        d = 0;
        if(i < number_individual_population) {
            for(int j = 0; j < dimension_one - 1; j++) {
                    c = P[i][j] ;
                    d = P[i][j+1];
                    cost += Distance[c][d];
            }
            cost += Distance[P[i][dimension_one-1]][P[i][0]];
            factorial_cost_one[i] = cost;
        }
        else if (skill_factor[i] == 2) factorial_cost_one[i] = 10e6;
        else if (skill_factor[i] == 1) {
            for(int j = 0; j < dimension_one - 1; j++) {
                    c = P[i][j] ;
                    d = P[i][j+1];
                    cost += Distance[c][d];
            }
            cost += Distance[P[i][dimension_one-1]][P[i][0]];
            factorial_cost_one[i] = cost;
        }

    }
}

void factorial_rank_one_function() {
    /*
    nguyen tac tinh rank da trinh bay o evaluate_individuals_tasks_one
    */
    int *tmp = new int[number_current_individual_population];
    int mark[number_current_individual_population] = {0};
    for( int i = 0; i < number_current_individual_population;i++)
        tmp[i] = factorial_cost_one[i];
    sort_cost(tmp,number_current_individual_population);
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
    /*
    1 decode P ra Q
    2 tinh cost cua cac ca the trong P theo Distance tuong ung cua no, o day la Distance cua Q: nguyen tac decode: duyet tu trai sang phai, gia tri nao lon hon dimension thi loai, cho den khi nao dat du so luong dimension cua task thi thoi
    3 xep hang rank cua no (nguyen tac xep hang: di tu trai sang phai, neu co cung cost thi gia tri cost phat hien truoc, rank cao hon)
    */
    for(int i = 0 ; i < number_current_individual_population ; i++) {
        int k = 0;
        for(int j = 0 ; j < dimension_one ; j ++)  {
            if( P[i][j] < dimension_two && k < dimension_two )  Q[i][k++] = P[i][j];
        }
    }
    factorial_cost_two_function();
    factorial_rank_two_function();
}

void factorial_cost_two_function() {
    /*
    1 cac ca the trong quan the ban dau ( = so luong number_individual_population ) duoc tinh nhu binh thuong
    2 cac ca the duoc tao ra trong qua trinh assortative, thi duoc tinh theo skillfactor
    3 gia tri cost doi voi task cua ca the ma skillfactor khac, duoc gan mac dinh la 10e6 
    */
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
                    cost += Distance_two[c][d];
            }
            cost += Distance_two[Q[i][dimension_two-1]][Q[i][0]];
            factorial_cost_two[i] = cost;
        }
        else if (skill_factor[i] == 1) factorial_cost_two[i] = 10e6;
        else if (skill_factor[i] == 2) {
            for(int j = 0; j < dimension_two - 1; j++) {
                    c = Q[i][j] ;
                    d = Q[i][j+1];
                    cost += Distance_two[c][d];
            }
            cost += Distance_two[Q[i][dimension_two-1]][Q[i][0]];
            factorial_cost_two[i] = cost;
        }
    }
}

void factorial_rank_two_function() {
    /*
    nguyen tac tinh rank da trinh bay o evaluate_individuals_tasks_two
    */
    int *tmp = new int[number_current_individual_population];
    int mark[number_current_individual_population] = {0};
    for( int i = 0; i < number_current_individual_population;i++)
        tmp[i] = factorial_cost_two[i];
    sort_cost(tmp,number_current_individual_population);
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
    /*
    tien hanh nhu trong MFEA
    1 chon 2 ca the bat ky trong MFEA: theo cach chon duoc trinh bay , thi luon tim duoc 2 ca the khac nhau
    2 muon doc ki, tham khao bai bao cua tac gia Multifactorial Evolution: Towards Evolutionary Multitasking Abhishek Gupta, Yew-Soon Ong*, and Liang Feng
    */
    float _rand = (rand()%100)*1.0/100;
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
/*
    1 chon 2 diem trong gen de tien hanh lai ghep
    2 giu lai diem giua 2 diem do, bao gom ca 2 diem do
    3 cac vi tri con lai duoc tinh dua tren bang anh xa: tap hop cac diem o giua do (theo thuat toan lai ghep hoan vi)
    4 qua trinh lai ghep duoc chia lam 3 gia doan chinh: giu lai doan giua
                                                         xu ly doan dau den diem cat dau tien
                                                         xu ly diem cat thu hai den het gene   
*/

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
/*
    chon 2 diem trong gene va doi cho chung la duoc
*/
// mutation đảo bit
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
    /*
    tinh scalar_fitness theo skillfactor cua no
    */
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
/*
    tien hanh lay du so luong ca the = voi quan the ban dau
    vi khi tien hanh assortative mating so luong ca the da tang gap doi 
    vi vay o cuoi ham, phai gan lai so luong ca the cua quan the
*/

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
    /*
    dimension_one phai >= dimension_two neu khong se bi loi
    */
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    char* pch =NULL;
    char* array_[5];
//filename;= fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/st70.tsp", "r");
//filename = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/eil101.tsp", "r");
//filename = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/rat99.tsp", "r");
//filename = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/rat783.tsp", "r");
//filename = fopen("/home/manhdo/Documents/TTTH_HHTT/ALL_tsp/a280.tsp", "r");
//filename = fopen("/home/manhdo/Documents/TTTH_HHTT/ALL_tsp/rat195.tsp", "r");
//filename = fopen("/home/manhdo/Documents/TTTH_HHTT/ALL_tsp/rat575.tsp", "r");

filename_one = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/eil101.tsp", "r");
filename_two = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/rat99.tsp", "r");

// filename_one = fopen("/home/manhdo/Documents/TTTH_HHTT/ALL_tsp/eil76.tsp", "r");
// filename_two = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/eil51.tsp", "r");


// filename_one = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/rat99.tsp", "r");
// filename_two = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/st70.tsp", "r");

// filename_one = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/st70.tsp", "r");
// filename_two = fopen("/home/manhdo/Documents/TTTH_HHTT/Test_TSP_Instance/eil51.tsp", "r");

    if (filename_one == NULL)
        exit(EXIT_FAILURE);

    for(int i = 0; i < 3 ; i++) {
        if ((read = getline(&line, &len, filename_one) )!= -1) {
//            printf("Retrieved line of length %zu:\n", read);
//            printf("%s", line);
        }
    }
    if ((read = getline(&line, &len, filename_one)) != -1) {
//            printf("Retrieved line of length %zu:\n", read);
//            printf("%s", line);
            pch = strtok(line, " ");
            int j = 0;
            while(pch != NULL) {
                array_[j++] = pch;
                pch = strtok(NULL,":");
            }
            // convert string to int 
            dimension_one = atoi(array_[1]);  // dimension_one
    }
    Distance = new int*[dimension_one];
// Distance
    for(int i = 0 ; i < dimension_one ; i++) {
        Distance[i] = new int[dimension_one];
    }
    for(int i = 0; i < 2 ; i++) {
        if ((read = getline(&line, &len, filename_one))!= -1) {

        }
    }
    node_tsp = new Node_Tsp[dimension_one]; // use to compute distance
    int k = 0;
    while ((read = getline(&line, &len, filename_one) )!= -1) {
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
    fclose(filename_one);

// read file 2
    if (filename_two == NULL)
        exit(EXIT_FAILURE);

    for(int i = 0; i < 3 ; i++) {
        if ((read = getline(&line, &len, filename_two)) != -1) {
//            printf("Retrieved line of length %zu:\n", read);
//            printf("%s", line);
        }
    }
    if ((read = getline(&line, &len, filename_two)) != -1) {
//            printf("Retrieved line of length %zu:\n", read);
//            printf("%s", line);
            pch = strtok(line, " ");
            int j = 0;
            while(pch != NULL) {
                array_[j++] = pch;
                pch = strtok(NULL,":");
            }
            dimension_two = atoi(array_[1]);  // dimension_one
            // dimension_two = dimension_one - 20;
            // dimension = (dimension_one > dimension_two) ? dimension_one : dimension_two ;
    }
    Distance_two = new int*[dimension_two];
// Distance_two
    for(int i = 0 ; i < dimension_two ; i++) {
        Distance_two[i] = new int[dimension_two];
    }
    for(int i = 0; i < 2 ; i++) {
        if ((read = getline(&line, &len, filename_two))!= -1) {

        }
    }
    node_tsp_two = new Node_Tsp[dimension_two]; // use to compute distance
    k = 0;
    while ((read = getline(&line, &len, filename_two)) != -1) {
        int j = 0 ;
        if (strcmp(line,"EOF") < 0) {
//            printf("%s", line);
            pch = strtok(line, " ");
            while(pch != NULL) {
                array_[j++] = pch;
                pch = strtok(NULL," ");
            }
            node_tsp_two[k].index = atoi(array_[0]);
            node_tsp_two[k].x = atoi(array_[1]);
            node_tsp_two[k].y = atoi(array_[2]);
            k++;
        }
    }

    fclose(filename_two);
    dimension = (dimension_one > dimension_two) ? dimension_one : dimension_two ;
}

void print() {

printf("-----------------Task1---------------------------------------------------------------------------------------------------------------------------\n");
    for(int i = 0 ; i  < number_current_individual_population ;i ++) {
        for(int j = 0; j  < dimension_one ; j++) {
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
     for(int i = 0 ; i  < dimension_one ;i ++) {
         for(int j = 0; j  < dimension_one ; j++) {
             printf("%-2d ",Distance[i][j]);
         }
         printf("\n");
     }

 printf("-----------------Distance---------------------------------------------------------------------------------------------------------------------------\n");
     for(int i = 0 ; i  < dimension_two ;i ++) {
         for(int j = 0; j  < dimension_two ; j++) {
             printf("%-2d ",Distance_two[i][j]);
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
if( rmp == 0.0) {
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

    printf("------------------index_divergents--------------------------------------------------------------------------------------------------------------------------\n");
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
