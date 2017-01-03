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

int main(void) {
    std::vector<int> events = {1, 2, 3};
    std::vector<int> states = {11, 22, 33};
    Fsm<> fsmObj(events, states, 11);
    fsmObj.registerEventHandler(1, 11, nullptr, Fsm<int, int>::StateList({22}));
    fsmObj.registerEventHandler(1, 22, nullptr, Fsm<int, int>::StateList({33}));
    std::cout << fsmObj.dump(event2String<int>, state2String<int>) << std::endl;
    
//    std::vector<std::string> e = {"e1", "e2", "e3"};
//    std::vector<std::string> s = {"s1", "s2", "s3"};
//    Fsm<std::string, std::string> fsm2(e, s, s.back());
//    std::cout << fsm2.dump(event2String<std::string>, state2String<std::string>)
//            << std::endl;

    return 0;
}
