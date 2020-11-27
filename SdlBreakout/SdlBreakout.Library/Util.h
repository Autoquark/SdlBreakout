#pragma once
#include <cstdarg>
#include <vector>

class Util
{
public:
    template<typename V>        // recursion-ender
    static void MakeVector(std::vector<V>& vec) {}

    template<typename V, typename T1, typename... Types>
    static void MakeVector(std::vector<V>& vec, T1&& t1, Types&&... args)
    {
        vec.emplace_back(std::move(t1));
        MakeVector(vec, args...);
    }

    template<typename V, typename... Types>
    static std::vector<V> MakeVector(Types&&... args)
    {
        std::vector<V> vector;
        MakeVector(vector, args...);
        return vector;
    }
};

