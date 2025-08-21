package DAA;

import java.util.HashMap;
import java.util.Map;

class FrequencyCounter {

    static void countFrequency(int arr[]) {
        HashMap<Integer, Integer> freqMap = new HashMap<>();

        for (int num : arr) {
            freqMap.put(num, freqMap.getOrDefault(num, 0) + 1);
        }

        for (Map.Entry<Integer, Integer> entry : freqMap.entrySet()) {
            System.out.println(entry.getKey() + "   " + entry.getValue());
        }
    }


    public static void main(String[] args) {
        int arr1[] = {10, 20, 20, 10, 10, 20, 5, 20};
        int arr2[] = {10, 20, 20};

        System.out.println("Example 1:");
        countFrequency(arr1);

        System.out.println("\nExample 2:");
        countFrequency(arr2);
    }
}
