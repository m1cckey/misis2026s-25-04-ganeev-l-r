
#pragma once
#ifndef ARRAYD_ARRAYD_HPP_20251120
#define ARRAYD_ARRAYD_HPP_20251120

#include <cstddef>

class ArrayT {
public:
  ArrayT() = default;

  ArrayT(const ArrayT&);
  
  //! \param size - начальный размер, 0 < size
  ArrayT(const std::ptrdiff_t size);
  
  ~ArrayT();
  
  ArrayT& operator=(const ArrayT&);

  [[nodiscard]] std::ptrdiff_t size() const noexcept { return size_; }
  
  //! \param size - новый размер, 0 <= size
  void resize(const std::ptrdiff_t size);
  
  //! \param idx - индекс  элемента, 0 <= idx < Size()
  [[nodiscard]] float& operator[](const std::ptrdiff_t idx);
  [[nodiscard]] float operator[](const std::ptrdiff_t idx) const;

  //! \param idx - индекс вставляемого элемента, 0 <= idx <= size 
  void insert(const std::ptrdiff_t idx, const float val);

  //! \param idx - индекс удаляемого элемента, 0 <= idx < size 
  void remove(const std::ptrdiff_t idx);

private:
  std::ptrdiff_t capacity_ = 0;  //!< размер буффера
  std::ptrdiff_t size_ = 0;      //!< число элементов в массиве
  float* data_ = nullptr;             //!< буффер
};

#endif
