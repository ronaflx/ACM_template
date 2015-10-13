const double PI = acos(-1);

template <typename Iterator>
void BitReversal(Iterator first, Iterator last,
                 std::random_access_iterator_tag) {
  typedef typename std::iterator_traits<Iterator>::difference_type size_type;
  size_type n = std::distance(first, last);
  size_type j = 0;
  for (size_type i = 0; i < n; i++) {
    if (i < j) iter_swap(first + i, first + j);
    size_type k = n;
    while (j & (k >>= 1)) j ^= k;
    j |= k;
  }
}

template <typename Iterator>
void BitReversal(Iterator first, Iterator last) {
  typedef typename std::iterator_traits<Iterator>::iterator_category category;
  BitReversal(first, last, category());
}

void FFT(vector<complex<double>>& cplx, bool inverted) {
  const double pi = inverted ? -PI : PI;
  const size_t n = cplx.size();
  BitReversal(cplx.begin(), cplx.end());
  for (size_t len = 1; len <= n; len <<= 1) {
    const complex<double> twiddle =
        complex<double>(cos(pi * 2 / len), sin(pi * 2 / len));
    for (size_t start = 0; start < n; start += len) {
      complex<double> w = 1;
      for (size_t j = 0; j < (len >> 1); j++) {
        const complex<double> t = w * cplx[start + j + (len >> 1)];
        const complex<double> u = cplx[start + j];
        cplx[start + j] = u + t;
        cplx[start + j + (len >> 1)] = u - t;
        w *= twiddle;
      }
    }
  }
  if (inverted)
    for (size_t i = 0; i < n; i++) cplx[i] /= n;
}
