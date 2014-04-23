package rbtree;

import java.util.Collection;
import java.util.LinkedList;
import java.util.List;
import java.util.Stack;

public class RedBlackTree<T extends Comparable<T>> {

	private TreeNode<T> root = null;
	private int size = 0;
	private List<T> sortedElements = new LinkedList<>();
	private boolean updated = false;

	public RedBlackTree() {

	}

	public int getSize() {
		return size;
	}

	TreeNode<T> getRoot() {
		return root;
	}

	/**
	 * 
	 * @param key
	 *            the element to search
	 * @return true if the key is found, else false
	 */
	public boolean search(T key) {
		TreeNode<T> node = search(root, key);
		return (node != null);
	}

	TreeNode<T> search(TreeNode<T> start, T key) {

		if (start == null || key.compareTo(start.key) == 0) {
			return start;
		}

		if (key.compareTo(start.key) < 0) {
			return search(start.left, key);
		} else {
			return search(start.right, key);
		}
	}

	public void remove(T key) {
		TreeNode<T> match = search(root, key);
		// no match, key is not in the tree
		if (match == null) {
			return;
		}

		TreeNode<T> nodeToRemove = inOrderPredecessor(match);
		if (nodeToRemove == null) {
			nodeToRemove = match;
		} else {
			// copy the key over, this is just normal binary search tree
			// operation
			match.key = nodeToRemove.key;
		}

		deleteNodeWithOneChild(nodeToRemove);
		updated = false;
	}

	private void deleteNodeWithOneChild(TreeNode<T> node) {

		assert (node.left == null || node.right == null);

		TreeNode<T> child = node.left == null ? node.right : node.left;

		// replace this node with its child
		// this node either has two null nodes, which are treated as black
		// or one red non-null node, plus a null node.
		boolean isLeftNode = isLeftChild(node);
		replaceNode(node, child);

		if (root == null) { //
			node = null;
			return;
		}

		// black node, red child, just change color
		if (node.color == Color.BLACK) {
			if (child != null && child.color == Color.RED) {
				child.color = Color.BLACK;
			} else {
				// black node with two null children, deleted side is already
				// null
				deleteRedSibling(node.parent, isLeftNode);
			}
		}

		// if the node is Red, it must have a black child, replace it with its
		// child is done
		node = null;

	}

	private void deleteRedSibling(TreeNode<T> parent, boolean isLeftChild) {

		TreeNode<T> sibling = isLeftChild ? parent.right : parent.left;
		// sibling should not be null, otherwise the deleted side has one more
		// black node
		assert (sibling != null);

		if (sibling.color == Color.RED) { // case 2
			parent.color = Color.RED; // parent previously must be black, as it
										// has red child
			sibling.color = Color.BLACK;
			if (isLeftChild) {
				leftRotate(parent);
			} else {
				rightRotate(parent);
			}
		} else {

			deleteBlackSibling(parent, isLeftChild, sibling);
		}

	}

	private void deleteBlackSibling(TreeNode<T> parent, boolean isLeftChild,
			TreeNode<T> sibling) {
		// now sibling is BLAK, it should have 1 or 2 red children

		if (isLeftChild && sibling.left != null
				&& sibling.left.color == Color.RED) {
			sibling.color = Color.RED;
			sibling.left.color = Color.BLACK;
			rightRotate(sibling);
			sibling = isLeftChild ? parent.right : parent.left;
			// rotate will change the relationship among nodes involved, need to
			// check it again
		} else if (!isLeftChild && sibling.right != null
				&& sibling.right.color == Color.RED) {
			sibling.color = Color.RED;
			sibling.right.color = Color.BLACK;
			leftRotate(sibling);
			// rotate will change the relationship among nodes involved, need to
			// check it again
			sibling = isLeftChild ? parent.right : parent.left;
		}

		// sibling is black
		if (isLeftChild && sibling.right != null
				&& sibling.right.color == Color.RED) {
			sibling.right.color = Color.BLACK;
			leftRotate(parent);
		}

		if (!isLeftChild && sibling.left != null
				&& sibling.left.color == Color.RED) {
			sibling.left.color = Color.BLACK;
			rightRotate(parent);
		}
	}

