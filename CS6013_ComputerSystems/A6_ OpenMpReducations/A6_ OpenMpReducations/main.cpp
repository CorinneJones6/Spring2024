/**
 * @file main.cpp
 * @brief Implementation of parallel array sum using C++ threads.
 *
 * The command to compile via cmdline: 
 * clang++ -std=c++17 -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp main.cpp -o main
 *
 *
 * @author Corinne Jones
 * @date 4/5/2024
 *
 */

#include <iostream>
#include <thread>
#include <fstream>
#include <omp.h>

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
    
    // "Atomically adds `local_sum` to `total_sum` with no synchronization or
    // ordering constraints on other reads or writes, guaranteeing only the
    // atomicity of this operation" (.cpp manual).
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
 * @return A pair containing the total sum of the array elements and the duration of the computation in nanoseconds.
 */
template<typename T>
auto parallel_sum_std(T a[], size_t N, size_t num_threads){
    
    // The atomic sum to force synchronization of addition to this amongst the threads
    atomic<T> total_sum(0);
    
    // Vector to hold the threads
    vector<thread> threads;
    
    // Start the clock to time the threads
    auto start_time = chrono::high_resolution_clock::now();
    
    // Determine how many segment sizes there will be
    size_t seg_size = N / num_threads;
    size_t start = 0;
    size_t end = 0;
    
    // This loop emplaces the threads, calling the .partial_sum method and calculating the segment each thready will calculate
    for(int i = 0; i < num_threads-1; i++){
        end = start + seg_size;
        threads.emplace_back(partial_sum<T>, a, ref(total_sum), start, end);
        start = end;
    }
    
    // If there is not an even division between N / num_threads, the last thread will go to the end of the array
    size_t R = N % num_threads;
    end = start + seg_size + R;
    threads.emplace_back(partial_sum<T>, a, ref(total_sum), start, end);
    
    // Wait for all the threads to finish
    for (auto& thread : threads) {
            thread.join();
        }
    
    // End the clock to time the threads
    auto end_time = chrono::high_resolution_clock::now();
    
    // Calculate the times
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
    
    // Return a pair holding the total_sum and the time in ms
    // Convert atomic<T> to int with .load()
    return make_pair(total_sum.load(), duration);
}

// =============== Part 2 =============== //


/**
 *@brief Computes the total sum of an array using OpenMp w/o built-in reduction.
 *
 * This function divides the array into segments based on the number of specified threads,
 * with each thread calculating a partial sum of its segment. The partial sums are then
 * added to form the total sum. The function also measures the execution time of
 * the parallel sum operation.
 *
 * @tparam T The data type of the array elements (e.g., int, float).
 * @param a Pointer to the first element of the array to be summed.
 * @param N The number of elements in the array.
 * @param num_threads The number of threads to be used for the computation.
 * @return A pair containing the total sum of the array elements and the duration of the computation in nanoseconds.
 */
template<typename T>
auto parallel_sum_omp1(T a[], size_t N, size_t num_threads){
    
    vector<T> partial_sums(num_threads, 0);
    
    // Set the number of threads for openMp
    omp_set_num_threads(num_threads);
    
    auto start_time = chrono::high_resolution_clock::now();
    
    #pragma omp parallel
    {
        size_t seg_size = N / num_threads;
        int ID = omp_get_thread_num();
        size_t start = ID * seg_size;
        
        // Calculate 'end' for all threads
        size_t end = (ID + 1) * seg_size;
        
        // Last thread covers all remaining elems (if not even)
        if (ID == num_threads - 1) {
            end = N;
        }
        
        // Every thread updats their portion of the sum
        for(size_t i = start; i < end; i++){
            partial_sums[ID] += a[i];
        }
    }
    
    // Add partial sums from all the threds
    T total_sum = 0;
    for(T sum : partial_sums){
        total_sum +=sum;
    }
    
    auto end_time = chrono::high_resolution_clock::now();
    
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();
    
    return make_pair(total_sum, duration);
    
}

// =============== Part 3 =============== //

/**
 *@brief Computes the sum of an array's elements in parallel using OpenMP's built-in reduction.
 *
 * This function uses the reduction feature of OpenMP to manage the addition
 * of results from each thread via parallelization. It measures the execution time to
 * determine the performance of parallel execution with OpenMP reduction.
 *
 * @tparam T The data type of the array elements. This type must support addition and initialization to 0.
 * @param a Pointer to the first element of the array to be summed. The array should be of type T and size N.
 * @param N The number of elements in the array.
 * @param num_threads The number of threads to be used for the computation. OpenMP dynamically assigns
 *                    work to threads based on this parameter.
 * @return A pair containing the total sum of the array elements and the duration of the computation in nanoseconds.
 */
