#===============================================================================
#  Copyright 2016 "Srivathsan Madhavan - m_dot_srivathsan_at_gmail_dot_com"
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.
#===============================================================================

#
# NOTE: This may not be the most ideal Makefile.  Simplicity was overwhelmingly
# preferred over everything else.
#

TARGET_MAIN = FSMTest
TARGETS     = $(TARGET_MAIN)
SRCS        = main.cpp Fsm.hpp Fsm.h

FLAGS += -std=c++11
ifndef RELEASE
	FLAGS += -g
else
	FLAGS += -O3
endif

.PHONY: all clean

all: $(TARGETS)

$(TARGET_MAIN): $(SRCS)
	g++ $(FLAGS) $(SRCS) -o $(TARGET_MAIN)

clean:
	@rm -f $(TARGETS)
