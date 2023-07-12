#include <iostream>
#include <vector>

class Matrix
{
public:
    int cols;
    int rows;
    int length;
    int occupied_square;
    int cum_square;
    int curProfit = 0;
    int maxProfit = 0;
    int cum_sum = 0;
    char *values;

    void allocate()
    {
        length = rows * cols;
        values = new char[length];
    }

    void eraseMatrix()
    {
        delete[] values;
    }

};

class Tile
{
    public:
        int width;
        int height;
        int square;
        bool active;
        char type;

};

class Coords
{
    public:
    int row_start;
    int row_end;
    int col_start;
    int col_end;
    int sum;
};

int count_reachable(Matrix *matrix, char** marked_space){
    int sum = 0;
    for (int i = 0; i < matrix->rows; i++){
        for (int j = 0; j < matrix->cols; j++){
            if ((*marked_space)[i * matrix->cols + j] == 0){
                sum += matrix->values[i * matrix->cols + j];
            }
        }
    }
    return sum;
}

int first_max(Matrix *matrix, char** marked_space){
    for (int i = 0; i < matrix->rows; i++){
        for (int j = 0; j < matrix->cols; j++){
            if ((*marked_space)[i * matrix->cols + j] == 0){
                return matrix->values[i * matrix->cols + j];
            }
        }
    }
    return 0;
}

void getInput(Matrix *matrix, std::vector<Tile*> *tiles, char** marked_space)
{
    int amoutOfDifferentTileShape, height, width, amoutOfCurrentTileShape;
    int tmp;
    //matrix
    std::cin >> matrix->rows >> matrix->cols;
    matrix->allocate();
    *marked_space = new char [matrix->rows * matrix->cols];

    for (int i = 0; i < matrix->length; i++)
    {
        std::cin >> tmp;
        matrix->values[i] = tmp;
        matrix->cum_sum += tmp;
    }
    matrix->occupied_square = 0;

    //tiles
    std::cin >> amoutOfDifferentTileShape;

    for (int i = 0; i < amoutOfDifferentTileShape; i++)
    {
        std::cin >> height >> width >> amoutOfCurrentTileShape;
        for (int j = 0; j < amoutOfCurrentTileShape; j++)
        {
            Tile *tile = new Tile();
            tile->height = height;
            tile->width = width;
            tile->active = true;
            tile->type = i + 20;
            tile->square = height * width;
            tiles->push_back(tile);
            matrix->cum_square += tile->square;
        }

    }

}

bool in_matrix(Matrix * matrix, int row, int col){
    if (row >= 0 && row < matrix->rows && col >= 0 && col < matrix->cols){
        return true;
    } 
    return false;
}

bool check_surroundings(Matrix *matrix, char **marked_space, Tile * tile, int row, int col){
    for (int i = row - 1; i <= row + tile->height; i++){
        if (i >= 0 && i < matrix->rows && col - 1 >= 0){
            if ((*marked_space)[i * matrix->cols + col - 1] > 8 ){return false;}
        }            
    }

    for (int i = row - 1; i <= row + tile->height; i++){
        if (i >= 0 && i < matrix->rows && col + tile->width < matrix->cols){
            if ((*marked_space)[i * matrix->cols + col + tile->width] > 8){return false;}
        }            
    }

    for (int i = col - 1; i <= col + tile->width; i++){
        if (row - 1 >= 0 && i >= 0 && i < matrix->cols){
            if ((*marked_space)[(row - 1) * matrix->cols + i] > 8){return false;}
        }            
    }

    for (int i = col - 1; i <= col + tile->width; i++){
        if (row + tile->height < matrix->rows && i >= 0 && i < matrix->cols){
            if ((*marked_space)[(row + tile->height) * matrix->cols + i] > 8){return false;}
        }            
    }
    return true;
}

Coords find_next_hor(Matrix *matrix, Tile *tile, char** marked_space, int sum){
    Coords coords;
    int i = 0;
    int j = 0;
    for (int row = 0; row < matrix->rows; row++){
        for (int col = 0; col < matrix->cols; col++){
            if ((*marked_space)[row * matrix->cols + col] == 0 && tile->width + col <= matrix->cols && row + tile->height <= matrix->rows){
                if (!check_surroundings(matrix, marked_space, tile, row, col)){continue;}
                coords.row_start = row;
                coords.row_end = row + tile->height - 1;
                coords.col_start = col;
                coords.col_end = col + tile->width - 1;
                for (i = row; i < row + tile->height; i++){
                    for (j = col; j < col + tile->width; j++){
                        sum += (matrix->values)[i * matrix->cols + j];
                    }
                }
                coords.sum = sum;
                return coords;  
            }
            
        }  
    }

    coords.sum = sum;
    return coords;        
}

Coords find_next_ver(Matrix *matrix, Tile *tile, char** marked_space, int sum){
    Coords coords;
    int i = 0;
    int j = 0;
    for (int col = 0; col < matrix->cols; col++){
        for (int row = 0; row < matrix->rows; row++){
            if ((*marked_space)[row * matrix->cols + col] == 0 && tile->width + col <= matrix->cols && row + tile->height <= matrix->rows){
                if (!check_surroundings(matrix, marked_space, tile, row, col)){continue;}
                coords.row_start = row;
                coords.row_end = row + tile->height - 1;
                coords.col_start = col;
                coords.col_end = col + tile->width - 1;
                for (i = row; i < row + tile->height; i++){
                    for (j = col; j < col + tile->width; j++){
                        sum += (matrix->values)[i * matrix->cols + j];
                    }
                }
                coords.sum = sum;
                return coords;  
            }
            
        }  
    }

    coords.sum = sum;
    return coords;        
}

