//
// Created by anvesh on 4/1/26.
//
/*
 * Given an integer array nums and an integer k,
 * return the k most frequent elements.
 *
 * Example:
 * Input: nums = [1,1,1,2,2,3]; k = 2;
 * Output: [1,2]
 * Input: nums = [1]; k = 1
 * Output: [1]
 * Input: nums = [1,2,1,2,1,2,3,1,3,2]; k = 2
 * Output: [1,2]
 *
 */

#include <algorithm>
#include <iostream>
#include <cstdint>
#include <queue>
#include <unordered_map>
#include <vector>

#include <ranges> // C++20 impls

class Solution
{
public:
    // we pass 'nums' by const reference to guarentee zero copying of  input
    static
    std::vector<int16_t> topKFrequentUnoptimisedISOCpp17(std::vector<int16_t>& nums, int k)
    {
        /*
         * 1. Traverse the array and use a hashmap (std::unordered_map) to count frequency of each number
         * 2. Copy elements and frequency from hashmap into array (or vector) of pairs
         * 3. Sort array of pairs in descending order based on frequencies
         * 4. Select top k : take first k elements from sorted array
         * TC: O(NlogN) where N is number of elements.
         * SC: O(N) to store hashmap and extra array of pairs
         */
        std::unordered_map<int16_t, int16_t> freq_map;
        for (const int16_t num: nums) ++freq_map[num];

        std::vector<std::pair<int16_t, int16_t>> freq_vec;
        // we reserve memory ahead of time
        // This completely eliminates invisible heap reallocations when scaling up
        freq_vec.reserve(freq_map.size());
        // c++17 explicitly extract 'num', 'freq'
        // emplace back constructs a pair inplace inside vec's memory buffer,
        // bypassing the creation of temp objects
        for (const auto& [num, freq] : freq_map) freq_vec.emplace_back(num, freq);

        // sort descending by frequency using fast, inline lambda
        std::sort(freq_vec.begin(), freq_vec.end(),
            [](const std::pair<int16_t, int16_t>& a, const std::pair<int16_t, int16_t>& b)
            {
                return a.second > b.second;
            });

        // optimisation 3 : reserve exactly 'k' slots so no reallocations happen during placement
        std::vector<int16_t> result;
        result.reserve(k);
        // ints are primitives, pusing is virtually free
        for (int16_t i = 0; i<k; i++) result.push_back(freq_vec[i].first);
        return result; // Return value optimisation (RVO); builds result into mem addrs expected by caller
    }

