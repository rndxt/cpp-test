#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

namespace {

class LongInt {
    using ll = long long;
    using d_buf = std::vector<ll>;

    d_buf digits_;
    bool is_non_negative_;
    static constexpr size_t base_ = 6ULL;  // 10^6 numeral system

   public:
    LongInt(){};
    LongInt(long long);
    LongInt(bool, d_buf);
    LongInt(const std::string &);

    bool is_zero() const;
    bool is_one() const;
    bool is_minus_one() const;
    void print();
    LongInt product(LongInt &);

   private:
    size_t up_to_power2(size_t);
    ll str_to_ll(const std::string &, size_t, size_t) const;
    d_buf long_mult(d_buf &, d_buf &) const;
    d_buf karatsuba_mult(d_buf &, d_buf &) const;
    void carry();
};

LongInt::LongInt(long long k) : is_non_negative_(k >= 0), digits_(1, k){};

LongInt::LongInt(bool sign, d_buf digits)
    : is_non_negative_(sign), digits_(digits){};

LongInt::LongInt(const std::string &s) {
    assert(s.size() != 0);

    is_non_negative_ = (s[0] == '-');
    size_t k = is_non_negative_;

    size_t number_size = up_to_power2(s.size() - k);
    digits_.resize(number_size);
    for (size_t i = 0; i < digits_.size(); ++i) {
        digits_[i] = 0;
    }
    size_t q = (s.size() - k) / base_;
    size_t r = (s.size() - k) % base_;
    for (size_t i = 0; i < q; ++i) {
        size_t ind = (q - i - 1) * base_ + r + k;
        digits_[i] = str_to_ll(s, ind, ind + base_);
    }
    digits_[q] = str_to_ll(s, k, r + k);
}

size_t LongInt::up_to_power2(size_t n) {
    assert(n > 0);
    size_t power2 = 1;
    while (base_ * power2 < n) {
        power2 *= 2;  //<<= 1;
    }
    return power2;
}

bool LongInt::is_zero() const { return digits_.size() == 1 && digits_[0] == 0; }

bool LongInt::is_one() const {
    return !is_non_negative_ && digits_.size() == 1 && digits_[0] == 1;
}

bool LongInt::is_minus_one() const { return is_one() && is_non_negative_; }

LongInt::ll LongInt::str_to_ll(const std::string &s, size_t st,
                               size_t end) const {
    // [st; end)
    assert(st < s.size() && end <= s.size() && st < end);
    ll res = 0;
    for (size_t i = st; i < end; ++i) {
        res = 10 * res + s[i] - '0';
    }
    return res;
}

LongInt::d_buf LongInt::long_mult(d_buf &a, d_buf &b) const {
    assert(a.size() != 0 && b.size() != 0);
    size_t n = a.size();
    d_buf c(2 * n, 0);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            c[i + j] += a[i] * b[j];
        }
    }
    return c;
}

LongInt::d_buf LongInt::karatsuba_mult(d_buf &a, d_buf &b) const {
    assert(a.size() != 0 && b.size() != 0);

    size_t n = a.size();
    if (n <= 32) {
        return long_mult(a, b);
    }

    size_t k = n / 2;

    d_buf a1(a.begin(), a.begin() + k);
    d_buf a2(a.begin() + k, a.end());

    d_buf b1(b.begin(), b.begin() + k);
    d_buf b2(b.begin() + k, b.end());

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

    left.resize(2 * n);
    for (auto i = 0; i < n; ++i) {
        left[k + i] += middle[i];
        left[n + i] += right[i];
    }
    return left;
}

LongInt LongInt::product(LongInt &rhs) {
    assert(digits_.size() && rhs.digits_.size() != 0);

    if (is_zero() || rhs.is_zero()) {
        return LongInt{0};
    }

    if (is_one()) {
        return rhs;
    }

    if (rhs.is_one()) {
        return *this;
    }

    if (is_minus_one()) {
        LongInt c(rhs);
        c.is_non_negative_ = !c.is_non_negative_;
        return c;
    }

    if (rhs.is_minus_one()) {
        LongInt c(*this);
        c.is_non_negative_ = !c.is_non_negative_;
        return c;
    }

    size_t max_size = std::max(digits_.size(), rhs.digits_.size());
    digits_.resize(max_size);
    rhs.digits_.resize(max_size);

    d_buf c = karatsuba_mult(digits_, rhs.digits_);

    LongInt r;
    r.is_non_negative_ = is_non_negative_ ^ rhs.is_non_negative_;
    r.digits_ = std::move(c);
    return r;
}

void LongInt::carry() {
    if (!digits_.empty()) {
        ll carry = 0;
        for (size_t i = 0; i < digits_.size(); ++i) {
            carry += digits_[i];
            digits_[i] = carry % 1000000;
            carry /= 1000000;
        }
        digits_.back() += carry;
    }
}

void LongInt::print() {
    if (digits_.empty()) {
        return;
    }
    if (is_zero()) {
        std::cout << 0 << std::endl;
        return;
    }

    carry();
    auto it = digits_.rbegin();
    for (; it != digits_.rend() && *it == 0; ++it)
        ; /* nothing */

    if (is_non_negative_) {
        std::cout << '-';
    }

    std::cout << *it;
    ++it;
    for (; it != digits_.rend(); ++it) {
        std::cout << *it << std::setw(6) << std::setfill('0');
    }
    std::cout << std::endl;
}

}  // end namespace

int main() {
    std::string s1, s2;
    std::cin >> s1 >> s2;
    LongInt a(s1);
    LongInt b(s2);
    LongInt c = a.product(b);
    c.print();
}
