/***********************************************************************************
 * Name:            Daria Shubina
 * Date:            15/02/19
 *
 * Compilation:     javac Solver.java
 * Execution:       java Solver.java <filename>
 * Dependencies:    algs4.jar Board.java
 *
 * Description:     A program to solve the 8- and 15-puzzle problem 
 *                  using the A* search algorithm. 
 * **********************************************************************************/

public class Solver {
    private class SearchNode implements Comparable<SearchNode> {
        private Board board;
        private int moves;
        private SearchNode previous;
        public SearchNode(Board board, int moves, SearchNode previous) {
            this.board = board;
            this.moves = moves;
            this.previous = previous;
        }
        public int priority() {
            return this.board.manhattan() + this.moves;        
        }
        public int compareTo(SearchNode that) {
            return this.priority() - that.priority();
        }
        public String toString() {
            String s = "";
            s += "priority = " + this.priority();
            s += "\nmoves = " + this.moves;
            s += "\nmanhattan = " + this.board.manhattan();
            s += "\n" + this.board.toString();
            return s;
        }
    }
    private int moves;
    private Stack<Board> solution;
    
    // find a solution to the initial board (using the A* algorithm)
    public Solver(Board initial) { 
        if (initial == null) throw new java.lang.NullPointerException("");
        Board twin = initial.twin();
        MinPQ<SearchNode> pq = new MinPQ<SearchNode>();
        pq.insert(new SearchNode(initial, 0 , null));
        pq.insert(new SearchNode(twin, 0 , null));
        SearchNode current = null;
        int step = 0;
        while (!pq.isEmpty()) {
            current = pq.delMin();
            if (!pq.isEmpty()) {
                SearchNode next = pq.min();
                if (current.board.manhattan() == next.board.manhattan()) {
                    if (next.board.hamming() < current.board.hamming()) {
                        next = pq.delMin();
                        pq.insert(current);
                        current = next;
                    }
                }
            }
            if (current.board.isGoal()) break;
            for (Board neighbor: current.board.neighbors()) {
                if (current.previous == null
                    || !neighbor.equals(current.previous.board))
                    pq.insert(
                        new SearchNode(neighbor, current.moves + 1, current));    
            }
            //StdOut.println("Step: " + ++step);
            //for (SearchNode sn: pq) StdOut.println(sn.toString());
        }
        this.moves = current.moves;
        this.solution = new Stack<Board>();
        while (!current.board.equals(initial) && !current.board.equals(twin)) {
            this.solution.push(current.board);
            current = current.previous;
        }
        this.solution.push(initial);
        if (current.board.equals(twin)) {
            this.solution = null;
            this.moves = -1;
        }   
    }
    
    public boolean isSolvable() { // is the initial board solvable?
        return this.solution != null;
    }
    // min number of moves to solve initial board; -1 if unsolvable
    public int moves() {
        return this.moves;
    }
    // sequence of boards in a shortest solution; null if unsolvable*/
    public Iterable<Board> solution() { 
        return this.solution;
    }
    public static void main(String[] args) { // solve a slider puzzle (given below)
        In in = new In(args[0]);
        int N = in.readInt();
        int[][] blocks = new int[N][N];
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                blocks[i][j] = in.readInt();
        Board initial = new Board(blocks);
        //StdOut.println(initial.toString());
        Solver s = new Solver(initial);
        if (!s.isSolvable()) {
            StdOut.println("No solution possible");
        } else {
            StdOut.println("Minimum number of moves = " + s.moves());
            for (Board b: s.solution()) StdOut.println(b.toString());
        }
    }
}
