#include <stdio.h>
#include "main.h"
#include <stdlib.h>

int main () {
    int rows = 0;
    int cols = 0;
    int * matrix = NULL;
    read_input(&matrix, &rows, &cols);

    rows = find_max_area(&matrix, rows, cols);

    free(matrix);
    matrix = NULL;

    printf("%d\n", rows);

    return 0;
}


static inline void read_input(int** matrix, int* rows, int* cols) {
    int i = 0;
    scanf("%d ", rows);
    scanf("%d \n", cols);

    *matrix = (int *)malloc(*rows * *cols * sizeof(int));


    while (i < *rows * (*cols)){
        switch (getc(stdin))
        {
        case '.':
            (*matrix)[i] = 0;
            i++;
            break;
        case 'o':
            (*matrix)[i] = 1;
            i++;
            break;
        case 'X':
            (*matrix)[i] = 2;
            i++;
            break;
        default:
            break;
            }
    }
}


static inline int find_max_area(int ** matrix, int rows, int cols){
    int len_of_bar = 0;
    int sum = 0;
    int start_col = 0;
    int stop_col = 0;
    int max_area = 0;
    int area = 0;

    for (int row = 1; row < rows - 1; row++){
        for (start_col = 0; start_col < cols; start_col++){
            sum = (*matrix)[row * cols + start_col];

            len_of_bar = 1;
            while (start_col + len_of_bar < cols){
                sum += (*matrix)[row * cols + start_col + len_of_bar];
                if (sum > 1){
                    break;
                }
                if (len_of_bar > 1){
                    stop_col = start_col + len_of_bar;
                    area = len_of_bar + 1 + check_vert_max(matrix, sum, row, start_col, stop_col, rows, cols) * 2;
                    if (area > max_area){
                        max_area = area;
                    }                
                }
                len_of_bar++;
            }
        }
    }
    return max_area;
}


static inline int check_vert_max(int ** matrix, int sum, int row, int start_col, int stop_col, int rows, int cols){
    int up_len = 0;
    int down_len = 0; 
    int up_sum = 0;
    int down_sum = 0;

    check_up(matrix, sum, row, start_col, stop_col, rows, cols, &up_len, &up_sum, &down_len, &down_sum);
    if (up_len == 0){
        return 0;
    }
    check_down(matrix, sum, row, start_col, stop_col, rows, cols, &up_len, &up_sum, &down_len, &down_sum);
    if (down_len == 0){
        return 0;
    }


    if (up_sum + down_sum - sum < 2){
        return (up_len + down_len);
        }
    
    sum = up_len;
    int old_down_len = down_len;
    int old_up_sum = up_sum;

    check_up(matrix, down_sum, row, start_col, stop_col, rows, cols, &up_len, &up_sum, &down_len, &down_sum);
    check_down(matrix, old_up_sum, row, start_col, stop_col, rows, cols, &up_len, &up_sum, &down_len, &down_sum);
    
    if ((up_len + old_down_len) > (sum + down_len)){
        return (up_len + old_down_len);
    }

    return (sum + down_len);
}

static inline void check_down(int **matrix, int sum, int row, int start_col, int stop_col, int rows, int cols, int *up_len, int *up_sum, int *down_len, int *down_sum){
    int start_row = row;
    int x = 0;
    while (row + 1 < rows){
        x = (*matrix)[(row + 1) * cols + start_col] + (*matrix)[(row + 1) * cols + stop_col];
        sum += x;
        if (sum > 1){
            *down_len = row - start_row;
            *down_sum = sum - x;
            return;
        }
        row++;
    }
        *down_len = row - start_row;
        *down_sum = sum;
}

static inline void check_up(int **matrix, int sum, int row, int start_col, int stop_col, int rows, int cols, int *up_len, int *up_sum, int *down_len, int *down_sum){
    int start_row = row;
    int x = 0;
    while (row - 1 >= 0){
        x = (*matrix)[(row - 1) * cols + start_col] + (*matrix)[(row - 1) * cols + stop_col];
        sum += x;
        if (sum > 1){
            *up_len = start_row - row;
            *up_sum = sum - x;
            return;
        }
    row--;
    }
    *up_len = start_row - row;

    *up_sum = sum;
}