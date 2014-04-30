#ifndef _sketchpad_node_
#define _sketchpad_node_


#include <list>
//using namespace std;
namespace sketchpad {
    
  enum class Color {
    Red,
      Black
   };

  template<typename T>
  struct Node {
  std::shared_ptr<Node<T>> parent = nullptr;
  std::shared_ptr<Node<T>> left = nullptr;
  std::shared_ptr<Node<T>> right = nullptr;

  Color color = Color::Red; // default color is black
  T key;

  Node()=default; // must declare, otherwise, other constructor will surpress it
  Node(T value):key(value){}


  void insert(std::shared_ptr<Node<T>> new_node);
  std::shared_ptr<Node<T>> find(T key);
  void remove(std::shared_ptr<Node<T>> node);
  void traverse(std::function<void(T)> callback);
  std::list<T> in_order_traversal();


  std::shared_ptr<Node<T>> find_min();
  void replace_node_in_parent(std::shared_ptr<Node<T>> node);

  private:

  bool is_left_child(std::shared_ptr<Node<T>>  node) const;
  bool is_right_child(std::shared_ptr<Node<T>> node) const;

  std::shared_ptr<Node<T>> grand_parent(std::shared_ptr<Node<T>> node) const;
  std::shared_ptr<Node<T>> uncle(std::shared_ptr<Node<T>> node) const;
  std::shared_ptr<Node<T>> sibling(std::shared_ptr<Node<T>>  node) const;

  std::shared_ptr<Node<T>> left_rotate(std::shared_ptr<Node<T>> root);
  std::shared_ptr<Node<T>> right_rotate(std::shared_ptr<Node<T>> root);

  std::shared_ptr<Node<T>> in_order_predecessor(std::shared_ptr<Node<T>> node) const;
  std::shared_ptr<Node<T>> in_order_successor(std::shared_ptr<Node<T>> node) const;

  void insert_case1_null_parent(std::shared_ptr<Node<T>> node);
  void insert_case2_black_parent(std::shared_ptr<Node<T>> node);
  void insert_case3_red_uncle(std::shared_ptr<Node<T>> node);
  void insert_case4_diff_side(std::shared_ptr<Node<T>> node);      
  void insert_case5_same_side(std::shared_ptr<Node<T>> node);

  void delete_node_with_one_child(std::shared_ptr<Node<T>> node);
    
    
        
  };
    
  template<typename T>
  void Node<T>::insert(std::shared_ptr<Node<T>> new_node) {
    if( new_node->key < key  ) {
      if ( left == nullptr) {
	left = new_node;
      } else {
	left->insert(new_node);
      }
    } else {
      if ( right == nullptr) {
	right = new_node;
      }else {
	right->insert(new_node);
      }
    }
  }
    
    
  template<typename T>
  std::shared_ptr<Node<T>> Node<T>::find(T key) {
        
    if ( key == this->key ) {
      std::shared_ptr<Node<T>> found(this);
      return found;
    }
        
    if ( key < this->key ) {
      if ( left == nullptr) {
	return nullptr;
      } else {
	return left->find(key);
      }
    } else {
      if ( right == nullptr) {
	return nullptr;
      }else {
	return right->find(key);
      }
    }
        
  }
    
    
  template<typename T>
  std::shared_ptr<Node<T>> Node<T>::find_min() {
    std::shared_ptr<Node<T>> current_node(this);
    while (current_node->left != nullptr) {
      current_node = current_node->left;
    }
        
    return current_node;
  }
    
  template<typename T>
  void Node<T>::replace_node_in_parent(std::shared_ptr<Node<T>> node) {
    std::shared_ptr<Node<T>> current_node(this);
    if ( parent != nullptr) {
      if (current_node == parent->left) {
	parent->left = node;
      } else {
	parent->right = node;
      }
    }
        
    if ( node != nullptr ) {
      node->parent = parent;
    }
  }
    
    
  template<typename T>
  void Node<T>::remove(std::shared_ptr<Node<T>> node) {
    if ( node->key < key ) {
      left->binary_tree_delete(node);
    } else if ( node->key > key ) {
      right->binary_tree_delete(node);
    } else {
      if (left != nullptr && right != nullptr) {
	auto succssor = right->find_min();
	key = succssor->key;
	succssor.binary_tree_delete(succssor);
      } else if ( left != nullptr) {
	replace_node_in_parent(left);
      } else if ( right != nullptr ) {
	replace_node_in_parent(right);
      } else {
	replace_node_in_parent(nullptr);
      }
    }
  }
    
    
  template <typename T>
  void Node<T>::traverse(std::function<void(T)> callback) {
    if ( left != nullptr )
      left->traverse(callback);
    callback(key);
    if ( right != nullptr)
      right->traverse(callback);
  }
    
    
  template<typename T>
  std::list<T> Node<T>::in_order_traversal() {
    std::list<T> sorted;
    traverse([&](T value){sorted.push_back(value);});
    return sorted;
        
  }



  template<typename T>
  bool Node<T>::is_left_child(std::shared_ptr<Node<T>>  node) const {
    return node->parent != nullptr && node == node->parent->left;
  }

  template<typename T>
  bool Node<T>::is_right_child(std::shared_ptr<Node<T>> node) const {
    return node->parent != nullptr && node == node->parent-right;
  }


