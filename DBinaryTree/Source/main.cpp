#include "..\Include\DBinaryTree.h"

#include <iostream>

int main()
{
	DBinaryTree<int>* tree = new DBinaryTree<int>();

	// right tree
	tree->insert(0);
	tree->insert(5);
	tree->insert(20);
	tree->insert(13);
	tree->insert(11);
	tree->insert(21);
	tree->insert(7);
	tree->insert(1);

	// left tree
	tree->insert(-16);
	tree->insert(-8);
	tree->insert(-11);
	tree->insert(-15);
	tree->insert(-20);
	tree->insert(-6);
	tree->insert(-1);
	tree->insert(-9);

	tree->remove(0);

	delete tree;

	system("pause");
	return 0;
}