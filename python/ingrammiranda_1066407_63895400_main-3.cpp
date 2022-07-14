#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <regex>
using namespace std;

class Node {
public:
    int id, height;
    string name;
    Node *left;
    Node *right;
    Node(int passedID, string passedName) {
        id = passedID;
        name = passedName;
        height = 1;
        left = nullptr;
        right = nullptr;
    }
};

class AVL {
private:
    Node* AVLRoot;

public: //I changed this

    Node* getAVLRoot() //new
    {
        return AVLRoot;
    }
    //Returns height with a null check
    int height(Node* root) {
        return root == nullptr ?  0 : root->height;
    }
    //Returns the new height of the node by taking the max of the left and right + 1
    int heightUpdate(Node* root) {
        return (height(root->right) > height(root->left) ? height(root->right) : height(root->left)) + 1;
    }
    Node* rotateLeft(Node* root) {
        Node* temp = root->right;
        root->right = temp->left;
        temp->left = root;
        root->height = heightUpdate(root);
        temp->height = heightUpdate(temp);
        return temp;
    }
    Node* rotateRight(Node* root) {
        Node* temp = root->left;
        root->left = temp->right;
        temp->right = root;
        root->height = heightUpdate(root);
        temp->height = heightUpdate(temp);
        return temp;
    }
    Node* rotateLeftRight(Node* root) {
        Node* temp = root->left;
        root->left == rotateRight(temp);
        return rotateLeft(root);
    }
    Node* rotateRightLeft(Node* root) {
        Node* temp = root->right;
        root->right == rotateLeft(temp);
        return rotateRight(root);
    }
    //balances the node
    Node* balance(Node* root, int* id) {
        //finds the balance based on height
        int balance = root == nullptr ? 0 : height(root->left) - height(root->right);
        //Left heavy
        if (balance > 1) {
            if (*id < root->left->id){
                root = rotateRight(root);
                //Left subtree is right heavy
            } else {
                root = rotateLeftRight(root);
            }
            //Right heavy
        } else if (balance < -1){
            //Right subtree is left heavy
            if (*id < root->right->id){
                root = rotateRightLeft(root);
            } else {
                root = rotateLeft(root);
            }
        }
        return root;
    }
    Node* insert(Node* root, int* id, string* name) { //how to return a bool
        if(root == nullptr) {
            //cout << "successful" << endl;
            return new Node(*id, *name);
        }
        if (*id > root->id) {
            root->right = insert(root->right, id, name);
        } else if (*id < root->id) {
            root->left = insert(root->left, id, name);
        }
        //node exists
        if(*id == root->id) {
            //cout << "unsuccessful";
        }
        root->height = heightUpdate(root);
        root = balance(root, id);
        return root;
    }
    Node* findRemove(Node* root, int* id) {
        if (root == nullptr) {
            //cout << "unsuccessful" << endl;
            return nullptr;
        }
        //continue search on right side
        if (*id > root->id) {
            root->right = findRemove(root->right, id);
            //continue search on left side
        } else if (*id < root->id) {
            root->left = findRemove(root->left, id);
            //found the node
        } else if (*id == root->id) {
            //cout << "successful" << endl;
            root = remove(root);
        }
        return root;
    }
    Node* removeInorder(Node* root, int* removeIndex, int* index, bool* success) {
        if (root == nullptr) return nullptr;
        //incremet up down left side
        root->left = removeInorder(root->left, removeIndex, index, success);
        //if inorder index is the one to be removed, remove it
        if (*removeIndex == *index) {
            root = remove(root);
            //cout << "successful" << endl;
            *success = true;
            return root;
        }
        *index = *index + 1;
        //increment down the right side
        root->right = removeInorder(root->right, removeIndex, index, success);
        return root;
    }
    Node* remove(Node* root) {
        if (root->left == nullptr && root->right == nullptr) {
            return nullptr;
        } else if (root->right != nullptr && root->left == nullptr) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == nullptr && root->left != nullptr) {
            Node* temp = root->left;
            delete root;
            return temp;
        } else {
            //inorder sucessor
            if (root->right->left == nullptr) {
                root->right->left = root->left;
                return root->right;
            } else {
                //find leftmost root, take its value and set it to the value of the root and then delete that leftmost node
                Node* seek = root->right;
                while(seek->left != nullptr) {
                    seek = seek->left;
                }
                root->id = seek->id;
                root->name = seek->name;
                delete seek;
            }
        }
        return root;
    }
    void printList(vector<Node*>* list) {
        //print first value without comma
        if (list->size() > 0) cout << list->at(0)->name;
        //print the rest of the list with commas
        for (int i = 1; i < list->size(); i++) {
            cout << ", " << list->at(i)->name;
        }
        cout << endl;
    }
    void listInorder(Node* root, vector<Node*>* names) {
        if (root == nullptr) return;
        listInorder(root->left, names);
        names->push_back(root);
        listInorder(root->right, names);
    }
    void listPreorder(Node* root, vector<Node*>* names) {
        if (root == nullptr) return;
        names->push_back(root);
        listPreorder(root->left, names);
        listPreorder(root->right, names);
    }
    void listPostorder(Node* root, vector<Node*>* names) {
        if (root == nullptr) return;
        listPostorder(root->left, names);
        listPostorder(root->right, names);
        names->push_back(root);
    }
    void search(Node* root, string* name, bool* success) {
        if(root == nullptr) return;
        if(*name == root->name) {
            //makes the digit print out 8 digits
            cout << setfill('0') << setw(8) << root->id << endl;
            *success = true;
        }
        //checks both sides becuase of duplicate names and the sorting based off id, not name
        search(root->left, name, success);
        search(root->right, name, success);
    }
    bool search(Node* root, int* id, bool* success) { //edited to make return a bool
        if (root == nullptr) return success;
        if (*id == root->id) {
            //cout << root->name << endl;
            *success = true;
        }
        //checks one side becuase sorted based off id, the search criteria
        if (*id > root->id) {
            search(root->right, id, success);
        } else if (*id < root->id) {
            search(root->left, id, success);
        }
        return success;
    }
    string& searchReturnString(Node* root, string id, string& name) //new
    {

        if (root == nullptr) return name;
        if (stoi(id) == root->id) {
            //cout << root->name << endl;
            name = root->name;
        }
        //checks one side becuase sorted based off id, the search criteria
        if (stoi(id) > root->id) {
            searchReturnString(root->right, id, name);
        } else if (stoi(id) < root->id) {
            searchReturnString(root->left, id, name);
        }
        return name;
    }
    //function to print the tree visually
    void prettyPrint(string prefix, Node* root, bool isLeft) {
        if (root == nullptr) return;
        cout << prefix << (isLeft ? "├──" : "└──" ) << root->id << endl;
        prettyPrint(prefix + (isLeft ? "│   " : "    "), root->left, true);
        prettyPrint(prefix + (isLeft ? "│   " : "    "), root->right, false);
    }
