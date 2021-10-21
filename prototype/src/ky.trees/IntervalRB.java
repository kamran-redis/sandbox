package ky.trees;

//https://algs4.cs.princeton.edu/33balanced/RedBlackBST.java.html
//https://algs4.cs.princeton.edu/93intersection/IntervalST.java.html
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;
import java.nio.charset.StandardCharsets;
import java.util.LinkedList;
import java.util.Queue;

public class IntervalRB< Value> {
    private Node root;
    private static final boolean RED = true;
    private static final boolean BLACK = false;

    private static class Interval implements Comparable<Interval>{
        int lo,hi;

        Interval(int lo, int hi) {
            this.lo =lo;
            this.hi = hi;
        }
        public int compareTo(Interval that) {
            return Integer.compare(this.lo,that.lo);
        }

        public  boolean intersects(Interval b) {
                if (this.hi < b.lo) return false;
                if (b.hi < this.lo) return false;
                return true;
        }
        public String toString() {
            return "(" + lo + "," + hi + ")";
        }
    }



    private class Node {
        private Interval key;
        private Value val;
        private Node left, right;
        private int count;
        private boolean color;
        private int max;

        public Node(Interval key, Value val) {
            this.key = key;
            this.val = val;
            this.count = 1;
            this.max = key.hi;
        }

        public Node(Interval key, Value val, boolean color) {
            this(key, val);
            this.color = color;
        }
    }
//===============================================================================================================
    private boolean isRed(Node x) {
        if (x == null) return false;
        return x.color == RED;
    }

    private int maxValue(Node x) {
        if (x == null) return 0;
        return x.max;
    }
    private int max3(int a, int b, int c) {
        return Math.max(a, Math.max(b, c));
    }

    private Node rotateLeft(Node h) {
        assert isRed(h.right);
        Node x = h.right;
        h.right = x.left;
        x.left = h;
        x.color = h.color;
        h.color = RED;
        //h is child of x now , so update h first
        h.count = 1 + size(h.left) + size(h.right);
        h.max = max3(h.key.hi, maxValue(h.left), maxValue(h.right));
        x.count = 1 + size(x.left) + size(x.right);
        x.max = max3(x.key.hi, maxValue(x.left), maxValue(x.right));

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
        h.max = max3(h.key.hi, maxValue(h.left), maxValue(h.right));
        x.count = 1 + size(x.left) + size(x.right);
        x.max = max3(x.key.hi, maxValue(x.left), maxValue(x.right));

        return x;
    }

    private void flipColors(Node h) {

        h.color = !h.color;
        h.left.color = !h.left.color;
        h.right.color = !h.right.color;
    }


    private Node moveRedLeft(Node h) {
        flipColors(h);
        if (isRed(h.right.left)) {
            h.right = rotateRight(h.right);
            h = rotateLeft(h);
        }
        return h;
    }

    private Node moveRedRight(Node h) {
        flipColors(h);
        if (isRed(h.left.left))
            h = rotateRight(h);
        return h;
    }

    private Node balance(Node h) {
        if (isRed(h.right)) h = rotateLeft(h);
        if (isRed(h.left) && isRed(h.left.left)) h = rotateRight(h);
        if (isRed(h.left) && isRed(h.right)) flipColors(h);
        h.count = size(h.left) + size(h.right) + 1;
        h.max = max3(h.key.hi, maxValue(h.left), maxValue(h.right));
        return h;
    }


//===============================================================================================================
    public void put(Interval key, Value val) {
        root = put(root, key, val);
        root.color = BLACK;
    }

    private Node put(Node x, Interval key, Value val) {
        if (x == null) return new Node(key, val, RED);

        int cmp = key.compareTo(x.key);
        if (cmp < 0) x.left = put(x.left, key, val);
        else if (cmp > 0) x.right = put(x.right, key, val);
        else x.val = val;
        x.count = 1 + size(x.left) + size(x.right);
        x.max = max3(x.key.hi, maxValue(x.left), maxValue(x.right));

        if (isRed(x.right) && !isRed(x.left)) x = rotateLeft(x);
        if (isRed(x.left) && isRed(x.left.left)) x = rotateRight(x);
        if (isRed(x.left) && isRed(x.right)) flipColors(x);

        return x;
    }

