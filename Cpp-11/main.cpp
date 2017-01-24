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
#include <string>
#include <algorithm>
#include "Fsm.h"

int getConsoleInput(
        std::string& cmd,
        const std::initializer_list<const char *>& validCmds = {}
    )
{
    auto end = validCmds.end();
    auto begin = validCmds.begin();
    
    std::cout << "Enter command/event: ";
    if (!std::getline(std::cin, cmd) || cmd == "quit" || cmd == "exit") {
        return 0; // for 'false'
    } else if (cmd == "?" || cmd == "help") {
        std::stringstream ss;
        ss << "[";
        int ctr = 0;
        for (auto c: validCmds) {
            ss  << (ctr++ > 0 ? ", " : "") << c;
        }
        ss << ", ?/help, exit/quit]";
        std::cerr << "Valid commands --> " << ss.str() << std::endl;
        return -1;
    } else if (validCmds.size() > 0 && std::find(begin, end, cmd) == end) {
        std::cerr << "ERROR: Illegal command specified" << std::endl;
        return -1;
    } else {
        return 1;
    }
};

void onStateChanged(
        const std::string& event,
        const std::string& currState,
        const std::string& nextState
    )
{
    auto transition = (currState != nextState ? "GOING TO" : "REMAINING IN");
    std::cout << "RAISED event='" << event << "' WHEN state='" << currState
            << "'; " << transition << " state='" << nextState << "'" << std::endl;
}

template <typename TEvent, typename TState>
void runFsm(
        Fsm<TEvent, TState>& fsm,
        const std::initializer_list<const char *>& validCmds = {}
    )
{
    std::string cmd;
    for (int ret = 0; (ret = getConsoleInput(cmd, validCmds)) != 0;) {
        if (ret != 1) {
            continue;
        } else if (cmd == "dump") {
            std::cout << fsm.dump() << std::endl;
        } else if (!fsm.raiseEvent(cmd, onStateChanged)) {
            std::cerr << "ERROR: failed to raise event '" << cmd << "'"
                    << std::endl;
        }
    }
}

void simpleTurnstileFsm() {
    Fsm<std::string, std::string> turnstile(
                {"ticket", "push"},  // Events
                {"Locked", "UNLOCKED"},             // States
                "Locked",                           // Initial State
                "Bengaluru Metro Passenger Turnstile"
            );
    if (!turnstile) {
        std::cout << turnstile.dump();
        return;
    }
    turnstile.registerTransition("ticket", "Locked", nullptr, {"UNLOCKED"});
    turnstile.registerTransition("push", "UNLOCKED", nullptr, {"Locked"});

    auto validCmds = {"ticket", "push", "dump"};
    runFsm(turnstile, validCmds);
}

void stockTradingFsm() {
    Fsm<std::string, std::string> dayTrade(
                {"buy", "sell", "close"},           // Events - trade types
                {"NONE", "LONG", "SHORT"},          // States - open positions
                "NONE",                             // Initial State - position
                "Vatsan's Stock-trading XChange"
            );
    if (!dayTrade) {
        std::cout << dayTrade.dump();
        return;
    }
    dayTrade.registerTransition("buy", "NONE", nullptr, {"LONG", "NONE"});
    dayTrade.registerTransition("sell", "NONE", nullptr, {"SHORT", "NONE"});
    dayTrade.registerTransition("sell", "LONG", nullptr, {"SHORT", "LONG"});
    dayTrade.registerTransition("buy", "SHORT", nullptr, {"LONG", "SHORT"});
    dayTrade.registerTransition("close", "SHORT", nullptr, {"NONE"});
    dayTrade.registerTransition("close", "LONG", nullptr, {"NONE"});
    
    auto validCmds = {"buy", "sell", "close", "dump"};
    runFsm(dayTrade, validCmds);
}

int main(void) {
//    stockTradingFsm();
    simpleTurnstileFsm();
    return 0;
}
