//
// Created by anvesh on 4/1/26.
//

#include <algorithm>
#include <iostream>
#include <vector>
#include <ranges>
#include <cmath>

class Solution
{
public:
    /*
     * Approach 1: Bruteforce
     * We iterate through every number from 2 up to n-1;
     * For each number, you check if it's prime.
     * To check if a num is prim
     */
    static int count_primes_bruteforce_cpp17(int n)
    {
        if (n <= 2) return 0; // neg num, 0, 1
        int count = 1; //starting at 1 since 2 is prime
        for (int i = 3; i < n; i += 2)
        {
            if (isPrime(i)) ++ count;
        }
        return count;
    }
    static int count_primes_bruteforce_cpp20(int n)
    {
        if (n <= 2) return 0;
        // creating view of numbers from 3 -> n-1, filtering to keep only odd number
        auto odd_numbers = std::views::iota(3, n)
                            | std::views::filter([](const int x){ return x%2 != 0;});
        // count how many nums in our view satisfy isPrime condition
        // we add 1 at end to account for prime number 2
        return 1 + std::ranges::count_if(odd_numbers, isPrime);
    }
    /*
     * Approach 2: Sieve of Eratosthenes
     *
     */
    static int count_primes_optimised_cpp17(int n)// 65ms
    {
        if (n <= 2) return 0;
        // c++ std::vector<bool> is space-optimised bitset; packs 8 booleans into single byte
        // For n = 5,000,000 this takes ~610 kb, allowing entire array to fit in mordern cpu L2/L3 cache.
        // Memory latency vanishes
        std::vector<bool> isPrime(n, true);
        // pre count initial primes, we assume 2 and all odd nums are prime
        int count = n/2;
        for (long long p = 3; p*p < n; p += 2)
        {
            if (isPrime[p])
            {
                for (long long i = p*p; i < n; i+= 2*p)
                {
                    if (isPrime[i])
                    {
                        isPrime[i] = false; count--;
                    }
                }
            }
        }
        return count;
    }
    static constexpr int count_primes_optimised_cpp20(int n) // 62 ms
    {
        if (n <= 2) return 0;
        std::vector<char> is_prime(n, 1);
        int count = n/2;

        for (long long p = 3; p*p < n; p += 2)
        {
            if (is_prime[p])
            {
                for (long long i = p*p; i<n; i+= 2*p)
                {
                    if (is_prime[i])
                    {
                        is_prime[i] = 0; count--;
                    }
                }
            }
        }
        return count;
    }

private:
    // extremely optimised bruteforce primality test (6k +/- 1 rule)
    static bool isPrime(const int n)
    {
        if (n <= 1) return false;
        if (n <= 3) return true; // 2, 3 are primes
        if (n % 2 == 0 || n % 3 == 0) return false;
        for (int i = 5; i*i <= n; i += 6) {if (n % i == 0 || n % (i + 2) == 0) return false;}
        return true;
    }
};

// LC 7 ms solution
using namespace  std;
class SolutionLC {
public:
    using ll = long long;
    static ll count_primes(ll n) {
        vector<ll> v;
        for (ll k = 1; k * k <= n; ++k) {
            v.push_back(n / k);
            v.push_back(k);
        }
        sort(v.begin(), v.end());
        v.erase(unique(v.begin(), v.end()), v.end());

        // return i such that v[i] = x
        // since v[i] = i + 1 for i <= sqrt(n) and v[v.size() - i] = n / i for i <= sqrt(n),
        // we can calculate index in O(1)
        ll sq = sqrt(n);
        auto geti = [&](ll x) {
            if (x <= sq) return (int)x - 1;
            else         return (int)(v.size() - (n / x));
        };

        vector<ll> dp(v.size());

        // S(n, 0) = n
        for (int i = 0; i < v.size(); ++i)
            dp[i] = v[i];

        int a = 0;
        for (ll p = 2; p * p <= n; ++p) {
            // this condition is true for primes
            if (dp[geti(p)] != dp[geti(p - 1)]) {
                ++a;
                for (int i = (int)v.size() - 1; i >= 0; --i) {
                    if (v[i] < p * p) break;
                    dp[i] -= dp[geti(v[i] / p)] - a;
                }
            }
        }

        return dp[geti(n)] - 1;
    }

    int countPrimes(int n) {
        if (n <= 2) return 0;
        return count_primes((ll)n-1);
    }
};

