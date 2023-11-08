#include <string>
#include <vector>
#include <iostream>

using namespace std;
using ll = long long;

struct long_int {
  vector<ll> digits_;
  bool is_negative;
};


bool is_zero(const long_int &a) {
  return a.digits_.size() == 1 && a.digits_[0] == 0;
}


bool is_one(const long_int &a) {
  return a.digits_.size() == 1 && a.digits_[0] == 1 && !a.is_negative;
}


bool is_minus_one(const long_int &a) {
  return is_one(a) && a.is_negative;
}


size_t up_to_power2(size_t n) {
  size_t power2 = 1ULL;
  while (6ULL * power2 < n) {
    power2 <<= 1;
  }
  return power2;
}


inline ll str_to_ll(const string& s, size_t st, size_t end) {
  // [st; end)
  ll res = 0;
  for (size_t i = st; i < end; ++i) {
    res = 10 * res + s[i] - '0';
  }
  return res;
}


long_int long_int_from_str(const string &s) {
  bool sign = false;  
  size_t k = 0;
  if (s[0]  == '-') {
    ++k;
    sign = true;
  }

  size_t number_size = up_to_power2(s.size() - k);
  vector<ll> digits(number_size, 0);

  size_t q = (s.size() - k) / 6ULL; 
  size_t r = (s.size() - k) % 6ULL;
  for (size_t i = 0; i < q; ++i) {
    size_t ind = (q-i-1) * 6ULL + r + k;
    digits[i] = str_to_ll(s, ind, ind + 6ULL);
  }
  digits[q] = str_to_ll(s, k, r+k);
  return {digits, sign};
}


vector<ll> long_mult(vector<ll> &a, vector<ll> &b) {
  size_t n = a.size();
  vector<ll> c(2*n, 0);
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      c[i + j] += a[i] * b[j];
    }
  }
  return c;
}


vector<ll> karatsuba_mult(vector<ll> &a, vector<ll> &b) {
  size_t n = a.size();

  if (n <= 32) {
    return long_mult(a, b);
  }

  size_t k = n / 2;

  vector<ll> a1(a.begin(), a.begin() + k);
  vector<ll> a2(a.begin() + k, a.end());

  vector<ll> b1(b.begin(), b.begin() + k);
  vector<ll> b2(b.begin() + k, b.end());

  auto left = karatsuba_mult(a1, b1);
  auto right = karatsuba_mult(a2, b2);

  for (size_t i = 0; i < k; ++i) {
    a1[i] += a2[i];
    b1[i] += b2[i];
  }

  auto middle = karatsuba_mult(a1, b1);
  for (size_t i = 0; i < n; ++i) {
    middle[i] -= left[i] + right[i];
  }

  vector<ll> c = std::move(left);
  c.resize(2*n);
  for (auto i = 0; i < n; ++i) {
      c[k + i] += middle[i];
      c[n + i] += right[i];
  }
  return c;
}


long_int product(long_int& a, long_int &b) {
  if (is_zero(a) || is_zero(b)) {
    vector<ll> c(1, 0);
    return long_int{c, false};
  }

  if (is_one(a)) { return b; } 
  if (is_one(b)) { return a; }

  if (is_minus_one(a)) {
    long_int c(b);
    c.is_negative = !c.is_negative;
    return c;
  }

  if (is_minus_one(b)) {
    long_int c(a);
    c.is_negative = !c.is_negative;
    return c;
  }

  size_t max_size = std::max(a.digits_.size(), b.digits_.size());
  a.digits_.resize(max_size);
  b.digits_.resize(max_size);
  bool sign = a.is_negative ^ b.is_negative;
  vector<ll> c = karatsuba_mult(a.digits_, b.digits_);
  return long_int{c, sign};
}


void carry(vector<ll> &v) {
  ll carry = 0;
  for (size_t i = 0; i < v.size(); ++i) {
    carry += v[i];
    v[i] = carry % 1000000;
    carry /= 1000000;   
  }
  v.back() += carry;
}


void print_long_int(long_int x) {
  if (is_zero(x)) {
    cout << 0;
    return;
  }

  carry(x.digits_);
  auto it = x.digits_.rbegin();
  for (; it != x.digits_.rend() && *it == 0; ++it);

  if (x.is_negative) { cout << '-'; }
  cout << *it;
  ++it;
  for (; it != x.digits_.rend(); ++it) {
    cout.fill('0');
    cout.width(6);
    cout << *it;
  }
  cout << endl;
}


int main() {
  std::string s1, s2;
  std::cin >> s1 >> s2;
  long_int a = long_int_from_str(s1);
  long_int b = long_int_from_str(s2);
  long_int c = product(a, b);
  print_long_int(c);
}
