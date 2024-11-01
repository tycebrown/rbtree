/*
 * rbtree.h
 *
 *  Created on: Apr 18, 2021
 *      Author: lambcoder
 */

#ifndef RBTREE_H_
#define RBTREE_H_
#include <type_traits>
#include <initializer_list>
#include <memory>
#include <iostream>
//#include "../print.cpp"
#define INIT_CN_OF_RBTREE(THIS) (THIS)->control_node->right = (THIS)->control_node->left = (THIS)->control_node->root = (THIS)->control_node;
namespace lambcoder{

template<typename T>
struct rbtree_simple_node{
	using value_type = T;
	using node_ptr = rbtree_simple_node<T>*;
	using const_node_ptr = const rbtree_simple_node<T>*;
	T obj{};
	enum class color_type: unsigned char{red,black} color{color_type::black};
	node_ptr root{nullptr}, left{nullptr}, right{nullptr};

	template<typename U, typename = std::enable_if_t<std::is_convertible_v<U,T>>>
	constexpr rbtree_simple_node(U&& obj = {}, color_type color = color_type::black,
			node_ptr root = nullptr, node_ptr left = nullptr, node_ptr right = nullptr) noexcept:
					obj{obj},color{color},root{root},left{left},right{right}{}
};

template<typename T, typename Compare = std::less<T>, typename Alloc = std::allocator<rbtree_simple_node<T>>>
class rbtree{
	mutable Alloc a;
	mutable Compare c;
	using sentinel_type = rbtree_simple_node<T>*;
	sentinel_type control_node = new rbtree_simple_node<T>{T{},  rbtree_simple_node<T>::color_type::black/*,
		control_node,control_node,control_node*/}; // has a ptr to root, leftmost node, rightmost node, and acts as sentinel
	size_t _size{};
public:
	using compare = Compare;
	using node_type = rbtree_simple_node<T>;
	using node_ptr = typename node_type::node_ptr;
	using const_node_ptr = typename node_type::const_node_ptr;
	using value_type = typename node_type::value_type;
	using reference = value_type&;
	using const_reference = const value_type&;
	using pointer = value_type*;
	using const_pointer = const value_type*;
	using size_type = std::size_t;
	using allocator_type = Alloc;
	using traits = std::allocator_traits<allocator_type>;
	using color_type = typename node_type::color_type;
private:
	constexpr void d_red_logic(node_ptr iter) noexcept{
		if(sibling(iter->root)->color == color_type::black){ // is the color of iter's uncle black?
			if(iter->root->right == iter &&  iter->root->root->right == iter->root){
				left_rotate(iter = iter->root);
			}
			else if(iter->root->left == iter &&  iter->root->root->left == iter->root){
				right_rotate(iter = iter->root);
			}
			else{
				for(std::uint8_t i{}; i < 2; ++i){
					if(iter->root->left == iter){
						right_rotate(iter);
					}
					else{
						left_rotate(iter);
					}
				}
			}
			iter->color = color_type::black;
			if(iter->left != control_node) iter->left->color = color_type::red;
			if(iter->right != control_node) iter->right->color = color_type::red;
		}
		else{
			iter->root->root->left->color = iter->root->root->right->color = color_type::black;
			if(iter->root->root != control_node->root){
				iter->root->root->color = color_type::red;
				if(iter->root->root->root->color == color_type::red){
					d_red_logic(iter->root->root);
				}
			}
		}
	}
	constexpr void d_black_logic(node_ptr dblack, node_ptr dblack_root) noexcept{
		node_ptr s{nullptr};
		if(dblack_root == control_node) return;
		bool issleft = dblack_root->left != dblack;
		s = (issleft ? dblack_root->left : dblack_root->right);
		if(s->color == color_type::red){
			if(issleft){
				right_rotate(s);
				s = dblack_root->right;
			}
			else {
				left_rotate(s);
				s = dblack_root->left;
			}
		}
		if(issleft && s->left->color == color_type::red) right_rotate(s);
		else if(!issleft && s->left->color == color_type::red) s = left_rotate(right_rotate(s->left));
		else if(issleft && s->right->color == color_type::red) s = right_rotate(left_rotate(s->right));
		else if(!issleft && s->right->color == color_type::red) left_rotate(s);
		else {
			if(s->root != control_node) s->color = color_type::red;
			if(dblack_root->color == color_type::red) dblack_root->color = color_type::black;
			else d_black_logic(dblack_root, dblack_root->root);
			return;
		}
		if(s->root != control_node) s->color = color_type::red;
		s->right->color = s->left->color = color_type::black;
		return;
	}
	constexpr void copy_logic(const const_node_ptr root, const const_node_ptr sentinel) noexcept{
		if(root == sentinel) return;
		insert(root->obj);
		copy_logic(root->left, sentinel);
		copy_logic(root->right,sentinel);
	}
	void clear_logic(node_ptr root, const const_node_ptr sentinel) const noexcept(noexcept(traits::destroy(a, root)) && noexcept(traits::deallocate(a, root, 1))){
		if(root == sentinel) return;
		clear_logic(root->left, sentinel);
		clear_logic(root->right, sentinel);
		traits::destroy(a, root);
		traits::deallocate(a, root, 1);
	}
public:
	node_ptr make_node(T&& t, color_type color = color_type::red,
			node_ptr root = nullptr, node_ptr left = nullptr, node_ptr right = nullptr) noexcept{
		if(root == nullptr) root = control_node;
		if(left == nullptr) left = control_node;
		if(right == nullptr) right = control_node;
		node_ptr tmp = traits::allocate(a, 1);
		traits::construct(a, tmp, std::move(t), color, root, left, right);
		return tmp;
	}
	node_ptr make_node(const T& t, color_type color = color_type::red,
			node_ptr root = nullptr, node_ptr left = nullptr, node_ptr right = nullptr) noexcept{
		if(root == nullptr) root = control_node;
		if(left == nullptr) left = control_node;
		if(right == nullptr) right = control_node;
		node_ptr tmp = traits::allocate(a, 1);
		traits::construct(a, tmp, t, color, root, left, right);
		return tmp;
	}
	explicit constexpr rbtree(const compare& c, const Alloc& alloc = Alloc{}): a{alloc}, c{c}{ INIT_CN_OF_RBTREE(this); }
	explicit constexpr rbtree(const Alloc& alloc): a{alloc}{ INIT_CN_OF_RBTREE(this); }
	constexpr rbtree(const rbtree& other) noexcept: a{other.a}, c{other.c}{
		INIT_CN_OF_RBTREE(this);
		copy_logic(static_cast<const_node_ptr>(other.control_node->root), other.control_node);
	}

