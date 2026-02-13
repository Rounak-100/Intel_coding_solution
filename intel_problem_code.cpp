#include <iostream>
#include <vector>
#include <queue>
#include <stdexcept>
#include <string>
#include <algorithm>

// FIFO Buffer Class

class FifoBuffer {
public:
    void Push(int value) {
        buffer_.push(value);
    }

    [[nodiscard]] bool IsEmpty() const noexcept {
        return buffer_.empty();
    }

    int Pop() {
        if (IsEmpty()) {
            throw std::underflow_error("FIFO underflow");
        }

        int value = buffer_.front();
        buffer_.pop();
        return value;
    }

private:
    std::queue<int> buffer_;
};

// Adder System Class

class AdderSystem {
public:
    explicit AdderSystem(std::vector<int> vec_a,
                         std::vector<int> vec_b)
        : input_a_(std::move(vec_a)),
          input_b_(std::move(vec_b)),
          max_pairs_(std::min(input_a_.size(), input_b_.size())) {}

    void LoadNext() {
        if (current_index_ >= max_pairs_) {
            std::cout << "[INFO] No more elements to load\n";
            return;
        }

        fifo_a_.Push(input_a_[current_index_]);
        fifo_b_.Push(input_b_[current_index_]);

        std::cout << "[LOAD] "
                  << input_a_[current_index_] << " , "
                  << input_b_[current_index_] << "\n";

        ++current_index_;
    }

    void Add() {
        if (addition_count_ >= max_pairs_) {
            std::cout << "[ERROR] No more additions possible\n";
            return;
        }

        if (fifo_a_.IsEmpty() || fifo_b_.IsEmpty()) {
            std::cout << "[ERROR] FIFO empty. Load elements first\n";
            return;
        }

        int lhs = fifo_a_.Pop();
        int rhs = fifo_b_.Pop();
        int result = lhs + rhs;

        results_.push_back(result);
        ++addition_count_;

        std::cout << "[ADD] "
                  << lhs << " + "
                  << rhs << " = "
                  << result << "\n";
    }

    void PrintResults() const {
        std::cout << "\nFinal Output Vector\n";

        if (results_.empty()) {
            std::cout << "[]\n";
            return;
        }

        std::cout << "[";

        for (std::size_t i = 0; i < results_.size(); ++i) {
            std::cout << results_[i];
            if (i != results_.size() - 1) {
                std::cout << ", ";
            }
        }

        std::cout << "]\n";
    }

private:
    std::vector<int> input_a_;
    std::vector<int> input_b_;
    std::vector<int> results_;    

    FifoBuffer fifo_a_;
    FifoBuffer fifo_b_;

    std::size_t current_index_{0};
    std::size_t max_pairs_{0};
    std::size_t addition_count_{0};
};

int main() {
    const std::vector<int> vector_a{1, 3, 5, 7};
    const std::vector<int> vector_b{2, 4, 6, 8};

    AdderSystem adder(vector_a, vector_b);

    std::cout << "  FIFO Adder System  \n";
    std::cout << "Commands:\n";
    std::cout << "  ENTER  -> Load next pair\n";
    std::cout << "  +      -> Add front elements\n";
    std::cout << "  q      -> Quit\n\n";

    std::string command;

    while (true) {
        std::getline(std::cin, command);

        if (command.empty()) {
            adder.LoadNext();
        }
        else if (command == "+") {
            adder.Add();
        }
        else if (command == "q") {
            std::cout << "Exiting system\n";
            break;
        }
        else {
            std::cout << "[WARNING] Invalid command\n";
        }
    }

    adder.PrintResults();

    return 0;
}
