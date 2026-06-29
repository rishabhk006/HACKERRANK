

template <>
struct Traits<Color> {
    static string name(int index) {
        string colors[] = {"red", "green", "orange"};
        if (index >= 0 && index < 3)
            return colors[index];
        return "unknown";
    }
};

template <>
struct Traits<Fruit> {
    static string name(int index) {
        string fruits[] = {"apple", "orange", "pear"};
        if (index >= 0 && index < 3)
            return fruits[index];
        return "unknown";
    }
};



// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna