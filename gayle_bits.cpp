#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>

int insertMinN(int N, int M, int i, int j)
{
    return (M<<i) | (N & (~0<<(j + 1))) | (N & ((1<<i) - 1));
}


bool getBit(int num, int i)
{
    return (num & (1<<i)) == (1<<i);     
}

void setBit(int &num, int i, int val)
{
    num = (num & ~(1<<i)) | (val<<i);
}

void mantissaOfDouble(double d)
{   
    std::cout << d << " -> ";
    std::vector<int> mantissa;
    int count = 32;
    while(d != 0 && count-- > 0) {
        d *= 2;
        if (d - 1.0 > -1e-32) {
            mantissa.push_back(1);
            d -= 1.0;
        } else {
            mantissa.push_back(0);
        }
    }
    if (d == 0) {
        std::cout << "0.";
        for (auto i: mantissa) std::cout << i;
    }
    else std::cout << "Error!";
    std::cout << '\n';
}

int nextSameNumOfBits(int n) 
{
    int count = -1;
    for (int i = 0; i < 32; ++i) {
        if (getBit(n, i)) {
            setBit(n, i, 0);
            ++count;
            if (!getBit(n, i + 1)) {
                setBit(n, i + 1, 1);
                while (count-- > 0) {
                    setBit(n, count, 1);
                }
                break;
            }
        }
    }
    return n;
}

int prevSameNumOfBits(int n)
{
    int i = 0;
    int msb = 0;
    int n2 = n;
    int count = 0;
    while (n2 >>= 1) ++msb;
    while (i <= msb) {
        if (getBit(n, i)) {
            ++count;
            if (i > 0 && !getBit(n, i - 1)) {
                setBit(n, i, 0);
                while (count-- > 0) {
                    setBit(n, --i, 1);
                }
                while (i-- >= 0) {
                    setBit(n, i, 0);
                }
                break;
            }
        }
        ++i;
    }
    return n;
}

int bitFlips(int a, int b)
{
    int x = a ^ b;
    int flips = (x % 2 == 1) ? 1 : 0;
    while(x >>= 1) {
        if (x % 2 == 1)  ++flips;
    }
    return flips;
}

int swapEvensOdds(int n)
{
    int n2 = n;
    int mask = 1;
    while (n2 >>= 1) {
        mask <<= 2;
        mask += 1;
    }
    int odds = n & (mask << 1);
    int evens = n & mask;
    return (odds >> 1) | (evens << 1);
}

int main()
{
    using std::cout;
    int N = 31;
    int M = 0;
    //std::cout << insertMinN(N, M, 2, 3) << '\n';
    /*mantissaOfDouble(0.75);
    mantissaOfDouble(1.0/4.0);
    mantissaOfDouble(1.0/256.0);
    mantissaOfDouble(1.0/3.0);*/
    assert (nextSameNumOfBits(5) == 6);
    assert (nextSameNumOfBits(3) == 5);
    assert (nextSameNumOfBits(0) == 0);
    assert (nextSameNumOfBits(14) == 19);
    assert (prevSameNumOfBits(5) == 3);
    assert (prevSameNumOfBits(3) == 3);
    assert (prevSameNumOfBits(6) == 5);
    assert (prevSameNumOfBits(49) == 44);

    std::cout << bitFlips(0, 7) << '\n';

    std::cout << swapEvensOdds(7) << '\n';

}
