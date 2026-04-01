//
// Created by anvesh on 4/1/26.
//

#include <algorithm>
#include <iostream>
#include <vector>
#include <ranges>
#include <math.h>

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
    static int count_primes_optimised_cpp17(int n)
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
    static constexpr int count_primes_optimised_cpp20(int n)
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


int main()
{
    // Example
    // n =      10, 0, 1
    // Output =  4, 0, 0
    const std::vector<int> n = {10,0,1};
    std::ranges::for_each(n, [](const int x)
    {
        std::cout<<"n: " <<x<<"; "<<"BruteforceCPP17: "<< Solution::count_primes_bruteforce_cpp17(x) << std::endl;
        std::cout<<"n: " <<x<<"; "<<"BruteforceCPP20: "<< Solution::count_primes_bruteforce_cpp20(x) << std::endl;
        std::cout<<"n: " <<x<<"; "<<"OptimisedCPP17: "<< Solution::count_primes_optimised_cpp17(x) << std::endl;
        std::cout<<"n: " <<x<<"; "<<"OptimisedCPP20: "<< Solution::count_primes_optimised_cpp20(x) << std::endl;
        std::cout << "---------" << std::endl;
    });

    // const int p = 50000000;
    // std::cout << p << std::endl;
    // std::cout << Solution::count_primes_optimised_cpp17(p) << std::endl;
    // constexpr int ans = Solution::count_primes_optimised_cpp20(p);
    // std::cout << ans << std::endl;

    long long l = 500000000ll;
    constexpr int li = 500000000;
    int li2 = 500000000;

    cout<< SolutionLC::count_primes(l)<<endl;
    cout<<li<<endl;
    const int sol = Solution::count_primes_optimised_cpp20(li);
    cout<<sol<<endl;
    cout<<li2<<endl;
    cout << Solution::count_primes_optimised_cpp17(li2)<<endl;
}