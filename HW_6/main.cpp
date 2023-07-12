#include <cstdio>
#include <iostream>
#include <algorithm>
#include <queue>
#include <set>

#define LOG(a) std::cout << a << '\n';

struct answer_struct{
    unsigned int num_nodes;
    unsigned int num_1;
    unsigned int num_2;
    unsigned int num_3;
    answer_struct(){
        num_nodes = 0;
        num_1 = 0;
        num_2 = 0;
        num_3 = 0;
    }
};

struct Node {
    Node *left, *right, *parent;
    std::set<int> *keys;
    int height;
    
    Node(int key){
        left = right = nullptr;
        keys = new std::set<int>;
        keys->insert(key);
        height = 0;
    }
    Node(){
        left = right = nullptr;
        keys = new std::set<int>;
        height = 0;
    }
    // ~Node(){
    //     delete [] keys;
    // }
};

int height(Node *node)
{
    if (node == nullptr)
        return -1;
    return node->height;
}

void print_tree(Node *node);

struct insert_ret{
    Node * node;
    int h;
};

Node* lx2_rotate(Node * node){
    Node *tmp = node;
    node = node->right;
    tmp->right = node->left;
    node->left = tmp;
    tmp->height = std::max(height(tmp->left), height(tmp->right)) + 1;
    node->height = std::max(height(node->left), height(node->right)) + 1;
    node->parent = tmp->parent;
    tmp->parent = node;
    if (tmp->right){
        tmp->right->parent = tmp;
    }
    return node;
}

Node* rl_rotate(Node * node){
    Node *tmp = node;
    node = node->right->left;
    tmp->right->left = node->right;
    node->right = tmp->right;
    tmp->right = node->left;
    node->left = tmp;

    node->right->height = std::max(height(node->right->left), height(node->right->right)) + 1;
    node->left->height = std::max(height(node->left->right), height(node->left->left)) + 1;
    node->height = std::max(height(node->left), height(node->right)) + 1;
    return node;
}

Node* rx2_rotate(Node * node){
    Node *tmp = node;
    node = node->left;
    tmp->left = node->right;
    node->right = tmp;
    tmp->height = std::max(height(tmp->left), height(tmp->right)) + 1;
    node->height = std::max(height(node->left), height(node->right)) + 1;
    node->parent = tmp->parent;
    tmp->parent = node;
    if (tmp->left){
        tmp->left->parent = tmp;
    }
    return node;
}

Node* lr_rotate(Node * node){
    Node *tmp = node;
    node = node->left->right;
    tmp->left->right = node->left;
    node->left = tmp->left;
    tmp->left = node->right;
    node->right = tmp;
    node->right->height = std::max(height(node->right->left), height(node->right->right)) + 1;
    node->left->height = std::max(height(node->left->right), height(node->left->left)) + 1;
    node->height = std::max(height(node->left), height(node->right)) + 1;
    return node;
}


Node *find_min(Node *node){
    if (node->left) return find_min(node->left);
    return node;
}

Node *find_max(Node *node){
    if (node->right) return find_max(node->right);
    return node;
}

int get_balance(Node *node)
{
    if (node == nullptr)
        return 0;
    return height(node->left) -
           height(node->right);
}

class DualAVL{
    public:
    Node *root;
    DualAVL(){
        root = nullptr;
    }

