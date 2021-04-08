//
// Copyright (c) 2021, Лев Воробьёв
// All rights reserved.
//
// SPDX-License-Identifier: BSD-3-Clause
//

#ifndef _CRUD_REPOSITORY_H
#define _CRUD_REPOSITORY_H

#include <memory>
#include <functional>

namespace dbo {
    using namespace std;

    template<typename T>
    class crud_repository {
    public:
        virtual ~crud_repository() = default;
        virtual shared_ptr<T> get(u_long id) const = 0;
        virtual u_long add(shared_ptr<T> const &object) = 0;
        virtual void edit(shared_ptr<T> const &object) = 0;
        virtual void each(function<void(const shared_ptr<T> &)> const &f) const = 0;
        virtual void remove(u_long id) = 0;
    };

}

#endif //_CRUD_REPOSITORY_H
