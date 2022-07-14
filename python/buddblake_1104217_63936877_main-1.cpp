//===============
//BLAKE BUDD
//COP3530
//GATOR_AVL TREE
//===============
#include <iostream>
#include <string>
#include <vector>

using namespace std;

//DATA STRUCTURE OUTLINE:
struct AVL_Node //creates the nodes that will be in the avl tree data structure
{
public: //data about the node
    AVL_Node *left;
    AVL_Node *right;
    int id; //student id
    string name; //student name

    //functions of node
    AVL_Node() //default constructor
    {
        id = 0;
        name = "";
        left = nullptr;
        right = nullptr;
    }

    AVL_Node(int studentId, string studentName) //parameterized constructor
    {
        left = nullptr;
        right = nullptr;
        id = studentId;
        name = std::move(studentName);
    }
};

class AVL_Tree //creates the actual structure of the AVL tree
{
public:
// root of the tree
    AVL_Node *root;

    AVL_Tree() {
        root = nullptr;
    }

//member functions
    //insertion ---------------------------------------------------------------------------------------
    AVL_Node *insertNode(AVL_Node *root, AVL_Node *node); // inserts a node into the tree(have to balance with bFactor) //working
    //removal -----------------------------------------------------------------------------------------------
    AVL_Node *minimumVal(AVL_Node *node); //returns the lowest value node, helps with the remove function  //working
    AVL_Node *removeNode(AVL_Node *root, int id); //removes a node from the tree - just have to do a BST deletion //working
    int removeInorder(AVL_Node *root, int removalNumber); //working
    AVL_Node *finalizeRemoveInorder(AVL_Node *root, int removalNumber);

    void inOrderIds(AVL_Node *root, vector<AVL_Node *> &inorderIDS);

    //search ----------------------------------------------------------------------------------------------------------------------
    void searchName(AVL_Node *root, int student_id,string &); //searches for the name of the student with the past in student id //working
    void printSearchName(AVL_Node *root, int students_id); //working
    void searchID(AVL_Node *root, string name,vector<int> &idsThatHaveName); //searches for the id of the students with the name passed in, may be multiple //working
    void printSearchID(AVL_Node *root, string name); //working
    //traversal ----------------------------------------------------------
    string printInOrder(AVL_Node *root); //creates a string of the inorder traversal  //working
    void PrintInOrderString(AVL_Node *root); //working
    string printPreOrder(AVL_Node *root); //creates a string of the  preorder traversal //working
    void PrintPreOrderString(AVL_Node *root); //working
    string printPostOrder(AVL_Node *root); //creates a string of the postorder traversal //working
    void printPostOrderString(AVL_Node *root); //working
    //Rotations ----------------------------------------------------------
    AVL_Node *leftRotation(AVL_Node *node); //left rotation //working
    AVL_Node *rightRotation(AVL_Node *node); //right rotation //working
    AVL_Node *balance(AVL_Node *root, AVL_Node *node); //uses all of the rotations so i can just pass the tree into the balance function each time //working
    //Balance Factor --------------------------------------------------------------------------
    int getBalanceFactor(AVL_Node *node); //returns the balance factor of the specific node //working
    void printLevelCount(AVL_Node *node); //prints the max height of the tree //working

};


//============== HEIGHT =========================//
//returns the height of a node -- did not include as part of the tree class just because it is barely used and works fine as this
int height(AVL_Node *node) {
    if (node == nullptr) //if the tree is empty
        return -1;
    else {
        int leftHeight = height(node->left); //gets the height of the left subtree
        int rightHeight = height(node->right); //gets the height of the right subtree
        if (leftHeight > rightHeight) //checks which one is greater and returns the greatest height in the tree
            return (leftHeight + 1);
        else
            return (rightHeight + 1);
    }
}

//=============== BALANCE FACTOR ================//
//gets the balance factor of a node
int AVL_Tree::getBalanceFactor(AVL_Node *node) {
    if (node == nullptr) //tree is empty, no balance factor
        return -1;
    else
        return height(node->left) - height(node->right); //uses left - right to get the balance factor
}

