//
// Created by Raphaël Dantzer on 10/01/17.
//

#ifndef VOXELS_UTILS_H
#define VOXELS_UTILS_H

#include <cstddef>
#include <fstream>

namespace utils {
template<std::size_t X = 0, std::size_t Y = 0, std::size_t Z = 0, typename T = std::size_t>
struct dimension_helper {
    /**
     * 3d to 1d index helper
     */
    static constexpr T linear(const T x, const T y, const T z) {
        return x + X * (y + Y * z);
    }

    static constexpr T width{X};

    static constexpr T height{Y};

    static constexpr T depth{Z};

    static constexpr T half_width{width / 2};
    static constexpr T half_height{height / 2};
    static constexpr T half_depth{depth / 2};

    static constexpr T size{width * height * depth};
};

/**
 * In case no c++14
 */
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
};

std::string readFile(const char *filename) {
    std::ifstream f(filename);
    std::string str;

    if (!f.is_open())
        throw std::runtime_error("Unable to open file");
    f.seekg(0, std::ios::end);
    str.reserve(static_cast<unsigned long>(f.tellg()));
    f.seekg(0, std::ios::beg);
    str.assign(std::istreambuf_iterator<char>(f), std::istreambuf_iterator<char>());
    f.close();
    return str;
}

}

#endif //VOXELS_UTILS_H
