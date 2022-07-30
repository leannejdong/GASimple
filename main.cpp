// Type your code here, or load an example.
#include <iostream>
#include <cmath>
#include <random>
#include <vector>
#include <algorithm>

constexpr int SAMPLE_SIZE = 1000; // How many top solutions to select
constexpr int NUM = 600000;       // Number of solutions per generation

struct Solution
{
    double rank, x,y,z;
    void fitness()
    {
        double ans = (6 * x + -y + std::pow(z,200)) - 25;
        rank = (ans == 0) ? 9999 : std::abs(1/ans);

    }
};

int main()
{

    // Create initial random solutions
    //
    std::random_device device;
    std::uniform_real_distribution<double> unif(-100000,100000);
    std::vector<Solution> solutions;


    for(int i = 0; i < NUM; i++)
        solutions.push_back(Solution{
            0,
            unif(device),
            unif(device),
            unif(device)
        });
    while(true)
    {
        // Run our fitness function
        //
        for(auto& s : solutions) { s.fitness(); }

        // Sort our solutions by rank
        //
        std::partial_sort(
            solutions.begin(),
            solutions.begin() + SAMPLE_SIZE,
            solutions.end(),
        [](const auto& lhs,const auto& rhs){
            return lhs.rank > rhs.rank;
        });

        // Print top solutions
        std::for_each(
            solutions.begin(),
            solutions.begin() + 10, [](const auto& s){
            std::cout << std::fixed
                << "Rank " << static_cast<int>(s.rank)
                << "\nx:" << s.x << " y:" << s.y << " z:" << s.z << " \n";
        });

        // Take top solutions
        //

        std::vector<Solution> sample;
        std::copy(
            solutions.begin(),
            solutions.begin() + SAMPLE_SIZE,
            std::back_inserter(sample)
        );
        solutions.clear();

        // Mutate the top solutions by %
        //
        std::uniform_real_distribution<double> m(0.99,1.01);
        std::for_each(sample.begin(), sample.end(), [&](auto& s){
            s.x *= m(device);
            s.y *= m(device);
            s.z *= m(device);
        });

        // Cross over
        //
        std::uniform_int_distribution<int> cross(0,SAMPLE_SIZE-1);
        for(int i = 0; i < NUM; i++)
        {
            solutions.push_back(Solution{
                0,
                sample[cross(device)].x,
                sample[cross(device)].y,
                sample[cross(device)].z,
            });
        }
    }

}