    public Value get(Interval key) {
        Node x = root;
        while (x != null) {
            int cmp = key.compareTo(x.key);
            if (cmp < 0) x = x.left;
            else if (cmp > 0) x = x.right;
            else return x.val;

        }
        return null;
    }

    public boolean contains(Interval key) {
        return get(key) != null;
    }

    public Interval min() {

        return min(root).key;
    }
    private Node min(Node x) {
        if (x.left == null) return x;
        else return min(x.left);
    }

    public Interval max() {

        return max(root).key;
    }

    // the largest key in the subtree rooted at x; null if no such key
    private Node max(Node x) {
        // assert x != null;
        if (x.right == null) return x;
        else  return max(x.right);
    }


    public int size() {
        return size(root);
    }

    private int size(Node x) {
        if (x == null) return 0;
        return x.count;
    }


    public int rank(Interval key) {
        return rank(key, root);
    }

    private int rank(Interval key, Node x) {
        if (x == null) return 0;
        int cmp = key.compareTo(x.key);
        if (cmp < 0) return rank(key, x.left);
        else if (cmp > 0) return 1 + size(x.left) + rank(key, x.right);
        else return size(x.left);

    }


    public void delete(Interval key) {
        if (!isRed(root.left) && !isRed(root.right))
            root.color = RED;
        root = delete(root, key);
        if (!isEmpty()) root.color = BLACK;
    }

