//
// Created by anvesh on 3/31/26.
//
#include <iostream>
#include <vector>
// #include <cstdint> // for int32_t ; but we have int default to 32!
#include <unordered_map>

class Solution
{
    public:
    // unoptimsed BruteForce Approach
    static std::vector<int> unoptimised_two_sum(const std::vector<int>& nums, const int target)
    {
        for (size_t i = 0; i<nums.size(); i++)
        {
            for (size_t j = i+1; j<nums.size(); j++)
            {
                // if they add up to the target we will return the indices
                if (nums[i] + nums[j] == target) return {static_cast<int>(i), static_cast<int>(j)};
            }
        }

        return {}; // returning empty vectors
    }

    static std::vector<int> optimised_two_sum(const std::vector<int> &nums, const int target)
    {
        // Optimised one pass hashmap solution
        // create a hashmap to store numbers we have seen {number_value : index}
        std::unordered_map<int, int> seen;
        for (size_t i = 0; i< nums.size(); ++i)
        {
            int complement = target - nums[i];
            auto it = seen.find(complement);
            if (it != seen.end())
            {
                // return the index of complement and current index
                return { it->second, static_cast<int>(i)};
            }

            // if not found we add current number to it's index
            seen[nums[i]] = static_cast<int>(i);
        }

        // Return empty if no sol is found
        return {};
    }

    static std::vector<int> twoSum(const std::vector<int>& nums, const int target)
    {
        std::unordered_map<int, int> seen;
        for (size_t i = 0; i < nums.size(); ++i)
        {
            int comp = target - nums[i];
            if (auto it = seen.find(comp); it != seen.end()) return {it->second, static_cast<int>(i)};
            seen[nums[i]] = static_cast<int>(i);
        }
        return {};
    }

};

int main()
{
    std::vector nums = { 2, 7, 11, 15 }; int target = 9; // this is {2,7}
    if (const std::vector<int> result = Solution::unoptimised_two_sum(nums, target); !result.empty())
        {std::cout << "Indices: "<< result[0] << result[1] <<std::endl;}
    else
        { std::cout << "No Pair Found"<<std::endl;}
    nums = {3,2,4}; target = 6;
    if (const std::vector<int> result = Solution::unoptimised_two_sum(nums, target); !result.empty())
        {std::cout << "Indices: "<< result[0] << result[1] <<std::endl;}
    else
        { std::cout << "No Pair Found"<<std::endl;}
    std::cout << "Optimised sols "<<std::endl;
    nums = {3,2,4};target = 6;
    if (const std::vector<int> result = Solution::optimised_two_sum(nums, target); !result.empty())
    {std::cout << "Indices: "<< result[0] << result[1] <<std::endl;}
    else
    { std::cout << "No Pair Found"<<std::endl;}
}

