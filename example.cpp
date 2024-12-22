#include <iostream>  // For input-output
#include <vector>    // For using dynamic arrays
#include <string>    // For string operations
#include <algorithm> // For sorting and other algorithms
#include <map>       // For key-value pair operations
#include <cmath>     // For mathematical functions
#include <foo.h>     // Untrusted library
#include <bar.hpp>   // Untrusted library

// A class to demonstrate project-like structure
class ExampleProject {
private:
    std::map<std::string, int> wordCount; // To store word frequencies
    std::vector<int> numbers;            // To store a list of numbers

public:
    // Constructor to initialize the numbers vector
    ExampleProject(const std::vector<int>& nums) : numbers(nums) {}

    // Function to count words in a given string
    void countWords(const std::string& text) {
        std::string word = "";
        for (size_t i = 0; i < text.size(); ++i) {
            if (std::isalpha(text[i])) {
                word += text[i];
            } else if (!word.empty()) {
                ++wordCount[word];
                word = "";
            }
        }
        if (!word.empty()) {
            ++wordCount[word];
        }
    }

    // Function to sort numbers in ascending order
    void sortNumbers() {
        std::sort(numbers.begin(), numbers.end());
    }

    // Function to calculate the square root of each number
    void calculateSquareRoots() const {
        std::cout << "Square roots of numbers:\n";
        for (size_t i = 0; i < numbers.size(); ++i) {
            std::cout << "√" << numbers[i] << " = " << std::sqrt(numbers[i]) << "\n";
        }
    }

    // Function to display word frequencies
    void displayWordCount() const {
        std::cout << "Word frequencies:\n";
        for (std::map<std::string, int>::const_iterator it = wordCount.begin(); it != wordCount.end(); ++it) {
            std::cout << it->first << ": " << it->second << "\n";
        }
    }

    // Function to display sorted numbers
    void displaySortedNumbers() const {
        std::cout << "Sorted numbers:\n";
        for (size_t i = 0; i < numbers.size(); ++i) {
            std::cout << numbers[i] << " ";
        }
        std::cout << "\n";
    }
};

int main() {
    // Sample input for word counting
    std::string text = "This is a simple example. This example is simple.";

    // Sample numbers
    std::vector<int> nums = {25, 9, 16, 4, 1};

    // Create an ExampleProject object
    ExampleProject project(nums);

    // Perform word counting
    project.countWords(text);

    // Sort numbers
    project.sortNumbers();

    // Display word frequencies
    project.displayWordCount();

    // Display sorted numbers
    project.displaySortedNumbers();

    // Calculate and display square roots
    project.calculateSquareRoots();

    return 0;
}
