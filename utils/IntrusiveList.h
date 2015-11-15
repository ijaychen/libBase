#ifndef INTRUSIVE_LIST_H
#define INTRUSIVE_LIST_H

#include "../Common.h"

#define INTRUSIVE_LIST(t) \
public:\
    base::utils::IntrusiveListMemberHook<t> list_member_hook_;\
    t* list_next() { return list_member_hook_.m_list_next; } \
    bool list_linked() const { return list_member_hook_.m_list != nullptr; }
#include <iostream>

namespace base
{
    namespace utils
    {
        template<typename T>
        class IntrusiveList;
        
        template<typename T>
        struct IntrusiveListMemberHook
        {
            IntrusiveListMemberHook()
                : m_list_pre(nullptr), m_list_next(nullptr), m_list(nullptr) {}
            T* m_list_pre;
            T* m_list_next;
            IntrusiveList<T>* m_list;
        };

        template<typename T>
        class IntrusiveList
        {
        public:
            IntrusiveList()
                : m_head(NULL), m_size(0) {}

            bool empty() const {
                return m_size == 0;
            }

            size_t size() const {
                return m_size;
            }

            T* front() {
                return m_head;
            }

            bool contains(T* item) const {
	      /*std::cout << "this = " << this << std::endl;
	      std::cout << "item = " << item << std::endl;
	      std::cout << "&item->list_member_hook_ = " << &(item->list_member_hook_) << std::endl;
	      std::cout << "item->list_member_hook_.list_ = " << item->list_member_hook_.list_ << std::endl;*/
                return item->list_member_hook_.m_list == this;
            }
            
            template<typename T1>
            static T1* next(T1* item) {
                return item->list_member_hook_.m_list_next;
            }

            T* erase(T* item) {
                T* next = nullptr;
                if (item == m_head) {
                    if (m_head->list_member_hook_.m_list_next != nullptr) {
                        m_head->list_member_hook_.m_list_next->list_member_hook_.m_list_pre = nullptr;
                    }
                    m_head = m_head->list_member_hook_.m_list_next;
                    next = m_head;
                } else {
                    if (item->list_member_hook_.m_list_next != nullptr) {
                        item->list_member_hook_.m_list_next->list_member_hook_.m_list_pre = item->list_member_hook_.m_list_pre;
                    }
                    if (item->list_member_hook_.m_list_pre != nullptr) {
                        item->list_member_hook_.m_list_pre->list_member_hook_.m_list_next = item->list_member_hook_.m_list_next;
                    }
                    next = item->list_member_hook_.m_list_next;
                }
                --m_size;
                item->list_member_hook_.m_list_next = nullptr;
                item->list_member_hook_.m_list_pre = nullptr;
                item->list_member_hook_.m_list = nullptr;
                return next;
            }

            void push_front(T* item) {
                if (m_head == NULL) {
                    item->list_member_hook_.m_list_pre = nullptr;
                    item->list_member_hook_.m_list_next = nullptr;
                    m_head = item;
                } else {
                    item->list_member_hook_.m_list_next = m_head;
                    item->list_member_hook_.m_list_pre = nullptr;
                    m_head->list_member_hook_.m_list_pre = item;
                    m_head = item;
                }
                item->list_member_hook_.m_list = this;
                ++m_size;
            }

        private:
            T* m_head;
            size_t m_size;
        };
    }
}

#endif
