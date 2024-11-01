#ifndef __PRINT_HORIZONTAL
#define __PRINT_HORIZONTAL
#include <iostream>
#include <type_traits>
//#include "src/rbtree.h"

enum class SHOWNULLP : std::uint8_t{
	f_false, f_true
};

enum class SHOWROOT : std::uint8_t{
	f_false, f_true
};

template<typename Node>
void print_horizontal(
		std::string prefix,
		const Node *const n,
		const Node *const sentinel,
		SHOWROOT srt_flag,
		SHOWNULLP snp_flag) {
    if (n != sentinel) {
        print_horizontal(prefix +
				"-------" +
        		(srt_flag == SHOWROOT::f_true ? "-----" : ""), n->right, sentinel, srt_flag, snp_flag);
        std::cout << prefix + ("|--> ") + (srt_flag == SHOWROOT::f_true ? "P->" +
        		(n->root != sentinel ? std::to_string(n->root->obj) : "N") + "," : "") +
        		(n->color == Node::color_type::red ? 'R' : 'B') + " : "
				<< n->obj << std::endl;
        print_horizontal(prefix +
        		(srt_flag == SHOWROOT::f_true ? "-----" : "") +
        		"-------", n->left, sentinel, srt_flag, snp_flag);
    }
    else if(snp_flag == SHOWNULLP::f_true){
        std::cout << prefix + ("|--> ")
				<< "NULLP" << std::endl;
    }
}
template<typename Tree>
void print_horizontal(
		const Tree& tree,
		SHOWROOT srt_flag = SHOWROOT::f_false,
		SHOWNULLP snp_flag = SHOWNULLP::f_false)
{
	std::cout << "\n\n";
    print_horizontal("", tree.root(), tree.sentinel(),srt_flag, snp_flag);
    std::cout << "\nSize: " << tree.size() << "; Leftmost: " <<
    		tree.leftmost()->obj << "; Rightmost: " << tree.rightmost()->obj << std::endl;
}
#endif
