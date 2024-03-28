#include <iostream>
#include <random>
#include <vector>

// Function for modular exponentiation (a^b mod m)
long long mod_pow(long long a, long long b, long long m) {
  a %= m;
  long long result = 1;
  while (b > 0) {
    if (b % 2 == 1) {
      result = (result * a) % m;
    }
    b >>= 1;
    a = (a * a) % m;
  }
  return result;
}


// Function to check primality using Lucas Primality Test
bool lucas_primality_test(long long n, int k) {
  if (n <= 2) {
    return false; // 0, 1, 2 are not prime
  }
  if (n % 2 == 0) {
    return n == 2; // Only 2 is prime among even numbers
  }

  // Factorize n - 1 (can be optimized with more advanced algorithms)
  std::vector<long long> factors;
  for (long long i = 3; i * i <= n - 1; i += 2) {
    if ((n - 1) % i == 0) {
      factors.push_back(i);
      while ((n - 1) % (i * i) == 0) {
        i *= i;
      }
    }
  }
  if (n - 1 > 1) {
    factors.push_back(n - 1);
  }

  // Seed value for Lucas sequence
  long long V = 2;

  for (int i = 0; i < k; ++i) {
    // Pick a random integer in the range [2, n - 1]
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long long> dis(2, n - 1);
    long long a = dis(gen);

    // Check congruence condition
    if (mod_pow(a, n - 1, n) != 1) {
      return false; // Composite number
    }

    // Check against all prime factors of n - 1
    bool isPrime = false;
    for (long long factor : factors) {
      if (mod_pow(V, (n - 1) / factor, n) != 1) {
        isPrime = true;
        break;
      }
    }

    if (isPrime) {
      return true; // Prime number
    }

    // Update V for next iteration
    V = (V * V - 2) % n;
  }

  // Possibly composite after k iterations
  return false;
}



int main() {
  long long n;
  int k;

  std::cout << "Enter an odd integer (n) to test: ";
  std::cin >> n;

  std::cout << "Enter the number of iterations (k) for accuracy: ";
  std::cin >> k;

  if (lucas_primality_test(n, k)) {
    std::cout << n << " is prime (with probability 1 - (1/2)^k)." << std::endl;
  } else {
    std::cout << n << " is composite or possibly composite." << std::endl;
  }

  return 0;
}