// estimates sub millisecond 0-1 ms (will test it!)
class SolutionLC_cpp23
{
public:
    static int countPrimes(int n)
    {
        // base case; lc expects strictly less than n
        if (n <= 2) return 0;
        n = n-1;
        const int sq = std::sqrt(n);

        // we eliminate the dynamically sized vector, std::sort and std::unique.
        // instead we allocate exactly two perfectly contiguous arrays.
        // size for N=5_000_000 is ~2237 ints. Takes ~8KB memory total.
        std::vector<int> small(sq + 1),large(sq + 1); // tracks primes <= n / i

        // initialise our states; assums all nums >= 2 are primes
        for (int i = 1; i <= sq; ++i)
        {
            small[i] = i -1;
            large[i] = (n/i) - 1;
        }

        // core lucy hedgehog DP mathematics
        for (int p = 2; p<=sq; ++p)
        {
            // if count has not changed from p-1 , p was crossed out. It's composite
            if (small[p] == small[p-1]) continue;

            int pc = small[p-1]; // primes counted strictly before p
            long long q = (long long)p * p;
            int end_i = std::min((long long)sq, n/q);

            // Branchless loop split
            // Instead of putting an `if (i*p <= sq)` inside loop, we calc the maths border beforehand
            // and split it into two independent loops
            int border = sq/p;
            int limit1 = std::min(end_i, border);

            // Loop 1 : conditions wehere (i*p) <= sq
            for (int i = 1; i<=limit1; ++i) large[i] -= large[i*p] - pc;
            // Loop 2 : conditions where (i*p) > sq
            for (int i = limit1+1; i<=end_i; ++i)
            {
                int w = (n/i) / p;
                large[i] -= small[w] - pc;
            }

            // update the small array
            // we loop backwards strictly because we need the data from the p-1 iteration
            for (int i = sq; i >= q; --i) small[i] -= small[i/p] - pc;
        }

        // large[1] tracks the number of primes <= n /1 , which means all primes!
        return large[1];


    }
};

// achives 2 ms !
class SolutionLC_cpp23_obfus {
public:
    static int countPrimes(int n) {
        if(n <= 2) return 0;
        n = n - 1;
        const int sq = std::sqrt(n);
        std::vector<int> small(sq+1), large(sq+1);
        for(int i = 1; i <= sq; ++i) { small[i] = i - 1; large[i] = (n/i) - 1; }
        for(int p = 2; p <= sq; ++p) {
            if (small[p] == small[p-1]) continue;
            long long q = (long long)p*p;
            int pc = small[p-1], end_i = std::min((long long)sq, n/q);
            int border = sq/p, limit1 = std::min(end_i, border);
            for(int i = 1; i <= limit1; ++i) large[i] -= large[i*p] - pc;
            for(int i = limit1+1; i <= end_i; ++i)
            { int w = (n/i)/p; large[i] -= small[w] - pc; }
            for(int i = sq; i >= q; --i) small[i] -= small[i/p] - pc;
        }
        return large[1];
    }
};

// The Absolute Limit (0ms - 1ms on LC)
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")

class SolutionLC_cpp23_Ultimate {
public:
    static int countPrimes(int n) {
        if (n <= 2) return 0;
        n -= 1;
        
        int sq = std::sqrt(n);
        
        static int small[45000];
        static int large[45000];
        static int n_div[45000]; 
        
        for (int i = 1; i <= sq; ++i) {
            n_div[i] = n / i;                     
            small[i] = (i - 1) / 2;               
            large[i] = (n_div[i] - 1) / 2;
        }
        
        for (int p = 3; p <= sq; p += 2) {
            if (small[p] == small[p - 1]) continue;
            
            int pc = small[p - 1]; 
            long long q = (long long)p * p;
            
            int end_i = std::min((long long)sq, n / q);
            int border = sq / p;
            int limit1 = std::min(end_i, border);
            
            for (int i = 1; i <= limit1; ++i) {
                large[i] -= large[i * p] - pc;
            }
            
            for (int i = limit1 + 1; i <= end_i; ++i) {
                int w = n_div[i] / p;
                large[i] -= small[w] - pc;
            }
            
            for (int i = sq; i >= q; --i) {
                small[i] -= small[i / p] - pc;
            }
        }
        
        return large[1] + 1;
    }
};

int main()
{
    long long l = 500000000ll;
    int li2 = 500000000;

    std::cout<<"n = "<<li2<<": \n";
    std::cout<<"SolutionLC: "<<SolutionLC::count_primes(l)<<"\n";
    std::cout<<"LC_cpp23: "<<SolutionLC_cpp23::countPrimes(li2)<<"\n";
    std::cout<<"LC_cpp23_obfus: "<<SolutionLC_cpp23_obfus::countPrimes(li2)<<"\n";
    std::cout<<"LC_cpp23_Ultimate: "<<SolutionLC_cpp23_Ultimate::countPrimes(li2)<<"\n";
}