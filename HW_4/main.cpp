#include <iostream>
#include <vector>
#include <queue>
#include <climits> 

class Village
{
public:
    int index;
    bool associate;
    std::vector <int> adj;
    int curr_food_supply;
    int num_used;
};

class Desert
{
public:
    int num_villages, num_roads, num_associate, nutrition;
    int num_in;
    Village *villages;

    void allocate()
    {
        villages = new Village[num_villages + 1];
    }

    void deallocate()
    {
        delete[] villages;
        villages = NULL;
    }
};

void travel_the_desert(Desert *desert)
{
    int spend_supply = 0, nutrition = 0;
    std::queue <Village *> village_queue;
    Village *curr, *current_village;
    desert->villages[1].num_used = 0;
    desert->villages[1].curr_food_supply = desert->nutrition;
    village_queue.push(&(desert->villages[1]));
    while (!village_queue.empty())
    {
        current_village = village_queue.front();
        village_queue.pop();
        spend_supply = current_village->curr_food_supply == 0 ? 1 : 0;
        for (int i = 0; i < current_village->adj.size(); i++)
        {
            curr = &desert->villages[current_village->adj[i]];
            if (curr->associate || spend_supply == 1)
            {
                nutrition = desert->nutrition;   
            }
            else {
                nutrition = current_village->curr_food_supply - 1;
            }

            if (
                (spend_supply + current_village->num_used < curr->num_used) ||
                (spend_supply + current_village->num_used == curr->num_used && nutrition > curr->curr_food_supply))
            {
                curr->num_used = spend_supply + current_village->num_used;
                curr->curr_food_supply = nutrition;
                village_queue.push(curr);   
            }
        }
    }
}

void get_input(Desert *desert){
    int from, to;
    desert->num_in = 0;
    std::cin >> desert->num_villages >> desert->num_roads >>
                desert->num_associate >> desert->nutrition;
    int i = 1;
    desert->allocate();

    for (i = 1; i <= desert->num_associate; i++)
    {
        desert->villages[i].index = i;
        desert->villages[i].associate = true;
        desert->villages[i].num_used = INT32_MAX;
        desert->villages[i].curr_food_supply = 0;        
    }
    
    for (i = desert->num_associate + 1; i <= desert->num_villages; i++)
    {
        desert->villages[i].index = i;
        desert->villages[i].associate = false;
        desert->villages[i].num_used = INT32_MAX;
        desert->villages[i].curr_food_supply = 0;  
    }

    for (i = 0; i <= desert->num_roads; i++)
    {
        std::cin >> from >> to;
        desert->villages[from].adj.push_back(to);
        desert->villages[to].adj.push_back(from);
    }
}

int main(int argc, char* argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL); 

    Desert *desert = new Desert();
    get_input(desert);
    
    travel_the_desert(desert);

    int my_max = 0;
    for (int i = 1; i <= desert->num_villages; i++)
    {
        if (desert->villages[i].num_used > my_max){
            my_max = desert->villages[i].num_used;
        }
        if (desert->villages[i].num_used == 0){
            desert->num_in++;
        }
    }

    std::cout << my_max << " " << desert->num_in << std::endl;

    desert->deallocate();
    delete desert;
    desert = NULL;

    return 0;
}