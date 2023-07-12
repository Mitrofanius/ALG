#include <stdio.h>
#include <cstdlib>
#include <vector>
#include <map>
#include <unordered_set>

#define INT_MIN -2147483648

struct Parameters{
    int num_villages, totem_cost, war_cost;
    int * arr_villagers;
    int ** arr_profit, **arr_population;

    void allocate() {
        arr_villagers = new int[2 * num_villages];
        arr_profit = new int*[num_villages * 2];
        arr_profit[0] = new int[num_villages * num_villages * 2];
        for(int i = 1; i < num_villages; i++){
            arr_profit[i] = arr_profit[0] + num_villages * i;
        } 
        arr_population = arr_profit + num_villages;
        arr_population[0] = arr_profit[0] + num_villages * num_villages;
        for(int i = 1; i < num_villages; i++){
            arr_population[i] = arr_population[0] + num_villages * i;
        } 
    }

    ~Parameters(){
        delete[] arr_villagers;
        delete[] arr_profit[0];
        delete[] arr_profit;
    }
};

struct Transaction{
    int first;
    int last;
    int value;
    Transaction(int a, int b, int c) : 
    first(a), last(b), value(c) {}
    Transaction(){}
};

void get_input(Parameters& params){
    scanf("%d %d %d", &params.num_villages, &params.totem_cost, &params.war_cost);
    params.allocate();
    for (int i = 0; i < params.num_villages; i++)
    {
        scanf("%d", &params.arr_villagers[i]);
    }   
}

int find_max_profit(Parameters& p){
    int max_profit = 0;
    int i = 0, j = 0;
    int positives = 0;
    int cnt = 0;
    int loc_max = 0;

    for (i = 0; i < p.num_villages; i++){
        for (j = i; j < p.num_villages; j++)
        {
            if (p.arr_profit[i][j] > 0){
                positives += 1;
            }
        }        
    }

    Transaction * arr_max = new Transaction[positives];
    int * arr_res = new int[positives];
    
    for (i = 0; i < p.num_villages; i++){
        loc_max = 0;
        for (j = i; j < p.num_villages; j++)
        {
            if (p.arr_profit[i][j] > loc_max){
                loc_max = p.arr_profit[i][j];
                arr_max[cnt].first = i;
                arr_max[cnt].last = j;
                arr_max[cnt].value = p.arr_profit[i][j];
                arr_res[cnt] = p.arr_profit[i][j];
                cnt++;
            }
        }        
    }
    
    
    for (i = 0; i < cnt; i++)
    {
        for (j = 0; j < i; j++)
        {
            if (arr_max[i].first <= arr_max[j].last) continue;
            if (arr_res[i] < arr_res[j] + arr_max[i].value)
                {
                    arr_res[i] =  arr_res[j] + arr_max[i].value;
                    if (arr_res[i] > max_profit) max_profit = arr_res[i];
                }
        }
        
    }
    
    delete[] arr_max;
    delete[] arr_res;
    return max_profit;
}

int do_business(Parameters &p){
    int max_profit = INT_MIN;
    int local_max = INT_MIN;
    int max_k = 0;
    int tmp = 0;

    for (int i = 0; i < p.num_villages; i++){
            p.arr_population[i][i] = p.arr_villagers[i];
    }

    for (int i = 1; i < p.num_villages; i++){
        for (int j = 0; j + i < p.num_villages; j++)
        {
            local_max = INT_MIN;
            for (int k = j; k < j + i; k++)
            {
                tmp = p.totem_cost 
                      - std::abs(p.arr_population[j][k] - p.arr_population[k + 1][j + i]) * p.war_cost
                      + p.arr_profit[j][k] + p.arr_profit[k + 1][j + i];
                if (tmp > local_max){
                    local_max = tmp;
                    max_k = k;
                }
            }
            p.arr_profit[j][j + i] = local_max;
            p.arr_population[j][j + i] = p.arr_population[j][max_k] + p.arr_population[max_k + 1][j + i];
        }
    }     

    max_profit = find_max_profit(p);
    
    return max_profit;
}

int main(int argc, char * argv[]){
    Parameters params;
    get_input(params);
    printf("%d\n", do_business(params));
    
    return 0;
}