    private Node delete(Node h, Interval key) {
        if (key.compareTo(h.key) < 0) {
            if (!isRed(h.left) && !isRed(h.left.left))
                h = moveRedLeft(h);
            h.left = delete(h.left, key);
        } else {
            if (isRed(h.left))
                h = rotateRight(h);
            if (key.compareTo(h.key) == 0 && (h.right == null))
                return null;
            if (!isRed(h.right) && !isRed(h.right.left))
                h = moveRedRight(h);
            if (key.compareTo(h.key) == 0) {
                Node x = min(h.right);
                h.key = x.key;
                h.val = x.val;
                h.right = deleteMin(h.right);
            } else h.right = delete(h.right, key);
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

    public void deleteMax() {
        if (!isRed(root.left) && !isRed(root.right))
            root.color = RED;

        root = deleteMax(root);
        if (!isEmpty()) root.color = BLACK;
    }

    private Node deleteMax(Node h) {
        if (isRed(h.left))
            h = rotateRight(h);

        if (h.right == null)
            return null;

        if (!isRed(h.right) && !isRed(h.right.left))
            h = moveRedRight(h);

        h.right = deleteMax(h.right);
        return balance(h);
    }


    public Interval select(int k) {
        if (k < 0 || k >= size()) {
            throw new IllegalArgumentException("argument to select() is invalid: " + k);
        }
        Node x = select(root, k);
        return x.key;
    }

    // the key of rank k in the subtree rooted at x
    private Node select(Node x, int k) {
        // assert x != null;
        // assert k >= 0 && k < size(x);
        int t = size(x.left);
        if      (t > k) return select(x.left,  k);
        else if (t < k) return select(x.right, k-t-1);
        else            return x;
    }

    /***************************************************************************
     *  Range count and range search.
     ***************************************************************************/

    /**
     * Returns all keys in the symbol table as an {@code Iterable}.
     * To iterate over all of the keys in the symbol table named {@code st},
     * use the foreach notation: {@code for (Key key : st.keys())}.
     * @return all keys in the symbol table as an {@code Iterable}
     */
    public Iterable<Interval> keys() {
        if (isEmpty()) return new LinkedList<>();
        return keys(min(), max());
    }

    /**
     * Returns all keys in the symbol table in the given range,
     * as an {@code Iterable}.
     *
     * @param  lo minimum endpoint
     * @param  hi maximum endpoint
     * @return all keys in the symbol table between {@code lo}
     *    (inclusive) and {@code hi} (inclusive) as an {@code Iterable}
     * @throws IllegalArgumentException if either {@code lo} or {@code hi}
     *    is {@code null}
     */
    public Iterable<Interval> keys(Interval lo, Interval hi) {
        if (lo == null) throw new IllegalArgumentException("first argument to keys() is null");
        if (hi == null) throw new IllegalArgumentException("second argument to keys() is null");

        Queue<Interval> queue = new LinkedList<>();
        // if (isEmpty() || lo.compareTo(hi) > 0) return queue;
        keys(root, queue, lo, hi);
        return queue;
    }

    // add the keys between lo and hi in the subtree rooted at x
    // to the queue
    private void keys(Node x, Queue<Interval> queue, Interval lo, Interval hi) {
        if (x == null) return;
        int cmplo = lo.compareTo(x.key);
        int cmphi = hi.compareTo(x.key);
        if (cmplo < 0) keys(x.left, queue, lo, hi);
        if (cmplo <= 0 && cmphi >= 0) queue.add(x.key);
        if (cmphi > 0) keys(x.right, queue, lo, hi);
    }

    /**
     * Returns the number of keys in the symbol table in the given range.
     *
     * @param  lo minimum endpoint
     * @param  hi maximum endpoint
     * @return the number of keys in the symbol table between {@code lo}
     *    (inclusive) and {@code hi} (inclusive)
     * @throws IllegalArgumentException if either {@code lo} or {@code hi}
     *    is {@code null}
     */
    public int size(Interval lo, Interval hi) {
        if (lo == null) throw new IllegalArgumentException("first argument to size() is null");
        if (hi == null) throw new IllegalArgumentException("second argument to size() is null");

        if (lo.compareTo(hi) > 0) return 0;
        if (contains(hi)) return rank(hi) - rank(lo) + 1;
        else              return rank(hi) - rank(lo);
    }
    /**
     * Returns the height of the BST (for debugging).
     * @return the height of the BST (a 1-node tree has height 0)
     */
    public int height() {
        return height(root);
    }
    private int height(Node x) {
        if (x == null) return -1;
        return 1 + Math.max(height(x.left), height(x.right));
    }

    /***************************************************************************
     *  Interval searching
     ***************************************************************************/

    // return an interval in data structure that intersects the given inteval;
    // return null if no such interval exists
    // running time is proportional to log N
    public Interval search(Interval interval) {
        return search(root, interval);
    }

    // look in subtree rooted at x
    public Interval search(Node x, Interval interval) {
        while (x != null) {
            if (interval.intersects(x.key)) return x.key;
            else if (x.left == null)             x = x.right;
            else if (x.left.max < interval.lo)  x = x.right;
            else  x = x.left;
        }
        return null;
    }


    // return *all* intervals in data structure that intersect the given interval
    // running time is proportional to R log N, where R is the number of intersections
    public Iterable<Interval> searchAll(Interval interval) {
        LinkedList<Interval> list = new LinkedList<Interval>();
        searchAll(root, interval, list);
        return list;
    }

    // look in subtree rooted at x
    public boolean searchAll(Node x, Interval interval, LinkedList<Interval> list) {
        boolean found1 = false;
        boolean found2 = false;
        boolean found3 = false;
        if (x == null)
            return false;
        if (interval.intersects(x.key)) {
            list.add(x.key);
            found1 = true;
        }
        if (x.left != null && x.left.max >= interval.lo)
            found2 = searchAll(x.left, interval, list);
        if (found2 || x.left == null || x.left.max < interval.hi)
            found3 = searchAll(x.right, interval, list);
        return found1 || found2 || found3;
    }

//=====================================================================
    /***************************************************************************
     *  Check integrity of red-black tree data structure.
     ***************************************************************************/
    private boolean check() {
        if (!isBST())            System.out.println("Not in symmetric order");
        if (!isSizeConsistent()) System.out.println("Subtree counts not consistent");
        if (!isRankConsistent()) System.out.println("Ranks not consistent");
        if (!isMaxConsisitent()) System.out.println("Max not consistent");
        if (!is23())             System.out.println("Not a 2-3 tree");
        if (!isBalanced())       System.out.println("Not balanced");
        return isBST() && isSizeConsistent() && isRankConsistent() && is23() && isBalanced();
    }

    // does this binary tree satisfy symmetric order?
    // Note: this test also ensures that data structure is a binary tree since order is strict
    private boolean isBST() {
        return isBST(root, null, null);
    }

    // is the tree rooted at x a BST with all keys strictly between min and max
    // (if min or max is null, treat as empty constraint)
    // Credit: Bob Dondero's elegant solution
    private boolean isBST(Node x, Interval min, Interval max) {
        if (x == null) return true;
        if (min != null && x.key.compareTo(min) <= 0) return false;
        if (max != null && x.key.compareTo(max) >= 0) return false;
        return isBST(x.left, min, x.key) && isBST(x.right, x.key, max);
    }

    // are the size fields correct?
    private boolean isSizeConsistent() { return isSizeConsistent(root); }
    private boolean isSizeConsistent(Node x) {
        if (x == null) return true;
        if (x.count != size(x.left) + size(x.right) + 1) return false;
        return isSizeConsistent(x.left) && isSizeConsistent(x.right);
    }

    // check that ranks are consistent
    private boolean isRankConsistent() {
        for (int i = 0; i < size(); i++)
            if (i != rank(select(i))) return false;
        for (Interval key : keys())
            if (key.compareTo(select(rank(key))) != 0) return false;
        return true;
    }



    // Does the tree have no red right links, and at most one (left)
    // red links in a row on any path?
    private boolean is23() { return is23(root); }
    private boolean is23(Node x) {
        if (x == null) return true;
        if (isRed(x.right)) return false;
        if (x != root && isRed(x) && isRed(x.left))
            return false;
        return is23(x.left) && is23(x.right);
    }

    private boolean isMaxConsisitent() { return isMaxConsisitent(root); }
    private boolean isMaxConsisitent(Node x) {
        if (x == null) return true;
        if (x.max !=  max3(x.key.hi, maxValue(x.left), maxValue(x.right))) return false;
        return isMaxConsisitent(x.left) && isMaxConsisitent(x.right);
    }

    // do all paths from root to leaf have same number of black edges?
    private boolean isBalanced() {
        int black = 0;     // number of black links on path from root to min
        Node x = root;
        while (x != null) {
            if (!isRed(x)) black++;
            x = x.left;
        }
        return isBalanced(root, black);
    }

    // does every path from the root to a leaf have the given number of black links?
    private boolean isBalanced(Node x, int black) {
        if (x == null) return black == 0;
        if (!isRed(x)) black--;
        return isBalanced(x.left, black) && isBalanced(x.right, black);
    }

//======================================================================

    public String toString() {
        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        PrintStream ps;
        try {
            ps = new PrintStream(baos, true, "UTF-8");
            inorder(root, ps, "", "");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        return new String(baos.toByteArray(), StandardCharsets.UTF_8);
    }


    private void inorder(Node x, PrintStream ps, String padding, String prefix) {
        if (x == null) return;
        ps.printf(padding + prefix + " %s:%d:%d\n", x.key, x.count, x.max);
        inorder(x.left, ps, padding + "\t", "L");
        inorder(x.right,ps, padding + "\t", "R");
    }

    public static void main(String[] argv) {

        testDelete();

//        int N = 100000;
//
//        // generate N random intervals and insert into data structure
//        IntervalRB<String> rb = new IntervalRB<>();
//
//        for (int i = 0; i < N; i++) {
//            int lo = (int) (Math.random() * N);
//            int hi = (int) (Math.random() * 50) + lo;
//            Interval interval = new Interval(lo, hi);
//            System.out.println(interval);
//            rb.put(interval, "" + i);
//        }
//
//        // print out tree statistics
//        System.out.println("height:          " + rb.height());
//        System.out.println("size:            " + rb.size());
//        System.out.println("integrity check: " + rb.check());
//        System.out.println();



        // generate random intervals and check for overlap
        /*for (int i = 0; i < N; i++) {
            int lo = (int) (Math.random() * 100);
            int hi = (int) (Math.random() * 10) + lo;
            Interval interval = new Interval(lo, hi);
            System.out.println(interval + ":  " + rb.search(interval));
            System.out.print(interval + ":  ");
            for (Interval x : rb.searchAll(interval))
                System.out.print(x + " ");
            System.out.println();
            System.out.println();
        }*/


    }

    static void testDelete() {
        IntervalRB<String> rb = new IntervalRB<>();
        rb.put(new Interval(5, 10), "" +5);
        rb.put(new Interval(15, 20), "" +15);
        System.out.println(rb);

        rb.delete(new Interval(5, 10));
        System.out.println("********************");
        System.out.println(rb);
        rb.delete(new Interval(15, 20));
        System.out.println("********************");
        System.out.println(rb);


    }


}