	template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
	constexpr rbtree(std::initializer_list<U> ilu, const Alloc& alloc): a{alloc}, c{}{
		INIT_CN_OF_RBTREE(this);
		for(const auto& i : ilu){
			insert(i);
		}
	}
	template<typename U, typename = std::enable_if_t<std::is_convertible_v<U, T>>>
	constexpr rbtree(std::initializer_list<U> ilu, const compare& c = {}, const Alloc& alloc = {}): a{alloc}, c{c}{
		INIT_CN_OF_RBTREE(this);
		for(const auto& i : ilu){
			insert(i);
		}
	}

	constexpr rbtree(rbtree&& other) noexcept/*(traits::is_always_equal)*/: a{other.a}, c{other.c},
	control_node{other.control_node}, _size{std::exchange(other._size, 0)}{
//		if(other.control_node->root == other.control_node) control_node->root = control_node->right = control_node->left = control_node;
		other.control_node = new node_type{T{},  rbtree_simple_node<T>::color_type::black};
		INIT_CN_OF_RBTREE(&other);
	}

	constexpr rbtree(){ INIT_CN_OF_RBTREE(this); }
	constexpr size_type size() const noexcept{ return _size; }
	constexpr bool empty() const noexcept{ return _size == 0;}

	rbtree& operator=(const rbtree& other) noexcept{
		if(&other == this) return *this;
		this->clear();
		c = other.c;
		a = other.a;
		copy_logic(static_cast<const_node_ptr>(other.control_node->root), other.control_node);
		return *this;
	}

