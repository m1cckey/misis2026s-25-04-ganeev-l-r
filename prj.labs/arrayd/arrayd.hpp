// 2025 by Polevoi Dmitry under Unlicense
#pragma once
#ifndef ARRAYD_ARRAYD_HPP_20251120
#define ARRAYD_ARRAYD_HPP_20251120

#include <cstddef>

class ArrayD {
public:
	ArrayD() = default;

	ArrayD(const ArrayD&);

	//! \param size
	ArrayD(const std::ptrdiff_t size);

	~ArrayD();

	ArrayD& operator=(const ArrayD&);

	[[nodiscard]] std::ptrdiff_t size() const noexcept { return size_; }

	//! \param size
	void resize(const std::ptrdiff_t size);

	//! \param idx 
	[[nodiscard]] float& operator[](const std::ptrdiff_t idx);
	[[nodiscard]] float operator[](const std::ptrdiff_t idx) const;

	//! \param idx
	void insert(const std::ptrdiff_t idx, const float val);

	//! \param idx
	void remove(const std::ptrdiff_t idx);

private:
	std::ptrdiff_t capacity_ = 0;  
	std::ptrdiff_t size_ = 0;      
	float* data_ = nullptr;            
};

#endif // !ARRAYD_ARRAYD_HPP_20251120
