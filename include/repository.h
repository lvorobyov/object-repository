//
// Copyright (c) 2021, Лев Воробьёв
// All rights reserved.
//
// SPDX-License-Identifier: BSD-3-Clause
//

#ifndef _REPOSITORY_H
#define _REPOSITORY_H

#include "object_repository.h"

namespace dbo {

    template <typename T>
    class repository : public object_repository<T> {
    private:
        repository() = default;

    public:
        static repository<T> &instance();
    };

    template<typename T>
    repository<T> &repository<T>::instance() {
        static repository<T> _instance;
        return _instance;
    }

}

#endif //_REPOSITORY_H
