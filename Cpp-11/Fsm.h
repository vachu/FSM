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
 * File:   Fsm.h
 * Author: Srivathsan Madhavan <m_dot_srivathsan_at_gmail_dot_com>
 *
 * Created on 26 December, 2016, 8:00 PM
 */
#include <map>
#include <vector>
#include <string>
#include <functional>

#ifndef FSM_H
#define FSM_H

class Fsm {
public:
    typedef std::function<std::string ()> Handler;
    typedef std::vector<std::string> StrList;
    
    Fsm() = delete;
    Fsm(const StrList& events, const StrList& states,
            const std::string& initState);
    virtual ~Fsm();
    
    bool registerEventHandler(
            const std::string& event,
            const std::string& currentState,
            Handler func,
            const StrList& nextStates = {}
        );
    bool raiseEvent(const std::string& event);
    std::string getCurrentState() const;
    
    operator bool() const;
    
private:
    typedef std::pair<std::string, std::string> EventStatePair;
    
    std::string m_currentState;
    StrList m_events, m_states;
    bool m_isFsmOk;

    std::map<EventStatePair, StrList> m_evStPairNextStates;
    std::map<EventStatePair, Handler> m_evStPairHandlers;

    bool isValid(const StrList& list, const std::string& state) const;
};

#endif /* FSM_H */

