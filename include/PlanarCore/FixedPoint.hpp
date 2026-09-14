#pragma once

#include <cstdint>
#include <limits>
#include <random>

namespace fp {
	template<typename T, typename T2, size_t decimalPoints>
	class Fixed {

		static_assert(std::is_integral_v<T>, "T must be an integral type");
		static_assert(std::is_integral_v<T2>, "T2 must be an integral type");
		static_assert(sizeof(T2) >= sizeof(T), "T2 should be at least as wide as T (prefer wider)");
		static_assert(decimalPoints < (8 * sizeof(T2)), "decimalPoints too large for T2 shifts");

	public:

		using rawType = T;
		using promoType = T2;

		rawType raw{};
		static constexpr promoType SCALE = promoType(1) << decimalPoints; // For casting to floating point

		constexpr Fixed() noexcept = default;

		constexpr Fixed(const Fixed&) noexcept = default;
		explicit constexpr Fixed(float num) noexcept { raw = T(num * static_cast<float>(1 << decimalPoints) + (num >= 0 ? 0.5 : -0.5)); }
		constexpr Fixed(int integer) noexcept { raw = (static_cast<T>(integer) << decimalPoints); }

	private:
		static constexpr Fixed fromRaw(rawType raw) noexcept {
			Fixed f;
			f.raw = raw;
			return f;
		}

		static constexpr promoType mulRaw(promoType raw1, promoType raw2) noexcept {
			return raw1 * raw2 >> decimalPoints;
		}

		static constexpr promoType divRaw(promoType dividend, promoType divisor) noexcept {
			return (dividend << decimalPoints) / divisor;
		}

	public:

		// Casts
		constexpr explicit operator float() const noexcept { return static_cast<float>(raw) / static_cast<float>(SCALE); }
		constexpr explicit operator int() const noexcept {
			return raw >> decimalPoints;
		}

		// Copy & assign
		constexpr Fixed& operator=(const Fixed&) noexcept = default;

		constexpr Fixed& operator=(const float num) noexcept { raw = T(num * static_cast<float>(1 << decimalPoints) + (num >= 0 ? 0.5 : -0.5)); return *this; }

		// Unary
		constexpr Fixed operator-() const noexcept { return fromRaw(-raw); }

		// Add & sub
		constexpr Fixed operator+(const Fixed& rhs) const noexcept { return fromRaw(raw + rhs.raw); }
		constexpr Fixed& operator+=(const Fixed& rhs) noexcept { raw = raw + rhs.raw; return *this; }

		constexpr Fixed operator-(const Fixed& rhs) const noexcept { return fromRaw(raw - rhs.raw); }
		constexpr Fixed& operator-=(const Fixed& rhs) noexcept { raw = raw - rhs.raw; return *this; }

		// Mul
		constexpr Fixed operator*(const Fixed& rhs) const noexcept { return fromRaw(promoType(raw) * promoType(rhs.raw) >> decimalPoints); }
		constexpr Fixed& operator*=(const Fixed& rhs) noexcept { raw = promoType(raw) * promoType(rhs.raw) >> decimalPoints; return *this; }

		// Divide
		constexpr Fixed operator/(const Fixed& rhs) const noexcept { if (rhs.raw == 0) return fromRaw(std::numeric_limits<rawType>::max()); return fromRaw(((promoType(raw) << decimalPoints) / promoType(rhs.raw))); }
		constexpr Fixed& operator/=(const Fixed& rhs) noexcept { raw = (promoType(raw) << decimalPoints) / promoType(rhs.raw); return *this; }

		// Comparisons
		constexpr bool operator==(const Fixed& rhs) const noexcept { return raw == rhs.raw; }
		constexpr bool operator!=(const Fixed& rhs) const noexcept { return raw != rhs.raw; }
		constexpr bool operator< (const Fixed& rhs) const noexcept { return raw < rhs.raw; }
		constexpr bool operator> (const Fixed& rhs) const noexcept { return rhs.raw < raw; }
		constexpr bool operator<=(const Fixed& rhs) const noexcept { return !(rhs.raw < raw); }
		constexpr bool operator>=(const Fixed& rhs) const noexcept { return !(rhs.raw > raw); }

		constexpr bool operator==(const float& rhs) const noexcept { Fixed rhsFixed{ rhs }; return raw == rhsFixed.raw; }
		constexpr bool operator!=(const float& rhs) const noexcept { Fixed rhsFixed{ rhs }; return raw != rhsFixed.raw; }
		constexpr bool operator< (const float& rhs) const noexcept { Fixed rhsFixed{ rhs }; return raw < rhsFixed.raw; }
		constexpr bool operator> (const float& rhs) const noexcept { Fixed rhsFixed{ rhs }; return rhsFixed.raw < raw; }
		constexpr bool operator<=(const float& rhs) const noexcept { Fixed rhsFixed{ rhs }; return !(rhsFixed.raw < raw); }
		constexpr bool operator>=(const float& rhs) const noexcept { Fixed rhsFixed{ rhs }; return !(rhsFixed.raw > raw); }

		// Print helper
		friend std::ostream& operator<<(std::ostream& out, const Fixed& num) {
			out << static_cast<float>(num);
			return out;
		}

		// Math functions
		static Fixed lerp(Fixed a, Fixed b, Fixed progress) noexcept {
			return a + progress * (b - a);
		}

		static Fixed rand() {
			thread_local static std::mt19937 rng(std::random_device{}());
			static std::uniform_int_distribution<std::mt19937::result_type> dist(0, std::pow(2, decimalPoints));

			Fixed fixed{ 0 };

			fixed.raw = static_cast<int>(dist(rng));

			return fixed;
		}

		static Fixed abs(Fixed num) noexcept {
			if (num < Fixed(0)) {
				num = -num;
			}

			return num;
		}

		static Fixed sqrt(Fixed num) {

			if (num < 0.00003f) return num; // Division by zero risk in newtons method if less than this

			Fixed guess(num / Fixed(2));
			constexpr Fixed epsilon(0.1f);
			Fixed lastIter(guess);

			while (true) {
				Fixed tempLastIter = lastIter;
				lastIter = newtonsMethod(num, lastIter);
				if (abs(tempLastIter - lastIter) < epsilon) {
					break;
				}
			}

			return lastIter;
		}

		static Fixed newtonsMethod(Fixed a, Fixed lastIter) {
			constexpr Fixed fixed2 = Fixed(2);
			constexpr Fixed fixed0 = Fixed(0);

			if (lastIter <= fixed0 && a <= fixed0) return (fixed0);

			//         = (lastIter * lastIter - a) / (fixed2 * lastIter); This is what we're doing
			Fixed iter = fromRaw(divRaw(mulRaw(lastIter.raw, lastIter.raw) - a.raw, mulRaw(fixed2.raw, lastIter.raw)));
			return lastIter - iter;
		}

		static Fixed pow(Fixed base, unsigned exp) noexcept {

			if (exp >= decimalPoints) return fromRaw(std::numeric_limits<rawType>::max());

			Fixed total = base;
			for (int i = 1; i < exp; ++i) {
				total *= base;
			}

			return total;
		}

		int round() const noexcept {
			return static_cast<int>(fromRaw(raw + promoType((1 << (decimalPoints - 1))))); // 0.5
		}
	};
}

using Q16_16 = fp:: Fixed<int32_t, int64_t, 16>;
using Q24_8 = fp::Fixed<int32_t, int64_t, 8>;
using Q8_8 = fp::Fixed<int8_t, int64_t, 8>;