/*
 * Copyright 2017 Srivathsan Madhavan <m_dot_srivathsan_at_gmail_dot_com>.
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
 * File:   FsmT.cpp
 * Author: Srivathsan Madhavan <m_dot_srivathsan_at_gmail_dot_com>
 * 
 * Created on 2 January, 2017, 10:06 AM IST (GMT+05:30)
 */

#include <algorithm>
#include "Fsm.h"

#ifndef __FSM_HPP__
#define __FSM_HPP__

namespace {
    template <typename T>
    bool isInList(const std::vector<T>& list, const T& val) {
        return (!list.empty() &&
                std::find(list.begin(), list.end(), val) != list.end());
    }
    
    template <typename T>
    bool hasDuplicates(const std::vector<T>& list) {
        for (auto elem: list) {
            if (std::count(list.begin(), list.end(), elem) > 1)
                return true;
        }
        return false;
    }
} // namespace

template <typename TEvent, typename TState>
Fsm<TEvent, TState>::Fsm(
                        const EventList& events,
                        const StateList& states,
                        const TState& initState
                    )
{
    m_isFsmOk = (
            events.size() > 0 && states.size() > 0 && isInList(states, initState)
        );
    if (!m_isFsmOk) return;
    
    if (hasDuplicates(events) || hasDuplicates(states)) {
        m_isFsmOk = false;
        return;
    }

    // Everything is fine; do the main initialization
    m_currentState = initState;
    m_events = events;
    m_states = states;
}

template <typename TEvent, typename TState>
Fsm<TEvent, TState>::~Fsm() {
}

template <typename TEvent, typename TState>
Fsm<TEvent, TState>::operator bool() const {
    return m_isFsmOk;
}

template <typename TEvent, typename TState>
bool Fsm<TEvent, TState>::registerEventHandler(
        const TEvent& event,
        const TState& currentState,
        Handler handler,
        const std::vector<TState>& nextStates
    )
{
    if (!m_isFsmOk || nextStates.size() == 0 ||
        !isInList(m_events, event) || !isInList(m_states, currentState))
    {
        return false;
    }
    for (auto nextState: nextStates) {
        if (!isInList(m_states, nextState))
            return false;
    }
    
    auto evStPair = std::make_pair(event, currentState);
    m_evStPairHandlers[evStPair] = handler;
    m_evStPairNextStates[evStPair] = nextStates;

    return true;
}

template <typename TEvent, typename TState>
bool Fsm<TEvent, TState>::raiseEvent(const TEvent& event) {
    if (!isInList(m_events, event) || !(*this))
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
    if (isInList(m_evStPairNextStates[evStPair], nextState) ||
        isInList(m_events, nextState) )
    {
        m_currentState = nextState;
        return true;
    }
    return false;
}

template <typename TEvent, typename TState>
TState Fsm<TEvent, TState>::getCurrentState() const {
    return m_currentState;
}

#endif // __FSM_HPP__