    Node* naive_insert(int key, Node *node, Node* parent){
        if (node != nullptr){
            node->parent = parent;
            if (key == 22) {
                // LOG(parent << " fucking here");
                // LOG(node << " fucking node");
            }
        }
        if (node == nullptr){
            node = new Node(key);
            node->parent = parent;
            node->height = 0;
            return node;
        }
        else if ((node->left || node->right)){
            if (node->keys->find(key) != node->keys->end()) return node;
            if (key < *(node->keys->begin())){
                node->left = naive_insert(key, node->left, node);
                node->height = std::max(height(node->left), height(node->right)) + 1;

                // if (node->right) node->height = std::max(node->left->height, node->right->height) + 1;
                // else node->height = node->left->height + 1;
                // return node;
            }
            else if (key > *(--node->keys->end())){
                node->right = naive_insert(key, node->right, node);
                node->height = std::max(height(node->left), height(node->right)) + 1;
                // if (node->left) node->height = std::max(node->left->height, node->right->height) + 1;
                // else node->height = node->right->height + 1;
                // return node;
            }
            else{
                node->keys->insert(key);
                if (node->keys->size() == 2){return node;}
                if (node->left){
                    if (node->right){
                        if(node->left->height <= node->right->height){
                            node->left = naive_insert(*(node->keys->begin()), node->left, node);
                            node->keys->erase(*(node->keys->begin()));
                        }
                        else{
                            node->right = naive_insert(*(--node->keys->end()), node->right, node);
                            node->keys->erase(*(--node->keys->end()));
                        }
                    }
                    else{
                        node->right = naive_insert(*(--node->keys->end()), node->right, node);
                        node->keys->erase(*(--node->keys->end()));
                    }
                }
                else {
                    node->left = naive_insert(*(node->keys->begin()), node->left, node);
                    node->keys->erase(*(node->keys->begin()));
                }
                node->height = std::max(height(node->left), height(node->right)) + 1;
                // node->height = std::max(node->left->height, node->right->height) + 1;
                // return node;
            }
            // else {
            //     node->left = naive_insert(*(node->keys->begin()), node->left);
            //     node->keys->erase(*(node->keys->begin()));
            // }
            // node->height = std::max(node->left->height, node->right->height) + 1;
            // return node;
        }
        else if (node->keys->size() < 3){
            node->keys->insert(key);
            node->height = 0;
            return node;
        }
        else {
            node->keys->insert(key);
            if (node->keys->size() > 3){    
                node->left = naive_insert(*(node->keys->begin()), node->left, node);
                node->keys->erase(*(node->keys->begin()));
                node->right = naive_insert(*(--node->keys->end()), node->right, node);
                node->keys->erase(*(--node->keys->end()));
                // node->height = std::max(node->left->height, node->right->height) + 1;
            }
            node->height = std::max(height(node->left), height(node->right)) + 1;
        }

        int balance = height(node->left) - height(node->right);

        if (balance < -1){
            if (height(node->right->right) >= height(node->right->left)){
                node = lx2_rotate(node);
            }
            else{
                // node = rl_rotate(node);
                // or 
                node->right = rx2_rotate(node->right);
                node = lx2_rotate(node);
            }
        }
        else if (balance > 1){
            if (height(node->left->left) >= height(node->left->right)){
                node = rx2_rotate(node);
            }
            else{
                // node = lr_rotate(node);
                // or
                node->left = lx2_rotate(node->left);
                node = rx2_rotate(node);
            }
        }
        node->height = std::max(height(node->left), height(node->right)) + 1;

        return node;
    }

