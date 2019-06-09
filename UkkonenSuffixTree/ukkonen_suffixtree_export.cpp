#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "ukkonen_suffixtree_export.h"

// Context related methods
extern "C" DllExport Tree* createTreeContext();
extern "C" DllExport void freeTreeContext(Tree* tree);

// Tree related
extern "C" DllExport void setInput(Tree* tree, char* input, int position, int size);
extern "C" DllExport void buildTree(Tree* tree);

// Find longest match
extern "C" DllExport void findLongestMatch(Tree* tree, char* input, int position, int size, int* displacement, int* length);

DllExport Tree* createTreeContext() {
	Tree* tree = (Tree*)malloc(sizeof(Tree));
	return tree;
}

DllExport void setInput(Tree* tree, char* input, int position, int size) {
	tree->input = input;
	tree->size = size;
	tree->position = position;
}

DllExport void buildTree(Tree* tree) {
	int i;
	tree->rootEnd = (int*)malloc(sizeof(int));
	*tree->rootEnd = -1;

	/*Root is a special node with start and end indices as -1,
	as it has no parent from where an edge comes to root*/
	tree->root = newNode(-1, tree->rootEnd, tree->root);

	tree->activeNode = tree->root; //First activeNode will be root
	for (i = 0; i < tree->size; i++)
		extendSuffixTree(tree, i);
}

DllExport void findLongestMatch(Tree * tree, char* input, int position, int size, int* displacement, int* length) {
	doTraversal(tree->root, input, position, size, displacement, length);
}

DllExport void freeTreeContext(Tree * tree) {
	freeNode(tree->root);

	free(tree->activeNode);
	free(tree->input);
	free(tree->lastNewNode);
	free(tree->rootEnd);
	free(tree->splitEnd);
	free(tree);
}

void freeNode(Node * node) {
	int i;
	for (i = 0; i < sizeof(node->children) / sizeof(Node); i++)
		if (node->children[i] != nullptr)
			freeNode(node->children[i]);

	free(node->children);
	free(node->suffixLink);
	free(node->end);
	free(node);
}

Node * newNode(int start, int* end, Node * link)
{
	Node* node = (Node*)malloc(sizeof(Node));
	int i;
	for (i = 0; i < MAX_CHAR; i++)
		node->children[i] = nullptr;

	/*For root node, suffixLink will be set to nullptr
	For internal nodes, suffixLink will be set to root
	by default in  current extension and may change in
	next extension*/
	node->suffixLink = link;
	node->start = start;
	node->end = end;

	/*suffixIndex will be set to -1 by default and
	  actual suffix index will be set later for leaves
	  at the end of all phases*/
	node->suffixIndex = -1;
	return node;
}

