#pragma once
#include <vector>

inline std::vector<int> minCoins(std::vector<int> coins, int V)
{
	std::vector<std::vector<int>> solution;
	solution.resize(V + 1);
	// table[i] will be storing the minimum number of coins
	// required for i value.  So table[V] will have result
	std::vector<int> table(V + 1);

	// Base case (If given value V is 0)
	table[0] = 0;

	// Initialize all table values as Infinite
	for (int i = 1; i <= V; i++)
		table[i] = INT_MAX;

	// Compute minimum coins required for all
	// values from 1 to V
	for (int i = 1; i <= V; i++)
	{
		// Go through all coins smaller than i
		for (int j = 0; j<coins.size(); j++)
			if (coins[j] <= i)
			{
				int sub_res = table[i - coins[j]];
				if (sub_res != INT_MAX && sub_res + 1 < table[i])
				{
					solution[i] = solution[i - coins[j]];
					solution[i].push_back(coins[j]);
					table[i] = sub_res + 1;
				}
			}
	}
	std::reverse(solution[solution.size() - 1].begin(), solution[solution.size() - 1].end());
	return solution[solution.size() - 1];
}