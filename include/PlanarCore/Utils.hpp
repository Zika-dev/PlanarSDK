#pragma once

#include <vector>
#include <type_traits>
#include <limits>
#include <random>
#include <bit>
#include <cstdint>

// Bit manipulation macros
#define GET_BITS(x, pos) ((x & ( 1 << pos)) >> pos)
#define CLEAR_BIT(x, pos) (x &= (~(1U<< pos)))
#define TOGGLE_BIT(x, pos) x ^= (1U<< pos)

struct RGB888 {

	RGB888(uint8_t R, uint8_t G, uint8_t B) : R(R), G(G), B(B) {}

	RGB888() = default;

	RGB888 applyBrightness(float brightness) {
		return RGB888(R * brightness, G * brightness, B * brightness);
	}

	uint8_t R, G, B;
};
	
union RGB565 {
	uint16_t value;
	struct {
		uint16_t  blue : 5;
		uint16_t  green : 6;
		uint16_t  red : 5;
	} color;

	RGB565() : value(0) { }
	RGB565(uint16_t value) : value(value) { }
};

inline RGB565 RGB888To565(uint8_t r, uint8_t g, uint8_t b) {
	RGB565 color{};

	color.color.red = (r * 31) / 255;
	color.color.green = (g * 63) / 255;
	color.color.blue = (b * 31) / 255;

	return color;
}

inline RGB565 hexTo565(int hex) {
	RGB565 color{};

	uint8_t r = (hex >> 16) & 0xFF;
	uint8_t g = (hex >> 8) & 0xFF;
	uint8_t b = hex & 0xFF;
 
	color.color.red = (r * 31) / 255;
	color.color.green = (g * 63) / 255;
	color.color.blue = (b * 31) / 255;

	return color;
}

enum class BlendMode {
	NORMAL,
	ADDITIVE
};

// Generic Vec2
template<typename T>
struct Vec2
{
    T x, y;

    // Constructors
    Vec2() : x(0), y(0) {}
    Vec2(T xx, T yy) : x(xx), y(yy) {}

    // Allow implicit conversion from Vec2<U> to Vec2<T>
    template<typename U>
    Vec2(const Vec2<U>& o)
      : x(static_cast<T>(o.x)), y(static_cast<T>(o.y))
    {}

    // Arithmetic
	template<typename U>
	auto operator+(const Vec2<U>& o) const {
		using R = std::common_type_t<T, U>;
		return Vec2<R>(R(x) + R(o.x), R(y) + R(o.y));
	}
	template<typename U>
	auto operator-(const Vec2<U>& o) const {
		using R = std::common_type_t<T, U>;
		return Vec2<R>(R(x) - R(o.x), R(y) - R(o.y));
	}
	template<typename U>
	auto operator*(const Vec2<U>& o) const {
		using R = std::common_type_t<T, U>;
		return Vec2<R>(R(x) * R(o.x), R(y) * R(o.y));
	}
	template<typename U>
	auto operator/(const Vec2<U>& o) const {
		using R = std::common_type_t<T, U>;
		return Vec2<R>(R(x) / R(o.x), R(y) / R(o.y));
	}

	// Scalar
	template<typename U>
	auto operator+(U scalar) const {
		using R = std::common_type_t<T, U>;
		return Vec2<R>(R(x) + R(scalar), R(y) + R(scalar));
	}
	template<typename U>
	auto operator-(U scalar) const {
		using R = std::common_type_t<T, U>;
		return Vec2<R>(R(x) - R(scalar), R(y) - R(scalar));
	}
	template<typename U>
	auto operator*(U scalar) const {
		using R = std::common_type_t<T, U>;
		return Vec2<R>(R(x) * R(scalar), R(y) * R(scalar));
	}
	template<typename U>
	auto operator/(U scalar) const {
		using R = std::common_type_t<T, U>;
		return Vec2<R>(R(x) / R(scalar), R(y) / R(scalar));
	}

    // Component wise equality
    template<typename U>
    bool operator==(const Vec2<U>& o) const {
        return x == static_cast<T>(o.x)
            && y == static_cast<T>(o.y);
    }
    template<typename U>
    bool operator!=(const Vec2<U>& o) const {
        return !(*this == o);
    }

	// Comparison
	template<typename U>
	bool operator<(const Vec2<U>& other) const {
		if (x != other.x) return x < other.x;
		return y < other.y;
	}
	template<typename U>
	bool operator>(const Vec2<U>& other) const {
		if (x != other.x) return x > other.x;
		return y > other.y;
	}

