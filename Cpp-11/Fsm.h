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
 * File:   FsmT.h
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

template <typename TEvent = int, typename TState = int>
class Fsm {
public:
    typedef std::function<TState ()> Handler;
    typedef std::vector<TState> StateList;
    typedef std::vector<TEvent> EventList;
    
    Fsm() = delete;
    Fsm(const EventList& events, const StateList& states,
            const TState& initState);
    virtual ~Fsm();

    bool registerEventHandler(
            const TEvent& event,
            const TState& currentState,
            Handler func,
            const StateList& nextStates = {}
        );
    bool raiseEvent(const TEvent& event);
    TState getCurrentState() const;
    
    operator bool() const;
    
private:
    typedef std::pair<TEvent, TState> EventStatePair;
    
    TState m_currentState;
    EventList m_events;
    StateList m_states;
    bool m_isFsmOk;

    std::map<EventStatePair, StateList> m_evStPairNextStates;
    std::map<EventStatePair, Handler> m_evStPairHandlers;
};

#include "Fsm.hpp"

#endif /* FSMT_H */