//============= ROTATE RIGHT ===============//

//============= Both rotations are the code that I wrote for Stepik 5.1.1 Balancing a binary search tree:
//Blake Budd: #485672062 correct
//September 26, 2021, 6:06 PM

//performs a right rotation of the tree
AVL_Node *AVL_Tree::rightRotation(AVL_Node *node) {
    AVL_Node* temp = node->left;
    node->left = temp->right;
    temp->right = node;
    return temp;
}

//============== ROTATE LEFT ===============//
//performs a left rotation of a tree
AVL_Node *AVL_Tree::leftRotation(AVL_Node *node) {
    AVL_Node* temp = node->right;
    node->right = temp->left;
    temp->left = node;
    return temp;
}

//==================== BALANCE AN INSERTION ====================//
//balances the tree - used in insert after the new node is inserted to make sure that it is balanced correctly
AVL_Node *AVL_Tree::balance(AVL_Node *root, AVL_Node *node) {
    int bFactor = getBalanceFactor(root);
    //left left case
    if (bFactor > 1 && node->id < root->left->id)
        return rightRotation(root);
    //right right case
    if (bFactor < -1 && node->id > root->right->id)
        return leftRotation(root);
    //left right case
    if (bFactor > 1 && node->id > root->left->id) {
        root->left = leftRotation(root->left);
        return rightRotation(root);
    }
    //right left case
    if (bFactor < -1 && node->id < root->right->id) {
        root->right = rightRotation(root->right);
        return leftRotation(root);
    }
    //return balanced tree
    return root;
}

//============================INSERT A NEW NODE ===================//
//Creating the member functions:
//inserts a node into the AVL tree
AVL_Node *AVL_Tree::insertNode(AVL_Node *root, AVL_Node *node) {
    if (root == nullptr) { //if the rorot is empty, you just have to make the root the new node
        root = node;
        cout << "successful" << endl;
        return root;
    }

    if (node->id < root->id) { //otherwise, check ids and insert that new node in the correct subtree
        root->left = insertNode(root->left, node);
    } else if (node->id > root->id) {
        root->right = insertNode(root->right, node);
    } else {
        cout << "unsuccessful" << endl;
        return root;
    }
    return balance(root, node); //balances the tree with the new node inserted and returns the balanced tree
}

//================= GET THE MINIMUM VALUE CHILD NODE ====================//
//function to return the node  that has the lesser value id
AVL_Node *AVL_Tree::minimumVal(AVL_Node *node) {
    AVL_Node *current = node;
    while (current->left != nullptr)
        current = current->left; //finds the least value node

    return current;
}

//===================== REMOVE A NODE ============================//
//removes a node from the tree when given the student id to remove
AVL_Node *AVL_Tree::removeNode(AVL_Node *root, int id) {
    // base case
    if (root == nullptr) {
        cout << "unsuccessful" << endl;
        return nullptr;
    } else if (id < root->id) { //otherwise, the node we are looking to delete is in the left subtree
        root->left = removeNode(root->left, id);
    } else if (id > root->id) { //else the node is in the right subtree
        root->right = removeNode(root->right, id);
    } else { //otherwise, the root is the node we are looking to delete
        if (root->left == nullptr) { //if there is only a right child
            AVL_Node *temp = root->right; //the new root is the right child of the root, because there is no left child
            delete root; //delete the old root
            cout << "successful" << endl;
            root = temp; //this returns the new root, without the node that needed to be deleted
        } else if (root->right == nullptr) { //if there is only a left child
            AVL_Node *temp = root->left; //same set up as if there was only a right child
            delete root;
            cout << "successful" << endl;
            root = temp;
        } else { //there are two children: (1)have to find the lowest value of the right,
                                        // (2)set the left of the new root to the left of the orig root(2),
                                        // (3)delete the old root because that is the node that needed to be deleted(3)
                                        // (4)then make that the new root(4),

            AVL_Node *temp = minimumVal(root->right); //(1)
            temp->left = root->left; //(2)
            delete root; //delete the old root data (3)
            cout << "successful" << endl;
            root = temp; //make the right node(4)
        }
    }
    return root; //returns the new tree without the node that needed to be deleted
}

