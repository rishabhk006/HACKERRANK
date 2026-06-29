

template <bool... digits>
int reversed_binary_value() {
    bool bits[] = {digits...};
    int value = 0;

    for (int i = 0; i < sizeof...(digits); i++) {
        if (bits[i])
            value += (1 << i);
    }

    return value;
}



// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna