//
// Created by Raphaël Dantzer on 10/01/17.
//

#ifndef VOXELS_CHUNKBUFFER_H
#define VOXELS_CHUNKBUFFER_H

#include "Data.h"

namespace buffer {
namespace detail {
/**
 * constants storage
 */
struct constants {
    template<class T>
    static constexpr T SCX{32};

    template<class T>
    static constexpr T SCY{2};

    template<class T>
    static constexpr T SCZ{16};
};

typedef utils::dimension_helper<
        constants::SCX<std::size_t>,
        constants::SCY<std::size_t>,
        constants::SCZ<std::size_t>
> dimension;

constexpr bool is_in_bounds(const int x, const int y, const int z) {
    return (x < 0 || x >= dimension::width || y < 0 || y >= dimension::height || z <= 0 || z >= dimension::depth);
}

}

template<class T>
class ChunkBuffer {
public:
    typedef std::unique_ptr<chunk::Data<T>> ChunkHdl;
private:
    std::vector<ChunkHdl> _buffer;

    inline bool isInBounds(const int x, const int y, const int z) const {
        std::size_t cx = (x + chunk::detail::dimension::width * (detail::dimension::width / 2)) /
                         chunk::detail::dimension::width;
        std::size_t cy = (y + chunk::detail::dimension::height * (detail::dimension::height / 2)) /
                         chunk::detail::dimension::height;
        std::size_t cz = (z + chunk::detail::dimension::depth * (detail::dimension::depth / 2)) /
                         chunk::detail::dimension::depth;
        return detail::is_in_bounds(cx, cy, cz);
    }

public:
    ChunkBuffer() {
        _buffer.reserve(detail::dimension::size);
        for (int x = 0; x < detail::dimension::width; x++)
            for (int y = 0; y < detail::dimension::height; y++)
                for (int z = 0; z < detail::dimension::depth; z++)
                    _buffer.push_back(std::make_unique<chunk::Data<T>>(x - detail::dimension::half_width,
                                                                       y - detail::dimension::half_height,
                                                                       z - detail::dimension::half_depth));

    }

    ~ChunkBuffer() {}

    T get(const int x, const int y, const int z) const {
        if (isInBounds(x, y, z))
            return T();
        return _buffer[detail::dimension::linear(x, y, z)]->get(
                x & (detail::dimension::width - 1),
                y & (detail::dimension::height - 1),
                z & (detail::dimension::depth - 1));
    }

    void set(const std::size_t x, const std::size_t y, const std::size_t z, const T &data) {
        if (isInBounds(x, y, z))
            return;
        _buffer[detail::dimension::linear(x, y, z)]->set(
                x & (detail::dimension::width - 1),
                y & (detail::dimension::height - 1),
                z & (detail::dimension::depth - 1),
                data);

    }

    const std::vector<ChunkHdl> &data() const {
        return _buffer;
    }

    template<typename F>
    void apply(const F &func) {
        for (auto &it : _buffer)
            func(it);
    }
};

}


#endif //VOXELS_CHUNKBUFFER_H
