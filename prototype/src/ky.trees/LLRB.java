package ky.trees;


public class LLRB<Key extends Comparable<Key>, Value> {
    private Node root;
    private static final boolean RED= true;
    private static final boolean BLACK= false;
    private class Node {
        private Key key;
        private Value val;
        private Node left, right;
        private int count;
        private boolean color;

        public Node(Key key, Value val) {
            this.key = key;
            this.val = val;
            this.count =1;
        }
        public Node(Key key, Value val,boolean color) {
            this(key, val);
            this.color = color;
        }

    }

    private boolean isRed(Node x) {
        if (x== null) return false;
        return x.color == RED;
    }

    private Node rotateLeft(Node h) {
        assert isRed(h.right);
        Node x = h.right;
        h.right = x.left;
        x.left =h;
        x.color = h.color;
        h.color = RED;
        //h is child of x now , so update h first
        h.count = 1 + size(h.left) + size(h.right);
        x.count = 1 + size(x.left) + size(x.right);

        return x;

    }

    private Node rotateRight(Node h) {
        assert isRed(h.left);
        Node x = h.left;
        h.left = x.right;
        x.right = h;
        x.color = h.color;
        h.color = RED;
        //h is child of x now , so update h first
        h.count = 1 + size(h.left) + size(h.right);
        x.count = 1 + size(x.left) + size(x.right);

        return x;
    }

    private void flipColors(Node h) {

        h.color = !h.color;
        h.left.color = !h.left.color;
        h.right.color = !h.right.color;
    }


    private Node moveRedLeft(Node h)
    {
        flipColors(h);
        if (isRed(h.right.left))
        {
            h.right = rotateRight(h.right);
            h = rotateLeft(h);
        }
        return h;
    }

    private Node moveRedRight(Node h)
    {
        flipColors(h);
        if (isRed(h.left.left))
            h = rotateRight(h);
        return h;
    }

    public void put(Key key, Value val) {
        root = put(root, key, val);
        root.color = BLACK;
    }

    private Node put(Node x, Key key, Value val) {
        if (x == null) return new Node(key, val,RED);

        int cmp = key.compareTo(x.key);
        if (cmp < 0) x.left = put(x.left, key, val);
        else if (cmp > 0) x.right = put(x.right, key, val);
        else  x.val = val;
        x.count = 1 + size(x.left) + size(x.right);

        if (isRed(x.right) && !isRed(x.left)) x = rotateLeft(x);
        if (isRed(x.left) && isRed(x.left.left)) x = rotateRight(x);
        if (isRed(x.left) && isRed(x.right)) flipColors(x);

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


    public void delete(Key key)
    {
        if (!isRed(root.left) && !isRed(root.right))
            root.color = RED;
        root = delete(root, key);
        if (!isEmpty()) root.color = BLACK;
    }

    private Node delete(Node h, Key key)
    {
        if (key.compareTo(h.key) < 0)
        {
            if (!isRed(h.left) && !isRed(h.left.left))
                h = moveRedLeft(h);
            h.left =  delete(h.left, key);
        }
        else
        {
            if (isRed(h.left))
                h = rotateRight(h);
            if (key.compareTo(h.key) == 0 && (h.right == null))
                return null;
            if (!isRed(h.right) && !isRed(h.right.left))
                h = moveRedRight(h);
            if (key.compareTo(h.key) == 0)
            {
                Node x = min(h.right);
                h.key = x.key;
                h.val = x.val;
                h.right = deleteMin(h.right);
            }
            else h.right = delete(h.right, key);
        }

        return balance(h);
    }

    public boolean isEmpty() {
        return root == null;
    }

    public void deleteMin() {
        if (!isRed(root.left) && !isRed(root.right))
            root.color = RED;
        root = deleteMin(root);
        if (!isEmpty()) root.color = BLACK;
    }


    private Node deleteMin(Node h) {
        if (h.left == null)
            return null;
        if (!isRed(h.left) && !isRed(h.left.left))
            h = moveRedLeft(h);
        h.left = deleteMin(h.left);
        return balance(h);
    }

    private Node balance(Node h)
    {
        if (isRed(h.right))                      h = rotateLeft(h);
        if (isRed(h.left) && isRed(h.left.left)) h = rotateRight(h);
        if (isRed(h.left) && isRed(h.right))     flipColors(h);
        h.count = size(h.left) + size(h.right) + 1;
        return h;
    }


    public void print() {
        inorder(root, "", "");
    }


    private void inorder(Node x, String padding, String prefix) {
        if (x == null) return ;
        System.out.printf(padding + prefix + " %d:%d:%b\n",x.key,x.count,x.color);
        inorder(x.left, padding+"\t", "L");
        inorder(x.right,padding+"\t", "R");
    }

    public static void main(String[] argv) {
        LLRB<Integer, String> bst = new LLRB();
        for (int i =0; i <20; i++) {
            bst.put(i, Integer.toString(i));
        }


        System.out.println(bst.get(5));
        bst.print();
        System.out.println(bst.size());
        System.out.println();
        bst.delete(4);
        bst.print();
        System.out.println(bst.size());
    }

}