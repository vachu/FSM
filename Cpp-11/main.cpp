/*
 *  Copyright 2016 "Srivathsan Madhavan - m_dot_srivathsan_at_gmail_dot_com"
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <iostream>
#include <utility>
#include "Fsm.h"

template <typename T>
std::string event2String(const T& val) {
    std::stringstream ss;
    ss << "Event-" << val;
    return ss.str();
}

template <typename T>
std::string state2String(const T& val) {
    std::stringstream ss;
    ss << "State-" << val;
    return ss.str();
}

void OnStateChange(
        const std::string& event,
        const std::string& currState,
        const std::string& nextState
    )
{
    std::cout << "RAISED event='" << event << "' WHEN state='" << currState
            << "'; GOING TO state='" << nextState << "'" << std::endl;
}

int main(void) {
    Fsm<std::string, std::string> turnstile(
                {"Insert Coin", "Push Turnstile"},
                {"Locked", "UNLOCKED"},
                "Locked"
            );
    if (!turnstile) {
        std::cout << turnstile.dump();
        return 1;
    }
    turnstile.registerTransition("Insert Coin", "Locked", nullptr, {"UNLOCKED"});
    turnstile.registerTransition("Push Turnstile", "UNLOCKED", nullptr, {"Locked"});
    
    turnstile.raiseEvent("Insert Coin", OnStateChange);
    turnstile.raiseEvent("Push Turnstile", OnStateChange);
    return 0;
}