    static
    std::vector<int16_t> topKFrequentUnoptimisedISOCpp20(std::vector<int16_t>& nums, int k)
    {
        std::unordered_map<int16_t, int16_t> freq_map;
        for (int16_t num : nums) ++freq_map[num]; // increment freq of number
        std::vector<std::pair<int16_t, int16_t>> freq_vec; freq_vec.reserve(freq_map.size());
        for (const auto& [nums, freq]: freq_map) freq_vec.emplace_back(nums, freq);
        // C++20 optimization: Projections
        // Instead of an inline lambda, we tell Ranges to sort strictly
        // on 'second' property of pair. THis provides cleaner compilation
        // and prevents custom function object overhead entirely.
        std::ranges::sort(freq_vec, std::greater<>{}, &std::pair<int16_t, int16_t>::second);
        std::vector<int16_t> result; result.reserve(k);
        // C++ 20 optimization: Lazy View Pipelines
        // std::views::take(k): stops parsing the vector exactly at 'k'
        // std::views::keys: Strips away the 'frequency' (the .second element of pair)  securely
        // we then dump this completely lazy, zero-copy pipeline directly into 'result'
        auto top_k_view = freq_vec
        | std::views::take(k)
        | std::views::keys;
        std::ranges::copy(top_k_view, std::back_inserter(result));
        return result;
    };
    /*
     * SEMI Optimized approach (Min-Heap / Priority Queue)
     * Logic: Instead of blindly sorting all unique elements (costing O(NlogN), we care about top k,
     * We can use min-heap data structure to maintain a running list of only the k most frequent elements
     * as we iterate through our freqs.
     * we use min heap and not max heap because popping throws lowest freq elem.
     * by time we finish checking all elements only k highest freq will have survived in heap
     *
     * TC: O(N log K): Generting map takes O(N). Inserting elem into heap of size k takes O(logk)
     * significantly faster than O(N log N); especially when k is small compared to N
     * SC: O(N+K): hashmap takes O(N) space in worst case and heap strictly takes O(k) space.
     */
    static
    std::vector<int16_t> topKFrequentSemiOptimisedISOCpp17(std::vector<int16_t>& nums, int k)
    {
        std::unordered_map<int16_t, int16_t> freq_map;
        for (int16_t num: nums) ++ freq_map[num];
        // we store pairs of {frequency, number}
        using Pair = std::pair<int16_t, int16_t>;
        // we know heap will NEVER exceed size k+1
        // must pre alloc underlying vector container to prevent hidden heap allocs
        std::vector<Pair> pre_alloc_container; pre_alloc_container.reserve(k+1);
        // inject pre-alloc vec into priority queue constructor
        // std::greater<Pair> creates a MIN heap (default is std::less making MAX heap)
        std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>>
        min_heap(std::greater<Pair>(), std::move(pre_alloc_container));
        for (const auto& [num, freq] : freq_map)
        {
            min_heap.emplace(freq, num); // emplace constructs in-place securely
            if (min_heap.size()>k) min_heap.pop(); // throws out smallest freq
        }
        std::vector<int16_t> result; result.reserve(k);
        while (!min_heap.empty())
        {
            result.push_back(min_heap.top().second); // we want number not freq
            min_heap.pop();
        }
        return result; // Guaranteed zero-copy return value optimisation (RVO)
    }

    static
    std::vector<int16_t> topKFrequentSemiOptimisedISOCpp20(std::vector<int16_t>& nums, int k)
    {
        std::unordered_map<int16_t, int16_t> freq_map;
        for (const int64_t num: nums) ++ freq_map[num];
        // we manually use vetor as a heap to unlock c++ projections; storing natively as {num, freq}
        std::vector<std::pair<int16_t, int16_t>> min_heap; min_heap.reserve(k+1);
        for (const auto& [num, freq]: freq_map)
        {
            min_heap.emplace_back(num, freq);
            // c++20 ranges::push_heap allows us to project
            // we tell it to use 'greater' logic but projec against the .second element (freq)
            std::ranges::push_heap(min_heap, std::greater<>{}, &std::pair<int16_t, int16_t>::second);
            if (min_heap.size() > k)
            {
                // ranges::pop_heap swaps the smallest element to absolue back of vector
                // we must visually pop_back() to delete it from memory
                std::ranges::pop_heap(min_heap, std::greater<>{}, &std::pair<int16_t, int16_t>::second);
                min_heap.pop_back();
            }
        }
        std::vector<int16_t> res; res.reserve(k);
        std::ranges::copy(min_heap | std::views::keys, std::back_inserter(res));
        return res;
    }

    /*
     * Optimised Approach (Bucket sort)
     * Sorting algorithm are restricted to O(N log N) time at best.
     * A heap limits sorting to just top k elems dropping it to O(N log K) but still involvs maths shifting
     * To hit true linear time O(N) we must completely skip maths sorting. We use bucket sort
     * Instead of sorting based on frequencies index, we place every num into bucket at its freq index.
     * (Eg. if num 5 has freq 3, we place 5 into bucket[3])
     * We loop backward from end of array to front. We pull elements out of these buckets sequentially
     * until we get k items.
     *
     * TC: O(N); there are mathematically zero sorting/comparision limits applied!
     * SC: O(N); hashmap requires O(N) memory and setting up the N sized bucket array demands O(N) memory
     */
    static
    std::vector<int16_t> topKFrequentOptimisedISOCpp17(std::vector<int16_t>& nums, int k)
    {
        std::unordered_map<int16_t, int16_t> freq_map;
        for (int16_t num : nums) ++ freq_map[num];
        // we create array of dynamic array (buckets)
        // since freq start at 1 and go up to N, size must be N+1
        // 0 index will naturally remain empty;
        std::vector<std::vector<int16_t>> buckets(nums.size()+1);
        // no sorting; direct memory placement;
        for (const auto& [num, freq]: freq_map) buckets[freq].push_back(num);

        std::vector<int16_t> res; res.reserve(k);

        for (int16_t i = buckets.size() - 1; i>=0; --i)
        {
            for (int16_t num : buckets[i])
            {
                res.push_back(num);
                if (res.size() == k) return res;
            }
        }

        return res;
    }

