/* 
 * C++ Program to Implement Solovay-Strassen Primality Test
 */
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include <time.h>
#include <chrono>

using namespace std::chrono;
using namespace std;

#define ll long long

/* 
 * modular exponentiation
 */
// modulo(a, (p - 1) / 2, p)
ll modulo(ll base, ll exponent, ll mod)
{
    ll x = 1;
    ll y = base;

    while (exponent > 0)
    {
        if (exponent % 2 == 1){
            if (y != 0){
                if (x > std::numeric_limits<ll>::max() / y + 1){
                    // cout << "in modulo funtion, x * y over limit of long long type with a = " << base << endl;
                    return -1;
                }
            }
            x = (x * y) % mod;
        }
        if (y != 0){
            if (y > std::numeric_limits<ll>::max() / y + 1){
                    // cout << "in modulo funtion, y * y over limit of long long type with a = " << base << endl;
                    return -1;
                }
        }
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}
/* 
 * calculates Jacobian(a/n) n>0 and n is odd 
 */
int calculateJacobian(ll a,ll n)
{
    if (!a) 
        return 0;
    int ans = 1;
    ll temp;
    if (a < 0)
    {
        a = -a;
        if (n % 4 == 3) 
            ans=-ans; 
    }
    if (a == 1) 
        return ans;
    while (a)
    {
        if (a < 0)
        {
            a = -a;
            if (n % 4 == 3) 
                ans = -ans;  
        }
        while (a % 2 == 0)
        {
            a = a / 2;
            if (n % 8 == 3 || n % 8 == 5) 
                ans = -ans;    
        }
        swap(a, n);
        if (a % 4 == 3 && n % 4 == 3) 
            ans = -ans;
        a = a % n;
        if (a > n / 2) 
            a = a - n; 
    }
    if (n == 1) 
        return ans;
    return 0; 
}
 
/* 
 * Solovay-Strassen Primality Test
 * Iterations determine the accuracy of the test 
 */
bool Solovay(ll p, int iteration)
{
    // cout << "sqrt: " << sqrt_p << endl;
    if (p < 2) 
        return false;
    if (p != 2 && p % 2 == 0) 
        return false;
    for (int i = 0; i < iteration; i++){
        
        ll a = rand() % (p - 1) + 1;
        ll jacobian = (p + calculateJacobian(a, p)) % p;
        ll mod = modulo(a, (p - 1) / 2, p);
        if (mod == -1){ // modulo is over limit of long long type
            i--;
            continue;
        }
        if (!jacobian || mod != jacobian){ 
            return false;
        }
    }
    return true;
}

/* 
 * Function to read number from .txt file
 */
vector<long long> readNumbersFromFile(const string& filename) {
    vector<long long> numbers; // Vector to store numbers from the file

    ifstream inputFile(filename); // Open the text file (.txt) for reading
    if (!inputFile.is_open()) {
        cerr << "Unable to open file!" << endl; // Print a message if the file cannot be opened
        return numbers; // Return an empty vector if the file cannot be opened
    }

    string line;
    while (getline(inputFile, line)) {
        long long number = stoll(line); // Convert string to integer
        numbers.push_back(number); // Add number to the vector
    }

    inputFile.close(); // Close the file after reading
    return numbers; // Return the vector containing the numbers read from the file
}

//test function
int test_one_numbers()
{
    int iteration = 50;
    ll num;

    cout<<"Enter integer n to test primality: ";
    cin>>num;

    if (Solovay(num, iteration))
        cout<<num<<" is prime"<<endl;
    else
        cout<<num<<" is not prime"<<endl;
    return 0;
}


void benchmark()
{
   cout << "Benchmarking... " << flush;

   double total_runtime = 0;
   long long total_test = 0;
   long long total_correct_output = 0;
   long long checking_number = 0;
   long long max_check_number = 1000000000;
   string datasets_file = "./dataset/billion-primes.txt";

   // open datasets file
   ifstream file(datasets_file);

   if (!file.is_open())
   {
      cout << "[!] Datasets file does not exist." << endl;
      file.close();
      return;
   }

   string line;
   getline(file, line);
   long long prime = stoll(line);

   // benchmark
   while (checking_number < max_check_number)
   {
      checking_number++;
      total_test++;

      auto start_clock = high_resolution_clock::now();
      bool is_checking_number_prime = Solovay(checking_number, 9);
      auto end_clock = high_resolution_clock::now();

      // Calculate miller-rabin runtime (ms)
      duration<double, milli> runtime = end_clock - start_clock;

      // check if miller-rabin return correct output
      if ((is_checking_number_prime && checking_number == prime) || (!is_checking_number_prime && checking_number != prime))
         total_correct_output++;

      // update new prime in file
      if ((checking_number >= prime) && getline(file, line))
         prime = stoll(line);

      total_runtime += runtime.count();

    //   cout << "checking_number: " << checking_number << endl;
    //   cout << "is_checking_number_prime: " << is_checking_number_prime << endl;
    //   if (checking_number > 100000){
    //     break;
    //   }
   }

   file.close();

   cout << "Done !" << endl
        << endl;

   cout << "=================================" << endl;
   cout << "- Total test        : " << total_test << endl;
   cout << "- Total correct     : " << total_correct_output << endl;
   cout << "- Total runtime     : " << total_runtime << " ms" << endl;
   cout << "=================================" << endl;
   cout << "[>] Average runtime: " << total_runtime / total_test << " ms" << endl;
   cout << "[>] Average correct: " << float(total_correct_output) / total_test * 100 << "%" << endl;
}

//main
int main(){

    srand(time(NULL));
    benchmark();
    return 0;

}