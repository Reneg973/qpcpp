//$file${src::qf::qf_qact.cpp} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: qpcpp.qm
// File:  ${src::qf::qf_qact.cpp}
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
//$endhead${src::qf::qf_qact.cpp} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#define QP_IMPL             // this is QP implementation
#include "qp_port.hpp"      // QP port
#include "qp_pkg.hpp"       // QP package-scope interface
#include "qsafe.h"          // QP Functional Safety (FuSa) Subsystem
#ifdef Q_SPY                // QS software tracing enabled?
    #include "qs_port.hpp"  // QS port
    #include "qs_pkg.hpp"   // QS facilities for pre-defined trace records
#else
    #include "qs_dummy.hpp" // disable the QS software tracing
#endif // Q_SPY

// unnamed namespace for local definitions with internal linkage
namespace {
Q_DEFINE_THIS_MODULE("qf_qact")
} // unnamed namespace

//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpcpp version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${QF::QActive::QActive} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace QP {

} // namespace QP
//$enddef${QF::QActive::QActive} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${QF::QActive::register_} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace QP {

//${QF::QActive::register_} ..................................................
void QActive::register_() noexcept {
    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    if (m_pthre == 0U) { // preemption-threshold not defined?
        m_pthre = m_prio; // apply the default
    }

    #ifndef Q_UNSAFE

    Q_REQUIRE_INCRIT(100, (0U < m_prio) && (m_prio <= QF_MAX_ACTIVE)
                      && (registry_[m_prio] == nullptr)
                      && (m_prio <= m_pthre));

    std::uint8_t prev_thre = m_pthre;
    std::uint8_t next_thre = m_pthre;

    std::uint_fast8_t p;
    for (p = static_cast<std::uint_fast8_t>(m_prio) - 1U; p > 0U; --p) {
        if (registry_[p] != nullptr) {
            prev_thre = registry_[p]->m_pthre;
            break;
        }
    }
    for (p = static_cast<std::uint_fast8_t>(m_prio) + 1U;
         p <= QF_MAX_ACTIVE; ++p)
    {
        if (registry_[p] != nullptr) {
            next_thre = registry_[p]->m_pthre;
            break;
        }
    }

    Q_ASSERT_INCRIT(190, (prev_thre <= m_pthre)
                          && (m_pthre <= next_thre));

    m_prio_dis  = static_cast<std::uint8_t>(~m_prio);
    m_pthre_dis = static_cast<std::uint8_t>(~m_pthre);

    #endif // Q_UNSAFE

    // register the AO at the QF-prio.
    registry_[m_prio] = this;

    QF_MEM_APP();
    QF_CRIT_EXIT();
}

} // namespace QP
//$enddef${QF::QActive::register_} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${QF::QActive::unregister_} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
namespace QP {

//${QF::QActive::unregister_} ................................................
void QActive::unregister_() noexcept {
    std::uint_fast8_t const p = static_cast<std::uint_fast8_t>(m_prio);

    QF_CRIT_STAT
    QF_CRIT_ENTRY();
    QF_MEM_SYS();

    Q_REQUIRE_INCRIT(200, (0U < p) && (p <= QF_MAX_ACTIVE)
                      && (registry_[p] == this));
    registry_[p] = nullptr; // free-up the priority level
    m_state.fun = nullptr; // invalidate the state

    QF_MEM_APP();
    QF_CRIT_EXIT();
}

} // namespace QP
//$enddef${QF::QActive::unregister_} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