// ==================== SEARCH FOR NAME GIVEN AN ID =================//
//searches the tree using the given id for the names
void AVL_Tree::searchName(AVL_Node *root, int student_id, string &name) {
    if (root != nullptr) { //if the root is not empty
        if (root->id == student_id) { //if it is the root, then  return the root
            name = root->name;
        } else if (student_id > root->id) { //look in the right subtree
            searchName(root->right, student_id, name);
        } else if (student_id < root->id) //look in the left subtree
            searchName(root->left, student_id, name);
    }
}

//prints the name of the student with that id
void AVL_Tree::printSearchName(AVL_Node *root, int students_id) {
    string nameWithId; //holds the name of the person with the id
    if (root != nullptr) {//if the root is not empty
        searchName(root, students_id, nameWithId); //calls the search name function to find the name
        if (!nameWithId.empty()) //if it was in the tree
            cout << nameWithId << endl; //prints the name
        else//otherwise, print unsuccessful
            cout << "unsuccessful" << endl;
    } else
        cout << "unsuccessful" << endl;
}

//=========================== SEARCH FOR AN ID GIVEN A NAME =========================//
//searches the tree for the ids of the students with the name passed in
void AVL_Tree::searchID(AVL_Node *root, string name, vector<int> &idsThatHaveName) {
    if (root != nullptr) {
        if (root->name == name)
            idsThatHaveName.push_back(root->id); //if the names match, it pushes that id into the vector
        searchID(root->right, name, idsThatHaveName); //searches both the left and the right subtree since we have to look through the whole tree
        searchID(root->left, name, idsThatHaveName);
    }

}

//prints the list of names
void AVL_Tree::printSearchID(AVL_Node *root, string name) {
    if (root != nullptr) {
        vector<int> idsWithName; //vector that will hold all of the ids that anyone with the name passed in has
        searchID(root, name, idsWithName); //searches using the above function
        if (!idsWithName.empty()) { //prints the list of student ids with the name that was passed in if there were any
            for (int i = idsWithName.size() - 1; i >= 0; i--)
                cout << idsWithName[i] << endl;
        } else
            cout << "unsuccessful" << endl;
    } else
        cout << "unsuccessful" << endl;
}

//==================================TRAVERSALS==============================//
//-INORDER------------------------------------------------------------------//
//gets the information of the inorder traversal - used in printInorderString
string AVL_Tree::printInOrder(AVL_Node *root) { //LRRight
    string inOrder;
    if (root == nullptr)
        inOrder = "";
    else {
        inOrder = printInOrder(root->left) + ", " + root->name + printInOrder(root->right);
    }
    return inOrder;
}

//prints an inorder traversal of the tree
void AVL_Tree::PrintInOrderString(AVL_Node *root) {
    if (root != nullptr) {
        string inOrderString = printInOrder(root);
        cout << inOrderString.substr(2) << endl; //substring at 2 one gets rid of the comma before the first name, all subsequent "print"In/Pre/Post"order traversal" follows this same structure
    }
}

//-PREORDER-----------------------------------------------------------------//
string AVL_Tree::printPreOrder(AVL_Node *root) { //RLRight
    string preOrder;
    if (root == nullptr)
        preOrder = "";
    else {
        preOrder = ", " + root->name + printPreOrder(root->left) + printPreOrder(root->right);
    }
    return preOrder;
}

