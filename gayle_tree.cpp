#include <iostream>
#include <utility> //pair
#include <cstdlib>
#include <vector>
#include <cmath>
#include <queue>
#include <list>

using namespace std;

class BinaryTree
{
public:
    int data;
    BinaryTree* left;
    BinaryTree* right;
    BinaryTree* parent;
    int size;
    BinaryTree(): left(NULL), right(NULL), parent(NULL), size(0) {}
    BinaryTree(int val): data(val), left(NULL), right(NULL), parent(NULL), size(1) {}
    BinaryTree(int val, BinaryTree* parent): data(val), left(NULL), right(NULL), parent(parent), size(1) {}
    bool isEnd()
    {
        return (left == NULL || right == NULL);
    }
    void push(int val)
    {
        ++size;
        if (size == 1) {
            this->data = val;
        } else {
            if (val <= data) {
                if (this->left == NULL) {
                    this->left = new BinaryTree(val, this);
                } else {  
                    this->left->push(val);
                }
            } else {
                if (this->right == NULL) {
                    this->right = new BinaryTree(val, this);
                } else {
                    this->right->push(val);
                }
            }
        }
    }
    BinaryTree* left_branch()
    {
        return this->left;
    }
    BinaryTree* right_branch()
    {
        return this->right;
    }
};

// puts into the pair "depth" the min and the max depths of leaves
// traversing the tree with DFS
void DFS(BinaryTree &tree, int d, pair<int, int> &depth)
{
    if (tree.left == NULL) {
        depth.first = std::min(depth.first, d);
        depth.second = std::max(depth.second, d);
    } else {
        DFS(*tree.left, d + 1, depth);
    }
    if (tree.right == NULL) {
        depth.first = std::min(depth.first, d);
        depth.second = std::max(depth.second, d);
    } else {
        DFS(*tree.right, d + 1, depth);
    }
}

//prints out the tree in level-order using BFS traversal
void BFS(BinaryTree *t)
{
    if (t->size == 0) return;
    
    list<int> row;
    list<list<int> > rows;

    queue<BinaryTree*> frontier;
    frontier.push(t);
    int nCur = 1;
    int nNext = nCur * 2;
    while (frontier.size() > 0) {
        BinaryTree *tree = frontier.front();
        frontier.pop();
        row.push_back(tree->data);
        --nCur;
        if (tree->left != NULL) { frontier.push(tree->left); }
        else { --nNext; }
        if (tree->right != NULL) { frontier.push(tree->right); }
        else { --nNext; }
        if (nCur == 0) {
            nCur = nNext;
            nNext = nCur * 2;
            rows.push_back(row);
            row.clear();
        }
    }

    for (auto row: rows) {
        for (auto elem: row) cout << elem << " ";
        cout << endl;
    }
}

bool isBalanced(BinaryTree &tree)
{
    pair<int, int> depths = make_pair(tree.size, 0); //min depth, max depth 
    DFS(tree, 1, depths);
    return std::abs(depths.first - depths.second) <= 1 ;
}

bool isBST(BinaryTree *tree) // uses DFS traversal
{
    if (tree->size == 0) return true;
    if (tree->left != NULL) {
        if (tree->data < tree->left->data) return false;
        if(!isBST(tree->left)) return false;    
    }
    if (tree->right != NULL) {
        if (tree->data >= tree->right->data) return false;
        if(!isBST(tree->right)) return false;    
    }
    return true;
}

void addToBST(vector<int> &v, BinaryTree *tree)
{
    if (v.size() > 0) {
        tree->push(v[v.size()/2]);
        vector<int> left(v.begin(), v.begin() + (v.size()/2)); 
        vector<int> right(v.begin() + (v.size()/2) + 1, v.end()); 
        addToBST(left, tree);
        addToBST(right, tree);
    }
}

// finds the successor of the node (nodes are themselves the BinaryTrees)
BinaryTree* successor(BinaryTree *tree)
{
    if (tree->right != NULL) {
        tree = tree->right;
        while (tree->left != NULL) {
            tree = tree->left;
        }
        return tree;
    } else if (tree->parent != NULL && tree->parent->left == tree) { //it is the left child itself
            return tree->parent;
    }
    return NULL;
}

