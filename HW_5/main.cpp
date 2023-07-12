#include <iostream>

struct Node
{
    int key;
    Node(int key)
    {
        this->key = key;
        this->left = NULL;
        this->right = NULL;
    }
    Node() = default;
    Node *left;
    Node *right;
};

void print_inorder(Node *node)
{
    if (node == NULL)
        return;
    print_inorder(node->left);

    std::cout << node->key << ' ';

    print_inorder(node->right);
}

Node *insert_bst(int key, Node *node)
{
    if (node == NULL)
        return new Node(key);

    if (key > node->key)
        node->right = insert_bst(key, node->right);
    else if (key < node->key)
        node->left = insert_bst(key, node->left);

    return node;
}

int minvalue(Node *node)
{
    while (node->left != NULL)
        node = node->left;
    return node->key;
}

Node *stupid_interval_insert(int low_key, int high_key, Node *node)
{
    for (int curr_key = low_key; curr_key <= high_key; curr_key++)
    {
        node = insert_bst(curr_key, node);
    }
    return node;
}

void special_dfs(Node *node, int *max_depth, int *num_nodes, int depth)
{
    if (node == NULL)
        return;

    special_dfs(node->left, max_depth, num_nodes, depth + 1);

    if (*max_depth < depth)
    {
        *max_depth = depth;
    }
    (*num_nodes)++;

    special_dfs(node->right, max_depth, num_nodes, depth + 1);
    delete node;
    node = NULL;
}

// if (root->key == 2)
// {
// std::cout << "its 2" << '\n';
// }

// Node *interval_insert(int a, int b, int parent_key, Node *root)
// {
//     if (a > b)
//     {
//         return NULL;
//     }
//     else if (root == NULL)
//     {
//         Node *node = new Node(a);
//         node->right = interval_insert(a + 1, b, node->key, node->right);
//         return node;
//     }
//     else if (b < root->key)
//     {
//         root->left = interval_insert(a, b, root->key, root->left);
//     }

//     else if (a <= root->key && root->key <= b)
//     {
//         root->left = interval_insert(a, root->key - 1, root->key, root->left);
//         // root = interval_insert(root->key, root->key, root->key, root);
//         root->right = interval_insert(root->key + 1, b, root->key, root->right);
//     }
//     else if (b > root->key)
//     {
//         root->right = interval_insert(a, b, root->key, root->right);
//     }
//     else if (b == root->key)
//     {
//         return root;
//     }
//     return root;
// }

Node *interval_insert(int a, int b, Node *root)
{
    if (a > b && root == NULL)
    {
        return NULL;
    }
    else if (root == NULL)
    {
        Node *node = new Node(a);
        node->right = interval_insert(a + 1, b, node->right);
        return node;
    }
    else if (a > root->key)
    {
        root->right = interval_insert(a, b, root->right);
    }

    else if (a <= root->key && root->key <= b)
    {
        root->left = interval_insert(a, root->key - 1, root->left);
        root->right = interval_insert(root->key + 1, b, root->right);
    }
    else if (a < root->key)
    {
        root->left = interval_insert(a, b, root->left);
    }

    return root;
}

Node *interval_delete(int a, int b, Node *root)
{
    if (a > b || root == NULL)
        return root;
    else if (a > root->key)
        root->right = interval_delete(a, b, root->right);
    else if (a < root->key && root->key <= b)
    {
        root->right = interval_delete(root->key + 1, b, root->right);
        root->left = interval_delete(a, root->key - 1, root->left);
        root = interval_delete(root->key, root->key, root);
    }
    else if (a < root->key)
        root->left = interval_delete(a, b, root->left);
    else if (a == root->key)
    {
        if (!root->right && !root->left)
        {
            delete root;
            root = NULL;
            return root;
        }
        else if (!root->right)
        {
            Node *left = root->left;
            delete root;
            root = NULL;
            return left;
        }
        else if (!root->left)
        {
            Node *right = root->right;
            delete root;
            root = NULL;
            return interval_delete(a + 1, b, right);
        }
        else
        {
            root->right = interval_delete(root->key + 1, b, root->right);
            if (!root->right)
                return interval_delete(a, a, root);
            root->key = minvalue(root->right);
            root->right = interval_delete(root->key, root->key, root->right);
        }
    }
    return root;
}
void print_tree(Node *node, int shift)
{
    if (node == NULL)
    {
        return;
    }
    shift++;
    print_tree(node->right, shift);

    for (int i = 0; i < shift; i++)
    {
        std::cout << "  ";
    }
    std::cout << node->key << std::endl;

    print_tree(node->left, shift);
}

void process_input(Node **rootik)
{
    int N = 0, low_key = 0, high_key = 0;
    char cmd;
    int depth = 0, num_nodes = 0;
    Node *root = *rootik;
    std::cin >> N;

    for (int i = 0; i < N; i++)
    {
        std::cin >> cmd >> low_key >> high_key;
        if (cmd == 'i')
            root = interval_insert(low_key, high_key, root);
        else
            root = interval_delete(low_key, high_key, root);
        // std::cout << "root" << ' ' << root->key << std::endl;
        // print_inorder(root);
        // std::cout << std::endl;
    }

    special_dfs(root, &depth, &num_nodes, 0);
    // print_inorder(root);
    // std::cout << std::endl;
    // print_tree(root, 0);
    std::cout << num_nodes << ' ' << depth << std::endl;
}

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    Node *root = NULL;
    process_input(&root);
    return 0;
}