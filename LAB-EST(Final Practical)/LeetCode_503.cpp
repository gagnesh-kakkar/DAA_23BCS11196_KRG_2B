class Solution {
public:
    vector<int> nextGreaterElements(vector<int>& nums) {
        int n = nums.size();
        vector<int> result(n, -1);

        stack<int> monoStack;

        for (int i = 2 * n - 1; i >= 0; i--) {
            int currentValue = nums[i % n];

            while (!monoStack.empty() && monoStack.top() <= currentValue) {
                monoStack.pop();
            }

            if (i < n && !monoStack.empty()) {
                result[i] = monoStack.top();
            }

            monoStack.push(currentValue);
        }

        return result;
    }
};
