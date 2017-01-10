//
// Created by Raphaël Dantzer on 10/01/17.
//

#ifndef VOXELS_CHUNK_H
#define VOXELS_CHUNK_H

#include "utils.h"

namespace chunk {
namespace detail {

/**
 * constants storage
 */
struct constants {
    template<class T>
    static constexpr T CX{16};

    template<class T>
    static constexpr T CY{32};

    template<class T>
    static constexpr T CZ{16};

    template<class T>
    static constexpr T CHUNKSLOTS{CX<T> * CY<T> * CZ<T>};
};

typedef utils::dimension_helper<
        constants::CX<std::size_t>,
        constants::CY<std::size_t>,
        constants::CZ<std::size_t>
> dimension;

}

using detail::dimension;

template<class T>
class Data {
protected:
    typedef std::unique_ptr<Data> ChunkPtr;

    std::vector<T> _blk;
    ChunkPtr _left, _right, _below, _above, _front, _back;

    bool _changed{true};
    bool _initialized{false};

public:

    int _x, _y, _z; //todo find something else

    Data() {};

    Data(int x, int y, int z) : _x(x), _y(y), _z(z) {};

    ~Data() {}

    T get(int x, int y, int z) const {
        if (x < 0)
            return _left ? _left->_blk[dimension::linear(x + dimension::width, y, z)] : 0;
        if (x >= dimension::width)
            return _right ? _right->_blk[dimension::linear(x - dimension::width, y, z)] : 0;
        if (y < 0)
            return _below ? _below->_blk[dimension::linear(x, y + dimension::height, z)] : 0;
        if (y >= dimension::height)
            return _above ? _above->_blk[dimension::linear(x, y - dimension::height, z)] : 0;
        if (z < 0)
            return _front ? _front->_blk[dimension::linear(x, y, z + dimension::depth)] : 0;
        if (z >= dimension::depth)
            return _back ? _back->_blk[dimension::linear(x, y, z - dimension::depth)] : 0;
        return _blk[dimension::linear(x, y, z)];
    }

    bool isBlocked(int x1, int y1, int z1, int x2, int y2, int z2) const {
        if (!_blk[dimension::linear(x1, y1, z1)])
            return true;
        //todo implement
    }

    void set(int x, int y, int z, const T &data) {
        if (x < 0) {
            if (_left)
                return _left->set(x + dimension::width, y, z, data);
        }
        if (x >= dimension::width) {
            if (_right)
                return _right->set(x - dimension::width, y, z, data);
        }
        if (y < 0) {
            if (_below)
                return _below->set(x, y + dimension::width, z, data);
        }
        if (y >= dimension::height) {
            if (_above)
                return _above->set(x, y - dimension::width, z, data);
        }
        if (z < 0) {
            if (_front)
                return _front->set(x, y, z + dimension::depth, data);
        }
        if (z >= dimension::depth) {
            if (_back)
                return _back->set(x, y, z - dimension::depth, data);
        }

        _blk[dimension::linear(x, y, z)] = data;
        _changed = true;

#ifdef NEIGHBOOR_VISIBILITY_UPDATE
        if (x == 0 && _left)
            _left->_changed = true;
        if (x == dimension::width - 1 && _right)
            _right->_changed = true;
        if (y == 0 && _below)
            _below->_changed = true;
        if (y == dimension::height - 1 && _above)
            _above->_changed = true;
        if (z == 0 && _front)
            _front->_changed = true;
        if (z == dimension::depth - 1 && _back)
            _back->_changed = true;
#endif
    }

    bool operator == (const Data &rhs) {
        return rhs._x == _x && rhs._y == _y && rhs._z == _z;
    }

    bool isChanged() const {
        return _changed;
    }

    void setChanged(bool changed) {
        _changed = changed;
    }

    bool isInitialized() const {
        return _initialized;
    }

    void setInitialized(bool initialized) {
        _initialized = initialized;
    }
};

template <typename T>
class DataHandler : public Data<T> {
public:
    virtual ~DataHandler() {};

    virtual void        handle() = 0;
    virtual void        update() = 0;
    virtual void        render() = 0;
};
}

#endif //VOXELS_CHUNK_H
