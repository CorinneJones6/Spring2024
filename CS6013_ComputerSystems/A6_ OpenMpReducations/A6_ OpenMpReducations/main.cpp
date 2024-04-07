/**
 * @file main.cpp
 * @brief Implementation of parallel array sum using C++ threads.
 *
 *
 * @author Corinne Jones
 * @date 4/5/2024
 *
 */

#include <iostream>
#include <thread>
using namespace std;

// =============== Part 1 =============== //

/**
 * @brief Computes the partial sum of a segment of an array and adds it to a total sum.
 *
 * This function iterates over a portion of the array from `start` to `end`,
 * computes the sum of this segment, and then atomically adds this partial sum
 * to the total sum. The atomic addition ensures thread safety.
 *
 * @tparam T The data type of the array elements.
 * @param a Pointer to the first elem in the array.
 * @param total_sum Atomic reference to the total sum variable to be updated.
 * @param start The starting index of the segment.
 * @param end The ending index of the segment (exclusive).
 */
template<typename T>
auto partial_sum(T *a, atomic<T>& total_sum, int start, int end){
    T local_sum = 0;
    for (size_t i = start; i < end; ++i) {
        local_sum += a[i];
    }
    
    // Atomically adds `local_sum` to `total_sum` with no synchronization or
    // ordering constraints on other reads or writes, guaranteeing only the
    // atomicity of this operation (.cpp manual).
    total_sum.fetch_add(local_sum, memory_order_relaxed);
}

/**
 * @brief Computes the sum of an array's elements in parallel using a specified number of threads.
 *
 * This function divides the work of summing the elements of an array across multiple threads.
 * Each thread computes a partial sum of a segment of the array, and these partial sums are then
 * atomically added to a total sum. The function measures the time taken to compute the sum and returns
 * both the total sum and the duration of the computation.
 *
 * @tparam T The data type of the array elements.
 * @param a The array to be summed.
 * @param N The number of elements in the array.
 * @param num_threads The number of threads to use for the computation.
 * @return A pair containing the total sum of the array elements and the duration of the computation in milliseconds.
 */
template<typename T>
pair<T, double> parallel_sum_std(T a[], size_t N, size_t num_threads){
    
    //The atomic sum to force synchronization of addition to this amongst the threads
    atomic<T> total_sum(0);
    
    //Vector to hold the threads
    vector<thread> threads;
    
    //Determine how many segment sizes there will be
    size_t seg_size = N / num_threads;
    
    size_t start = 0;
    size_t end = 0;
    
    //This loop emplaces the threads, calling the .partial_sum method and calculating the segment each thready will calculate
    for(int i = 0; i < num_threads-1; i++){
        end = start + seg_size;
        threads.emplace_back(partial_sum<T>, a, ref(total_sum), start, end);
        start = end;
    }
    
    //If there is not an even division between N / num_threads, the last thread will go to the end of the array
    size_t R = N % num_threads;
    end = start + seg_size + R;
    threads.emplace_back(partial_sum<T>, a, ref(total_sum), start, end);
    
    //Start the clock to time the threads
    auto start_time = chrono::high_resolution_clock::now();
    
    for (auto& thread : threads) {
            thread.join();
        }
    
    //End the clock to time the threads
    auto end_time = chrono::high_resolution_clock::now();
    
    //Calculate the times
    chrono::duration<double, milli> duration = end_time - start_time;
    
    //Return a pair holding the total_sum and the time in ms
    return make_pair(total_sum.load(), duration.count());
}

// =============== Part 2 =============== //

template<typename T>
auto parallel_sum_omp1(T a[], size_t N, size_t num_threads){
    
}

// =============== Part 3 =============== //

template<typename T>
auto parallel_sum_omp_builtin (T a[], size_t N, size_t num_threads){
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    return 0;
}
