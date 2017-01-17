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
 * File:   Fsm.h
 * Author: Srivathsan Madhavan <m_dot_srivathsan_at_gmail_dot_com>
 *
 * Created on 2 January, 2017, 10:06 AM
 */

#ifndef FSMT_H
#define FSMT_H

#include <map>
#include <vector>
#include <string>
#include <functional>
#include <initializer_list>

template <typename TEvent = int, typename TState = int>
class Fsm {
public:
    typedef std::function<void (const TEvent&, const TState&, const TState&)>
        OnStateChangedFunc;
    typedef std::function<std::string (const TEvent&)> Event2StringFunc;
    typedef std::function<std::string (const TState&)> State2StringFunc;
    typedef std::function<TState ()> HandlerFunc;
    typedef std::vector<TState> StateList;
    typedef std::vector<TEvent> EventList;
    
    Fsm() = delete;
    Fsm(const EventList& events, const StateList& states,
            const TState& initState);
    Fsm(
        std::initializer_list<TEvent> events,
        std::initializer_list<TState> states,
        const TState& initState
    );
    virtual ~Fsm();

    bool registerTransition(
            const TEvent& event,
            const TState& currentState,
            HandlerFunc func,
            const StateList& nextStates = {}
        );
    bool registerTransition(
            const TEvent& event,
            const TState& currentState,
            HandlerFunc func,
            std::initializer_list<TState> nextStates = {}
        );
    bool raiseEvent(const TEvent& event, OnStateChangedFunc funcStChg = nullptr);
    TState getCurrentState() const;
    
    operator bool() const;
    std::string dump(
                    Event2StringFunc e2s = nullptr,
                    State2StringFunc s2s = nullptr
                ) const;
    
private:
    typedef std::pair<TEvent, TState> EventStatePair;
    
    TState m_currentState;
    EventList m_events;
    StateList m_states;
    bool m_isFsmOk;

    std::map<EventStatePair, StateList> m_evStPairNextStates;
    std::map<EventStatePair, HandlerFunc> m_evStPairHandlers;
    
    Event2StringFunc m_ev2str;
    State2StringFunc m_st2str;
};

#include "Fsm.hpp"

#endif /* FSMT_H */

