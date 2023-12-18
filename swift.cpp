#include <bits/stdc++.h>

using namespace std;

struct Node {
    int start;
    int *end;
    int LeafIndex;
    struct Node *child[256];
    struct Node *Suffix_Link;
};

int Count_Nodes = 0;
int Internal_Nodes = 0;
int Leaf_Nodes = 0;

Node *RootNode = NULL;

string Input_String;
string Previous_Edge;
string temp;
string temp_string = " Connected to the root ";

Node *Last_Created_Node = NULL;
Node *Active_Node = NULL;

int Active_Edge = -1;
int Active_Length = 0;

int Remainin_Nodes = 0;
int leafEnd = -1;
int *Root_End = NULL;
int *Divide_Node = NULL;
int mysize = -1;

Node *Create_Node(int start, int *end) {
    Node *node = new Node(); // Use new instead of malloc
    for (int i = 0; i < 256; i++)
        node->child[i] = NULL;
    node->start = start;
    node->end = end;
    node->Suffix_Link = RootNode;
    node->LeafIndex = -1;
    return node;
}

int Next_Char(Node *currNode) {
    int Edge_Length = (*(currNode->end)) - (currNode->start) + 1;

    if (Active_Length >= Edge_Length) {
        Active_Edge = Active_Edge + Edge_Length;
        Active_Length -= Edge_Length;
        Active_Node = currNode;
        return 1;
    }
    return 0;
}

void Suffix_Indexing(Node *n, int H) {
    if (n == NULL)
        return;

    if (n->start != -1) {
        int i = n->start;
        int j = *(n->end);

        int k;
        for (k = i; k <= j; k++) {
            cout << Input_String[k];
            Previous_Edge += (Input_String[k]);
        }
    }
    int leaf = 1;
    for (int i = 0; i < 256; i++) {
        if (n->child[i] != NULL) {
            if (leaf == 1 && n->start != -1) {
                printf(" [%d]", n->LeafIndex);
                Internal_Nodes++;
                cout << " --> Internal Node" << endl;
            }

            leaf = 0;
            int Edge_Length = (*(n->end)) - (n->start) + 1;
            Suffix_Indexing(n->child[i], H + Edge_Length);
        }
    }
    if (leaf == 1) {
        n->LeafIndex = mysize - H;
        printf(" [%d]", n->LeafIndex);
        Leaf_Nodes++;
        cout << " --> Leaf Node\n";
        Previous_Edge.erase();
    }
}

// Debugging the code phase by phase
void Debug_by_phase(int i) {
    cout << "\nPhase " << i << endl;
    cout << "Remainin_Nodes = " << Remainin_Nodes << "  Active_Length = " << Active_Length << endl;

    if (Active_Edge == -1)
        cout << "Active_Edge = " << Active_Edge << endl;
    else
        cout << "Active_Edge = " << Active_Edge << "  Active_Character  = " << Input_String[Active_Edge] << endl;
}

int main() {
    Previous_Edge = "";
    Input_String = "abcab$";

    mysize = Input_String.size();
    Root_End = new int; // Use new instead of malloc
    *Root_End = -1;

    RootNode = Create_Node(-1, Root_End);

    Active_Node = RootNode;

    // Phases for making the Tree
    for (int i = 0; i < mysize; i++) {
        Debug_by_phase(i);

        leafEnd = i;
        Remainin_Nodes++;
        Last_Created_Node = NULL;

        while (Remainin_Nodes > 0) {
            if (Active_Length == 0) {
                Active_Edge = i;
            }
            if (Active_Node->child[Input_String[Active_Edge]] == NULL) {
                Active_Node->child[Input_String[Active_Edge]] = Create_Node(i, &leafEnd);

                if (Last_Created_Node != NULL) {
                    Last_Created_Node->Suffix_Link = Active_Node;
                    Last_Created_Node = NULL;
                }
            } else {
                Node *next = Active_Node->child[Input_String[Active_Edge]];
                if (Next_Char(next)) {
                    continue;
                }

                if (Input_String[next->start + Active_Length] == Input_String[i]) {
                    if (Last_Created_Node != NULL && Active_Node != RootNode) {
                        Last_Created_Node->Suffix_Link = Active_Node;
                        Last_Created_Node = NULL;
                    }
                    Active_Length++;
                    break;
                }

                Divide_Node = new int; // Use new instead of malloc
                *Divide_Node = next->start + Active_Length - 1;

                Node *split = Create_Node(next->start, Divide_Node);
                Active_Node->child[Input_String[Active_Edge]] = split;

                split->child[Input_String[i]] = Create_Node(i, &leafEnd);
                next->start += Active_Length;
                split->child[Input_String[Active_Edge]] = next;

                if (Last_Created_Node != NULL) {
                    Last_Created_Node->Suffix_Link = split;
                }

                Last_Created_Node = split;
            }

            Remainin_Nodes--;
            if (Active_Node == RootNode && Active_Length > 0) {
                Active_Length--;
                Active_Edge = i - Remainin_Nodes + 1;
            } else if (Active_Node != RootNode) {
                Active_Node = Active_Node->Suffix_Link;
            }
        }
    }

    cout << endl
         << endl;

    int H = 0; // labeling at the leaf index

    Suffix_Indexing(RootNode, H);
    cout << endl
         << endl;
    Count_Nodes = Internal_Nodes + Leaf_Nodes;
    cout << "Number of nodes created -> " << Count_Nodes << endl;
    cout << "Number of Internal_Nodes created -> " << Internal_Nodes << endl;
    cout << "Number of Leaf_Nodes created --> " << Leaf_Nodes << endl;

    return 0;
}