template<typename T>
auto parallel_sum_omp_builtin(T a[], size_t N, size_t num_threads) {
    // Variable for reduction
    T total_sum = 0;

    // Set the desired number of threads
    omp_set_num_threads(num_threads);

    auto start_time = chrono::high_resolution_clock::now();

    // OpenMP parallel region with reduction clause
    #pragma omp parallel for reduction(+:total_sum)
    for(size_t i = 0; i < N; i++) {
        // Each thread adds its elements to total_sum, openMP handles reduction
        total_sum += a[i];
    }

    auto end_time = chrono::high_resolution_clock::now();
    
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();

    return make_pair(total_sum, duration);
}

void strongScalingTest(){
    ofstream outFile1("/Users/corinnejones/GitHubSchool/Spring2024/CS6013_ComputerSystems/Strong_Scaling_StdDouble.txt");
    ofstream outFile2("/Users/corinnejones/GitHubSchool/Spring2024/CS6013_ComputerSystems/Strong_Scaling_Omp1Double.txt");
    ofstream outFile3("/Users/corinnejones/GitHubSchool/Spring2024/CS6013_ComputerSystems/Strong_Scaling_BuiltInDouble.txt");
    
    if (!outFile1.is_open() || !outFile2.is_open() || !outFile3.is_open()) {
        cerr << "Failed to open file for writing." << endl;
        return;
    }
    
    //Prints sum, num_threads, then time
    
    int N = 0;
    int STEP = 1000000;
    size_t num_threads = 0;

    for(int i = 0; i < 6; i++){
        N += STEP;
        vector<long> arr(N, 1);

        outFile1 << "Array Size: " << N << endl;
        outFile1 << "Std" << endl;

        for(int k = 1; k < 17; k ++){
            num_threads = k ;

            auto [sum, time] = parallel_sum_std(arr.data(), N, num_threads);
            outFile1 << num_threads <<  "," << time << endl;
        }
    }
    
    N=0;
    for(int i = 0; i < 6; i++){
        N += STEP;
        vector<long> arr(N, 1);

        // Populate the array
        for (int j = 0; j < N; j++) {
            arr[j] = 1;
        }

        outFile2 << "Array Size: " << N << endl;
        outFile2 << "Omp1" << endl;

        num_threads = 0;
        for(int k = 1; k < 17; k ++){
            num_threads = k;

            auto [sum, time] = parallel_sum_omp1(arr.data(), N, num_threads);
            outFile2 << num_threads <<  "," << time << endl;
        }
    }

    N=0;
    for(int i = 0; i < 6; i++){
        N += STEP;
        vector<long> arr(N, 1);

        // Populate the array
        for (int j = 0; j < N; j++) {
            arr[j] = 1;
        }

        outFile3 << "Array Size: " << N << endl;
        outFile3 << "BuiltIn" << endl;

        for(int k = 1; k < 17; k ++){
            num_threads = k;

            auto [sum, time] = parallel_sum_omp_builtin(arr.data(), N, num_threads);
            outFile3 << num_threads <<  "," << time << endl;
        }
    }
    
    outFile1.close();
    outFile2.close();
    outFile3.close();
}

void weakScalingTest(){
    ofstream outFile1("/Users/corinnejones/GitHubSchool/Spring2024/CS6013_ComputerSystems/Weak_Scaling_Std.txt");
    
    ofstream outFile2("/Users/corinnejones/GitHubSchool/Spring2024/CS6013_ComputerSystems/Omp1.txt");
    
    ofstream outFile3("/Users/corinnejones/GitHubSchool/Spring2024/CS6013_ComputerSystems/BuiltIn.txt");
    int N = 0;
    int STEP = 1000000;
    size_t num_threads = 0;
    
    for(int i = 0; i < 5; i++){
        N += STEP;
        num_threads +=3;
        vector<int> arr(N, 1);
        
        outFile1 << "Array Size: " << N << endl;
//        outFile1 << "Num threads " << num_threads << endl;

        auto [sum, time] = parallel_sum_std(arr.data(), N, num_threads);
        outFile1  << num_threads <<  "," << time << endl;

        auto [sum_omp1, time_omp1] = parallel_sum_omp1(arr.data(), N, num_threads);
        outFile2  << num_threads <<  "," << time_omp1 << endl;

        auto [sum_builtin, time_builtin] = parallel_sum_omp_builtin(arr.data(), N, num_threads);
        outFile3  << num_threads <<  "," << time_builtin << endl;
    }
}

int main(int argc, const char * argv[]) {
    
    strongScalingTest();
    
    weakScalingTest();
    
    
    return 0;
}
