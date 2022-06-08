
class Node{
  public:
  int isLeaf;     	// 1 if this node is a leaf
  int whichLevel;       // the level this node is at
  int index;  	      // its LUT index -- assigned in pass 1
  int numChildren;      // number of children this node has
  long numTimes;        // # colors seen in this sub-block
  long Rsum, Gsum, BSum;// sum of color values accumulated
  Node* nextCand;// candidate for reducing at this level
  Node* child[8];// the 8 children of this node
};
insertTree(Node& node, BYTE rgb[])
{ // insert 24-bit color rgb into subtree tree
	if(node == NULL)
		makeNewNode(node); // make & initialize a new node 
	if(node.isLeaf)
	{
		node.numTimes++;  // inc # of pixels represented
		addColors(node, rgb); // sum the color values
	}
	else
		childIndex = 4 * rgb[0] + 2 * rgb[1] + rgb[2];
insertTree(childIndex, rgb);
}
void reduceTree(void)
{
	thisNode = findReducibleNode();
	thisNode.isLeaf = 1;
	numLeaves -= (thisNode.numChildren - 1);
	freeChildren(thisNode);
}
