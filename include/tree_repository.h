//
// Copyright (c) 2021, Лев Воробьёв
// All rights reserved.
//
// SPDX-License-Identifier: BSD-3-Clause
//

#ifndef _TREE_REPOSITORY_H
#define _TREE_REPOSITORY_H

#include "object_repository.h"
#include <boost/optional.hpp>

namespace dbo {
    using namespace std;
    using boost::optional;

    /**
     * Репозиторий для объектов, которые создаются в большом количестве автоматически
     */
    template<typename T>
    class tree_repository : public object_repository<T> {
        mutable optional<size_t> _first;
        mutable map<u_long, shared_ptr<T>> _tree;

    protected:
        tree_repository() = default;

    public:
        static tree_repository<T> &instance();

    protected:
        /**
         * Сохраняет объект в памяти
         * @param id номер объекта, должен быть больше 0
         * @param object сохраняемый объект
         */
        void save(u_long id, const shared_ptr<T> &object) const override;

        bool exists(u_long id) const override;
    };

    template<typename T>
    tree_repository<T> &tree_repository<T>::instance() {
        static tree_repository<T> _instance;
        return _instance;
    }

    template<typename T>
    void tree_repository<T>::save(u_long id, const shared_ptr<T> &object) const {
        if (! _first)
            _first = id;
        if (id >= _first.value())
            object_repository<T>::save(id, object);
        else
            _tree[id] = object;
    }

    template<typename T>
    bool tree_repository<T>::exists(u_long id) const {
        return _first && id < _first.value()? _tree.find(id) != _tree.end() : object_repository<T>::exists(id);
    }

}

#endif //_TREE_REPOSITORY_H