void AVL_Tree::PrintPreOrderString(AVL_Node *root) {
    if (root != nullptr) {
        string preOrderString = printPreOrder(root);
        cout << preOrderString.substr(2) << endl; //starting at two gets rid of the comma and the space out front
    }
}
//-POSTORDER------------------------------------------------------------------//
string AVL_Tree::printPostOrder(AVL_Node *root) { //LRightR
    string postOrder;
    if (root == nullptr)
        postOrder = "";
    else {
        postOrder = printPostOrder(root->left) + printPostOrder(root->right) + ", " + root->name;
    }
    return postOrder;
}

void AVL_Tree::printPostOrderString(AVL_Node *root) {
    if (root != nullptr) {
        string postOrderString = printPostOrder(root);
        cout << postOrderString.substr(2) << endl; //starting at two gets rid of the comma and the space out front
    }
}

//=============================== REMOVE INORDER =============================//
void AVL_Tree::inOrderIds(AVL_Node *root, vector<AVL_Node *> &inorderIDS) {
    //returns the vector of nodes arranged inorder
    if (root != nullptr) {
        inOrderIds(root->left, inorderIDS);
        inorderIDS.push_back(root);
        inOrderIds(root->right, inorderIDS);
    }
}

int AVL_Tree::removeInorder(AVL_Node *root, int removalNumber) {
    // call the inorder ids function
    int removalId;

    vector<AVL_Node *> id;
    inOrderIds(root, id);
    //returns the id of the node that needs to be deleted
    removalId = id[removalNumber]->id;

    return removalId;
}

AVL_Node *AVL_Tree::finalizeRemoveInorder(AVL_Node *root, int removalNumber) {
    if (root != nullptr) {
        int remove = removeInorder(root, removalNumber); //calls the two functions above, from that, it gets the id of the correct node that needs to be deleted
        return removeNode(root, remove); //removes that id from the tree
    } else
        cout << "unsuccessful" << endl;
    return root;
}

//================================== PRINT THE LEVEL COUNT ============================================//
void AVL_Tree::printLevelCount(AVL_Node *root) {
    int level = height(root); //just gets the height of the longest path from the leaf to the root
    cout << level + 1 << endl;
}


