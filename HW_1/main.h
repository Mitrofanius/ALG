static inline void read_input(int ** matrix, int* rows, int* cols);
static inline int find_max_area(int ** matrix, int rows, int cols);
static inline int check_vert_max(int ** matrix, int sum, int row, int start_col, int stop_col, int rows, int cols);
static inline void check_down(int **matrix, int sum, int row, int start_col, int stop_col, int rows, int cols, int *up_len, int *up_sum, int *down_len, int *down_sum);
static inline void check_up(int **matrix, int sum, int row, int start_col, int stop_col, int rows, int cols, int *up_len, int *up_sum, int *down_len, int *down_sum);