// finds the closest common ancestor of two nodes in a binary tree
BinaryTree* commonAncestor(BinaryTree *node1, BinaryTree *node2)
{
    int depth1 = 0, depth2 = 0;
    BinaryTree *aux = node1;
    while (aux->parent != NULL) { // log(N)
        aux = aux->parent;
        ++depth1;
    }
    aux = node2;
    while (aux->parent != NULL) { // log(N)
        aux = aux->parent;
        ++depth2;
    }
    while (node1 != node2) { // log(N)
        if (depth2 > depth1) {
            --depth2;
            node2 = node2->parent;
        } else if (depth1 > depth2){
            --depth1;
            node1 = node1->parent;
        } else {
            node1 = node1->parent;
            node2 = node2->parent;
        }
    }
    return node1;
}

bool isEqual(BinaryTree *tree1, BinaryTree *tree2)
{
    if ((tree1->size != 0) ^ (tree2->size != 0)) return false; 
    if (tree1->size != 0 && tree1->data != tree2->data) return false;
    if (((tree1->left == NULL) ^ (tree2->left == NULL)) || ((tree1->right == NULL) ^ (tree2->right == NULL))) return false;
    if (tree1->left != NULL && tree2->left != NULL && !isEqual(tree1->left, tree2->left)) return false;
    if (tree1->right != NULL && tree2->right != NULL && !isEqual(tree1->right, tree2->right)) return false;
    return true;
}

bool isSubtree(BinaryTree *tree, BinaryTree *subtree)
{
    int tree_depth = std::ceil(std::log2(tree->size + 1)) - 1;
    int subtree_depth = std::ceil(std::log2(subtree->size + 1)) - 1;
    queue<BinaryTree*> frontier;
    frontier.push(tree);
    int cur = 1;
    int next = 2;
    while (tree_depth > subtree_depth) {
        --cur;
        if (frontier.front()->left == NULL) {
            --next;
        } else {
            frontier.push(frontier.front()->left);
        }
        if (frontier.front()->right == NULL) {
            --next;
        } else {
            frontier.push(frontier.front()->right);
        }
        frontier.pop();
        if (cur == 0) {
            cur = next;
            next = cur*2;
            --tree_depth;
        }
    }

    while (!frontier.empty()) {
        if (isEqual(frontier.front(), subtree)) return true;
        frontier.pop();
    }
    return false;
}

void pathsOfLen(BinaryTree *tree, int xs, int S, vector<vector<pair<int, int> > > &paths, vector<pair<int, int> > path, int idx)
{
    if (tree->size > 0) {
        int x = tree->data;
        path.push_back(make_pair(idx, x));
        xs += x;   
        while (xs > S) {
            xs -= path.front().second;
            path.erase(path.begin());
        } 
        if (xs == S && path.size() > 0){
            paths.push_back(path);
        }
        if (tree->left != NULL) {
            pathsOfLen(tree->left, xs, S, paths, path, idx*2 + 1);
        }
        if (tree->right != NULL) {
            pathsOfLen(tree->right, xs, S, paths, path, idx*2 + 2);
        }
    }
}

vector< vector<pair<int, int> > > pathsOfLength(BinaryTree *tree, int S) // DFS
{
    vector<vector<pair<int, int> > > paths;
    vector<pair<int, int> > path; // first - 012, second - weight
    pathsOfLen(tree, 0, S, paths, path, 0);
    return paths;
}

int main()
{
    BinaryTree t(5);
    t.push(3);
    t.push(1);
    t.push(6);
    t.push(4);
    //cout << isBalanced(t) << '\n';
    //BFS(&t);

    vector<int> v = {0,1,2,3,5,6,7,8,9,10,11};
    vector<int> v2 = {5,8,9,10,11};
    BinaryTree tr, tr2;
    addToBST(v, &tr);
    BFS(&tr);
    addToBST(v2, &tr2);
    //tr2.left->right->data = 4;
    BFS(&tr2);
    //cout << "is BST " << isBST(&tr) << '\n';

    //cout << successor(&tr)->data << '\n';
    
    //cout << commonAncestor(tr.left->left, tr.right->right->left)->data << endl;

    //cout << isSubtree(&tr, &tr2) << endl;
    vector<int> v3 = {1, 1, 2, 1, 0, 1, 1, 4, 1, 0, 2};
    BinaryTree tr3;
    addToBST(v3, &tr3);
    BFS(&tr3);
    vector<vector<pair<int, int> > > paths = pathsOfLength(&tr, 3);
    for (auto path :paths) {
        for (auto node : path) {
            cout << node.first << "(" << node.second << ") -- ";
        }
        cout << endl;
    }

    return 0;
}