int main() {
    AVL_Tree avlTree;
    string tempLine;

    //====first line is the number of commands====//
    int commandCount;

    getline(cin, tempLine); //string of the number of commands;

    commandCount = stoi(tempLine); //int of the number of commands

    //substring part idea came from :https://www.cplusplus.com/reference/string/basic_string/substr/
    for (int i = 0; i < commandCount; i++) {
        // regex from : c++ crash course by Josh Lospinoso


        getline(cin, tempLine);     //get the line of input
        auto TempCommand = tempLine.find(" "); //finds the first part of the line , which would be the command
        string tempCommand = tempLine.substr(0, TempCommand);  //this gets the first part of the command without the name or anything after the command

        //===============================INSERT FUNCTION ======================================//
        if (tempCommand == "insert") {


            //must get the name and the number(string) of the student that needs to be inputted.
            auto nameTemp = tempLine.find('\"');
            nameTemp++;
            auto tempId = tempLine.find('\"', nameTemp);
            string name = tempLine.substr(nameTemp, tempId - nameTemp);

            //checking validity of whether or not the name is correct

            bool validity = true; //initialized to true
            //go through each letter in the name and check isAlpha
            for (char i : name)
            {
                if (!isalpha(i) && (i != ' ')) {
                    validity = false;
                    break;
                }
            }
            tempId += 2;
            string studentID = tempLine.substr(tempId);

            //check validity of whether or not the id is 8 digits and all digits
            if (studentID.size() != 8) {
                validity = false;
            }
            for (char i : studentID) {
                if (!isdigit(i)) {
                    validity = false;
                    break;
                }
            }

            //changing the studentID(string) to an integer
            int studentIDNumber;
            studentIDNumber = stoi(studentID);

            //if all was correct, it is good to be put into the tree
            if (validity) {
                auto *temp = new AVL_Node(studentIDNumber, name); //creates a new node with the new students name and ID number
                avlTree.root = avlTree.insertNode(avlTree.root, temp); // puts that into the tree, depending on whether or not the insert allows it to be put in
            } else
                cout << "unsuccessful" << endl;
        }
        //===============================================REMOVE FUNCTION ===========================================//

        if (tempCommand == "remove") {
            TempCommand++;
            string number = tempLine.substr(TempCommand); //get the student id that needs to be removed
            bool valid = true; //to check validity
            for (char i : number) { //check if each is a number
                if (!isdigit(i)) {
                    valid = false;
                    break;
                }
            }
            if (number.size() != 8 || !valid) { //if each as a number, check if it is 8 digits long
                cout << "unsuccessful" << endl;
            } else {
                int removalId = stoi(number); //if it is valid, create an integer with the student id number
                avlTree.root = avlTree.removeNode(avlTree.root, removalId); //call the remove node on that student id number
            }
        }
        //============================================== SEARCH FUNCTION ===============================================//
        if (tempCommand == "search") {
            if (tempLine.find('\"') != string::npos)
            {//the npos part is from what i found on : https://www.cplusplus.com/reference/string/basic_string/npos/ in addition to the substring page

                //SEARCH ID = RETURNS LIST OF NAMES WITH THAT ID
                auto one = tempLine.find('\"');
                one++;
                auto two = tempLine.find('\"', one);

                //name to be searched
                string name = tempLine.substr(one, two - one);
                bool validName = true;//to check the validity of the name
                for (unsigned int i = 0; i < name.size(); i++) { //checks if the entire name is characters, otherwise it is not valid
                    if (!isalpha(name.at(i)) && !name.at(i == ' '))
                        validName = false;
                }
                if (validName) { //if it is valid, then it searches for the ids of the students with that name
                    avlTree.printSearchID(avlTree.root, name);
                } else
                    cout << "unsuccessful" << endl;
            } else {
                //SEARCH NAME, RETURNS THE NAME OF THE STUDENTS WITH THE ID PASSED IN
                TempCommand++;
                string num = tempLine.substr(TempCommand);
                int studentID = 0;
                bool validID = true;//checks validity
                for (char i : num) { //makes sure each digit is a number
                    if (!isdigit(i)) {
                        validID = false;
                        break;
                    }
                }
                if (num.size() == 8 && validID) { //if it is all valid and only 8 digits
                    studentID = stoi(num); //create an int with the student id from the temp string we had
                    avlTree.printSearchName(avlTree.root, studentID); //call print search name on that id
                } else {
                    cout << "unsuccessful" << endl; //if it was not valid
                }
            }
        }
        //====================================== PRINT IN ORDER ===================================================//
        if (tempCommand == "printInorder") {
            avlTree.PrintInOrderString(avlTree.root); //just calls the print in order function

        }
        //===================================== PRINT PRE ORDER =====================================================//
        if (tempCommand == "printPreorder") {
            avlTree.PrintPreOrderString(avlTree.root); //just calls the print pre order function

        }
        //===================================== PRINT POST ORDER ====================================================//
        if (tempCommand == "printPostorder") {
            avlTree.printPostOrderString(avlTree.root); //just calls the print post order function

        }
        //===================================== PRINT LEVEL COUNT ===================================================//
        if (tempCommand == "printLevelCount") {
            avlTree.printLevelCount(avlTree.root); //just calls the print level count function

        }
        //===================================== REMOVE INORDER =======================================================//
        if (tempCommand == "removeInorder") {

            TempCommand++;
            //temporary string holding the number to be removed in an inorder traversal of the tree
            string num = tempLine.substr(TempCommand);
            int n;
            bool valid = true; //checks if it is correct (if it is a number)
            n = stoi(num);
            for (char i : num) {
                if (!isdigit(i)) {
                    valid = false;
                    break;
                }
            }

            if (valid) { //if it is valid, calls the finalizeRemoveInorder function, which in it calls the two helper functions above that function
                avlTree.root = avlTree.finalizeRemoveInorder(avlTree.root, n);
            } else {
                cout << "unsuccessful" << endl;
            }
        }
    }
    return 0;
}
