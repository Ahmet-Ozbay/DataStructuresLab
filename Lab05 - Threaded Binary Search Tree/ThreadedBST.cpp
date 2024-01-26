#include "ThreadedBST.h"
#include<iostream>
using namespace std;

///-----------------------------------------------
/// Erases all nodes in the tree
/// 
void ThreadedBST::eraseTreeNodes(BSTNode* node)
{
	BSTNode* curr = this->min();
	while (curr)
	{
		BSTNode* n = this->next(curr);
		delete curr;
		curr = n;
	}
} // end-eraseTreeNodes

///-----------------------------------------------
/// Adds a given key to the BST
/// 
void ThreadedBST::add(int key)
{
	// Create a new root node with the given key if the tree is empty
	if (root == nullptr)
	{
		root = new BSTNode(key);
		root->left = root->right = nullptr;
		return;
	}

	// Temporary pointers for iteration
	BSTNode* tempNode = root;
	BSTNode* parent = nullptr;

	// Find the correct position for the new key
	while (tempNode)
	{
		parent = tempNode;

		// Key already exists
		// Return without adding a duplicate
		if (key == tempNode->key)
			return;

		// Decide whether to go left or right
		if (key > tempNode->key)
		{
			if (tempNode->rightLinkType == CHILD)
				tempNode = tempNode->right;
			else
				break;
		}
		else
		{
			if (tempNode->leftLinkType == CHILD)
				tempNode = tempNode->left;
			else
				break;
		}
	}

	// Create a new node
	BSTNode* newNode = new (std::nothrow) BSTNode(key);
	if (newNode == nullptr)
	{
		cerr << "Error: Memory allocation for a new node failed." << endl;
		return;
	}

	// Set the new node's link types
	newNode->leftLinkType = THREAD;
	newNode->rightLinkType = THREAD;

	// Attach the new node to the parent
	if (key < parent->key)
	{
		newNode->left = parent->left;
		newNode->right = parent;
		parent->leftLinkType = CHILD;
		parent->left = newNode;
	}
	else
	{
		newNode->left = parent;
		newNode->right = parent->right;
		parent->rightLinkType = CHILD;
		parent->right = newNode;
	}
} // end-add

///-----------------------------------------------
/// Removes a given key from the BST (if it exists)
/// 
void ThreadedBST::remove(int key)
{
	// Temporary pointer for iteration
	BSTNode* parent = nullptr;
	BSTNode* child = root;

	// Traverse over the tree to find the node
	while (child != nullptr)
	{
		if (key == child->key)
			break;

		parent = child;

		// Decide to go left or right
		if (key < child->key)
		{
			if (child->leftLinkType == CHILD)
				child = child->left;
			else
				break;
		}
		else
		{
			if (child->rightLinkType == CHILD)
				child = child->right;
			else
				break;
		}
	}

	// The node to be removed
	BSTNode* current = child;

	// If the node has two children; left and right
	if (child->rightLinkType == CHILD && child->leftLinkType == CHILD)
	{
		parent = child;
		current = child->left;

		// Find the maximum in the left subtree
		while (current->rightLinkType == CHILD)
		{
			parent = current;
			current = current->right;
		}

		// Replace the key of the node to be removed 
		// With the maximum key in the left subtree
		child->key = current->key;
	}

	// Pointers to find the predecessor or successor
	BSTNode* max = nullptr;
	BSTNode* min = nullptr;

	// Adjust the pointers and link types 
	// Based on the relationship between parent and current
	if (parent->key >= current->key)
	{
		// If the current node is a thread and has no right child
		if (current->rightLinkType == THREAD && current->leftLinkType == THREAD)
		{
			parent->left = current->left;
			parent->leftLinkType = THREAD;
		}

		// If the current node has a thread on the right,
		// Find the max in the left and update links
		else if (current->rightLinkType == THREAD)
		{
			max = current->left;

			while (max->rightLinkType == CHILD)
			{
				max = max->right;
			}

			max->right = parent;
			parent->left = current->left;
		}

		// If the current node has a child on the right,
		// Find the min in the right and update links
		else
		{
			min = current->right;

			while (min->leftLinkType == CHILD)
			{
				min = min->left;
			}

			min->left = current->left;
			parent->left = current->right;
		}

		// Delete the current node
		delete current;
	}
	else
	{
		// Logic is same as the above for the left subtree
		if (current->rightLinkType == THREAD && current->leftLinkType == THREAD)
		{
			parent->right = current->right;
			parent->rightLinkType = THREAD;
		}
		else if (current->rightLinkType == THREAD)
		{
			max = current->left;

			while (max->rightLinkType == CHILD)
			{
				max = max->right;
			}

			max->right = current->right;
			parent->right = current->left;
		}
		else
		{
			min = current->right;

			while (min->leftLinkType == CHILD)
			{
				min = min->left;
			}

			min->left = parent;
			parent->right = current->right;
		}

		// Delete the current node
		delete current;
	}
} // end-remove

