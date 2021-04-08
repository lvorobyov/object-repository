//
// Copyright (c) 2021, Лев Воробьёв
// All rights reserved.
//
// SPDX-License-Identifier: BSD-3-Clause
//

#ifndef _BASE_REPOSITORY_H
#define _BASE_REPOSITORY_H

#include "crud_repository.h"
#include <mutex>

namespace dbo {

    template<typename T>
    class base_repository : public crud_repository<T> {
        mutable mutex _cs;

    public:
        shared_ptr<T> get(u_long id) const override;

        u_long add(const shared_ptr<T> &object) override;

        void edit(const shared_ptr<T> &object) override;

        void each(const function<void(const shared_ptr<T> &)> &f) const override;

        void remove(u_long id) override;

    protected:
        virtual crud_repository<T> &super() const = 0;

        virtual void store(shared_ptr<T> const &object) const = 0;
    };

    template<typename T>
    shared_ptr<T> base_repository<T>::get(u_long id) const {
        lock_guard<mutex> lc(_cs);
        auto object = super().get(id);
        store(object);
        return object;
    }

    template<typename T>
    u_long base_repository<T>::add(const shared_ptr<T> &object) {
        lock_guard<mutex> lc(_cs);
        store(object);
        return super().add(object);
    }

    template<typename T>
    void base_repository<T>::edit(const shared_ptr<T> &object) {
        super().edit(object);
    }

    template<typename T>
    void base_repository<T>::each(const function<void(const shared_ptr<T> &)> &f) const {
        lock_guard<mutex> lc(_cs);
        super().each([&](const shared_ptr<T> &obj) {
            store(obj);
            f(obj);
        });
    }

    template<typename T>
    void base_repository<T>::remove(u_long id) {
        super().remove(id);
    }

}

#endif //_BASE_REPOSITORY_H
