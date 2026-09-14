#pragma once

#include <memory>

struct UncloneableComponentBase {
	static constexpr bool cloneable = false;
};

template<typename T>
struct CloneableComponentBase {
	static constexpr bool cloneable = true;
	virtual std::unique_ptr<T> clone() const = 0;
};

template<typename Derived, typename Base>
struct CloneableComponent : Base {
	std::unique_ptr<Base> clone() const override {
		return std::make_unique<Derived>(static_cast<const Derived&>(*this));
	}

private:
	CloneableComponent() = default;

	friend Derived;
};