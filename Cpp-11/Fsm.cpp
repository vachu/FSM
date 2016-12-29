/*
 * Copyright 2016 Srivathsan Madhavan <m_dot_srivathsan_at_gmail_dot_com>.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* 
 * File:   Fsm.cpp
 * Author: Srivathsan Madhavan <m_dot_srivathsan_at_gmail_dot_com>
 * 
 * Created on 26 December, 2016, 8:00 PM IST (GMT+05:30)
 */
#include <algorithm>

#include "Fsm.h"

namespace {
    bool isEmptyString(const std::string& s) {
        return (s.empty() || s.find_first_not_of(" \t\r\n\v\f\0") == s.npos);
    }
}

Fsm::Fsm(const StrList& events, const StrList& states, const std::string& initState)
{
    auto stBegin = states.begin(); auto stEnd = states.end();
    auto evBegin = events.begin(); auto evEnd = events.end();
    m_isFsmOk = (
            events.size() > 0 &&
            states.size() > 0 &&
            std::find_if(stBegin, stEnd, isEmptyString) == stEnd && // no empty state value
            std::find_if(evBegin, evEnd, isEmptyString) == evEnd && // no empty event value
            isValid(states, initState)
        );
    if (!m_isFsmOk) return;
    
    // check for repetitions in events and states vectors
    for (auto ev: events) {
        if (std::count(evBegin, evEnd, ev) > 1) {
            m_isFsmOk = false;
            return;
        }
    }
    for (auto st: states) {
        if (std::count(stBegin, stEnd, st) > 1) {
            m_isFsmOk = false;
            return;
        }
    }

    // Everything is fine; do the main initialization
    m_currentState = initState;
    m_events = events;
    m_states = states;
}

Fsm::~Fsm() {
}

Fsm::operator bool() const {
    return m_isFsmOk;
}

bool Fsm::registerEventHandler(
        const std::string& event,
        const std::string& currentState,
        Handler handler,
        const StrList& nextStates
    )
{
    if (!m_isFsmOk || nextStates.size() == 0 ||
        !isValid(m_events, event) || !isValid(m_states, currentState))
    {
        return false;
    }
    for (auto nextState: nextStates) {
        if (!isValid(m_states, nextState))
            return false;
    }
    
    auto evStPair = std::make_pair(event, currentState);
    m_evStPairHandlers[evStPair] = handler;
    m_evStPairNextStates[evStPair] = nextStates;

    return true;
}

bool Fsm::isValid(const StrList& list, const std::string& val) const {
    return (m_isFsmOk && !list.empty() && !isEmptyString(val) &&
            std::find(list.begin(), list.end(), val) != list.end());
}

bool Fsm::raiseEvent(const std::string& event) {
    if (!isValid(m_events, event) || !(*this))
        return false;
    
    auto end = m_evStPairHandlers.end();
    auto evStPair = std::make_pair(event, m_currentState);
    auto itrFound = m_evStPairHandlers.find(evStPair);
    auto handler = itrFound->second;
    if (itrFound == end || !handler) {
        if (m_evStPairNextStates.find(evStPair) != m_evStPairNextStates.end()) {
            m_currentState = m_evStPairNextStates[evStPair][0];
        }
        return true;
    }
    
    auto nextState = handler();
    if (!isValid(m_evStPairNextStates[evStPair], nextState))
        return false;
    
    m_currentState = nextState;
    return true;
}

std::string Fsm::getCurrentState() const {
    return m_currentState;
}
