/***************************************************************
 * 
 * Author:          Daria Shubina
 * Date:            15/03/09
 * Compilation:     javac TwoSum.java
 * Execution:       java -ea TwoSum
 * Description:     Given an integer K and an array of integers,
 *                  find how many combination of two integers
 *                  (not necessarily distinct) sum up to K.
 *                  A[i] + A[j] = K: (i, j) and (j, i) 
 *                  count as two combinations.
 *                  A[i] + A[i] = K: (i, i) count as a one.
 *
/***************************************************************/                 


import java.util.Arrays;

public class TwoSum {
    public static int solution(int K, int[] A) {
        int n = A.length;
        if (n == 0) return 0;
        Arrays.sort(A);
        int[] values = new int[n];
        int[] counts = new int[n];
        values[0] = A[0];
        counts[0] = 1;
        int j = 0; //counter of unique elements
        for (int i = 1; i < n; ++i) {
            if (A[i] != values[j]) values[++j] = A[i];
            ++counts[j];
        }

        int lo = 0;
        int hi = j;
        int sumsCount = 0;
        while (lo <= hi) {
            int sum = values[lo] + values[hi];
            if (sum > K) --hi;
            else if (sum < K) ++lo;
            else {
                int coef = lo == hi ? 1 : 2;
                sumsCount += coef * counts[lo] * counts[hi];
                --hi;
                ++lo;
            }
        }
        return sumsCount;
    }
    public static void main(String[] args) {
        int[] a1 = new int[] {8, -2, 3, 5, 1, 1, 7, 4, 6};
        int[] a2 = new int[] {1, 1, 1, 1};
        int[] a3 = new int[] {1, 1, 1, 1, 2, 2, 2, 2};
        int[] a4 = new int[] {1, 1, 1, 1, 2, 2, 2, 2, -1, 4};
        int[] a5 = new int[] {-8, 2, -3, -5, -1, -1, -7, -4, -6};

        assert (solution(6, a1) == 7);
        assert (solution(2, a2) == 16);
        assert (solution(1, a3) == 0);
        assert (solution(2, a2) == 16);
        assert (solution(3, a3) == 32);
        assert (solution(3, a4) == 34);
        assert (solution(-6, a5) == 7);
    }
}