	// Unary
	auto operator-() const {
		Vec2<T> v;
		v.x = -x;
		v.y = -y;
		return v;
	}

	T isZero() const {
		return abs(x) + abs(y) == 0;
	}

	template<typename U>
	T dot(Vec2<U> other) const {
		return x * other.x + y * other.y;
	}

	T magnitude() const {
		return sqrtf(x * x + y * y);
	}

	T invMagnitude() const {
		float mag = magnitude();
		if (mag == 0) {
			return std::numeric_limits<float>::max();
		}
		return 1.0f / mag;
	}

	T sqrMagnitude() const {
		return x * x + y * y;
	}

	Vec2<T> normalized() const {
		float invMag = invMagnitude();
		return Vec2(x * invMag, y * invMag);
	}

	Vec2<int> round() const {
		return Vec2<int>(x + 0.5f, y + 0.5f);
	}

	void clear() {
		x = 0;
		y = 0;
	}

	Vec2<T> inverse() const {
		return Vec2(1.0f / x, 1.0f / y);
	}

	Vec2<T> abs() const {
		return Vec2(std::abs(x), std::abs(y));
	}

	float distance(Vec2<T> other) const {
		return sqrtf((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
	}

	float distanceSqr(Vec2<T> other) const {
		return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y);
	}
};

namespace std {
	template <>
	struct hash<Vec2<int32_t>> {
		std::size_t operator()(const Vec2<int32_t>& v) const noexcept {
			std::size_t h1 = std::hash<int32_t>{}(v.x);
			std::size_t h2 = std::hash<int32_t>{}(v.y);
			return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
		}
	};
}

struct Region {
	uint32_t bitmapStart;
	uint16_t lineOffset, length;
};

using Shape = std::vector<std::vector<Region>>;
using Row = std::vector<Region>;

struct TextureDescriptor {
	Vec2<uint16_t> size;
	RGB565* bitmapPtr;
	uint32_t bitmapSize;
	uint16_t regionCount;
	Shape shape;
};

struct WavData {
	std::vector<uint8_t> samples;

	bool isValid() const noexcept {
		return samples.size() > 0;
	}
};

inline float lerp(float a, float b, float t) {
	return a + t * (b - a);
}

inline float isApproximately(float val, float midPoint, float offset) {
	return val <= midPoint + offset && val >= midPoint - offset;
}

inline size_t writeInt16(uint8_t* out, int16_t value, size_t& index) noexcept {
	out[index + 0] = (static_cast<int8_t>((value) & 0xFF));
	out[index + 1] = (static_cast<int8_t>((value >> 8) & 0xFF));
	index += 2;

	return index;
}

inline size_t writeInt32(uint8_t* out, int32_t value, size_t& index) noexcept {
	out[index + 0] = (static_cast<int8_t>(value & 0xFF));
	out[index + 1] = (static_cast<int8_t>((value >> 8) & 0xFF));
	out[index + 2] = (static_cast<int8_t>((value >> 16) & 0xFF));
	out[index + 3] = (static_cast<int8_t>((value >> 24) & 0xFF));
	index += 4;

	return index;
};

inline size_t writeFloat(uint8_t* out, float value, size_t& index) noexcept {
	writeInt32(out, std::bit_cast<uint32_t>(value), index);

	return index;
}

inline uint16_t readInt16(const uint8_t* in, size_t& index) noexcept {
	index += 2;
	return uint16_t(in[-2 + index]) | (uint16_t(in[-1 + index]) << 8);
}

inline uint32_t readInt32(const uint8_t* in, size_t& index) noexcept {
	index += 4;
	return uint32_t(in[-4 + index]) | (uint32_t(in[-3 + index]) << 8) | (uint32_t(in[-2 + index]) << 16) | (uint32_t(in[-1 + index]) << 24);
}

inline float readFloat(const uint8_t* in, size_t& index) noexcept {
	return std::bit_cast<float>(readInt32(in, index));
}

static std::mt19937 rng(std::random_device{}());
static std::uniform_int_distribution<std::mt19937::result_type> dist(0, std::numeric_limits<uint32_t>::max());

inline uint32_t fastRand(uint32_t n) {
	return static_cast<uint32_t>(dist(rng));
}

inline float fastRandf() {
	return fastRand(std::numeric_limits<uint32_t>::max()) / static_cast<float>(std::numeric_limits<uint32_t>::max());
}

inline int getSignf(const float f) { return 1 - (int)(((unsigned int&)f) >> 31) * 2; }