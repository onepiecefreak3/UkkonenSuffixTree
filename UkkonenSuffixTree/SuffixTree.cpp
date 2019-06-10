#include "UkkonenExport.h"
#include "SuffixTreeNode.cpp"

class SuffixTree
{
public:
	SuffixTreeNode* root = nullptr;

	SuffixTree();
	~SuffixTree();

	void Build(char* input, int position, int size);

private:
	int* rootEnd;

	SuffixTreeNode* lastNewNode = nullptr;
	SuffixTreeNode* activeNode = nullptr;

	int activeEdge = -1;
	int activeLength = 0;

	int remainingSuffixCount = 0;
	int leafEnd = -1;
	int* splitEnd = nullptr;

	void ExtendSuffixTree(char* input, int position);
	int WalkDown(SuffixTreeNode* node);
};

SuffixTree::SuffixTree()
{
	rootEnd = new int();
}

SuffixTree::~SuffixTree()
{
	delete root;
	delete rootEnd;

	delete lastNewNode;
	delete activeNode;

	delete splitEnd;
}

void SuffixTree::Build(char* input, int position, int size)
{
	/*Root is a special node with start and end indices as -1,
	as it has no parent from where an edge comes to root*/
	root = new SuffixTreeNode(-1, rootEnd, root);

	activeNode = root; //First activeNode will be root
	for (int i = position; i < size; i++)
		ExtendSuffixTree(input, i);
}

void SuffixTree::ExtendSuffixTree(char* input, int position)
{
	/*Extension Rule 1, this takes care of extending all
	leaves created so far in tree*/
	leafEnd = position;

	/*Increment remainingSuffixCount indicating that a
	new suffix added to the list of suffixes yet to be
	added in tree*/
	remainingSuffixCount++;

	/*set lastNewNode to nullptr while starting a new phase,
	 indicating there is no internal node waiting for
	 it's suffix link reset in current phase*/
	lastNewNode = nullptr;

	//Add all suffixes (yet to be added) one by one in tree
	while (remainingSuffixCount > 0) {

		if (activeLength == 0)
			activeEdge = position; //APCFALZ

		// There is no outgoing edge starting with
		// activeEdge from activeNode
		if (activeNode->children[input[activeEdge] & 0x000000FF] == nullptr)
		{
			//Extension Rule 2 (A new leaf edge gets created)
			activeNode->children[input[activeEdge] & 0x000000FF] = new SuffixTreeNode(position, &leafEnd, root);

			/*A new leaf edge is created in above line starting
			 from  an existng node (the current activeNode), and
			 if there is any internal node waiting for it's suffix
			 link get reset, point the suffix link from that last
			 internal node to current activeNode. Then set lastNewNode
			 to nullptr indicating no more node waiting for suffix link
			 reset.*/
			if (lastNewNode != nullptr)
			{
				lastNewNode->suffixLink = activeNode;
				lastNewNode = nullptr;
			}
		}
		// There is an outgoing edge starting with activeEdge
		// from activeNode
		else
		{
			// Get the next node at the end of edge starting
			// with activeEdge
			SuffixTreeNode* next = activeNode->children[input[activeEdge] & 0x000000FF];
			if (WalkDown(next)) //Do walkdown
			{
				//Start from next node (the new activeNode)
				continue;
			}

			/*Extension Rule 3 (current character being processed
			  is already on the edge)*/
			if (input[next->start + activeLength] == input[position])
			{
				//If a newly created node waiting for it's 
				//suffix link to be set, then set suffix link 
				//of that waiting node to curent active node
				if (lastNewNode != nullptr && activeNode != root)
				{
					lastNewNode->suffixLink = activeNode;
					lastNewNode = nullptr;
				}

				//APCFER3
				activeLength++;
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
			splitEnd = new int();
			*splitEnd = next->start + activeLength - 1;

			//New internal node
			SuffixTreeNode* split = new SuffixTreeNode(next->start, splitEnd, root);
			activeNode->children[input[activeEdge] & 0x000000FF] = split;

			//New leaf coming out of new internal node
			split->children[input[position] & 0x000000FF] = new SuffixTreeNode(position, &leafEnd, root);
			next->start += activeLength;
			split->children[input[next->start] & 0x000000FF] = next;

			/*We got a new internal node here. If there is any
			  internal node created in last extensions of same
			  phase which is still waiting for it's suffix link
			  reset, do it now.*/
			if (lastNewNode != nullptr)
			{
				/*suffixLink of lastNewNode points to current newly
				  created internal node*/
				lastNewNode->suffixLink = split;
			}

			/*Make the current newly created internal node waiting
			  for it's suffix link reset (which is pointing to root
			  at present). If we come across any other internal node
			  (existing or newly created) in next extension of same
			  phase, when a new leaf edge gets added (i.e. when
			  Extension Rule 2 applies is any of the next extension
			  of same phase) at that point, suffixLink of this node
			  will point to that internal node.*/
			lastNewNode = split;
		}

		/* One suffix got added in tree, decrement the count of
		  suffixes yet to be added.*/
		remainingSuffixCount--;
		if (activeNode == root && activeLength > 0) //APCFER2C1
		{
			activeLength--;
			activeEdge = position - remainingSuffixCount + 1;
		}
		else if (activeNode != root) //APCFER2C2
		{
			activeNode = activeNode->suffixLink;
		}
	}
}

int SuffixTree::WalkDown(SuffixTreeNode * node)
{
	int length = node->CalculateLength();

	/*activePoint change for walk down (APCFWD) using
	 Skip/Count Trick  (Trick 1). If activeLength is greater
	 than current edge length, set next  internal node as
	 activeNode and adjust activeEdge and activeLength
	 accordingly to represent same activePoint*/
	if (activeLength >= length)
	{
		activeEdge += length;
		activeLength -= length;
		activeNode = node;
		return 1;
	}

	return 0;
}