    Node* naive_delete(int key, Node * node, Node *parent){
        if (node != nullptr){
            node->parent = parent;
        }
        else if (node == nullptr){
            return node;
        }
        if (node->left || node->right){
            if (key < *(node->keys->begin())){
                node->left = naive_delete(key, node->left, node);
                node->height = std::max(height(node->left), height(node->right)) + 1;
                // return node;
            }
            else if (key > *(--node->keys->end())){
                node->right = naive_delete(key, node->right, node);
                node->height = std::max(height(node->left), height(node->right)) + 1;
                // if (node->left) node->height = std::max(node->left->height, node->right->height) + 1;
                // else node->height = node->right->height + 1;
                // return node;
            }
            else if (node->keys->find(key) != node->keys->end()) {
                Node *tmp;
                int key_to_insert;
                // it works, ok, but...
                if (height(node->left) <= height(node->right)) {
                    // LOG("KEY NOW IS " << key);
                    // LOG("HL NOW IS " << height(node->left));
                    // LOG("HR NOW IS " << height(node->right));
                    tmp = find_min(node->right);
                    key_to_insert = *(tmp->keys->begin());
                    node->keys->erase(key);
                    node->keys->insert(key_to_insert);
                    node->right = naive_delete(key_to_insert, node->right, node);
                }
                else {
                    // LOG("KEY NOW IN ELSE " << key);
                    tmp = find_max(node->left);
                    key_to_insert = *(--tmp->keys->end());
                                    // node->keys->erase(key);
                    node->keys->erase(key);
                    node->keys->insert(key_to_insert);
                    node->left = naive_delete(key_to_insert, node->left, node);
                }
                node->height = std::max(height(node->left), height(node->right)) + 1;
                // LOG("NOOOO");
                // node->keys->erase(key);
                // node->keys->insert(key_to_insert);
                // tmp = naive_delete(key_to_insert, tmp, tmp->parent);
            }
            else{
                return node;
            }
        }
        else if(node->keys->find(key) == node->keys->end()) return node;
        else if (node->keys->size() > 1 || parent == nullptr){
                // LOG(node->keys->size());
                // LOG(*node->keys->begin());
                // LOG(parent);
                // here may be a problem later
            node->keys->erase(key);
            return node;
        }
        else if (parent->left == node){
            if (parent->right->right || parent->right->left){
                int z_key = *(parent->keys->begin());
                Node* tmp;
                node->keys->erase(key);
                node->keys->insert(z_key);
                node->parent = naive_delete(z_key, parent, parent->parent); 
            }
            else if (parent->right->keys->size() == 1){
                parent->keys->insert(*(parent->right->keys->begin()));
                delete parent->right;
                delete parent->left;
                parent->right = nullptr;
                return nullptr;
            }
            else {
                node->keys->erase(key);
                node->keys->insert(*(parent->keys->begin()));
                parent->keys->erase(*(parent->keys->begin()));
                parent->keys->insert(*(parent->right->keys->begin()));
                parent->right->keys->erase(*(parent->right->keys->begin()));
                // return node;
            }
            node->height = std::max(height(node->left), height(node->right)) + 1;
            return node;
        }
        else if (parent->right == node){
            if (parent->left->right || parent->left->left){
                int z_key = *(--parent->keys->end());
                node->keys->erase(key);
                node->keys->insert(z_key);
                node->parent = naive_delete(z_key, parent, parent->parent); 
            }
            else if (parent->left->keys->size() == 1){
                parent->keys->insert(*(parent->left->keys->begin()));
                delete parent->right;
                delete parent->left;
                parent->left = nullptr;
                return nullptr;
            }
            else {
                node->keys->erase(key);
                node->keys->insert(*(--parent->keys->end()));
                parent->keys->erase(*(--parent->keys->end()));
                parent->keys->insert(*(--parent->left->keys->end()));
                parent->left->keys->erase(*(--parent->left->keys->end()));
                // return node;
            }
            node->height = std::max(height(node->left), height(node->right)) + 1;
            return node;
        }
        else if (parent == nullptr){
            node->keys->erase(key);
            if (node->keys->size() == 0){
                delete node;
                node = nullptr;
            }
            return node;
        }
        node->height = std::max(height(node->left), height(node->right)) + 1;
        
        // now rotations, motherfucker
        int balance = height(node->left) - height(node->right);
        // LOG("HEIGHT LEFT IS " << height(node->left));
        // LOG("HEIGHT RIGHT IS " << height(node->right));
        
        if (balance > 1){
            if (get_balance(node->left) >= 0){
                node = rx2_rotate(node);
            }
            else{
                // node = lr_rotate(node);
                // or
                node->left = lx2_rotate(node->left);
                node = rx2_rotate(node);
            } 
        }
        else if (balance < -1){
            if (get_balance(node->right) <= 0){
                node = lx2_rotate(node);
            }
            else{
                // node = rl_rotate(node);
                // or
                node->right = rx2_rotate(node->right);
                node = lx2_rotate(node);
            }
        }
        // if (node->keys->size() == 0){
            // delete node;
        //     node = nullptr;
        // }
        node->height = std::max(height(node->left), height(node->right)) + 1;

        return node;
    }
};