  template<typename T>
  std::shared_ptr<Node<T>> Node<T>::grand_parent(std::shared_ptr<Node<T>> node ) const {
    if ( node->parent != nullptr && node->parent->parent != nullptr) {
      return node->parent->parent;
    }

    return nullptr;
  }

  template<typename T>
  std::shared_ptr<Node<T>> Node<T>::uncle(std::shared_ptr<Node<T>> node ) const {
    std::shared_ptr<Node<T>> gp = grand_parent(node);
    if ( gp == nullptr) {
      return nullptr;
    }

    if ( node->parent == gp->left ) {
      return gp->right;
    } else {
      return gp->left;
    }
  }

  template<typename T>
  std::shared_ptr<Node<T>> Node<T>::sibling(std::shared_ptr<Node<T>> node) const {
        
    if (is_left_child(node)){
      return node->parent->right;
    } else {
      return node->parent->left;
    }
        
    return nullptr;
  }


  template<typename T>
  std::shared_ptr<Node<T>> Node<T>::left_rotate(std::shared_ptr<Node<T>> root) {
    std::shared_ptr<Node<T>> saved_parent = root->parent;
    std::shared_ptr<Node<T>> saved_right = root->right;
    std::shared_ptr<Node<T>> saved_right_left = root->right->left;

    if ( saved_parent == nullptr ) {
      saved_right->parent = nullptr;
    } else {
      bool lchild = is_left_child(root);
      if ( lchild ) {
	saved_parent->left = saved_right;
      } else {
	saved_parent->right = saved_right;
      }
      saved_right->parent = saved_parent;
    }

    saved_right->left = root;

    root->parent = saved_right;
    root->right = saved_right_left;
    if ( saved_right_left != nullptr) {
      saved_right_left->parent = root;
    }

    //TODO: tree rotation may change the root of the root of the tree
    return saved_right;
  }

  template<typename T>
  std::shared_ptr<Node<T>> Node<T>::right_rotate(std::shared_ptr<Node<T>> sub) {
    std::shared_ptr<Node<T>> saved_parent = sub->parent;
    std::shared_ptr<Node<T>> saved_left = sub->left;
    std::shared_ptr<Node<T>> saved_left_right = sub->left->right;

    if ( saved_parent == nullptr) {
      saved_left->parent = nullptr;
    } else {
      bool rchild = is_left_child(sub);
      if ( rchild ) {
	saved_parent->left = saved_left;
      } else {
	saved_parent->right = saved_left;
      }
      saved_left->parent = saved_parent;
    }

    saved_left->right = sub;
    sub->parent = saved_left;
    sub->left = saved_left_right;
    if ( saved_left_right != nullptr) {
      saved_left_right->parent = sub;
    }

    return saved_left;
  }

  template<typename T>
  std::shared_ptr<Node<T>> Node<T>::in_order_predecessor(std::shared_ptr<Node<T>> node) const {
    assert(node != nullptr);

    std::shared_ptr<Node<T>> predecessor = node->left;
    while ( predecessor != nullptr && predecessor->right != nullptr) {
      predecessor = predecessor->right;
    }
    return predecessor;
  }

  template<typename T>
  std::shared_ptr<Node<T>> Node<T>::in_order_successor(std::shared_ptr<Node<T>> node) const {
    assert( node != nullptr);

    std::shared_ptr<Node<T>> successor = node->right;
    while ( successor != nullptr && successor->left != nullptr) {
      successor = successor->left;
    }

    return successor;
  }


  template<typename T>
  void Node<T>::insert_case1_null_parent(std::shared_ptr<Node<T>> node) {
    if ( node->parent == nullptr) {
      node->color = Color::Black;
    } else {
      insert_case2_black_parent(node);
    }
  }

  template<typename T>
  void Node<T>::insert_case2_black_parent(std::shared_ptr<Node<T>> node) {
    if ( node->parent->color == Color::Black ) {
      return;
    } else {
      insert_case3_red_uncle(node);
    }
  }

  template<typename T>
  void Node<T>::insert_case3_red_uncle(std::shared_ptr<Node<T>> node) {
    std::shared_ptr<Node<T>> un = uncle(node);

    if ( un != nullptr && un->color == Color::Red) {
      node->parent->color = Color::Black;
      un->color = Color::Black;
      std::shared_ptr<Node<T>> gp = grand_parent(node);
      gp->color = Color::Red;
      insert_case1_null_parent(gp);
    } else {
      insert_case4_diff_side (node);
    }
  }

  template<typename T>
  void Node<T>::insert_case4_diff_side(std::shared_ptr<Node<T>> node) {
    if ( is_left_child(node->parent) && is_right_child(node) ) {
      //TODO: rotation may change the root of the tree, check
      left_rotate(node->parent);
      node = node->left;
    } else if ( is_right_child(node->parent) && is_left_child(node) ) {
      // TODO: rotation may change the root of the tree, check
      right_rotate(node->parent);
      node=node->right;
    }

    insert_case5_same_side(node);
  }


  template<typename T>
  void Node<T>::insert_case5_same_side(std::shared_ptr<Node<T>> node) {
    auto gp = grand_parent(node);
    node->parent->color = Color::Black;
    gp->color = Color::Red;

    if ( is_left_child(node) ) {
      // TODO: rotation may change ....
      right_rotate(gp);
    }else {
      // TODO:
      left_rotate(gp);
    }
  }

    
    

}




#endif
