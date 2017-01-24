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
 * File:   Fsm.hpp
 * Author: Srivathsan Madhavan <m_dot_srivathsan_at_gmail_dot_com>
 * 
 * Created on 2 January, 2017, 10:06 AM IST (GMT+05:30)
 */

#ifndef __FSM_HPP__
#define __FSM_HPP__

#include <algorithm>
#include <sstream>
#include "Fsm.h"

namespace {
    template <typename T>
    bool isIn(const std::vector<T>& list, const T& val) {
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
    
    template <typename T>
    std::string default2String(const T& val) {
        std::stringstream ss;
        ss << val;
        return ss.str();
    }
    
    template <typename T>
    void dumpList(
            const std::vector<T>& vec,
            std::stringstream& ss,
            std::function<std::string (const T&)>& funcT2String = nullptr,
            const std::string& delim = ", "
        )
    {
        if (!funcT2String) funcT2String = default2String<T>;
        
        ss << "[";
        int ctr = 0;
        for (auto v: vec) {
            ss  << (ctr++ > 0 ? delim : "") << funcT2String(v);
        }
        ss << "]";
    }
} // namespace

template <typename TEvent, typename TState>
Fsm<TEvent, TState>::Fsm(
                        const EventList& events,
                        const StateList& states,
                        const TState& initState,
                        const std::string& name
                    )
    : m_events(events), m_states(states), m_currentState(initState), m_name(name)
{
    m_isFsmOk = (
            events.size() > 0 && states.size() > 0 && isIn(states, initState) &&
            !hasDuplicates(events) && !hasDuplicates(states)
        );
}

template <typename TEvent, typename TState>
Fsm<TEvent, TState>::Fsm(
        std::initializer_list<TEvent> events,
        std::initializer_list<TState> states,
        const TState& initState,
        const std::string& name
    ) : Fsm(EventList(events), StateList(states), initState, name)
{}

template <typename TEvent, typename TState>
Fsm<TEvent, TState>::~Fsm() {
}

template <typename TEvent, typename TState>
Fsm<TEvent, TState>::operator bool() const {
    return m_isFsmOk;
}

template <typename TEvent, typename TState>
bool Fsm<TEvent, TState>::registerTransition(
        const TEvent& event,
        const TState& currentState,
        HandlerFunc handler,
        std::initializer_list<TState> nextStates
    )
{
    return registerTransition(
            event,
            currentState,
            handler,
            StateList(nextStates)
        );
}
template <typename TEvent, typename TState>
bool Fsm<TEvent, TState>::registerTransition(
        const TEvent& event,
        const TState& currentState,
        HandlerFunc handler,
        const std::vector<TState>& nextStates
    )
{
    if (!m_isFsmOk || nextStates.size() == 0 ||
        !isIn(m_events, event) || !isIn(m_states, currentState))
    {
        return false;
    }
    for (auto nextState: nextStates) {
        if (!isIn(m_states, nextState))
            return false;
    }
    
    auto evStPair = std::make_pair(event, currentState);
    m_evStPairHandlers[evStPair] = handler;
    m_evStPairNextStates[evStPair] = nextStates;

    return true;
}

template <typename TEvent, typename TState>
bool Fsm<TEvent, TState>::raiseEvent(
                            const TEvent& event,
                            OnStateChangedFunc funcOnStateChanged
                        )
{
    if (!isIn(m_events, event) || !(*this)) return false;
    
    auto end        = m_evStPairHandlers.end();
    auto evStPair   = std::make_pair(event, m_currentState);
    auto itrFound   = m_evStPairHandlers.find(evStPair);
    auto nextState  = m_currentState;
    auto handler    = (itrFound != end ? itrFound->second : nullptr);
    if (!handler) {
        if (m_evStPairNextStates.find(evStPair) != m_evStPairNextStates.end())
            nextState = m_evStPairNextStates[evStPair][0];
    } else {
        nextState = handler();
        if (m_evStPairNextStates.size() == 0) {
            if (!isIn(m_states, nextState))
                return false;
        } else {
            if (!isIn(m_evStPairNextStates[evStPair], nextState))
                return false;
        }
    }
    
    if (funcOnStateChanged) {
        auto prevState = m_currentState;
        m_currentState = nextState;
        funcOnStateChanged(event, prevState, nextState);
    }
    return true;
}

template <typename TEvent, typename TState>
TState Fsm<TEvent, TState>::getCurrentState() const {
    return m_currentState;
}

template <typename TEvent, typename TState>
std::string Fsm<TEvent, TState>::dump(
                                    Event2StringFunc e2s,
                                    State2StringFunc s2s
                                ) const
{
    if (!e2s) e2s = default2String<TEvent>;
    if (!s2s) s2s = default2String<TState>;
    
    std::stringstream ss;
    ss  << "FSM Name     : '" << m_name << "'"                  << std::endl
        << "Status       : " << (m_isFsmOk ? "OK" : "ERROR")    << std::endl
        << "Valid Events : "; dumpList(m_events, ss, e2s); ss   << std::endl
        << "Valid States : "; dumpList(m_states, ss, s2s); ss   << std::endl
        << "Current State: " << s2s(m_currentState)             << std::endl
        << "Registered Transitions: "                           << std::endl;
    auto hEnd = m_evStPairHandlers.end();
    auto nsEnd = m_evStPairNextStates.end();
    for (auto ev: m_events) {
        for (auto st: m_states) {
            auto evstPair = std::make_pair(ev, st);
            auto itrEvStH = m_evStPairHandlers.find(evstPair);
            auto itrEvStNS = m_evStPairNextStates.find(evstPair);
            if (itrEvStH == hEnd && itrEvStNS == nsEnd)
                continue;

            ss << "\tON event='" << e2s(ev) << "' WHEN state='" << s2s(st);
            ss << "' AND handler=" << ((itrEvStH != hEnd && itrEvStH->second)
                                           ? "{....}" : "<NULL>");
            ss << " THEN NEXT state="; dumpList(itrEvStNS->second, ss, s2s, " | ");
            ss << std::endl;
        }
    }
    return ss.str();
}

#endif // __FSM_HPP__
