#include <stdio.h>
#include <iostream> 
#include <vector>
#include <sstream>
#include <string>

using namespace std;

int main() {
    int arr[1001] = {};
    int temp = 0;

    string S;
    getline(std::cin, S);
    istringstream T(S);

    while(T >> temp){
        arr[temp] += 1;}

    for(int i=0; i<1001; i++){
        if (arr[i] > 1){
            printf(" %d", i);}
    }
    printf("\n");

    return 0;
}