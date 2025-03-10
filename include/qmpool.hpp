//$file${include::qmpool.hpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: qpcpp.qm
// File:  ${include::qmpool.hpp}
//
// This code has been generated by QM 5.3.0 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// This code is covered by the following QP license:
// License #    : LicenseRef-QL-dual
// Issued to    : Any user of the QP/C++ real-time embedded framework
// Framework(s) : qpcpp
// Support ends : 2024-12-31
// License scope:
//
// Copyright (C) 2005 Quantum Leaps, LLC <state-machine.com>.
//
//                    Q u a n t u m  L e a P s
//                    ------------------------
//                    Modern Embedded Software
//
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-QL-commercial
//
// This software is dual-licensed under the terms of the open source GNU
// General Public License version 3 (or any later version), or alternatively,
// under the terms of one of the closed source Quantum Leaps commercial
// licenses.
//
// The terms of the open source GNU General Public License version 3
// can be found at: <www.gnu.org/licenses/gpl-3.0>
//
// The terms of the closed source Quantum Leaps commercial licenses
// can be found at: <www.state-machine.com/licensing>
//
// Redistributions in source code must retain this top-level comment block.
// Plagiarizing this software to sidestep the license obligations is illegal.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//
//$endhead${include::qmpool.hpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#ifndef QMPOOL_HPP_
#define QMPOOL_HPP_

#ifndef QF_MPOOL_SIZ_SIZE
    #define QF_MPOOL_SIZ_SIZE 2U
#endif
#ifndef QF_MPOOL_CTR_SIZE
    #define QF_MPOOL_CTR_SIZE 2U
#endif

namespace QP {

#if (QF_MPOOL_SIZ_SIZE == 1U)
    using QMPoolSize = std::uint8_t;
#elif (QF_MPOOL_SIZ_SIZE == 2U)
    using QMPoolSize = std::uint16_t;
#elif (QF_MPOOL_SIZ_SIZE == 4U)
    using QMPoolSize = std::uint32_t;
#else
    #error "QF_MPOOL_SIZ_SIZE defined incorrectly, expected 1U, 2U, or 4U"
#endif

#if (QF_MPOOL_CTR_SIZE == 1U)
    using QMPoolCtr = std::uint8_t;
#elif (QF_MPOOL_CTR_SIZE == 2U)
    using QMPoolCtr = std::uint16_t;
#elif (QF_MPOOL_CTR_SIZE == 4U)
    using QMPoolCtr = std::uint32_t;
#else
    #error "QF_MPOOL_CTR_SIZE defined incorrectly, expected 1U, 2U, or 4U"
#endif

} // namespace QP

#define QF_MPOOL_EL(evType_) struct { \
    QP::QFreeBlock sto_[((sizeof(evType_) - 1U) \
                          / sizeof(QP::QFreeBlock)) + 1U]; }
//$declare${QF::QFreeBlock} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace QP {

//${QF::QFreeBlock} ..........................................................
class QFreeBlock {
private:
    QFreeBlock * m_next;

#ifndef Q_UNSAFE
    std::uintptr_t m_next_dis;
#endif // ndef Q_UNSAFE
    friend class QMPool;
}; // class QFreeBlock

} // namespace QP
//$enddecl${QF::QFreeBlock} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$declare${QF::QMPool} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace QP {

//${QF::QMPool} ..............................................................
class QMPool {
private:
    QFreeBlock * m_start;
    QFreeBlock * m_end;
    QFreeBlock * volatile m_free_head;
    QMPoolSize m_blockSize;
    QMPoolCtr m_nTot;
    QMPoolCtr volatile m_nFree;
    QMPoolCtr m_nMin;

public:
    QMPool()
      : m_start(nullptr),
        m_end(nullptr),
        m_free_head(nullptr),
        m_blockSize(0U),
        m_nTot(0U),
        m_nFree(0U),
        m_nMin(0U)
    {}

#ifdef Q_XTOR
    ~QMPool();
#endif // def Q_XTOR
    void init(
        void * const poolSto,
        std::uint_fast32_t const poolSize,
        std::uint_fast16_t const blockSize) noexcept;
    void * get(
        std::uint_fast16_t const margin,
        std::uint_fast8_t const qs_id) noexcept;
    void put(
        void * const block,
        std::uint_fast8_t const qs_id) noexcept;
    QMPoolSize getBlockSize() const noexcept;
    QMPoolCtr getNMin() const noexcept {
        return m_nMin;
    }
    QMPoolCtr getNFree() const noexcept {
        return m_nFree;
    }

private:
    QMPool(QEQueue const & other) = delete;
    QMPool & operator=(QMPool const & other) = delete;

public:

#ifdef QF_ISR_API
    void * getFromISR(
        std::uint_fast16_t const margin,
        std::uint_fast8_t const qs_id) noexcept;
#endif // def QF_ISR_API

#ifdef QF_ISR_API
    void putFromISR(
        void * const b,
        std::uint_fast8_t const qs_id) noexcept;
#endif // def QF_ISR_API
}; // class QMPool

} // namespace QP
//$enddecl${QF::QMPool} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#endif // QMPOOL_HPP_
