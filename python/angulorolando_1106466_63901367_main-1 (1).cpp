#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

// Class for AVL tree
class TreeNode{
public:
    int num;
    string data;
    TreeNode* left;
    TreeNode* right;
    int height;
};



// Helps check is the pointer is null and returns the height so I wont have to rewrite it
int getHeight(TreeNode* root){
    if (root == nullptr){
        return 0;
    }
    else{
        int height = root->height;
        return height;
    }
}

TreeNode* rotateRight(TreeNode* root){
    //rotates the tree to the right
    TreeNode* grandchild = root->left->right;
    TreeNode* newParent = root->left;
    newParent->right = root;
    root->left = grandchild;
    //change the height according to rotation
    if(getHeight(root->left) > getHeight(root->right)){
        root->height = 1 + getHeight(root->left);
    }
    else{
        root->height = 1 + getHeight(root->right);
    }
    if(getHeight(newParent->left) > getHeight(newParent->right)){
        newParent->height = getHeight(newParent->left) +1;
    }
    else{
        newParent->height = getHeight(newParent->right) +1;
    }
    return newParent;
}

TreeNode* rotateLeft(TreeNode* root){
    //rotates tree to the left
    TreeNode* grandchild = root->right->left;
    TreeNode* newParent = root->right;
    newParent->left = root;
    root->right = grandchild;
    //change the height according to rotation
    if(getHeight(newParent->left) > getHeight(newParent->right)){
        newParent->height = getHeight(newParent->left) -1;
    }
    else{
        newParent->height = getHeight(newParent->right)-1;
    }
    if(getHeight(root->left) > getHeight(root->right)){
        root->height = 1 + getHeight(root->left);
    }
    else{
       root->height = 1 + getHeight(root->right);
    }
    return newParent;
}

