#pragma once

#include "SDK/Numeric/Euclid.h"

namespace sdk {
    namespace numeric{
        template<typename T, typename C, T Modulus>
        class MR
        {
        private:
            struct tag_plus {}; // indicates value is in range [0, 2 * Modulus)
            struct tag_minus {}; // indicates value is in range (-Modulus, Modulus)
            struct tag_good {}; // indicates value is in range

            T value;

            static_assert(std::numeric_limits<C>::max() / Modulus / Modulus > 0, "compute type is too small");
            static_assert(Modulus < std::numeric_limits<T>::max() / 2, "storage type is too small");

            void reduce(tag_plus)
            {
                if (value >= Modulus)
                    value -= Modulus;
            }

            void reduce(tag_minus)
            {
                if (value < 0)
                    value += Modulus;
            }

            void reduce(tag_good) {}

        public:
            typedef T value_type;
            typedef C compute_type;
            static const T modulus = Modulus;

            MR() : value(0) {}
            MR(C value) : value(value % Modulus) { reduce(tag_minus()); }
            template<typename tag_t>
            MR(T value, tag_t tag) : value(value) { reduce(tag); }

            MR &operator=(C value) { this->value = value % Modulus; reduce(tag_minus()); return *this; }

            MR operator +(MR b) const { return MR(value + b.value, tag_plus()); }
            MR operator *(MR b) const { return MR(C(value) * C(b.value) % Modulus, tag_good()); }

            MR &operator +=(MR b) { value += b.value; reduce(tag_plus()); return *this; }
            MR &operator *=(MR b) { value = C(value) * C(b.value) % Modulus; return *this; }

            bool operator==(MR b) const { return value == b.value; }
            bool operator!=(MR b) const { return value != b.value; }

            T get() const { return value; }

            // These are only valid if the Modulus is prime, or at least if
            // the dividend is relatively prime to the modulus
            MR inverse() const
            {
                assert(value);
                return MR(invMod(C(value), C(Modulus)), tag_good());
            }
            MR operator /(MR b) const { return *this * b.inverse(); }
            MR &operator /=(MR b) { return *this *= b.inverse(); }
        };
    }
}