	constexpr rbtree& operator=(rbtree&& other) noexcept{
		if(&other == this) return *this;
		this->clear();
		c = other.c;
		a = other.a;
//		delete control_node;
		std::swap(control_node, other.control_node);
//		if(other.control_node->root == other.control_node) control_node->root = control_node->right = control_node->left = control_node;
//		if(control_node->root == other.control_node) control_node->root = control_node->right = control_node->left = control_node;
//		*other.control_node = node_type({}, color_type::black, other.control_node, other.control_node, other.control_node);
		return *this;
	}

	constexpr node_ptr right_rotate(node_ptr key) noexcept{
		if(key->root == control_node->root){
			control_node->root = key;
		}
		else if(key->root->root->left == key->root){
			key->root->root->left = key;
		}
		else{
			key->root->root->right = key;
		}
		if(key == control_node->right){
			control_node->right = key->root;
		}
		if(key->right != control_node) key->right->root = key->root;
		key->root->left = key->right;
		key->right = key->root;
		key->root = key->root->root;
		key->right->root = key;
		return key;
	}
	constexpr node_ptr left_rotate(node_ptr key) noexcept{
		if(key->root == control_node->root){
			control_node->root = key;
		}
		else if(key->root->root->left == key->root){
			key->root->root->left = key;
		}
		else{
			key->root->root->right = key;
		}
		if(key == control_node->left){
			control_node->left = key->root;
		}
		if(key->left != control_node) key->left->root = key->root;
		key->root->right = key->left;
		key->left = key->root;
		key->root = key->root->root;
		key->left->root = key;
		return key;
	}
	constexpr const_node_ptr sibling(const_node_ptr iter) const noexcept{
		if(iter == control_node->root) return control_node;
		else if(iter == control_node) return control_node;
		return (iter->root->left == iter ? iter->root->right : iter->root->left);
	}
	constexpr node_ptr sibling(const_node_ptr iter) noexcept{
		return const_cast<node_ptr>(static_cast<const rbtree<T,Compare,Alloc>*>(this)->sibling(iter));
	}
	constexpr node_ptr sentinel() noexcept{ return control_node;}
	constexpr node_ptr root() noexcept{ return control_node->root;}
	constexpr node_ptr leftmost() noexcept{ return control_node->left;}
	constexpr node_ptr rightmost() noexcept{ return control_node->right;}
	constexpr const_node_ptr sentinel() const noexcept{ return control_node;}
	constexpr const_node_ptr root() const noexcept{ return control_node->root;}
	constexpr const_node_ptr leftmost() const noexcept{ return control_node->left;}
	constexpr const_node_ptr rightmost() const noexcept{ return control_node->right;}
	constexpr const_node_ptr find(const T& obj) const noexcept{
		for(node_ptr iter = control_node->root; iter != control_node;){
			if(c(obj,iter->obj)){
				iter = iter->left;
			}
			else if(c(iter->obj,obj)){
				iter = iter->right;
			}
			else{
				return iter;
			}
		}
		return control_node;
	}
	constexpr node_ptr find(const T& obj) noexcept{
		for(node_ptr iter = control_node->root; iter != control_node;){
			if(c(obj,iter->obj)){
				iter = iter->left;
			}
			else if(c(iter->obj,obj)){
				iter = iter->right;
			}
			else{
				return iter;
			}
		}
		return control_node;
	}
	node_ptr insert(T&& obj) noexcept{
		if(_size == 0){
			_size = 1;
			return control_node->left = control_node->right = control_node->root =
					make_node(std::move(obj), color_type::black);
		}
		else{
			node_ptr iter = control_node->root;
			for(;;){
				if(c(obj, iter->obj)){
					if(iter->left == control_node){
						iter->left = make_node(std::move(obj), color_type::red, iter);
						if(iter == control_node->left){
							control_node->left = iter->left;
						}
						iter = iter->left;
						break;
					}
					else{
						iter = iter->left;
					}
				}
				else if(c(iter->obj, obj)){
					if(iter->right == control_node){
						iter->right = make_node(std::move(obj), color_type::red, iter);
						if(iter == control_node->right){
							control_node->right = iter->right;
						}
						iter = iter->right;
						break;
					}
					else{
						iter = iter->right;
					}
				}
				else{
					iter->obj = std::move(obj);
					return iter;
				}
			}
			++_size;
			if(iter->root->color == color_type::black){
				return iter;
			}
			else{
				d_red_logic(iter);
			}
			return iter;
		}
	}
	node_ptr insert(const T& obj) noexcept{
		if(_size == 0){
			_size = 1;
			return control_node->left = control_node->right = control_node->root =
					make_node(obj, color_type::black);
		}
		else{
			node_ptr iter = control_node->root;
			for(;;){
				if(c(obj, iter->obj)){
					if(iter->left == control_node){
						iter->left = make_node(obj, color_type::red, iter);
						if(iter == control_node->left){
							control_node->left = iter->left;
						}
						iter = iter->left;
						break;
					}
					else{
						iter = iter->left;
					}
				}
				else if(c(iter->obj, obj)){
					if(iter->right == control_node){
						iter->right = make_node(obj, color_type::red, iter);
						if(iter == control_node->right){
							control_node->right = iter->right;
						}
						iter = iter->right;
						break;
					}
					else{
						iter = iter->right;
					}
				}
				else{
					iter->obj = obj;
					return iter;
				}
			}
			++_size;
			if(iter->root->color == color_type::black){
				return iter;
			}
			else{
				d_red_logic(iter);
			}
			return iter;
		}
	}
	node_ptr erase(T&& obj) noexcept{
		node_ptr where = find(std::forward<T>(obj));
		if(where == control_node) return control_node;
		node_ptr next{where->right};
		for(; next != control_node && next->left != control_node; next = next->left);

		node_ptr tmp{nullptr};
		--_size;
		node_ptr dblack{nullptr}, dblack_root{nullptr};
		if(next == control_node){
			if(where == control_node->left)
				control_node->left = control_node->left->root;
			else if(where == control_node->right)
				control_node->right = control_node->right->root;
			dblack_root = where->root;
			tmp = (where->root != control_node ? where->root->right == where ?
					where->root->right : where->root->left : control_node->root) = where->left;
			if(where->color == color_type::black && where->left->color == color_type::black) dblack = where->left;
			else if(where->color == color_type::black) where->left->color = color_type::black;

			traits::destroy(a, where);
			traits::deallocate(a, where, 1);
//			return tmp;
		}
		else{
			if(next == control_node->left)
				control_node->left = control_node->left->root;
			else if(next == control_node->right)
				control_node->right = control_node->right->root;
			dblack_root = tmp = next->root;
			(next->root != control_node ? next->root->right == next ?
								next->root->right : next->root->left : control_node->root) = next->right;
			if(next->color == color_type::black && next->right->color == color_type::black) dblack = next->right;
			else if(next->color == color_type::black) next->right->color = color_type::black;

			where->obj = next->obj;
			traits::destroy(a, next);
			traits::deallocate(a, next,1);
//			return tmp;
		}
		if(dblack != nullptr){
			d_black_logic(dblack, dblack_root);
		}

		return tmp;
	}
	void clear() noexcept{
		clear_logic(control_node->root, control_node);
		_size = 0;
		*control_node = {T{}, color_type::black};
		INIT_CN_OF_RBTREE(this);
	}
	~rbtree(){
		clear_logic(control_node->root, control_node);
		_size = 0;
		delete control_node;
//		*control_node = node_type(T{}, color_type::black, control_node, control_node, control_node);
	}
};
};
#endif /* RBTREE_H_ */
