#ifndef _sketchpad_binary_search_tree_
#define  _sketchpad_binary_search_tree_



#include "node.hpp"


namespace sketchpad {
    

  template<typename T>
  class binary_search_tree {
  public:
    void insert(T key);
    bool find(T key);
    void remove(T key); // delete is a key word
    std::list<T> sort(const std::list<T>& in);
    void traverse(std::function<void(T)> callback);
  
  private:
      
    std::shared_ptr<Node<T>> root = nullptr;
  
  };


  template<typename T>
  void binary_search_tree<T>::insert(T value) {
    std::shared_ptr<Node<T>> node(new Node<T>(value));
    if ( root == nullptr) {
      root = node;
    } else {
      root->insert(node);
    }

  }

  template<typename T>
  bool binary_search_tree<T>::find(T key) {
    if ( root == nullptr) return false;

    std::shared_ptr<Node<T>> found = root->find(key);

    return found != nullptr;

  }


  template<typename T>
  void binary_search_tree<T>::remove(T key) {
    std::shared_ptr<Node<T>> node(new Node<T>(key));
    root->binary_tree_delete(node);
  }


  template<typename T>
  void binary_search_tree<T>::traverse(std::function<void(T)> callback) {
    root->traverse_binary_search_tree(callback);
  }


  template<typename T>
  std::list<T> binary_search_tree<T>::sort(const std::list<T>& in) {
      


    for ( T t: in ) {
        insert(t);
    }
    return root->in_order_traversal();
  }

}

#endif