	// this node has at most one non-leaf child
	private void replaceNode(TreeNode<T> node, TreeNode<T> child) {

		if (child == null) { // this node has two leaf nodes
			if (node.parent == null) { // node to delete is already the root
				root = null; // replace it with a null node
			} else {
				if (isLeftChild(node)) {
					node.parent.left = null;
				} else {
					node.parent.right = null;
				}
			}
		} else {
			if (node.parent == null) {
				child.parent = null;
				root = child;
			} else {
				if (isLeftChild(node)) {
					node.parent.left = child;
				} else {
					node.parent.right = child;
				}
				child.parent = node.parent;
			}
		}
	}

	/**
	 * @return the elements in the tree as a updated list
	 */
	public List<T> sort() {

		if (updated) {
			return sortedElements;
		}

		sortedElements.clear();

		Visitor<T> visitor = new Visitor<T>() {
			@Override
			public void accept(T key) {
				sortedElements.add(key);
			}
		};

		inOrderTraversal(root, visitor);

		updated = true;
		return sortedElements;
	}

	private void inOrderTraversal(TreeNode<T> node, Visitor<T> visitor) {

		if (node.left != null) {
			inOrderTraversal(node.left, visitor);
		}

		visitor.accept(node.key);

		if (node.right != null) {
			inOrderTraversal(node.right, visitor);
		}

	}

	// this version does not use recursion.
	private void inOrderTraversal2(TreeNode<T> node, Visitor<T> visitor) {
		Stack<TreeNode<T>> stack = new Stack<>();
		stack.push(node);

		while (!stack.isEmpty()) {
			TreeNode<T> current = stack.pop();
			if (current.visited == true) {
				visitor.accept(current.key);
				current.visited = false;
			} else {
				if (current.right != null) {
					stack.push(current.right);
				}
				current.visited = true;
				stack.push(current);

				if (current.left != null) {
					stack.push(current.left);
				}
			}
		}
	}

	public void insert(T key) {

		TreeNode<T> node = new TreeNode<>(key);
		bstInsert(root, node);

		insertCase1NullParent(node);

		size += 1;
		// the tree has changes, need to updated it again
		updated = false;
	}

	public void insert(Collection<T> keys) {
		for (T key : keys) {
			insert(key);
		}
	}

	private void bstInsert(TreeNode<T> start, TreeNode<T> node) {

		if (start == null) {
			root = start = node;
		} else if (node.compareTo(start) < 0) {
			if (start.left == null) {
				start.left = node;
				node.parent = start;
			} else {
				bstInsert(start.left, node);
			}
		} else if (node.compareTo(start) > 0) {
			if (start.right == null) {
				start.right = node;
				node.parent = start;
			} else {
				bstInsert(start.right, node);
			}
		}
	}

	private void insertCase1NullParent(TreeNode<T> node) {
		if (node.parent == null) {
			node.color = Color.BLACK;
		} else {
			insertCase2BlackParent(node);
		}
	}

	private void insertCase2BlackParent(TreeNode<T> node) {
		if (node.parent.color == Color.BLACK) {
			return;
		} else {
			insertCase3RedUncle(node);
		}
	}

	// read parent, red uncle
	private void insertCase3RedUncle(TreeNode<T> node) {
		TreeNode<T> uncle = uncleNode(node);
		if (uncle != null && uncle.color == Color.RED) {
			node.parent.color = Color.BLACK;
			uncle.color = Color.BLACK;
			TreeNode<T> gp = grandParentNode(node);
			gp.color = Color.RED;
			insertCase1NullParent(gp);
		} else {
			insertCase4DiffSide(node);
		}

	}

	// red parent, black uncle/no uncle. Now it violates "children of red nodes
	// must
	// be black". We cannot simply change the color of the current node, as it
	// will violate property 5: from a node each path consists the same number
	// of
	// black nodes
	private void insertCase4DiffSide(TreeNode<T> node) {
		if (isLeftChild(node.parent) && isRightChild(node)) {
			leftRotate(node.parent);
			node = node.left;

		} else if (isRightChild(node.parent) && isLeftChild(node)) {
			rightRotate(node.parent);
			node = node.right;

		}

		insertCase5SameSide(node);
	}

	private void insertCase5SameSide(TreeNode<T> node) {
		TreeNode<T> gp = grandParentNode(node);
		node.parent.color = Color.BLACK;
		gp.color = Color.RED;

		if (isLeftChild(node)) {
			rightRotate(gp);
		} else {
			leftRotate(gp);
		}
	}

