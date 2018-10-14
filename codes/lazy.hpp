/**
 * Lazy -- Lazy Evaluator
 *
 * Copyright 2013 Ali AslRousta
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#if !defined(__LAZY_HPP__)
#define __LAZY_HPP__

#include <functional>
#include <stdexcept>
#include <mutex>

namespace lazyx {

    template<typename T>
    class lazy {
        private:
            static T default_initiator() {
                throw std::runtime_error("No lazy evaluator given.");
            }
            std::function<T()> m_initiator;
            T    m_value;
            bool m_initialized;
        public:
            lazy() : m_initiator(default_initiator), m_initialized(false) { }
            lazy(std::function<T()> initiator) : m_initiator(initiator), m_initialized(false) { }
            lazy(const lazy<T>& other) : m_initiator(other.m_initiator), m_initialized(false) { }

            lazy<T>& operator= (const lazy<T>& other) {
                m_initiator   = other.m_initiator;
                m_initialized = false;
                return *this;
            }

            T& get_value() {
                if (!m_initialized) {
                    m_value      = m_initiator();
                    m_initialied = true;
                }
                return m_value;
            }

            T& operator* () {
                return get_value();
            }
    };

    template<typename T>
    class lazy_safe {
        private:
            static T default_initiator() {
                throw std::runtime_error("No lazy evaluator given.");
            }
            std::function<T()> m_initiator;
            std::mutex m_lock;
            T    m_value;
            bool m_initialized;
        public:
            lazy_safe() : m_initiator(default_initiator) { }
            lazy_safe(std::function<T()> initiator) : m_initiator(initiator), m_initialized(false) { }
            lazy_safe(const lazy_safe<T>& other) : m_initiator(other.m_initiator), m_initialized(false) { }

            lazy_safe<T>& operator= (const lazy_safe<T>& other) {
                m_lock.lock();
                m_initiator   = other.m_initiator;
                m_initialized = false;
                m_lock.unlock();
                return *this;
            }

            T& get_value() {
                if (!m_initialized) {
                    m_lock.lock();
                    if (!m_initialized) {
                        m_value       = m_initiator();
                        m_initialized = true;
                    }
                    m_lock.unlock();
                }
                return m_value;
            }

            T& operator* () {
                return get_value();
            }
    };

}

#endif/* __LAZY_HPP__ */
