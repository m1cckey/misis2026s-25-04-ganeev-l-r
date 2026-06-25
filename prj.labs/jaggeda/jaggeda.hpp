// 2025 by Sugrobov A.A. under Unlicense

#pragma once
#ifndef JAGGEDA_JAGGEDA_HPP_20250525
#define JAGGEDA_JAGGEDA_HPP_20250525

#include <cstddef>
#include <cstdint>
#include <initializer_list>


class JaggedA {
public:
    //! \brief Proxy class for row access via operator[]
    class RowProxy {
    public:
        RowProxy() = default;
        RowProxy(std::int32_t* row, std::size_t size) noexcept;

        //! \param col - column index, no bounds checking
        [[nodiscard]] std::int32_t& operator[](std::size_t col) noexcept;
        [[nodiscard]] const std::int32_t& operator[](std::size_t col) const noexcept;

        [[nodiscard]] std::size_t size() const noexcept;
    private:
        std::int32_t* row_ = nullptr;  
        std::size_t size_ = 0;        
    };

    JaggedA() = default;
    ~JaggedA();

    //! \param rows - 
    //! \param default_row_size - 
    JaggedA(std::size_t rows, std::size_t default_row_size = 0);

    JaggedA(std::initializer_list<std::initializer_list<std::int32_t>> init);

    JaggedA(const JaggedA& src);
    JaggedA& operator=(const JaggedA& rhs);

    JaggedA(JaggedA&& other) noexcept;
    JaggedA& operator=(JaggedA&& other) noexcept;

    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] std::size_t size(std::size_t row) const;
    [[nodiscard]] std::size_t row_size(std::size_t row) const;
    [[nodiscard]] bool empty() const noexcept;

    //! \param new_row_count - new number of rows, new rows are empty
    void resize_rows(std::size_t new_row_count);

    //! \param row - row index
    //! \param new_size - new row size, existing data is preserved
    void resize_row(std::size_t row, std::size_t new_size);

    void clear();

    void push_back_row(std::size_t row_size = 0);
    void pop_back_row();

    //! \param pos - insertion position, 
    void insert_row(std::size_t pos, std::size_t row_size = 0);

    //! \param pos - row to erase, 
    void erase_row(std::size_t pos);

    //! \param row - row index,
    [[nodiscard]] RowProxy operator[](std::size_t row);
    [[nodiscard]] const RowProxy operator[](std::size_t row) const;

    //! \param row - row index, 
    //! \param col - column index, 
    [[nodiscard]] std::int32_t& at(std::size_t row, std::size_t col);
    [[nodiscard]] const std::int32_t& at(std::size_t row, std::size_t col) const;

    [[nodiscard]] std::int32_t** data() noexcept;
    [[nodiscard]] const std::int32_t* const* data() const noexcept;

    void swap(JaggedA& other) noexcept;

    [[nodiscard]] bool operator==(const JaggedA& other) const;
    [[nodiscard]] bool operator!=(const JaggedA& other) const;

private:
    std::int32_t** data_ = nullptr;    
    std::size_t* row_sizes_ = nullptr; 
    std::size_t row_count_ = 0;        
    std::size_t row_capacity_ = 0;     
};

#endif // !JAGGEDA_JAGGEDA_HPP_20250525
