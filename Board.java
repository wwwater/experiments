/***********************************************************************************
 * Name:            Daria Shubina
 * Date:            15/02/19
 *
 * Compilation:     javac Board.java
 * Execution:       java Board.java <filename>
 * Dependencies:    algs4.jar
 *
 * Description:     An auxilary class which implements the board and its properties 
 *                  in the 8- and 15-puzzle problem. 
 * **********************************************************************************/
public class Board {
    private long board;
    private long goal;
    private int N;
    private int zero;
    private int manh;
    private int ham;

    
    private int value(int idx) {
        return (int) ((this.board >> idx*4) & 15);
    }
    private long swapped(int i1, int i2) {
        long difference = ((this.board >> i1*4) ^ (this.board >> i2*4)) & 15;
        return this.board ^ (difference << i1*4) ^ (difference << i2*4);
    } 

    // construct a board from an N-by-N array of blocks
    // (where blocks[i][j] = block in row i, column j)
    public Board(int[][] blocks) {      
        this.N = blocks.length;
        if (this.N < 2 || this.N > 4) throw new IllegalArgumentException(" > 4");
        this.board = 0;
        this.goal = 0;

        int countM = 0;
        int countH = 0;
        int idx = this.N * this.N - 1;
        for (int i = this.N - 1; i >= 0;  --i) {
            for (int j = this.N - 1; j >= 0;  --j) {
                int val = blocks[i][j];
                if (val == 0) this.zero = idx;
                this.board = (this.board << 4) + val;
                if (idx > 0) this.goal = (this.goal << 4) + idx;
                
                if (val != (idx + 1) && val != 0) {
                    int col = (val - 1) % this.N; // goal-coordinates
                    int row = (val - col - 1) / this.N;
                    countM += Math.abs(row - i) + Math.abs(col - j);
                    ++countH; 
                }
                --idx;
            }
        }
        this.manh = countM;
        this.ham = countH;
    }
    private Board(int len, long longBoard, long g, int z) {
        this.board = longBoard;
        this.N = len;
        this.goal = g;
        this.zero = z;
        this.manh = -1;
        this.ham = -1;
    }

    public int dimension() { // board dimension N
        return this.N;
    }

    public int hamming() { // number of blocks out of place
        if (this.ham == -1) {
        int count = 0;
        int idx = 0;
        for (int i = 0; i < this.N; i++) {
            for (int j = 0; j < this.N; j++) {
                int val = (int)((this.board >> idx*4) & 15);
                if (val != ++idx && val != 0) ++count;
            }
        }
        this.ham = count;
        return count;
        }
        return this.ham;
    }
    public int manhattan() { // sum of Manhattan distances between blocks and goal
        if (this.manh == -1) {
        int count = 0;
        int idx = 0;
        for (int i = 0; i < this.N; i++) {
            for (int j = 0; j < this.N; j++) {
                int val = (int) ((this.board >> idx*4) & 15);
                if (val != ++idx && val != 0) {
                    int col = (val - 1) % this.N; // goal-coordinates
                    int row = (val - col - 1) / this.N;
                    count += Math.abs(row - i) + Math.abs(col - j);
                }
            }
        }
        this.manh = count;
        return count;
        }
        return this.manh;
    }
    public boolean isGoal() { // is this board the goal board?
        return (this.board == this.goal);
    }
    // a boadr that is obtained by exchanging two adjacent blocks in the same row
    public Board twin() {
        int n = this.N;
        if (this.zero > 1)
            return new Board(n, swapped(0, 1), this.goal, this.zero);
        return new Board(n, swapped(n*n - 1, n*n - 2), this.goal, this.zero);
    }
    public boolean equals(Object y) { // does this board equal y?
        if (y == null) return false;
        if (y.getClass() != this.getClass()) return false;
        Board that = (Board) (y);
        if (this == that) return true;
        if (this.N != that.dimension()) return false;
        return (this.board == that.board);
    }
    public Iterable<Board> neighbors() { // all neighboring boards
        Queue<Board> neighbors = new Queue<Board>();
        long g = this.goal;
        int z = this.zero;
        int col = z % this.N;
        int row = (z - col) / this.N;
        Board left, right, above, below;
        if (col > 0) { // left
            left = new Board(this.N, swapped(z, z - 1), g, z - 1);
            int val = this.value(z - 1);
            int realCol = (val - 1) % this.N;
            left.manh = this.manh - Math.abs(col - 1 - realCol) + Math.abs(col - realCol);
            neighbors.enqueue(left);
        }
        if (col < this.N - 1) { // right
            right = new Board(this.N, swapped(z, z + 1), g, z + 1);
            int val = this.value(z + 1);
            int realCol = (val - 1) % this.N;
            right.manh = this.manh - Math.abs(col + 1 - realCol) + Math.abs(col - realCol);
            neighbors.enqueue(right);
        }
        if (row > 0) { // above
            above = new Board(this.N, swapped(z, z - this.N), g, z - this.N);
            int val = this.value(z - this.N);
            int realCol = (val - 1) % this.N;
            int realRow = (val - realCol - 1) / this.N;
            above.manh = this.manh - Math.abs(row - 1 - realRow) + Math.abs(row - realRow);
            neighbors.enqueue(above);
        }
        if (row < this.N - 1) { // below
            below = new Board(this.N, swapped(z, z + this.N), g, z + this.N);
            int val = this.value(z + this.N);
            int realCol = (val - 1) % this.N;
            int realRow = (val - realCol - 1) / this.N;
            below.manh = this.manh - Math.abs(row + 1 - realRow) + Math.abs(row - realRow);
            neighbors.enqueue(below);
        }
        return neighbors;
    }
     // string representation of this board (in the output format specified below)
    public String toString() {
        String s = "" + this.N + '\n';
        for (int i = 0; i < this.N; i++) {
            for (int j = 0; j < this.N; j++)
                s += String.format("%4d ", (this.board >> (i*this.N + j)*4) & 15);
            s += '\n';
        }
        return s;
    }

    public static void main(String[] args) { // unit tests (not graded)
        // create initial board from file
        In in = new In(args[0]);
        int N = in.readInt();
        int[][] blocks = new int[N][N];
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                blocks[i][j] = in.readInt();
        Board initial = new Board(blocks);
        StdOut.println(initial.toString() + " " + initial.manhattan() + " " + initial.hamming());
        for (Board n: initial.neighbors()) 
            StdOut.println(n.toString() + " " + n.manhattan());
    }
}