//public:
    AVL() {
        AVLRoot = nullptr;
    }
    void insert(int id, string name) { //must return a bool
        AVLRoot = insert(AVLRoot, &id, &name);

    }
    string searchReturnString(string id) //new, fixing
    {
        string name; //should initialize it
        name = searchReturnString(AVLRoot, id, name);
        return name;

    }
    bool search(int id) { //edited
        bool success = false;
        success = search(AVLRoot, &id, &success);
        //if (!success) cout << "unsuccessful" << endl;
        return success;
    }
    void search(string name) {
        bool success = false;
        search(AVLRoot, &name, &success);
        //if (!success) cout << "unsuccessful" << endl;
    }
    void printInorder() {
        vector<Node*> nodes;
        listInorder(AVLRoot, &nodes);
        printList(&nodes);
    }
    void printPreorder() {
        vector<Node*> nodes;
        listPreorder(AVLRoot, &nodes); //populates the vector
        printList(&nodes);
    }
    void printPostorder() {
        vector<Node*> nodes;
        listPostorder(AVLRoot, &nodes);
        printList(&nodes);
    }
    void printLevelCount() {
        cout << height(AVLRoot) << endl;
    }
    void prettyPrintTesting() {
        prettyPrint("", AVLRoot, false);
    }
    void remove(int id) {
        AVLRoot = findRemove(AVLRoot, &id);
    }
    void removeInorder(int removeIndex) {
        int index = 0;
        bool success = false;
        AVLRoot = removeInorder(AVLRoot, &removeIndex, &index, &success);
        //if (!success) cout << "unsuccessful" << endl;
    }
};

class OrderedMap //my code
{
public:

    AVL avlTree;

    OrderedMap() //Should initialize underlying tree structure which your peer created
    {

    }

