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

int main(void) {
    std::vector<int> events = {1, 2, 3};
    std::vector<int> states = {11, 22, 33};
    Fsm<> fsmObj(events, states, 11);
    
    std::cout << fsmObj << ", " << fsmObj.getCurrentState() << std::endl;
    return 0;
}
