//
// Created by Lev on 07.03.2020.
//

#ifndef _CUSTOM_REPOSITORY_H
#define _CUSTOM_REPOSITORY_H

#include "object_repository.h"

namespace dbo {

    template <typename T, template <typename> class Base = object_repository>
    class custom_repository : public Base<T> {
    private:
        custom_repository();

    public:
        template<class ... Args>
        shared_ptr<T> find(Args &&... args);

        shared_ptr<T> get(u_long id) const override;

        u_long add(shared_ptr<T> const &object) override;

        void edit(shared_ptr<T> const &object) override;

        void remove(u_long id) override;

        static custom_repository<T,Base> &instance();
    };

    template<typename T, template <typename> class Base>
    custom_repository<T,Base>::custom_repository() {
        // perform database initiaization
    }

    template<typename T, template <typename> class Base>
    template<class... Args>
    shared_ptr<T> custom_repository<T,Base>::find(Args &&... args) {
        shared_ptr<T> object;
        // perform the database query
        if (object) {
            auto id = object->get_id();
            if (id) {
                if (! Base<T>::exists(id))
                    Base<T>::save(id, object);
                return Base<T>::get(id);
            }
        }
        return object;
    }

    template<typename T, template <typename> class Base>
    shared_ptr<T> custom_repository<T,Base>::get(u_long id) const {
        shared_ptr<T> object = Base<T>::get(id);
        if (! object) {
            // perform the database query
            if (object && id)
                Base<T>::save(id, object);
        }
        return object;
    }

    template<typename T, template <typename> class Base>
    u_long custom_repository<T,Base>::add(const shared_ptr<T> &object) {
        u_long id = object->get_id();
        if (id) {
            Base<T>::add(object);
        } else {
            // perform insertion
            if (id)
                Base<T>::save(id, object);
        }
        return id;
    }

    template<typename T, template <typename> class Base>
    void custom_repository<T,Base>::edit(const shared_ptr<T> &object) {
        // perform database update
    }

    template<typename T, template <typename> class Base>
    void custom_repository<T,Base>::remove(u_long id) {
        // perform deletion
        Base<T>::remove(id);
    }

    template<typename T, template <typename> class Base>
    custom_repository<T,Base> &custom_repository<T,Base>::instance() {
        static custom_repository<T,Base> _instance;
        return _instance;
    }

}

#endif //_CUSTOM_REPOSITORY_H
