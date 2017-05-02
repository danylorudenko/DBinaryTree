#include "..\Include\DBinaryTree.h"

#include <iostream>

int main()
{
	DBinaryTree<int> tree;

	tree.insert(10);
	tree.insert(5);
	tree.insert(20);
	tree.insert(13);
	tree.insert(11);
	tree.insert(21);
	tree.insert(7);

	std::cout << tree.get(13) << std::endl;

	system("pause");
	return 0;
}