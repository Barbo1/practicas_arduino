// Online C++ compiler to run C++ program online
#include <iostream>
#include <cstdint>

class RandomF {
  private:
    uint32_t m, c, a, xN;

    static constexpr uint32_t bound[13] {
      6, 7, 8, 10, 12, 17, 24, 41, 85, 256, 1626, 65536, 0xFFFFFFFF
    };

    uint32_t gcd (uint32_t n, uint32_t m) {
      int64_t t;
      uint32_t q = n | m;
      q = ~q & (q - 1);
      q = (q & 0x55555555) + ((q >> 1) & 0x55555555);
      q = (q & 0x33333333) + ((q >> 2) & 0x33333333);
      q = (q & 0x0F0F0F0F) + ((q >> 4) & 0x0F0F0F0F);
      q = (q & 0x00FF00FF) + ((q >> 8) & 0x00FF00FF);
      q = (q & 0x0000FFFF) + ((q >> 16) & 0x0000FFFF);
      m >>= q; 
      n >>= q; 

      if ((n & 1) == 1) { t = -(int64_t)m; goto B4; }
      else t = n;
B3:
      t = t >> 1;
B4:
      if ((t & 1) == 0) goto B3;
      if (t > 0) n = t;
      else m = -t;
      t = n - (int64_t)m;
      if (t != 0) goto B3;

      return n * (1 << q);
    }

    uint32_t gcd_primes (uint32_t n, uint32_t m) {
      int64_t t;
      goto B5; 
    B3:
      t >>= 1;
    B4:
      if ((t & 1) == 0) { goto B3; }
      if (t > 0) { n = t; }
      else { m = -t; }
    B5:
      t = n - (int64_t)m;
      if (t != 0) { goto B3; }

      return n;
    }

    /* made to numbers of 16 bits. */
    void discompress_primes (uint32_t E, uint32_t* S, uint32_t* n) {
      if (E <= 1) {
        S[0] = 1;
        S[1] = E;
        return;
      }

      uint16_t P = 1, q; 

      /* iteration in prime 2: */
      q = ~E & (E - 1);
      q = (q & 0x55555555) + ((q >> 1) & 0x55555555);
      q = (q & 0x33333333) + ((q >> 2) & 0x33333333);
      q = (q & 0x0F0F0F0F) + ((q >> 4) & 0x0F0F0F0F);
      q = (q & 0x00FF00FF) + ((q >> 8) & 0x00FF00FF);
      q = (q & 0x0000FFFF) + ((q >> 16) & 0x0000FFFF);
      if (q > 0) {
        S [P++] = 2;
        E >>= q;
      }
      
      /* iteration in prime 3: */
      if (E != 1) {
        q = gcd_primes (E, 0xCFD41B91);
        if (q > 1) {
          S [P++] = 3;
          E /= q;
        }
      }

      /* iteration with the rest of the numbers. */
      uint32_t o=2, i = 0;
      while (E != 1) {
        o ^= 6;
        *n += o;
        while (*n >= bound[i]) {
          i++;
        }

        q = 1;
        for (int j = 0; j < 13-i; j++) {
          q *= *n;
        }

        q = gcd_primes (E, q);
        if (*n > 1000) {exit (1);}
        if (q > 1) {
          S [P++] = *n;
          E /= q;
        }
      }
      S[0] = P;
    }

  public:

    RandomF (uint16_t m) {
      this->m = m;
      uint32_t n = 1;
      uint32_t divisors[17];
      discompress_primes (m, divisors, &n);

      /* calculating a... */
      this->a = 1;
      for (uint8_t i = 0; i < divisors[0]; i++) {
        this->a *= divisors[i];
      }
      if ((~m & 0x0003) == 0x0003) {
        this->a *= 2;
      }
      this->a++;

      /* calculating c... */
      uint16_t res = 1, j = 1;
      bool a;
      n += 10;
      while (gcd (m, n) != 1) {
        n++;
      }
      this->c = n;

      /* set x0... */
      this->xN = 0;
    }

    uint16_t generate () {
      this->xN = (this->xN * this->a + this->c) % this->m;
      return this->xN;
    }
};

class Random { 
  private: 
    uint32_t c, a, xN;

    static constexpr uint32_t primes[2] {
      641, 6700417
    };
    
    static constexpr uint32_t relatives[32] {
      19, 8191, 337, 431, 89, 257, 2833, 353, 4649, 673, 193, 449, 9901, 97, 
      1777, 2113, 65537, 113, 953, 683, 1321, 26317, 37171, 1013, 1657, 1103, 
      13367, 5419, 1429, 20857, 241, 101
    };
    
  public:
    Random (uint32_t seed) {
      /* Calculating c... */
      this->c = 1;
      for (int i = 0; i < 16; i++) {
        if (seed & (1 << i)) {
          this->c *= relatives[i];
        }
      }

      /* Calculating a... */
      this->a = 2;
      seed = (~seed >> 5) + 0x0000FFFF;
      for (int i = 16; i < 32; i++) {
        if (seed & (1 << i)) {
          this->a *= relatives[i];
        }
      }
      this->a += 1;
      
      /* Set xN to 0... */
      this->xN = 0;
    }

    uint32_t generate () {
      this->xN = this->a * this->xN + this->c;
      return this->xN;
    }
};

int main() {
  Random rand = Random (11372797);
  for (int i = 0; i < 1000; i++) {
    std::cout << rand.generate () << ", ";
  }
  std::cout << std::endl;
}
