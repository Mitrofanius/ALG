#include <iostream>

enum Color {Red, White};

class Bead
{
public:
    enum Color color;
    int adj[3];
    int markedAdj[3];
    int index;
    int numOfAdjMO;
};

class BeadsChain
{
public:
    int length, numRed;
    int minimalDiff;
    Bead *array; 
    int bestOne;
    int bestTwo;
    
    void allocate()
    {
        array = new Bead[length + 1];
    }

    void deallocate()
    {
        delete[] array;
    }
};

int smallest(int x, int y, int z){
    return std::min(std::min(x, y), z);
}

void getInputAndInit(BeadsChain *beadschain){
    int from, to;
    beadschain->bestOne = beadschain->bestTwo = 2147483647;
    beadschain->minimalDiff = 2147483647;
    std::cin >> beadschain->length >> beadschain->numRed;
    beadschain->allocate();
    for (int i = 1; i <= beadschain->length; i++){
        beadschain->array[i].adj[0] = beadschain->array[i].adj[1] = beadschain->array[i].adj[2] = 0;
        beadschain->array[i].markedAdj[0] = beadschain->array[i].markedAdj[1] = beadschain->array[i].markedAdj[2] = 0;
        beadschain->array[i].index = i;
        if (i <= beadschain->numRed){
            beadschain->array[i].color = Red;
            continue;
        }
        beadschain->array[i].color = White;
    }

    while (std::cin >> from >> to){
        beadschain->array[from].adj[beadschain->array[from].numOfAdjMO++] = to;
        beadschain->array[to].adj[beadschain->array[to].numOfAdjMO++] = from;
    }
}

void estimateTwo(BeadsChain * beadschain, int index, int secondInd, int diff){
    int minAB = std::min(index, secondInd);
    int maxAB = std::max(index, secondInd);
    if (diff < beadschain->minimalDiff){
        beadschain->minimalDiff = diff;
        beadschain->bestOne = minAB;
        beadschain->bestTwo = maxAB;
        return;
    }

    if (((minAB < beadschain->bestOne) || (minAB == beadschain->bestOne && maxAB < beadschain->bestTwo))){
        beadschain->minimalDiff = diff;
        beadschain->bestOne = minAB;
        beadschain->bestTwo = maxAB;
    }
}

int mainDFS(BeadsChain *beadschain, int index, int prevRed, int prevInd){

    int isRed = 0;
    int currRed = prevRed;
    if (beadschain->array[index].color == Red){
        isRed++;
        currRed += isRed;
    }

    int sumToReturn = isRed;

    for (int i = 0; i < beadschain->array[index].numOfAdjMO; i++){
        if (beadschain->array[index].adj[i] != 0) {
            if (beadschain->array[index].adj[i] == prevInd){
                beadschain->array[index].markedAdj[i] = prevRed;
                continue;
            }
            beadschain->array[index].markedAdj[i] = mainDFS(beadschain, beadschain->array[index].adj[i], currRed, index);
            sumToReturn += beadschain->array[index].markedAdj[i];
        }
    }
    return sumToReturn;
}

void secondDFS(BeadsChain *beadschain, int index, int prevRed, int prevInd){
    int isRed = 0;
    int currRed = prevRed;
    if (beadschain->array[index].color == Red){
        isRed++;
        currRed += isRed;
    }

    if (prevInd == 0){
        for (int i = 0; i < beadschain->array[1].numOfAdjMO; i++){
            secondDFS(beadschain, beadschain->array[index].adj[i],
            currRed + beadschain->array[1].markedAdj[(i + 2) % 3] + beadschain->array[1].markedAdj[(i + 1) % 3], index);
            
            int diff = std::abs(beadschain->array[index].markedAdj[i] - (currRed +
                beadschain->array[1].markedAdj[(i + 2) % 3] + beadschain->array[1].markedAdj[(i + 1) % 3]));         
            if (diff <= beadschain->minimalDiff){   
            estimateTwo(beadschain, index, beadschain->array[index].adj[0], diff);
            }
        }
        return;
    }

    if (beadschain->array[index].numOfAdjMO == 1){
        return;
    }

    if (beadschain->array[index].numOfAdjMO == 2){

        if (beadschain->array[index].adj[0] == prevInd){
            beadschain->array[index].markedAdj[0] = prevRed;
            secondDFS(beadschain, beadschain->array[index].adj[1], currRed, index);

            int diff = std::abs(beadschain->array[index].markedAdj[1] - currRed);
            if (diff <= beadschain->minimalDiff){
                estimateTwo(beadschain, index, beadschain->array[index].adj[1], diff);
                }
        }
        else {
            beadschain->array[index].markedAdj[1] = prevRed;
            secondDFS(beadschain, beadschain->array[index].adj[0], currRed, index);
            int diff = std::abs(beadschain->array[index].markedAdj[0] - currRed);
            if (diff <= beadschain->minimalDiff){
                estimateTwo(beadschain, index, beadschain->array[index].adj[0], diff);
                }
        }
        return;
    }


    if (beadschain->array[index].adj[0] == prevInd){
        beadschain->array[index].markedAdj[0] = prevRed;
        secondDFS(beadschain, beadschain->array[index].adj[1], currRed + beadschain->array[index].markedAdj[2], index);
        secondDFS(beadschain, beadschain->array[index].adj[2], currRed + beadschain->array[index].markedAdj[1], index);

    } 
    else if (beadschain->array[index].adj[1] == prevInd){
        beadschain->array[index].markedAdj[1] = prevRed;
        secondDFS(beadschain, beadschain->array[index].adj[2], currRed + beadschain->array[index].markedAdj[0], index);
        secondDFS(beadschain, beadschain->array[index].adj[0], currRed + beadschain->array[index].markedAdj[2], index);
    }    
    else if (beadschain->array[index].adj[2] == prevInd){
        beadschain->array[index].markedAdj[2] = prevRed;
        secondDFS(beadschain, beadschain->array[index].adj[0], currRed + beadschain->array[index].markedAdj[1], index);
        secondDFS(beadschain, beadschain->array[index].adj[1], currRed + beadschain->array[index].markedAdj[0], index);
    }

    int diff = 0;
    if (beadschain->array[index].adj[0] != prevInd && ((diff = std::abs(beadschain->array[index].markedAdj[0]
     - beadschain->array[index].markedAdj[1] - beadschain->array[index].markedAdj[2] - isRed)) <= beadschain->minimalDiff)){    
            estimateTwo(beadschain, index, beadschain->array[index].adj[0], diff);
    }
    if (beadschain->array[index].adj[1] != prevInd && ((diff = std::abs(beadschain->array[index].markedAdj[1]
     - beadschain->array[index].markedAdj[2] - beadschain->array[index].markedAdj[0] - isRed)) <= beadschain->minimalDiff)){    
            estimateTwo(beadschain, index, beadschain->array[index].adj[1], diff);

    }
    if (beadschain->array[index].adj[2] != prevInd && ((diff = std::abs(beadschain->array[index].markedAdj[2]
     - beadschain->array[index].markedAdj[1] - beadschain->array[index].markedAdj[0] - isRed)) <= beadschain->minimalDiff)){    
            estimateTwo(beadschain, index, beadschain->array[index].adj[2], diff);
    }
    return;
}

int main(){
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL); 

    BeadsChain *beadschain = new BeadsChain();
    getInputAndInit(beadschain);

    mainDFS(beadschain, 1, 0, 1);
    secondDFS(beadschain, 1, 0, 0);
    std::cout << beadschain->bestOne << " "<< beadschain->bestTwo << std::endl;
    beadschain->deallocate();
    delete beadschain;
    return 0;
}