	TreeNode<T> grandParentNode(TreeNode<T> node) {
		if (node.parent != null && node.parent.parent != null) {
			return node.parent.parent;
		}

		return null;
	}

	TreeNode<T> uncleNode(TreeNode<T> node) {

		TreeNode<T> gp = grandParentNode(node);
		if (gp == null) {
			return null;
		}

		if (node.parent == gp.left) {
			return gp.right;
		} else {
			return gp.left;
		}
	}

	TreeNode<T> siblingNode(TreeNode<T> node) {
		assert (node != null);

		if (isLeftChild(node)) {
			return node.parent.right;
		} else {
			return node.parent.left;
		}
	}

	boolean isLeftChild(TreeNode<T> node) {
		return (node.parent != null && node == node.parent.left);
	}

	boolean isRightChild(TreeNode<T> node) {
		return (node.parent != null && node == node.parent.right);
	}

	TreeNode<T> leftRotate(TreeNode<T> sub) {

		TreeNode<T> savedParent = sub.parent;
		TreeNode<T> savedRight = sub.right;
		TreeNode<T> savedRightLeft = sub.right.left;

		if (savedParent == null) {
			savedRight.parent = null;
		} else {
			boolean left = isLeftChild(sub);
			if (left) {
				savedParent.left = savedRight;
			} else {
				savedParent.right = savedRight;
			}
			savedRight.parent = savedParent;
		}

		// sub.right now point to some thing else
		savedRight.left = sub;

		sub.parent = savedRight;
		sub.right = savedRightLeft;
		if (savedRightLeft != null) {
			savedRightLeft.parent = sub;
		}

		// rotate may change the root
		if (root == sub) {
			root = savedRight;
		}

		return savedRight;

	}

	// return the new root of the sub tree
	TreeNode<T> rightRotate(TreeNode<T> sub) {
		TreeNode<T> savedParent = sub.parent;
		TreeNode<T> savedLeft = sub.left;
		TreeNode<T> savedLeftRight = sub.left.right;

		if (savedParent == null) {
			savedLeft.parent = null;
		} else {
			boolean left = isLeftChild(sub);
			if (left) {
				savedParent.left = savedLeft;
			} else {
				savedParent.right = savedLeft;
			}
			savedLeft.parent = savedParent;
		}
		savedLeft.right = sub;

		sub.parent = savedLeft;
		sub.left = savedLeftRight;
		if (savedLeftRight != null) {
			savedLeftRight.parent = sub;
		}

		if (root == sub) {
			root = savedLeft;
		}

		return savedLeft;
	}

	TreeNode<T> inOrderPredecessor(TreeNode<T> node) {
		assert (node != null);

		TreeNode<T> predecessor = node.left;
		while (predecessor != null && predecessor.right != null) {
			predecessor = predecessor.right;
		}
		return predecessor;
	}

	TreeNode<T> inOrderSuccessor(TreeNode<T> node) {
		assert (node != null);
		TreeNode<T> successor = node.right;
		while (successor != null && successor.left != null) {
			successor = successor.left;
		}
		return successor;
	}

	static enum Color {
		RED, BLACK
	};

	static class TreeNode<T extends Comparable<T>> implements Comparable<TreeNode<T>> {
		TreeNode<T> parent = null;
		TreeNode<T> left = null;
		TreeNode<T> right = null;
		Color color = Color.RED;
		T key = null;
		
		boolean visited = false;

				
		public TreeNode() {

		}

		public TreeNode(T key) {
			this.key = key;
		}

		public TreeNode(TreeNode<T> parent) {
			this(parent, null, null, Color.BLACK);
		}

		public TreeNode(TreeNode<T> parent, TreeNode<T> left, TreeNode<T> right,
				Color color) {
			this.parent = parent;
			this.left = left;
			this.right = right;
			this.color = color;
		}

		@Override
		public int compareTo(TreeNode<T> other) {
			return key.compareTo(other.key);
		}

		@Override
		public String toString() {
			return key.toString() + "(" + color.toString() + ")" + ":"
					+ (left == null ? "null" : left.key) + ","
					+ (right == null ? "null" : right.key);
		}

	}

	// an interface allows customizing the operation while traversal the tree
	static interface Visitor<T> {
		public void accept(T key);
	}

}
