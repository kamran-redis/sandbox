package ky.trees;


public class BST<Key extends Comparable<Key>, Value> {
    private Node root;

    private class Node {
        private Key key;
        private Value val;
        private Node left, right;
        private int count;

        public Node(Key key, Value val) {
            this.key = key;
            this.val = val;
            this.count =1;
        }
    }

    public void put(Key key, Value val) {
        root = put(root, key, val);
    }

    private Node put(Node x, Key key, Value val) {
        if (x == null) return new Node(key, val);//bug

        int cmp = key.compareTo(x.key);
        if (cmp < 0) x.left = put(x.left, key, val);
        else if (cmp > 0) x.right = put(x.right, key, val);
        else  x.val = val;
        x.count = 1 + size(x.left) + size(x.right);
        return x;
    }

    public Value get(Key key) {
        Node x = root;
        while (x != null) {
            int cmp = key.compareTo(x.key);
            if (cmp < 0) x = x.left;
            else if (cmp > 0) x = x.right;
            else  return x.val;

        }
        return null;
    }

    public void delete(Key key) {
            root = delete(root, key);
    }

    private Node delete(Node x, Key key) {
        if (x == null ) return null;
        int cmp = key.compareTo(x.key);
        if (cmp < 0 ) x.left = delete(x.left, key);
        else if (cmp > 0) x.right = delete(x.right, key);
        else {
            if (x.right == null) return x.left;
            Node t = x;
            x = min(t.right);
            x.right = deleteMin(t.right);
            x.left = t.left;
        }
        x.count = 1 + size(x.left) + size(x.right);
        return x;
    }

    private Node deleteMin(Node x) {
        if (x.left == null) return x.right;
        x.left = deleteMin(x.left);
        x.count = 1 + size(x.left) + size(x.right);
        return x;
    }
    private Node min(Node x) {
        if (x.left == null) return x;
        else return min(x.left);
    }



    public int  size() {
        return size(root);
    }

    private int size(Node x) {
        if (x == null) return 0;
        return x.count;
    }

    public int rank(Key key) {
        return rank(key, root);
    }

    private int rank(Key key, Node x) {
        if (x == null) return 0;
        int cmp = key.compareTo(x.key);
        if (cmp < 0) return rank(key, x.left);
        else if (cmp > 0) return 1 + size(x.left) + rank(key, x.right);
        else  return size(x.left);

    }

    public void print() {
        inorder(root);
    }


    private void inorder(Node x) {
        if (x == null) return ;
        System.out.printf("%d:%s:%d\n",x.key,x.val,x.count);
        inorder(x.left);
        inorder(x.right);
    }

    public static void main(String[] argv) {
        BST<Integer, String> bst = new BST();
        bst.put(4, "four");
        bst.put(5, "five");
        bst.put(6, "six");
        bst.put(1, "one");
        bst.put(2, "two");
        bst.put(3, "three");

        System.out.println(bst.get(5));
        System.out.println(bst.size());
        bst.print();
        System.out.println(bst.size());
        System.out.println();
        bst.delete(4);
        bst.print();
        System.out.println(bst.size());
    }

}