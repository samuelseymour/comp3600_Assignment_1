#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;
#include <unordered_map>

std::unordered_map<long long, int> memo; // Use a hash-based map

long long calculateHash(int s, int highest_num_item_k, int n, const vector<int>& used) {
    const int prime = 31; // Choose a prime number
    long long hashValue = 0;

    hashValue = hashValue * prime + s;
    hashValue = hashValue * prime + highest_num_item_k;
    hashValue = hashValue * prime + n;

    for (int u : used) {
        hashValue = hashValue * prime + u;
    }

    return hashValue;
}

int compute_answer_sorted(int n, int s, vector<int> prices_sorted, int highest_num_item_k) {
    // cout << "Running CAS, first ele: " << prices_sorted[0] << "\n";

    long long hashKey = calculateHash(s, highest_num_item_k, n, prices_sorted);

    //if (memo.count(hashKey) > 0) {
        // cout << "Been here before, params n: " << n << " s:" << s << " highest no. k: " << highest_num_item_k << "\n";
    //    return memo[hashKey]; // always 0?
    //}

    for (int k = 0; k < n; k++) {
        int ps_k = prices_sorted[k];
        if (highest_num_item_k > 0 && ps_k > s) {
            // cout << "with lowest price of: " << ps_k << ", I know to stop here as s is " << s << "\n";
            // highest_num_item_k = min(0, highest_num_item_k);
            int returnValue = s == 0;
            memo[hashKey] = returnValue;
            return returnValue; // JUST return 0? no
        } else if (highest_num_item_k > 1 && 2 * ps_k > s) {
            // cout << "with lowest price of: " << ps_k << ", I know to only mult by 1 as s is " << s << "\n";
            highest_num_item_k = min(1, highest_num_item_k);
        }

        // cout << "How long does this take? " << "\n";
        std::vector<int> higher_prices_sorted(prices_sorted.begin() + k + 1, prices_sorted.begin() + n + 1);
        // cout << "This long " << "\n";

        for (int h = 0; h < highest_num_item_k; h++) {
            int num_same_item = h + 1;

            if (highest_num_item_k - num_same_item == 0) {
                int last_subtraction = s - num_same_item * ps_k;

                if (last_subtraction == 0) {
                    memo[hashKey] = 1;
                    return 1;
                } else if (last_subtraction < 0) {
                    // cout << "all further k will be bigger and we have exceeded s" << "\n";
                    memo[hashKey] = 0;
                    return 0;
                } else {
                    break;
                }
            } else {
                if (compute_answer_sorted(n - k - 1, s - num_same_item * ps_k, higher_prices_sorted, highest_num_item_k - num_same_item) == 1) {
                    memo[hashKey] = 1;
                    return 1;
                }
            }
        }
    }

    int returnValue = s == 0;
    memo[hashKey] = returnValue;
    return returnValue; // JUST return 0? no
}

int compute_answer(int n, int s, vector<int> &prices) {
    memo.clear();
    std::vector<int> used(n);
    int len_used = 0;

    for (int i = 0; i < n; i++) {
        // cout << "i : " << i << "\n";
        int p_i = prices[i];
        // cout << "p_i : " << p_i << "\n";
        int num_try_i;
        if (p_i > s) {
            // cout << "p_i too big, skipping and trashing it as s is: " << s << "\n";
            continue;
        } else if (p_i * 2 > s) {
            // cout << "p_i big, but only 1 x it as s is: " << s << "\n";
            num_try_i = 1;
        } else if (p_i * 3 == s) {
            return 1;
        } else {
            // cout << "p_i not big enough on its own, but only 2x it as 3x doesn't work. " << "\n";
            num_try_i = 2;
        }

        for (int j = 0; j < num_try_i; j++) {
            int num_same_item = j + 1;
            // cout << "trying " << num_same_item << "x" << " by subtracting " << num_same_item * p_i << " from s: " << s;
            // cout << " giving: " << s - num_same_item * p_i << "\n";

            if (compute_answer_sorted(len_used, s - num_same_item * p_i, used, 3 - num_same_item) == 1) {
                return 1;
            }
        }

        if (len_used == 0) {
            // cout << "first item, adding it to used: " << p_i << "\n";
            used[0] = p_i;
            len_used++;
        } else {

            int insertIndex = len_used;
            int left = 0;
            int right = len_used - 1;

            while (left <= right) {
                int mid = left + (right - left) / 2;
                if (used[mid] == p_i) {
                    insertIndex = mid;
                    break;
                } else if (used[mid] < p_i) {
                    left = mid + 1;
                } else {
                    right = mid - 1;
                    insertIndex = mid;
                }
            }

            // Shift elements to make space for the insertion
            for (int j = len_used; j > insertIndex; j--) {
                used[j] = used[j - 1];
            }

            // Insert p_i at the correct position and increase the length
            used[insertIndex] = p_i;
            len_used++;
        }
    }

    return 0;
}

int main() {
    // Read input
    int n, s;
    cin >> n >> s;
    vector<int> prices(n); // dynamic array of length n
    for (int i = 0; i < n; i++) {
        cin >> prices[i];
    }

    // Compute answer
    // cout << "Computing: ..." << "\n";
    int answer = compute_answer(n, s, prices);

    // Print output
    cout << answer << "\n";
    return 0;
}
