#pragma once

#define DllExport   __declspec( dllexport )
#define MAX_CHAR 256

typedef struct {
	// Data input
	char* input = nullptr;
	// Length of data
	int size = -1;
	// Position in data
	int position = -1;

	// Root node
	Node* root = nullptr;

	/*lastNewNode will point to newly created internal node,
	  waiting for it's suffix link to be set, which might get
	  a new suffix link (other than root) in next extension of
	  same phase. lastNewNode will be set to nullptr when last
	  newly created internal node (if there is any) got it's
	  suffix link reset to new internal node created in next
	  extension of same phase. */
	Node* lastNewNode = nullptr;
	Node* activeNode = nullptr;

	/*activeEdge is represeted as input string character
	  index (not the character itself)*/
	int activeEdge = -1;
	int activeLength = 0;

	// remainingSuffixCount tells how many suffixes yet to
	// be added in tree
	int remainingSuffixCount = 0;
	int leafEnd = -1;
	int* rootEnd = nullptr;
	int* splitEnd = nullptr;
} Tree;

struct SuffixTreeNode {
	struct SuffixTreeNode* children[MAX_CHAR];

	//pointer to other node via suffix link
	struct SuffixTreeNode* suffixLink;

	/*(start, end) interval specifies the edge, by which the
	 node is connected to its parent node. Each edge will
	 connect two nodes,  one parent and one child, and
	 (start, end) interval of a given edge  will be stored
	 in the child node. Lets say there are two nods A and B
	 connected by an edge with indices (5, 8) then this
	 indices (5, 8) will be stored in node B. */
	int start;
	int* end;

	/*for leaf nodes, it stores the index of suffix for
	  the path  from root to leaf*/
	int suffixIndex;
};

typedef struct SuffixTreeNode Node;