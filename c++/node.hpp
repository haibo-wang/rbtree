#ifndef _sketchpad_node_
#define _sketchpad_node_


#include <list>
//using namespace std;
namespace sketchpad {
    
    template<typename T>
    struct Node {
        std::shared_ptr<Node<T>> parent = nullptr;
        std::shared_ptr<Node<T>> left = nullptr;
        std::shared_ptr<Node<T>> right = nullptr;
        
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
    
}


#endif