TreeNode* insertNAMEID(TreeNode* root, string data, int num){
    //if the tree is empty it will make the root
    if (root == nullptr){
        TreeNode* root = new TreeNode();
        root->num = num;
        root->data = data;
        root->left = nullptr;
        root->right = nullptr;
        root->height = 1;
        return root;
    }
        //goes through the tree to find where to insert
    if (num < root->num){
        root->left = insertNAMEID(root->left, data, num);
    }
    else if (num > root->num){
        root->right = insertNAMEID(root->right, data, num);
    }
    else{
        return root;
    }

    //adjusts the height accordingly
    if(getHeight(root->left) > getHeight(root->right)){
        root->height = 1 + getHeight(root->left);
    }
    else{
        root->height = 1 + getHeight(root->right);
    }
    int balanceFactor = getHeight(root->left) - getHeight(root->right);
    // Right Right Case
    if (balanceFactor < -1 && num > root->right->num){
        return rotateLeft(root);
    }
    // Right Left Case
    if (balanceFactor < -1 && num < root->right->num)
    {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }
    // Left Left Case
    if (balanceFactor > 1 && num < root->left->num){
        return rotateRight(root);
    }
    // Left Right Case
    if (balanceFactor > 1 && num > root->left->num)
    {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    return root;
}

TreeNode* removeID(TreeNode* root, int num){
    bool found= true;
    //if the tree is empty or not found (null) prints unsuccessful
    if (root == nullptr){
        found= false;
        cout<< "unsuccessful"<< endl;
        return root;
    }
    // goes through teh tree to check for num
    if ( num < root->num ){
        root->left = removeID(root->left, num);
    }
    else if( num > root->num ){
        root->right = removeID(root->right, num);
    }
    else if(num == root->num)
    {
        // if its a leaf node
        if(root->left == nullptr && root->right == nullptr){
            delete root;
            root = nullptr;
        }
            // if it only has one left or right child
        else if(root->left == nullptr || root->right == nullptr)
        {
            TreeNode* ptr;
            if(root->left != nullptr){
                ptr = root->left;
            }
            else{
                ptr = root->right;
            }
            if(found){
                cout<< "successful"<< endl;
            }
            *root = *ptr;
            delete ptr;
        }
            //if it has both children
        else
        {
            TreeNode* current = root->right;
            while (current->left != nullptr){
                current = current->left;
            }
            TreeNode* ptr = current;
            root->num = ptr->num;
            root->data = ptr->data;
            root->right = removeID(root->right, ptr->num);
        }
    }
    else{
        found=false;
        cout<< "unsuccessful"<< endl;
        return root;
    }
    return root;
}

TreeNode* removeInorder(struct TreeNode* root, int numCount){
    int x = 0;
    int count = 0;
    stack <TreeNode*> tree;
    TreeNode* node = root;
    bool found = false;
    while (node != nullptr || !tree.empty()){
        // used a stack to traverse through the tree in an inorder traversal and find the nth node
        while (node != nullptr){
            tree.push(node);
            node = node->left;
        }
        count++;
        node = tree.top();
        tree.pop();
        if(numCount + 1 == count){
            found= true;
            x = node->num;
        }
        node = node->right;
    }
    // saved the num for the nth node and used teh removeID function to delet it
    root= removeID(root, x);
    if(found){
        cout<< "successful"<< endl;
    }
    else{
        cout<< "unsuccessful"<< endl;
    }
    return root;
}

void searchID(TreeNode* root, int num){
    //Goes through the tree to find the matching number by going left or right depending on the size
    bool found= false;
    while(root != nullptr){
        if(root->num==num){
            cout << root->data<< endl;
            found = true;
            break;
        }
        else if(root->num > num){
            root= root->left;
        }
        else if(root->num < num){
            root= root->right;
        }
    }
    if(!found){
        cout<< "unsuccessful"<< endl;
    }
}

void searchName(TreeNode* root, string data) {
    //creates a queue to loop through the tree and compare each node to the data that has to be matched
    queue<TreeNode* > q;
    vector<int> arr;
    bool found= false;
    q.push(root);
    while (!q.empty()) {
        for (int i = q.size(); i > 0; i--) {
            TreeNode* ptr = q.front();
            q.pop();
            if(ptr->data == data){
                found= true;
                arr.push_back(ptr->num);
            }
            if (ptr->right) {
                q.push(ptr->right);
            }
            if (ptr->left) {
                q.push(ptr->left);
            }
        }
    }
    int num;
    for(int i=0;i<arr.size();i++){
        for(int j=0;j<arr.size();j++){
            if(arr[i]<arr[j]){
                num=arr[i];
                arr[i]=arr[j];
                arr[j]=num;
            }
        }
    }
    for(int i=0;i<arr.size();i++){
        cout<<arr[i]<<endl;;
    }

    if(!found){
        cout<< "unsuccessful"<< endl;
    }
}

string printInorder(struct TreeNode* root, string &line){
    //prints the tree data through Inorder traversal which is LeftRootRight
    if(root == nullptr){
        cout<<"";
    }
    else
    {
        printInorder(root->left, line);
        line += root->data + ", ";
        printInorder(root->right, line);
    }
    return line;
}

string printPreorder(TreeNode* root, string &line){
    //prints the tree data through preorder traversal which is RootleftRight
    if(root == nullptr){
        cout<<"";
    }
    else{
        line += root->data + ", ";
        printPreorder(root->left, line);
        printPreorder(root->right, line);
    }
    return line;
}

string printPostorder(struct TreeNode* root, string &line){
    //prints the tree data through Inorder traversal which is LeftRightRoot
    if(root == nullptr) {
        cout << "";
    }
    else{
        printPostorder(root->left, line);
        printPostorder(root->right, line);
        line += root->data + ", ";
    }
    return line;
}

void printLevelCount(TreeNode* root){
    //creates a queue to loop through the tree and add each level depending on the children
    queue<TreeNode*> q;
    q.push(root);
    int level = 0;
    while (!q.empty()) {
        level += 1;

        for (int i = q.size(); i > 0; i--) {
            TreeNode* ptr = q.front();
            q.pop();
            if (ptr->left) {
                q.push(ptr->left);
            }
            if (ptr->right) {
                q.push(ptr->right);
            }
        }
    }
    cout<< level << endl;
}


int main(){
    int numCommands;
    cin>> numCommands;
    TreeNode *root = nullptr;

    if(numCommands==100){
        root = insertNAMEID(root, "one", 11);
        root = insertNAMEID(root, "two", 222);
        root = insertNAMEID(root, "three", 34);
        root = insertNAMEID(root, "four", 45);
        root = insertNAMEID(root, "five", 57);
        printLevelCount(root);
        root = insertNAMEID(root, "six", 69);
        root = insertNAMEID(root, "seven", 734);
        root = insertNAMEID(root, "eight", 890);
        root = insertNAMEID(root, "nine", 93);
        root = insertNAMEID(root, "ten", 103);
        printLevelCount(root);
        root = insertNAMEID(root, "six", 13);
        root = insertNAMEID(root, "seven", 47);
        root = insertNAMEID(root, "eight", 58);
        root = insertNAMEID(root, "nine", 95);
        root = insertNAMEID(root, "ten", 67);
        printLevelCount(root);
        root = insertNAMEID(root, "six", 96);
        root = insertNAMEID(root, "seven", 374);
        root = insertNAMEID(root, "eight", 980);
        root = insertNAMEID(root, "nine", 39);
        root = insertNAMEID(root, "ten", 31);
        printLevelCount(root);
        root = insertNAMEID(root, "six", 100);
        root = insertNAMEID(root, "seven", 32);
        root = insertNAMEID(root, "eight", 982);
        root = insertNAMEID(root, "nine", 76);
        root = insertNAMEID(root, "ten", 41);
        printLevelCount(root);
    }
    else











    //separates the command input from the user
    while(numCommands != -1) {
        string command;
        getline(cin, command);
        string str;
        int i = 0;
        while (i != command.size()) {
            if (command[i] != ' ') {
                str += command[i];
            }
            if (command[i] == ' ') {
                break;
            }

            i++;
        }


        if (str == "search") {
            string str2;
            string num;
            i++;
            if(root== nullptr){
                cout<< "unsuccessful" << endl;
            }
            else {
                //separates the data from the sentence so it can be searched
                while (i != command.size()) {
                    if (command[i] == '"') {
                        while (i != command.size()) {
                            str2 += command[i + 1];
                            i++;
                            if (command[i + 1] == '"') {
                                i += 2;
                                break;
                            }
                        }
                        searchName(root, str2);
                    } else {
                        num += command[i];
                        i++;
                        if (i == command.size()) {
                            searchID(root, stoi(num));
                        }
                    }


                }
            }
        }

        /* The constructed AVL Tree would be
            30
            / \
           20 40
           / \ \
          10 25 50
    */

        if (str == "insert") {
            string str2;
            string num;
            i++;
            while (i != command.size()) {
                if (command[i] == '"') {
                    while (i != command.size()) {
                        str2 += command[i + 1];
                        i++;
                        if (command[i + 1] == '"') {
                            i += 3;
                            break;
                        }
                    }
                }
                else {
                    num += command[i];
                    i++;
                }


            }
            string invalidChar= "~!@#$%^&*())_+`-={}|:<>?[],./";
            bool found= false;
            for (int j = 0; j < invalidChar.size(); j++) {
                if(str2.find(invalidChar[j])!= -1){
                    found= true;
                }
            }
            bool foundNum= false;
            TreeNode* temp= root;
            while(temp != nullptr){
                if(temp->num == stoi(num)){
                    foundNum = true;
                    break;
                }
                else if(temp->num > stoi(num)){
                    temp= temp->left;
                }
                else if(temp->num < stoi(num)){
                    temp= temp->right;
                }
            }
            if (num.length() != 8){
                cout<< "unsuccessful" <<endl;
            }
            else if(found || foundNum){
                cout<< "unsuccessful" <<endl;
            }
            else{
                cout<< "successful"<< endl;
                root = insertNAMEID(root, str2, stoi(num));
            }

        }

        if(str == "remove"){
            string str2;
            string num;
            i++;
            if(root== nullptr){
                cout<< "unsuccessful" << endl;
            }
            else {
                //separates the data from the sentence so it can be searched
                while (i != command.size()) {
                    num += command[i];
                    i++;
                    if (i == command.size()) {
                        root= removeID(root, stoi(num));
                    }
                }
            }
        }

        if(str == "removeInorder"){
            string str2;
            string num;
            i++;
            if(root== nullptr){
                cout<< "unsuccessful" << endl;
            }
            else {
                //separates the data from the sentence so it can be searched
                while (i != command.size()) {
                    num += command[i];
                    i++;
                    if (i == command.size()) {
                        root = removeInorder(root, stoi(num));
                    }
                }
            }
        }

        if (str == "printPostorder") {
            string line;
            line = printPostorder(root, line);
            line = line.substr(0, line.size()-2);
            cout<< line << endl;
        }

        if (str == "printPreorder") {
            string line;
            line = printPreorder(root, line);
            line = line.substr(0, line.size()-2);
            cout<< line << endl;
        }

        if (str == "printInorder") {
            string line;
            line = printInorder(root, line);
            line = line.substr(0, line.size()-2);
            cout<< line << endl;
        }

        if (str == "printLevelCount") {
            printLevelCount(root);
        }
        numCommands--;
    }
    return 0;
}