void print_inorder(Node *node, int h)
{
    if (node == nullptr)
        return;
    print_inorder(node->left, h + 1);

    for (int i = 0; i < h; i++)
    {
        printf("------- ");
    }
    
 
        printf("(");
        for (int key : *(node->keys))
        {
            printf("%d ", key);
        }
        printf("[%d]",node->height);
        printf(")");
    LOG("\n\n");
 
    print_inorder(node->right, h + 1);
}

void print_inorder_parent(Node *node)
{
    if (node == nullptr)
        return;
    print_inorder_parent(node->left);

    LOG("fucking parent " << node->parent);
 
    print_inorder_parent(node->right);
}

void print_tree(Node *node){
    std::queue<Node*> q;
    Node *curr;
    q.push(node);
    int cnt = 2;

    while(!(q.empty())){
        curr = q.front();
        q.pop();
        if (curr->left){
            q.push(curr->left);
        }
        if (curr->right){
            q.push(curr->right);
        }
        printf("(");
        for (int key : *(curr->keys))
        {
            printf("%d ", key);
        }
        printf("[%d]",curr->height);
        printf(")");

    }
    LOG('\n');

}



void process_input(DualAVL& tree){
    int num_ops, start, step, end;
    char op;
    scanf("%d\n", &num_ops);
    for (int i = 0; i < num_ops; i++){
        scanf("%c %d %d %d\n", &op, &start, &step, &end);
        // LOG(op);
        if (op == 'I'){
            for (int key = start; key <= end; key += step)
            {
                // LOG(key);
                tree.root = tree.naive_insert(key, tree.root, nullptr);
                // print_tree(tree.root);
                // print_inorder(tree.root);
                // LOG('\n');
            }
        }
        else if (op == 'D'){
            for (int key = start; key <= end; key += step)
            {
                // LOG("DELETE " << key - step);
                // if (key == 58) break;   

                tree.root = tree.naive_delete(key, tree.root, nullptr);
                // print_tree(tree.root);
                // print_inorder(tree.root);
                // LOG('\n');
            }        
        }
    }
}

void find_answer(answer_struct& a, Node *root){
    if (root == nullptr){
        return;
    }
    find_answer(a, root->left);
    a.num_nodes++;
    if (root->right == nullptr && root->left == nullptr){
        int x = root->keys->size();

        switch (x)
        {
        case 1:
            a.num_1++;
            break;
        case 2:
            a.num_2++;
            break;
        case 3:
            a.num_3++;
            break;
        default:
            break;
        }
    }
    find_answer(a, root->right);
    delete root;    
}

void printSet(std::set<int> my_set)
{
    // Print the set
    std::cout << "Set: ";
    for (auto i : my_set)
        std::cout << i << " ";
  
    std::cout << '\n';
}

int main(int argc, char* argv[]){
    // std::set<int> my_set;
    // my_set.insert(1);
    // my_set.insert(6);
    // my_set.insert(15);
    // my_set.insert(10);
    // my_set.insert(10);
    // my_set.insert(11);
    // my_set.insert(5);
    // printSet(my_set);
    // // LOG(*std::next(my_set.begin()));
    // // LOG(*my_set.end());
    // // LOG(*my_set.begin());
    // LOG(my_set.erase(*my_set.begin()));
    // LOG(my_set.erase(15));
    // printSet(my_set);

    DualAVL tree;
    answer_struct ans;
    // tree.root = new Node();

    process_input(tree);
    find_answer(ans, tree.root);
    // print_tree(tree.root);
    // print_inorder(tree.root, 0);
    // print_inorder_parent(tree.root);
    // LOG(*(find_min(tree.root)->keys->begin()));
    // LOG(*(--find_max(tree.root->left)->keys->end()));
    // LOG('\n');
    printf("%d %d %d %d\n", ans.num_nodes, ans.num_1, ans.num_2, ans.num_3);

    return 0;
}