void extendSuffixTree(Tree * tree, int pos)
{
	/*Extension Rule 1, this takes care of extending all
	leaves created so far in tree*/
	tree->leafEnd = pos;

	/*Increment remainingSuffixCount indicating that a
	new suffix added to the list of suffixes yet to be
	added in tree*/
	tree->remainingSuffixCount++;

	/*set lastNewNode to nullptr while starting a new phase,
	 indicating there is no internal node waiting for
	 it's suffix link reset in current phase*/
	tree->lastNewNode = nullptr;

	//Add all suffixes (yet to be added) one by one in tree
	while (tree->remainingSuffixCount > 0) {

		if (tree->activeLength == 0)
			tree->activeEdge = pos; //APCFALZ

		// There is no outgoing edge starting with
		// activeEdge from activeNode
		if (tree->activeNode->children[tree->input[tree->activeEdge] & 0x000000FF] == nullptr)
		{
			//Extension Rule 2 (A new leaf edge gets created)
			tree->activeNode->children[tree->input[tree->activeEdge] & 0x000000FF] =
				newNode(pos, &tree->leafEnd, tree->root);

			/*A new leaf edge is created in above line starting
			 from  an existng node (the current activeNode), and
			 if there is any internal node waiting for it's suffix
			 link get reset, point the suffix link from that last
			 internal node to current activeNode. Then set lastNewNode
			 to nullptr indicating no more node waiting for suffix link
			 reset.*/
			if (tree->lastNewNode != nullptr)
			{
				tree->lastNewNode->suffixLink = tree->activeNode;
				tree->lastNewNode = nullptr;
			}
		}
		// There is an outgoing edge starting with activeEdge
		// from activeNode
		else
		{
			// Get the next node at the end of edge starting
			// with activeEdge
			SuffixTreeNode* next = tree->activeNode->children[tree->input[tree->activeEdge] & 0x000000FF];
			if (walkDown(tree, next))//Do walkdown
			{
				//Start from next node (the new activeNode)
				continue;
			}
			/*Extension Rule 3 (current character being processed
			  is already on the edge)*/
			if (tree->input[next->start + tree->activeLength] == tree->input[pos])
			{
				//If a newly created node waiting for it's 
				//suffix link to be set, then set suffix link 
				//of that waiting node to curent active node
				if (tree->lastNewNode != nullptr && tree->activeNode != tree->root)
				{
					tree->lastNewNode->suffixLink = tree->activeNode;
					tree->lastNewNode = nullptr;
				}

				//APCFER3
				tree->activeLength++;
				/*STOP all further processing in this phase
				and move on to next phase*/
				break;
			}

			/*We will be here when activePoint is in middle of
			  the edge being traversed and current character
			  being processed is not  on the edge (we fall off
			  the tree). In this case, we add a new internal node
			  and a new leaf edge going out of that new node. This
			  is Extension Rule 2, where a new leaf edge and a new
			internal node get created*/
			tree->splitEnd = (int*)malloc(sizeof(int));
			*tree->splitEnd = next->start + tree->activeLength - 1;

			//New internal node
			SuffixTreeNode* split = newNode(next->start, tree->splitEnd, tree->root);
			tree->activeNode->children[tree->input[tree->activeEdge] & 0x000000FF] = split;

			//New leaf coming out of new internal node
			split->children[tree->input[pos] & 0x000000FF] = newNode(pos, &tree->leafEnd, tree->root);
			next->start += tree->activeLength;
			split->children[tree->input[next->start] & 0x000000FF] = next;

			/*We got a new internal node here. If there is any
			  internal node created in last extensions of same
			  phase which is still waiting for it's suffix link
			  reset, do it now.*/
			if (tree->lastNewNode != nullptr)
			{
				/*suffixLink of lastNewNode points to current newly
				  created internal node*/
				tree->lastNewNode->suffixLink = split;
			}

			/*Make the current newly created internal node waiting
			  for it's suffix link reset (which is pointing to root
			  at present). If we come across any other internal node
			  (existing or newly created) in next extension of same
			  phase, when a new leaf edge gets added (i.e. when
			  Extension Rule 2 applies is any of the next extension
			  of same phase) at that point, suffixLink of this node
			  will point to that internal node.*/
			tree->lastNewNode = split;
		}

		/* One suffix got added in tree, decrement the count of
		  suffixes yet to be added.*/
		tree->remainingSuffixCount--;
		if (tree->activeNode == tree->root && tree->activeLength > 0) //APCFER2C1
		{
			tree->activeLength--;
			tree->activeEdge = pos - tree->remainingSuffixCount + 1;
		}
		else if (tree->activeNode != tree->root) //APCFER2C2
		{
			tree->activeNode = tree->activeNode->suffixLink;
		}
	}
}

int walkDown(Tree * tree, SuffixTreeNode * currNode)
{
	/*activePoint change for walk down (APCFWD) using
	 Skip/Count Trick  (Trick 1). If activeLength is greater
	 than current edge length, set next  internal node as
	 activeNode and adjust activeEdge and activeLength
	 accordingly to represent same activePoint*/
	if (tree->activeLength >= edgeLength(currNode))
	{
		tree->activeEdge += edgeLength(currNode);
		tree->activeLength -= edgeLength(currNode);
		tree->activeNode = currNode;
		return 1;
	}
	return 0;
}

int edgeLength(SuffixTreeNode * n) {
	return *(n->end) - (n->start) + 1;
}

void doTraversal(Node * n, char* input, int position, int size, int* displacement, int* length)
{
	if (n == nullptr)
		return; // no match possible

	//If node n is not root node, then traverse edge 
	//from node n's parent to node n. 
	if (n->start != -1)
	{
		int res = traverseEdge(input, position, size, n->start, *(n->end), displacement, length);
		if (res != 0)
			return;  // matching stopped (res = -1) 
	}

	//Get the character index to search 
	position += edgeLength(n);

	//If there is an edge from node n going out 
	//with current character str[idx], traverse that edge 
	if (n->children[input[position]] != nullptr)
		doTraversal(n->children[input[position]], input, position, size, displacement, length);
	else
		return;  // no more children
}

int traverseEdge(char* input, int position, int size, int start, int end, int* displacement, int* length)
{
	*displacement = start;

	int k = 0;
	//Traverse the edge with character by character matching 
	for (k = start; k <= end && position < size; k++, position++)
	{
		if (input[k] != input[position])
			return -1;  // stopped matching

		*length++;
	}

	return 0;  // matched whole node
}