    bool insert(const string ID, const string NAME) //how to call the tree made by the OrderedMap constructor
    {
        //search for ID, if it returns true, the insertion will be false bc it was already there
        bool successful = false;
        successful = avlTree.search(stoi(ID));
        avlTree.insert(stoi(ID), NAME); //might lose ID info
        if (!successful) //might work
        {
            return false; //the search found it before we even inserted it
        }
        else
        {
            return true;
        }
    }

    string search(const string ID) //new, broken
    {
        string stringToUse = ID;
        return avlTree.searchReturnString(stringToUse); //should return a string
    }

    string traverse() //Return a string containing comma separated names from a pre-order traversal of the tree
    {
        //similar logic
        vector<Node*> nodes;
        avlTree.listPreorder(avlTree.getAVLRoot(), &nodes);

        string preorderTraversal = "";
        if (nodes.size() > 0)
        {
            preorderTraversal.append(nodes[0]->name);
        }

        for(int i = 1; i < nodes.size(); i++)
        {
            preorderTraversal.append(", ");
            preorderTraversal.append(nodes[i]->name);
        }
        return preorderTraversal;
    }

    bool remove(const string ID)
    {
        bool successful = false; //should be false, true for testing
        successful = avlTree.search(stoi(ID)); //printing problem is here

        avlTree.remove(stoi(ID));

        if (successful)
        {
            return true; //the search found it, so then we removed it
        }
        else
        {
            return false; //we didn't find it, so we couldn't have removed it
        }
    }

    friend class AVL; //maybe this will help
};

//END OF AVL CLASS
//BEGINING OF MAIN CLASS

//takes in the commands from console and adds them to a vector
void getCommands(vector<string>* commands) {
    int inputLines = 0;
    cin >> inputLines;
    string input;
    for(int i = 0; i < inputLines + 1; i++) {
        getline(cin, input);
        if(input != "") commands->push_back(input);
    }
}

//determines if the id is strictly numerical and 8 digits long
int validID (string *s) {
    size_t intSize;
    int possibleInt = 0;
    try {
        possibleInt = stoi(*s, &intSize);
    } catch (...) {
        return -1;
    }
    if (intSize != s->size() || s->size() != 8) return -1;
    return possibleInt;
}

//determines if the index is strictly numerical
int validIndex (string *s) {
    size_t intSize;
    int possibleInt = 0;
    try {
        possibleInt = stoi(*s, &intSize);
    } catch (...) {
        return -1;
    }
    if (intSize != s->size()) return -1;
    return possibleInt;
}

//determines if the name is alphabetical or a space and incased in ""
string validName (string* s) {
    int nameStart = s->find("\"");
    int nameEnd = s->substr(nameStart + 1).find("\"");
    if (nameStart != -1 && nameEnd != -1 && nameStart != nameEnd) {
        string possibleName = s->substr(nameStart + 1, nameEnd);
        if (regex_match(possibleName, regex("[a-zA-Z ]+"))) {
            return possibleName;
        }
    }
    return "";
}

//determines if the name is alphabetical or a space and incased in ""
//determines the end of the name and beginning of the third argument
string validName (string* s, int* argEnd) {
    int nameStart = s->find("\"");
    int nameEnd = s->substr(nameStart + 1).find("\"");
    if (nameStart != -1 && nameEnd != -1 && nameStart != nameEnd) {
        string possibleName = s->substr(nameStart + 1, nameEnd);
        if (regex_match(possibleName, regex("[a-zA-Z ]+"))) {
            if (nameEnd + 3 < s->length()) *argEnd = nameEnd + 3;
            return possibleName;
        }
        return "";
    }
    return "";
}

//prints the id with leading zeros till 8 digits long
std::string to_zero_lead(const int value)
{
    ostringstream ss;
    ss << setw(8) << setfill('0') << value;
    return ss.str();
}

int main()
{
    OrderedMap myMap;
    int lines = 0;
    std::string command = "", ufid = "", name = "";
    std::cin >> lines;
    while(lines--)
    {
        std::cin >> command;
        if(command == "insert")
        {
            std::cin >> ufid >> name;
            std::cout << myMap.insert(ufid, name) << "\n";
        }
        else if(command == "search")
        {
            std::cin >> ufid;
            std::cout << myMap.search(ufid) << "\n";
        }
        else if(command == "traverse")
        {
            std::cout << myMap.traverse() << "\n";
        }
        else if(command == "remove")
        {
            std::cin >> ufid;
            std::cout << myMap.remove(ufid) << "\n";
        }
    }

    return 0;
}