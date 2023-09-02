//this is one of the two files that you need to submit

using namespace std;
//you are NOT allowed to include additional libraries yourself

//you do NOT need to include "tree.h" in this file
//this is NOT a cpp file, but simply a part of "tree.h"
//see the bottom of tree.h for explanation
//just write your tree implementation here right away


//deep copy constructor
//hint: as our general Tree implementation is inspired by the lecture notes's BST's, check it out for ideas
template <typename T>
Tree<T>::Tree(const Tree& another){
	if(another.isEmpty()){
		return;
	}

	*this = another;
}

//destructor
//hint: as our general Tree implementation is inspired by the lecture notes's BST's, check it out for ideas
template <typename T>
Tree<T>::~Tree(){
	delete root;
}


//deep assignment
//remember to check for self-assignment: if it is a self-assignmnt, do NOT copy
template <typename T>
const Tree<T>& Tree<T>::operator=(const Tree& another){
	if(root == another.root){
		return *this;
	}

	Node<T>* temp;
	temp = new Node<T>(*another.root);

	delete root;
	root = temp;

	return *this;
}


//search in the whole tree
//and return the tree pointer to the tree with the root node that contains the specified data
//return nullptr if the specified data is not found
template <typename T>
Tree<T>* Tree<T>::find(const T& data){
	if(isEmpty()){
		return nullptr;
	}
	if(root->data == data){
		return this;
	}
	else{
		for(int i = 0; i < root->childCount; ++i){
			Tree<T>* check = root->children[i].find(data);
			if(check != nullptr)
				return check;
		}
	}

	return nullptr;
}

//this is the const version of find
template <typename T>
const Tree<T>* Tree<T>::find(const T& data) const{
	if(isEmpty()){
		return nullptr;
	}
	if(root->data == data){
		const Tree<T>* temp_found = this;
		return temp_found;
	}
	else{
		for(int i = 0; i < root->childCount; ++i){
			const Tree<T>* check = root->children[i].find(data);
			if(check != nullptr)
				return check;
		}
	}

	return nullptr;
}

//return the depth of the node that contains the specified data in this whole tree
//return -1 if the tree is empty or the data is not found
//note: root node has a depth of 0
template <typename T>
int Tree<T>::getDepth(const T& data) const{
	if(isEmpty()){
		return -1;
	}
	if(find(data) == nullptr){
		return -1;
	}

	if(root->data == data){
		return 0;
	}
	else{
		for(int i = 0; i < root->childCount; ++i){
			Tree<T>* check = root->children[i].find(data);
			if(check != nullptr)
				return 1 + root->children[i].getDepth(data);
		}
	}

	return -1;
}
//return the descendant count of the node that contains the specified data in this whole tree
//return -1 if the tree is empty or the data is not found
//note: any direct or indirect child of a node is considered as a descendant of a node
//see sample output for examples
//hint: you may make use of the overloaded version of this function that takes no parameter
template <typename T>
int Tree<T>::getDescendantCount(const T& data) const{
	if(isEmpty()){
		return -1;
	}

	const Tree<T>* search_tree = find(data);

	if(search_tree == nullptr){
		return -1;
	}

	return search_tree->getDescendantCount();
}


//return the descendant count of the root node in this whole tree
//return -1 if the tree is empty
template <typename T>
int Tree<T>::getDescendantCount() const{
	if(isEmpty()){
		return -1;
	}

	if(root->childCount == 0){
		return 0;
	}
	else{
		int count{0};
		for(int i = 0; i < root->childCount; ++i){
			count += 1 + root->children[i].getDescendantCount();
		}
		return count;
	}

	return -1;
}


//add a new root with the specified data to the tree, and return true at the end
//if the tree already has a root (i.e. non-empty tree), then make a new root while keeping the existing tree
//that means, you will create a new root node with the specified data; it also has exactly one child which is the old root
//note: if the specified data already exists in the tree, do nothing and return false
template <typename T>
bool Tree<T>::addRoot(const T& data){
	if(find(data)){
		return false;
	}

	if(isEmpty()){
		root = new Node<T>(data);
		return true;
	}

	Node<T>* temp = new Node<T>(*root);
	root->data = data;
	root -> childCount = 1;
	delete []root->children;
	root->children = new Tree<T>[1];
	root->children[0].root = temp;
	temp = nullptr;

	return true;
}


//add a new node (child with data being childData) to an existing node (parent with data being parentData) in the tree, and return true at the end
//however, if the parent node is not found in the tree or if the child data is already in the tree, do nothing and return false
//you should add the child to the parent node's children array so that the children[0] is the first child ever added, children[1] is the second one, and so on
//this is to make sure your output is always the same as ours
template <typename T>
bool Tree<T>::addChild(const T& parentData, const T& childData){

	Tree<T>* birth = find(parentData);
	if(!birth){
		return false;
	}

	if(find(childData)){
		return false;
	}


	if(birth->root->childCount == 0){
		delete []birth->root->children;
		birth->root->children = new Tree<T>[1];
		birth->root->children[0].root = new Node<T> (childData);
	}

	else{
		Tree<T>* new_children_tree = new Tree<T>[birth->root->childCount + 1];
		for(int i = 0; i < birth->root->childCount; ++i){
			new_children_tree[i] = birth->root->children[i];
		}
		new_children_tree[birth->root->childCount].root = new Node<T> (childData);

		delete[] birth->root->children;
		birth->root->children = new_children_tree;
		new_children_tree = nullptr;
	}

	++birth->root->childCount;

	return true;
}
