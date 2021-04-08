//
// Copyright (c) 2021, Лев Воробьёв
// All rights reserved.
//
// SPDX-License-Identifier: BSD-3-Clause
//

#ifndef _OBJECT_REPOSITORY_H
#define _OBJECT_REPOSITORY_H

#include <vector>
#include <algorithm>
#include "pure_repository.h"

namespace dbo {

    class already_added : public exception {
    public:
        const char *what() const noexcept override {
            return "The object already added";
        }
    };

    template<typename T>
    class object_repository : public pure_repository<T> {
        mutable vector<shared_ptr<T>> _objects;

    protected:
        object_repository() = default;

    public:
        shared_ptr<T> get(u_long id) const override;

        shared_ptr<T> find(const function<bool(shared_ptr<T> const &)> &pred) const;

        u_long add(shared_ptr<T> const &object) override;

        void edit(shared_ptr<T> const &object) override;

        void each(function<void(const shared_ptr<T> &)> const &f) const override;

        void remove(u_long id) override;

        void remove(const function<bool(shared_ptr<T> const &)> &pred);

        static object_repository<T> &instance();

    protected:
        /**
         * Сохраняет объект в памяти
         * @param id номер объекта, долэен быть больше 0
         * @param object сохраняемый объект
         */
        void save(u_long id, const shared_ptr<T> &object) const override;

        bool exists(u_long id) const override;
    };

    template<typename T>
    shared_ptr<T> object_repository<T>::get(u_long id) const {
        shared_ptr<T> object;
        if (_objects.size() >= id && id > 0)
            object = _objects[id - 1];
        return object;
    }

    template<typename T>
    shared_ptr<T> object_repository<T>::find(const function<bool(const shared_ptr<T> &)> &pred) const {
        auto it = find_if(_objects.begin(), _objects.end(), pred);
        return it != _objects.end() ? *it : nullptr;
    }

    template<typename T>
    u_long object_repository<T>::add(const shared_ptr<T> &object) {
        u_long id = object->get_id();
        if (id) {
            if (! exists(id))
                save(id, object);
            else
                throw already_added();
        } else {
            _objects.push_back(object);
            id = _objects.size();
            object->set_id(id);
        }
        return id;
    }

    template<typename T>
    void object_repository<T>::edit(const shared_ptr<T> &object) {
        u_long id = object->get_id();
        if (_objects.size() >= id && id > 0)
            _objects[id - 1] = object;
    }

    template<typename T>
    void object_repository<T>::each(function<void(const shared_ptr<T> &)> const &f) const {
        for_each(_objects.begin(), _objects.end(), f);
    }

    template<typename T>
    void object_repository<T>::remove(u_long id) {
        if (_objects.size() >= id && id > 0)
            _objects[id - 1].reset();
    }

    template<typename T>
    void object_repository<T>::remove(const function<bool(const shared_ptr<T> &)> &pred) {
        auto it = _objects.begin();
        do {
            it = find_if(it, _objects.end(), pred);
            if (it == _objects.end())
                break;
            it->reset();
        } while (true);
    }

    template<typename T>
    object_repository<T> &object_repository<T>::instance() {
        static object_repository<T> _instance;
        return _instance;
    }

    template<typename T>
    bool object_repository<T>::exists(u_long id) const { return _objects.size() >= id && id > 0 && _objects[id - 1]; }

    template<typename T>
    void object_repository<T>::save(u_long id, const shared_ptr<T> &object) const {
        if (_objects.size() < id)
            _objects.resize(id);
        _objects[id-1] = object;
    }

}

#endif //_OBJECT_REPOSITORY_H
