#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>
#include <algorithm>

#define LOG(a) std::cout << a << "\n"

struct Egg{
    int x;
    int y;
    // Egg(int x, int y):x(x), y(y){}
    Egg(){};
};

struct game_parameters{
    int width;
    int max_floor;
    int num_eggs;
    int * big_boy;
    int * marked_floor;
    int * scores;
    int * prev_scores;
    int * positions;
    Egg * eggs;
    ~game_parameters(){
        delete[] eggs;
        delete[] big_boy;
    }
};

void get_input(game_parameters& params){
    scanf("%d %d", &params.width, &params.num_eggs);
    params.eggs = new Egg[params.num_eggs];
    params.big_boy = new int[params.width * 4];
    params.scores = params.big_boy;
    params.prev_scores = params.scores + params.width;
    params.positions = params.prev_scores + params.width;
    params.marked_floor = params.positions + params.width;
    for (int i = 0; i < params.num_eggs; i++){
        scanf("%d %d", &params.eggs[i].x, &params.eggs[i].y);
        params.eggs[i].x--;
    }
    std::sort(params.eggs, params.eggs + params.num_eggs, [](Egg a, Egg b){return a.y < b.y;});
    params.max_floor = params.eggs[params.num_eggs - 1].y;
    params.positions[0] = params.positions[1] = 1;
}

int max_int_in_array(int * arr, int start, int end){
    int max_val = 0;
    for (int i = start; i <= end; i++)
    {
        if (max_val < arr[i]){
            max_val = arr[i];
        }
    }
    return max_val;
}

void fill_level(game_parameters& params, int level_counter){

}

int play_game(game_parameters& params){
    int round = 1;
    int egg_pointer = 0;
    int tmp_egg_pointer = 0;
    int tmp_result = 0;
    int *tmp_scores;
    int level_egg_counter = 0;
    int empty_level_counter = 0;
    if (params.width < params.max_floor){
        for (round = 1; round < params.width - 1; round++)
        {
            while(egg_pointer < params.num_eggs && params.eggs[egg_pointer].y == round){
                params.marked_floor[params.eggs[egg_pointer].x] = 1;
                egg_pointer++;
            }

            for (int j = 1; j <= round; j++)
            {
                // if (!params.positions[j % params.width]){ 
                //     params.positions[j % params.width] = 1;    
                //     break;
                // }
                tmp_result = params.marked_floor[j] + params.marked_floor[j + 1];
                params.scores[j] = std::max({params.prev_scores[j - 1], params.prev_scores[j], params.prev_scores[j + 1]}) + tmp_result;    
            }
            params.scores[0] = std::max(params.prev_scores[0], params.prev_scores[1]) + params.marked_floor[0] + params.marked_floor[1];

            while(tmp_egg_pointer < params.num_eggs && params.eggs[tmp_egg_pointer].y == round){
                params.marked_floor[params.eggs[tmp_egg_pointer].x] = 0;
                tmp_egg_pointer++;
            }    

            tmp_scores = params.scores;
            params.scores = params.prev_scores;
            params.prev_scores = tmp_scores; 
        // for (int i = 0; i < params.width; i++)
        // {
        //     printf("%d ", tmp_scores[i]);
        // }
        // printf("\n");
        }   
    }



    for (int i = round; i <= params.max_floor; i++)
    {

        while(egg_pointer < params.num_eggs && params.eggs[egg_pointer].y == i){
            if (empty_level_counter != 0){
                // fill_level(params, empty_level_counter);
                // int max_val = max_int_in_array(tmp_scores, params.width);


                for (int ind = 0; ind < params.width - 1; ind++){
                    params.scores[ind] = max_int_in_array(tmp_scores,
                    std::max(ind - empty_level_counter, 0),
                    std::min(ind + empty_level_counter, params.width - 2)); 
                }

                        tmp_scores = params.scores;
        params.scores = params.prev_scores;
        params.prev_scores = tmp_scores;                   
                // for (int ind = 0; ind < params.width; ind ++){
                //     params.prev_scores[ind] = max_val; 
                // }
            }
            level_egg_counter++;
            empty_level_counter = 0;
            params.marked_floor[params.eggs[egg_pointer].x] = 1;
            egg_pointer++;
        }
        if (!level_egg_counter){ 
            empty_level_counter++;
                continue;
        }
        // if (empty_level_counter >= params.width / 10){
            // continue;
        // }
        // empty_level_counter++;

        for (int j = 1; j < params.width - 1; j++)
        {
            tmp_result = params.marked_floor[j] + params.marked_floor[j + 1];
            params.scores[j] = std::max({params.prev_scores[j - 1], params.prev_scores[j], params.prev_scores[j + 1]}) + tmp_result;    
        }
        params.scores[0] = std::max(params.prev_scores[0], params.prev_scores[1]) + params.marked_floor[0] + params.marked_floor[1];

        while(tmp_egg_pointer < params.num_eggs && params.eggs[tmp_egg_pointer].y == i){
            level_egg_counter--;   
            params.marked_floor[params.eggs[tmp_egg_pointer].x] = 0;
            tmp_egg_pointer++;
        }    
        
        tmp_scores = params.scores;
        params.scores = params.prev_scores;
        params.prev_scores = tmp_scores;     

        // for (int i = 0; i < params.width; i++)
        // {
        //     printf("%d ", tmp_scores[i]);
        // }
        // printf("\n");   
    }

    int max_val = 0;
    for (int i = 0; i < params.width - 1; i++)
    {
        if (max_val < tmp_scores[i]){
            max_val = tmp_scores[i];
        }
    }

    return max_val;
}

int main(int argc, char * argv[]){
    game_parameters params;
    get_input(params);
    // LOG(params.num_eggs);
    // LOG(params.eggs[0].x);
    // for (int i = 0; i < params.width; i++)
    // {
    //     printf("%d ", params.scores[i]);
    // }
    // printf("\n");
    //     for (int i = 0; i < params.width; i++)
    // {
    //     printf("%d ", params.positions[i]);
    // }
    // for (int i = 0; i < params.num_eggs; i++)
    // {
    //     printf("%d %d\n", params.eggs[i].x, params.eggs[i].y);
    // }
    // printf("\n");
    // Egg egg1(2, 1);
    // delete
    LOG(play_game(params)); 
    return 0;
}