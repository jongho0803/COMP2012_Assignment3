//this is one of the two files that you need to submit

#include "infectionAnalyzer.h"
#include <fstream> //for reading file
#include <queue> //you may use STL queue
#include <algorithm> //you may use STL algorithm
using namespace std;
//you are NOT allowed to include additional libraries yourself

//destructor
//dellocate all the trees
InfectionAnalyzer::~InfectionAnalyzer(){
	for(size_t i = 0; i < trees.size(); ++i)
		delete trees[i];
	trees.clear();
}


//load the infection file of the specified filename, and construct the infection trees, return true at the end
//for the file format and examples of the resulting trees, see webpage and sample output
//if the file cannot be loaded, do nothing and return false
//note: it is one of the more challenging tasks. if you cannot implement it completely, you may simply manually construct some trees for testing the other parts of your assignment
bool InfectionAnalyzer::loadInfectionFile(const string filename){
	ifstream fin(filename);

	if (!fin) {
		return false;
	}

	string parent {};
	string child {};

	while(fin >> parent >> child)
	{
		if(!isInfected(parent) && !isInfected(child) ){
			Tree<string>* infectionTree = new Tree<string>;
			infectionTree->addRoot(parent);
			infectionTree->addChild(parent, child);

			trees.push_back(infectionTree);
		}

		else if(isInfected(parent) ^ isInfected(child)){
			for(size_t i = 0; i < trees.size(); ++i){
				if(trees[i]->find(parent)){
					trees[i]->addChild(parent, child);

					break;
				}

				if(trees[i]->find(child)){
					trees[i]->addRoot(parent);

					break;
				}
			}
		}

		else{
			Tree<string>* parent_tree = nullptr;
			Tree<string>* child_tree = nullptr;
			int parent_pos = -1;
			int child_pos = -1;
			for(size_t i = 0; i < trees.size(); ++i){
				if(!parent_tree)
					parent_tree = trees[i]->find(parent);
				if(parent_tree && parent_pos < 0)
					parent_pos = i;
				if(!child_tree)
					child_tree = trees[i]->find(child);
				if(child_tree && child_pos < 0)
					child_pos = i;
			}
			Tree<string>* child_network = new Tree<string>[parent_tree->root->childCount + 1];
			for(int j = 0; j < parent_tree->root->childCount; ++j){
				child_network[j] = parent_tree->root->children[j];
			}
			child_network[parent_tree->root->childCount] = *child_tree;

			++parent_tree->root->childCount;
			delete[] parent_tree->root->children;
			parent_tree->root->children = child_network;
			child_network = nullptr;

			delete trees[child_pos];
			trees.erase(trees.begin() + child_pos);
		}
	}
	fin.close();

	return true;
}


//return true if the specified name can be found in any of the infection trees
//return false otherwise
bool InfectionAnalyzer::isInfected(const string name) const{
	for(size_t i = 0; i < trees.size(); ++i){
		if(trees[i]->find(name))
			return true;
	}
	return false;
}

/*template <typename T>
Tree<T>* getParent(Tree<T>* trees, T name){
	if(trees->root->data == name)
		return trees;
	else{
		for(size_t i = 0; i < trees->root->childCount; i ++){
			Tree<T>* check = getParent(&trees->root->children[i], name);
			if(!check)
				return check;
		}
	}
	return nullptr;
}*/


//check all trees, and find the node with the specified name, and return the name of its parent
//if the node with the specified name is a root node of a tree (i.e. no parent), then return "ZERO" to indicate that the person might be a patient-zero
//if the specified name is not found at all in all trees, return "NA"
//see the sample output for examples
//you cannot use recursion for this function
//for hints on solving this iteratively, you may refer to the webpage
string InfectionAnalyzer::getInfectionSource(const string name) const{

	if(!isInfected(name)){
		return "NA";
	}

	for(size_t i = 0; i < trees.size(); ++i){
		if(trees[i]->root->data == name){
			return "ZERO";
		}
	}

	vector<Tree<string>*> all_node;
	int node_count = 0;
	int change = 0;
	Tree<string>* source = nullptr;

	for(size_t i = 0; i < trees.size(); i ++){
		node_count += trees[i]->getDescendantCount() + 1;
	}

	//while(node_count){

		if(!change){
			for(size_t i = 0; i < trees.size(); i ++){
				all_node.push_back(trees[i]);
				node_count--;
				change ++;
			}
		}

		for(unsigned int i = all_node.size() - change; i < all_node.size(); i ++){
			for(int j = 0; j < all_node[i]->root->childCount; j ++){
				all_node.push_back(&all_node[i]->root->children[j]);
				node_count--;
				change ++;
			}
		}
	//}

	for(size_t i = 0; i < all_node.size(); i ++){
		for(int j = 0; j < all_node[i]->root->childCount; j ++){
			if(all_node[i]->root->children[j].root->data == name)
				source = all_node[i];
		}
	}

	all_node.clear();

	return source->root->data;
}


//return the depth of the node with the specified name
//if the specified name is not found at all in all trees, return -1
int InfectionAnalyzer::getInfectionGeneration(const string name) const{
	if(!isInfected(name)){
			return -1;
	}

	for(size_t i = 0; i < trees.size(); ++i){
		int depth = trees[i]->getDepth(name);
		if(depth != -1)
			return depth;
	}

	return -1;
}

bool compare(pair<string, int> a, pair<string, int> b){
	if(a.second == b. second)
		return (a.first < b.first);
	else
		return (a.second > b.second);
}

//generate the infectionPowerVector and return it
//the infection power of a person is defined as the number of descendants it has in a tree
//the name-power pairs should be ordered by the infection power in descending order
//if two pairs have the the same power, then they are ordered by their names in ascending order (hint: you can directly use > or < to compare strings)
//see the sample output for examples
//you cannot use recursion for this function
//for hints on solving this iteratively, you may refer to the webpage
//note: you should clear the infectionPowerVector at the beginning of this function
const vector<pair<string, int>>& InfectionAnalyzer::getInfectionPowerVector(){
	vector<Tree<string>*> all_node;
	int node_count = 0;
	int change = 0;

	for(size_t i = 0; i < trees.size(); i ++){
		node_count += trees[i]->getDescendantCount() + 1;
	}

	while(node_count){

		if(!change){
			for(size_t i = 0; i < trees.size(); i ++){
				all_node.push_back(trees[i]);
				node_count--;
				change ++;
			}
		}

		for(unsigned int i = all_node.size() - change; i < all_node.size(); i ++){
			for(int j = 0; j < all_node[i]->root->childCount; j ++){
				all_node.push_back(&all_node[i]->root->children[j]);
				node_count--;
				change ++;
			}
		}
	}

	for(size_t i = 0; i < all_node.size(); i ++){
		infectionPowerVector.push_back(make_pair(all_node[i]->root->data, all_node[i]->getDescendantCount()) );
	}

	sort(infectionPowerVector.begin(), infectionPowerVector.end(), compare);

	all_node.clear();

	return infectionPowerVector;
}
