//
// Copyright (c) 2021, Лев Воробьёв
// All rights reserved.
//
// SPDX-License-Identifier: BSD-3-Clause
//

#ifndef _PURE_REPOSITORY_H
#define _PURE_REPOSITORY_H

#include "crud_repository.h"

namespace dbo {
    using namespace std;

    /**
     * Репозиторий для объектов, которые создаются в большом количестве автоматически
     */
    template<typename T>
    class pure_repository : public crud_repository<T> {
    protected:
        pure_repository() = default;

    public:
        shared_ptr<T> get(u_long id) const override { return nullptr; }

        u_long add(shared_ptr<T> const &object) override { return 0; }

        void edit(shared_ptr<T> const &object) override {}

        void each(function<void(const shared_ptr<T> &)> const &f) const override {}

        void remove(u_long id) override {}

        static pure_repository<T> &instance();

    protected:
        /**
         * Сохраняет объект в памяти
         * @param id номер объекта, долэен быть больше 0
         * @param object сохраняемый объект
         */
        virtual void save(u_long id, const shared_ptr<T> &object) const {}

        virtual bool exists(u_long id) const { return false; }
    };

    template<typename T>
    pure_repository<T> &pure_repository<T>::instance() {
        static pure_repository<T> _instance;
        return _instance;
    }

}

#endif //_PURE_REPOSITORY_H
