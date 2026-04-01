//
// Created by anvesh on 3/31/26.
//
#include <iostream>
#include <string>
#include <algorithm> // for std::sort
#include <ranges> // C++20 std::ranges magic

/*
 * Anagram: if two strings have same characters with same frequency !
 * Here we are given two string, we have to return true if they are anagram , false if they are not
 * 1. Length of both strings should match
 * 2. Frequency of all the chars should match
 */

class Solution
{
public:
    static bool is_anagram_sorting_approach(std::string &s, std::string &t)
    {
        // we return false if both string have different lengths
        if (s.size() != t.size()) return false;

        // sort both string in place
        // std::sort(s.begin(), s.end()); std::sort(t.begin(), t.end());

        // if we use c++20 we can do
        std::ranges::sort(s); std::ranges::sort(t);
         return s == t;

    }

    static bool is_anagram_sorting_optimal(const std::string &s, const std::string &t)
    {
        if (s.size() != t.size()) return false;
         // frequency array - 26 lowercase english letters initialized to 0
        int counts[26] = {};
        for (int i = 0; i < s.size(); ++i)
        {
            counts[s[i] - 'a']++; // increment the count for character found in s
            counts[t[i] - 'a']--; // decrement the count for character found in t
        }

        return std::ranges::all_of(counts, [](const int count){return count == 0;});


    }
    // static bool isAnagram(const std::string& s, const std::string& t){}
};

int main()
{
    std::string a = "anagram";
    std::string b = "nagaram";
    std::string c = "rat";
    std::string d = "car";
    std::cout << Solution::is_anagram_sorting_approach(a, b)<<" "<<std::endl;;
    std::cout << Solution::is_anagram_sorting_approach(c, d)<<" "<<std::endl;;
    std::cout << "Optimal approach "<<std::endl;
    std::cout << Solution::is_anagram_sorting_optimal(a, b)<<" "<<std::endl;;
    std::cout << Solution::is_anagram_sorting_optimal(c, d)<<" "<<std::endl;;

    return 0;

}