    static
    std::vector<int16_t> topKFrequentOptimisedISOCpp20(std::vector<int16_t>& nums, int k)
    {
        std::unordered_map<int16_t, int16_t> freq_map;
        for (int16_t num : nums) ++ freq_map[num];
        std::vector<std::vector<int16_t>> buckets(nums.size()+1);
        for (const auto& [num, freq] : freq_map) buckets[freq].push_back(num);
        std::vector<int16_t> res; res.reserve(k);
        /*
         * c++20 pipeline magic
         * 1. views::reverse -> reads the buckets array from right to left
         * 2. views::join -> Flattens the 2D vector<vector<int>> into flat 1D sequence,
         * intelligently ignoring any empty inner bucket vectors
         * 3. views::take(k) -> A logic gae that forcefully shut down entire iteration pipleine the moment
         * exactly k elements are yielded.
         */
        auto top_k_views = buckets | std::views::reverse | std::views::join | std::views::take(k);
        std::ranges::copy(top_k_views, std::back_inserter(res));
        return res;

    };

    template<typename T>
    static void print_size(const T& t) {std::cout<<sizeof(t)<<std::endl;}
};

int main()
{
    // constexpr int16_t k = - 10000;
    // Solution::print_size(k);
    std::vector<int16_t> nums = {1,1,1,2,2,3}; int16_t k = 2;
    std::vector result  = Solution::topKFrequentUnoptimisedISOCpp17(nums, k);
    std::vector result2 = Solution::topKFrequentUnoptimisedISOCpp20(nums, k);
    std::vector result3 = Solution::topKFrequentSemiOptimisedISOCpp17(nums, k);
    std::vector result4 = Solution::topKFrequentSemiOptimisedISOCpp20(nums, k);
    std::vector result5 = Solution::topKFrequentOptimisedISOCpp17(nums, k);
    std::vector result6 = Solution::topKFrequentOptimisedISOCpp20(nums, k);
    // std::cout<<result<<std::endl;
    // C++ 20 way to print vector
    // std::ranges::copy(result, std::ostream_iterator<int16_t>(std::cout, " "));
    // std::cout<<"Unoptimised C++17 "<<std::endl;
    // std::ranges::copy(result2, std::ostream_iterator<int16_t>(std::cout, " "));
    // std::cout<<"Unoptimised C++20 "<<std::endl;
    // C++ 11 way below
    std::cout<<"Unoptimised ISOC++17: ";
    for (auto const val: result) std::cout<<val<<" "; std::cout<<std::endl;
    std::cout<<"Unoptimised ISOC++20: ";
    for (auto const val: result2) std::cout<<val<<" "; std::cout<<std::endl;
    std::cout<<"Semioptimised ISOC++17: ";
    for (auto const val: result3) std::cout<<val<<" "; std::cout<<std::endl;
    std::cout<<"Semioptimised ISOC++20: ";
    for (auto const val: result4) std::cout<<val<<" "; std::cout<<std::endl;
    std::cout<<"Optimised     ISOC++17: ";
    for (auto const val: result5) std::cout<<val<<" "; std::cout<<std::endl;
    std::cout<<"Optimised     ISOC++20: ";
    for (auto const val: result6) std::cout<<val<<" "; std::cout<<std::endl;

    return 0;

}