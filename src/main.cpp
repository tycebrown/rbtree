//============================================================================
// Name        : rbtree.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "rbtree.h"
#include "print.cpp"
using namespace lambcoder;
void print_recursive(rbtree<int>::const_node_ptr root, rbtree<int>::const_node_ptr sentinel) noexcept{
	if(root == sentinel) return;
	print_recursive(root->left, sentinel);
	std::cout << '{' << root->obj << " : " <<
			(root->color == rbtree<int>::color_type::red ? "red" : "black") << "} ";
	print_recursive(root->right, sentinel);
}
void print_recursive(const rbtree<int>& tree) noexcept{
	std::cout << tree.size() << " : [ ";
	print_recursive(tree.root(), tree.sentinel());
	std::cout << "]\n";
}
void print_recursive_preorder(rbtree<int>::const_node_ptr root, rbtree<int>::const_node_ptr sentinel) noexcept{
	if(root == sentinel) return;
	std::cout << '{' << root->obj << " : " <<
			(root->color == rbtree<int>::color_type::red ? "red" : "black") << "} ";
	print_recursive_preorder(root->left, sentinel);
	print_recursive_preorder(root->right, sentinel);
}
void print_recursive_preorder(const rbtree<int>& tree) noexcept{
	std::cout << tree.size() << " : [ ";
	print_recursive_preorder(tree.root(), tree.sentinel());
	std::cout << "]\n";
}

//enum class SHOWNULLP : std::uint8_t{
//	f_false, f_true
//};
//
//enum class SHOWROOT : std::uint8_t{
//	f_false, f_true
//};
//
//void print_horizontal(
//		std::string prefix,
//		rbtree<int>::const_node_ptr n,
//		rbtree<int>::const_node_ptr sentinel,
//		SHOWROOT srt_flag,
//		SHOWNULLP snp_flag) {
//    if (n != sentinel) {
//        print_horizontal(prefix +
//				"-------" +
//        		(srt_flag == SHOWROOT::f_true ? "-----" : ""), n->right, sentinel, srt_flag, snp_flag);
//        std::cout << prefix + ("|--> ") + (srt_flag == SHOWROOT::f_true ? "P->" +
//        		(n->root != sentinel ? std::to_string(n->root->obj) : "N") + "," : "") +
//        		(n->color == rbtree<int>::color_type::red ? 'R' : 'B') + " : "
//				<< n->obj << std::endl;
//        print_horizontal(prefix +
//        		(srt_flag == SHOWROOT::f_true ? "-----" : "") +
//        		"-------", n->left, sentinel, srt_flag, snp_flag);
//    }
//    else if(snp_flag == SHOWNULLP::f_true){
//        std::cout << prefix + ("|--> ")
//				<< "NULLP" << std::endl;
//    }
//}
//void print_horizontal(
//		const rbtree<int>& tree,
//		SHOWROOT srt_flag = SHOWROOT::f_false,
//		SHOWNULLP snp_flag = SHOWNULLP::f_false)
//{
//	std::cout << "\n\n";
//    print_horizontal("", tree.root(), tree.sentinel(),srt_flag, snp_flag);
//    std::cout << "\nSize: " << tree.size() << "; Leftmost: " <<
//    		tree.leftmost()->obj << "; Rightmost: " << tree.rightmost()->obj << std::endl;
//}
int main() {
//	rbtree<int> tree;
//	print_horizontal(tree);
//	tree.insert(8);
//	print_horizontal(tree);
//	tree.insert(18);
//	print_horizontal(tree);
//	tree.insert(5);
//	print_horizontal(tree);
//	tree.insert(15);
//	print_horizontal(tree);
//	tree.insert(17);
//	print_horizontal(tree);
//	tree.insert(25);
//	print_horizontal(tree);
//	tree.insert(40);
//	print_horizontal(tree);
//	tree.insert(80);
//	print_horizontal(tree);
//	tree.erase(17);
//	print_horizontal(tree);
//	tree.erase(80);
//	print_horizontal(tree);
//	tree.erase(25);
//	print_horizontal(tree);
//	tree.erase(8);
//	print_horizontal(tree);
//	tree.erase(40);
//	print_horizontal(tree);
	std::cout << "--------------------------------------------------------------------------------------------\n";
	rbtree<int> tree2{16,8,20,4,12,18,22,2,6,10,14,17,19,21,24,1,3,5,7,9,11,13,15,23,25};// @suppress("Invalid arguments")
	print_horizontal(tree2);
	const rbtree<int> tree3{std::move(tree2)}; // @suppress("Invalid arguments")
	print_horizontal(tree3);
	tree2 = tree3;
	print_horizontal(tree2);

	return 0;
}