void mark_best_sollution(Matrix *matrix, Tile *tile, char** marked_space, Coords *coords){
    for (int row = coords->row_start; row <= coords->row_end; row++){
        for (int col = coords->col_start; col <= coords->col_end; col++){
            (*marked_space)[row * matrix->cols + col] = 9;
        }
    }

    int row = coords->row_start;
    int col = coords->col_start;
    for (int i = row - 1; i <= row + tile->height; i++){
        if (i >= 0 && i < matrix->rows && col - 1 >= 0){
            (*marked_space)[i * matrix->cols + col - 1] += 1;
        }
    }

    for (int i = row - 1; i <= row + tile->height; i++){
        if (i >= 0 && i < matrix->rows && col + tile->width < matrix->cols){
            (*marked_space)[i * matrix->cols + col + tile->width] += 1;}  
    }

    for (int i = col; i < col + tile->width; i++){
        if (row - 1 >= 0 && i >= 0 && i < matrix->cols){
            (*marked_space)[(row - 1) * matrix->cols + i] += 1;
        }            
    }

    for (int i = col; i < col + tile->width; i++){
        if (row + tile->height < matrix->rows && i >= 0 && i < matrix->cols){
            (*marked_space)[(row + tile->height) * matrix->cols + i] += 1;
        }            
    }

}

void unmark_coords(Matrix *matrix, char** marked_space, Coords *coords, Tile *tile){

    for (int row = coords->row_start; row <= coords->row_end; row++){
        for (int col = coords->col_start; col <= coords->col_end; col++){
            (*marked_space)[row * matrix->cols + col] = 0;
        }
    }

    int row = coords->row_start;
    int col = coords->col_start;
    for (int i = row - 1; i <= row + tile->height; i++){
        if (i >= 0 && i < matrix->rows && col - 1 >= 0){
            (*marked_space)[i * matrix->cols + col - 1] -= 1;
        }
    }

    for (int i = row - 1; i <= row + tile->height; i++){
        if (i >= 0 && i < matrix->rows && col + tile->width < matrix->cols){
            (*marked_space)[i * matrix->cols + col + tile->width] -= 1;}  
    }

    for (int i = col; i < col + tile->width; i++){
        if (row - 1 >= 0 && i >= 0 && i < matrix->cols){
            (*marked_space)[(row - 1) * matrix->cols + i] -= 1;
        }            
    }

    for (int i = col; i < col + tile->width; i++){
        if (row + tile->height < matrix->rows && i >= 0 && i < matrix->cols){
            (*marked_space)[(row + tile->height) * matrix->cols + i] -= 1;
        }            
    }
}

void solve_it(Matrix *matrix, std::vector<Tile*> *tiles, char** marked_space, int sum, int *p_to_best_sum, int level){  
    if (*p_to_best_sum >= sum + count_reachable(matrix, marked_space)){
        return;
    } 
    Coords coords_ver, coords_hor, *best_coords;  

    char prev_tile_type = 19;
    for (Tile * tile: (*tiles)){

        int old_sum = sum;


        if (tile->active && tile->type != prev_tile_type){

            tile->active = false;
            prev_tile_type = tile->type;

            coords_hor = find_next_hor(matrix, tile, marked_space, sum);
            coords_ver = find_next_ver(matrix, tile, marked_space, sum);

            if (coords_hor.sum >= coords_ver.sum){
                best_coords = &coords_hor;
                sum = coords_hor.sum;
                if (sum > old_sum){
                    if (sum >= *p_to_best_sum){
                        *p_to_best_sum = sum;
                    }
                    mark_best_sollution(matrix, tile, marked_space, best_coords);
                    solve_it(matrix, tiles, marked_space, sum, p_to_best_sum, level + 1);
                    unmark_coords(matrix, marked_space, best_coords, tile);
                    sum = old_sum;
            
                }
            }

            if (coords_hor.sum < coords_ver.sum){
                best_coords = &coords_ver;
                sum = coords_ver.sum;
                if (sum > old_sum){
                    if (sum >= *p_to_best_sum){
                        *p_to_best_sum = sum;
                    }
                    mark_best_sollution(matrix, tile, marked_space, best_coords);
                    solve_it(matrix, tiles, marked_space, sum, p_to_best_sum, level + 1);
                    unmark_coords(matrix, marked_space, best_coords, tile);
                    sum = old_sum;
                
                    }   
                }
            tile->active = true;
        }
    }
}



int main(int argc, char *argv[]){
    int sum, p_to_best_sum;
    sum = p_to_best_sum = 0;
    std::vector<Tile *> *tiles = new std::vector<Tile *>();
    Matrix *matrix = new Matrix();
    char * marked_space;

    getInput(matrix, tiles, &marked_space);

    solve_it(matrix, tiles, &marked_space, sum, &p_to_best_sum, 0);
    std::cout << p_to_best_sum << std::endl;

    //deletion
    matrix->eraseMatrix();
    delete matrix;
    delete[] marked_space;

    return 0;
}