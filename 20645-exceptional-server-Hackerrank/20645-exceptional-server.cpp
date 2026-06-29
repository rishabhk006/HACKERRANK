

	try {
    cout << Server::compute(A, B) << endl;
}
catch (bad_alloc &e) {
    cout << "Not enough memory" << endl;
}
catch (exception &e) {
    cout << "Exception: " << e.what() << endl;
}
catch (...) {
    cout << "Other Exception" << endl;
}



// Synced seamlessly with LeetHub Pro
// Pro features: https://bit.ly/leethubpro | Free version: https://bit.ly/leethubv4
// Get it here: https://chromewebstore.google.com/detail/bcilpkkbokcopmabingnndookdogmbna