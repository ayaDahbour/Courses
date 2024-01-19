void bucket_sort(int arr[], int n) {
    int max_val = arr[0];
    int min_val = arr[0];
    for (int i = 1; i < n; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
        if (arr[i] < min_val) {
            min_val = arr[i];
        }
    }
    int range_val = max_val - min_val + 1;
    int bucket_size = 10;
    int num_buckets = (range_val + bucket_size - 1) / bucket_size;
    struct bucket buckets[num_buckets];
    for (int i = 0; i < num_buckets; i++) {
        buckets[i].count = 0;
        buckets[i].values = (int*) malloc(bucket_size * sizeof(int));
    }
    for (int i = 0; i < n; i++) {
        int bucket_idx = (arr[i] - min_val) / bucket_size;
        buckets[bucket_idx].values[buckets[bucket_idx].count] = arr[i];
        buckets[bucket_idx].count++;
    }
    for (int i = 0; i < num_buckets; i++) {
        for (int j = 1; j < buckets[i].count; j++) {
            int k = j;
            while (k > 0 && buckets[i].values[k] < buckets[i].values[k-1]) {
                int temp = buckets[i].values[k];
                buckets[i].values[k] = buckets[i].values[k-1];
                buckets[i].values[k-1] = temp;
                k--;
            }
        }
    }
    int idx = 0;
    for (int i = 0; i < num_buckets; i++) {
        for (int j = 0; j < buckets[i].count; j++) {
            arr[idx] = buckets[i].values[j];
            idx++;
        }
        free(buckets[i].values);
    }
}
