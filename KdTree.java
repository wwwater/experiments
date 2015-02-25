/***********************************************************************************
 * Name:            Daria Shubina
 * Date:            15/02/25
 *
 * Compilation:     javac KdTree.java
 * Execution:       java KdTree.java <filename>
 * Dependencies:    StdDraw.java
 *
 * Description:     KdTree is a mutable data type that implements a 2d-tree,
 *                  which is a generalization of a BST to two-dimensional keys.
 *                  The idea is to build a BST with points in the nodes,
 *                  using the x- and y-coordinates of the points as keys 
 *                  in strictly alternating sequence. 
 **********************************************************************************/

public class KdTree {
    private int size;
    private KdNode root;
    
    public KdTree() { // construct an empty set of points
        this.size = 0;
    }
    public boolean isEmpty() { // is the set empty?
        return (this.size == 0);
    }
    public int size() { // number of points in the set 
        return this.size;
    }
    // add the point to the set (if it is not already in the set)
    public void insert(Point2D point) {
        boolean added = true;
        if (this.size == 0) 
            this.root = new KdNode(point, true, new RectHV(0.0, 0.0, 1.0, 1.0));
        else added = add(point, this.root, 0.0, 0.0, 1.0, 1.0);
        if (added) ++this.size;
    }
    public boolean contains(Point2D p) { // does the set contain point p?
        return containsIn(p, this.root); 
    }
    public void draw() { // draw all points to standard draw
        traverseAndDraw(this.root);
    }
     // all points that are inside the rectangle 
    public Iterable<Point2D> range(RectHV rect) {
        Queue<Point2D> points = new Queue<Point2D>(); 
        traverseForRange(rect, points, this.root);
        return points;
    }
    // a nearest neighbor in the set to point p; null if the set is empty 
    public Point2D nearest(Point2D p) {
        return traverseAndFind(p, null, Double.POSITIVE_INFINITY, this.root);
    }


    private class KdNode {
        private Point2D point;
        private boolean vertical;
        private KdNode left;
        private KdNode right;
        private RectHV rect;
        public KdNode(Point2D p, boolean vert, RectHV r) {
            this.point = p;
            this.vertical = vert;
            this.rect = r;
            this.left = null;
            this.right = null;
        }
    }

    private boolean add(Point2D point, KdNode node, 
                      double l, double b, double r, double t) {
        if (point.equals(node.point)) return false;
        boolean added = true;
        double left = l;
        double bottom = b;
        double right = r;
        double top = t;
        KdNode child = node.left;
        boolean leftChild = true;

        if (node.vertical) {
            if (point.x() < node.point.x()) 
                right = node.point.x();
            else { 
                left = node.point.x(); 
                leftChild = false;
                child = node.right;
            }
        } else {
            if (point.y() < node.point.y()) 
                top = node.point.y();
            else { 
                bottom = node.point.y(); 
                leftChild = false; 
                child = node.right;
            }
        }
        
        if (child == null) {
            KdNode x = new KdNode(point, !node.vertical,
                                  new RectHV(left, bottom, right, top));
            if (leftChild) node.left = x;
            else node.right = x;
        } else {
            added = add(point, child, left, bottom, right, top);
        }    
        return added;
    }
    private boolean containsIn(Point2D point, KdNode node) {
        if (node == null) return false;
        if (node.point.equals(point)) return true;
        if ((node.vertical && point.x() < node.point.x())
            || (!node.vertical && point.y() < node.point.y()))
            return containsIn(point, node.left);
        return containsIn(point, node.right);
    }
    private void traverseAndDraw(KdNode node) {
        if (node == null) return;
        StdDraw.setPenColor(StdDraw.BLACK);
        StdDraw.setPenRadius(0.005);
        node.point.draw();
        drawLine(node);
        if (node.left != null) traverseAndDraw(node.left);
        if (node.right != null) traverseAndDraw(node.right);
    }
    private void drawLine(KdNode node) {
        StdDraw.setPenRadius(0.0005);
        if (node.vertical) {
            StdDraw.setPenColor(StdDraw.RED);
            StdDraw.line(node.point.x(), node.rect.ymin(), 
                         node.point.x(), node.rect.ymax());
        } else {
            StdDraw.setPenColor(StdDraw.BLUE);
            StdDraw.line(node.rect.xmin(), node.point.y(), 
                         node.rect.xmax(), node.point.y());
        }
    }
    private void traverseForRange(RectHV rect, Queue<Point2D> points, KdNode node) {
        if (node == null) return;
        if (rect.contains(node.point)) points.enqueue(node.point);
        boolean goLeft = true;
        boolean goRight = true;
        if (node.vertical) {
            if (rect.xmax() < node.point.x()) goRight = false;
            if (rect.xmin() > node.point.x()) goLeft = false;
        } else {
            if (rect.ymax() < node.point.y()) goRight = false;
            if (rect.ymin() > node.point.y()) goLeft = false;
        }
        if (goRight) traverseForRange(rect, points, node.right);
        if (goLeft) traverseForRange(rect, points, node.left);
    }
    private Point2D traverseAndFind(Point2D point, Point2D cn,
                                    double cm, KdNode node) {
        double currentMin = cm;
        Point2D currentNearest = cn;
        if (node == null) return currentNearest;
        double dist = node.point.distanceSquaredTo(point);
        if (dist < currentMin) {
            currentMin = dist;
            currentNearest = node.point;
        }
        boolean goLeft = false;
        if ((node.vertical && (point.x() < node.point.x()))
        || (!node.vertical && (point.y() < node.point.y()))) goLeft = true;
        KdNode child = node.right;
        if (goLeft) child = node.left;
        if (child != null) currentNearest = traverseAndFind(point, currentNearest,
                                                            currentMin, child);

        //find out whether to go in the other direction too
        if (goLeft) child = node.right; //switch to the other side
        else child = node.left;
        currentMin = point.distanceSquaredTo(currentNearest);
        if (child != null && currentMin > child.rect.distanceSquaredTo(point))
            currentNearest = traverseAndFind(point, currentNearest, 
                                             currentMin, child);
        return currentNearest;
    }

    // unit testing of the methods (optional) 
    public static void main(String[] args) {
        KdTree t = new KdTree();
        
        In in = new In(args[0]);      // input file
        while (!in.isEmpty()) {
            double x = in.readDouble();
            double y = in.readDouble();
            t.insert(new Point2D(x, y));
        }
        StdOut.println("The KdTree of size " + t.size() + " is built.");
        t.draw();
        
        Point2D n = t.nearest(new Point2D(0.81, 0.3));
        StdDraw.setPenRadius(0.01);
        StdDraw.setPenColor(StdDraw.GREEN);
        n.draw();
        StdOut.println("Nearest: " + n.toString());
        
        /*
        for (double i = 0.0; i < 1.0; i += 0.1)
            for (double j = 0.0; j < 1.0; j += 0.1)
                //StdOut.println("Nearest to (" + i + ", " + j + ") is " + 
                               t.nearest(new Point2D(i, j));
        */
        
        RectHV r = new RectHV(0.5, 0.2, 0.7, 0.6);
        StdDraw.setPenRadius(0.005);
        StdDraw.setPenColor(StdDraw.GREEN);
        r.draw();
        StdDraw.setPenRadius(0.007);
        StdDraw.setPenColor(StdDraw.GRAY);
        int inRange = 0;
        for (Point2D p: t.range(r)) {
            ++inRange; 
            p.draw();
        }
        StdOut.println("There are " + inRange + " points in the rectangle.");
    }
}
