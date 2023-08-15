#include <iostream>
#include <vector>
using namespace std;


int compute_answer(int n, int s, vector<int> &prices) {

    sort(prices.begin(), prices.end());

    for (int pointer_last = n-1; pointer_last >= 0; pointer_last--) {
        
        int pointer_middle = -1;
        int pointer_first = -1;
        int sum_pointer_last = pointer_last == -1 ? 0 : prices[pointer_last];
        int sum_pointer_middle = pointer_middle == -1 ? 0 : prices[pointer_middle];
        int sum_pointer_first = pointer_first == -1 ? 0 : prices[pointer_first];

        if (sum_pointer_first + sum_pointer_middle + sum_pointer_last == s) {
            return 1;
        }
        
        pointer_middle = pointer_last;

        while (pointer_middle >= pointer_first)
        {
            sum_pointer_last = pointer_last == -1 ? 0 : prices[pointer_last];
            sum_pointer_middle = pointer_middle == -1 ? 0 : prices[pointer_middle];
            sum_pointer_first = pointer_first == -1 ? 0 : prices[pointer_first];
            if (sum_pointer_first + sum_pointer_middle + sum_pointer_last == s)
            {
                return 1;
            }
            else if (sum_pointer_first + sum_pointer_middle + sum_pointer_last > s)
            {
                pointer_middle--;
            }
            else if (sum_pointer_first + sum_pointer_middle + sum_pointer_last < s)
            {
                pointer_first++;
            }    
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