///-----------------------------------------------
/// Searches a given key in the ThreadedBST
/// Return a pointer to the node that holds the key
/// If the key is not found, return NULL
/// 
BSTNode* ThreadedBST::find(int key)
{
	// Start from the root
	BSTNode* temp = root;
	while (temp)
	{
		// Check if the current node has the key
		if (key == temp->key) {
			return temp;
		}
		// If the target is less than the current
		if (key < temp->key) {
			temp = temp->left;
		}
		// If the target is bigger move to the right
		else {
			temp = temp->right;
		}
	}
	// Return NULL if the key doesn't exist
	return NULL;
} // end-find

///-----------------------------------------------
/// Returns the minimum key in the ThreadedBST
/// Return a pointer to the node that holds the key
/// If the key is not found, return NULL
/// 
BSTNode* ThreadedBST::min()
{
	// Control if the tree is empty
	if (root == NULL) {
		return NULL;
	}

	// Start from the root
	BSTNode* temp = root;

	// Go to the left end
	while (temp->left) {
		temp = temp->left;
	}

	// Return the left end
	return temp;
} // end-min

///-----------------------------------------------
/// Returns the maximum key in the ThreadedBST
/// Return a pointer to the node that holds the key
/// If the key is not found, return NULL
/// 
BSTNode* ThreadedBST::max()
{
	// Control if the tree is empty
	if (root == NULL) {
		return NULL;
	}

	// Start from the root
	BSTNode* temp = root;

	// Go to the right end
	while (temp->right) {
		temp = temp->right;
	}

	// Return the right end
	return temp;
} // end-max

///-----------------------------------------------
/// Given a valid pointer to a node in ThreadedBST,
/// returns a pointer to the node that contains the inorder predecessor
/// If the inorder predecessor does not exist, returns NULL
/// 
BSTNode* ThreadedBST::previous(BSTNode* node)
{
	// Check if the node is empty
	if (node == NULL) {
		return NULL;
	}

	// Check if the left link is a thread (no left child)
	if (node->leftLinkType == THREAD) {
		return node->left;
	}
	else {
		// Move to the left child
		node = node->left;

		// Go to the right end of the left subtree
		while (node->rightLinkType == CHILD) {
			node = node->right;
		}
	}

	return node; // Return the inorder predecessor node
}

///-----------------------------------------------
/// Given a valid pointer to a node in ThreadedBST,
/// returns a pointer to the node that contains the inorder successor
/// If the inorder successor does not exist, returns NULL
/// 
BSTNode* ThreadedBST::next(BSTNode* node)
{
	// Check if the node is empty
	if (node == NULL) {
		return NULL;
	}

	// Check if the right link is a thread (no right child)
	if (node->rightLinkType == THREAD) {
		return node->right;
	}
	else {

		// Move to the right child 
		node = node->right;

		// Go to the left end of the right subtree
		while (node->leftLinkType == CHILD) {
			node = node->left;
		}
	}

	return node; // Return the inorder successor node
} // end-next