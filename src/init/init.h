// This file is part of dont_trust.
// Copyright (C) 2023 Kenedy Henrique Bueno Silva

// dont_trust is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef init
#define init
#include <stdbool.h>
#include <stdint.h>
#include <signal.h>

#define ONION_ADDR 0
#define IPV4_ADDR 1

typedef struct {
  char *ipV4;
  char *onionAddress;
  //is true when the any of the above is not given
  bool shouldActAsServer;
  int8_t chosenOption;
} Configs;

void getConfigs(int argc, char *argv[], Configs *configsToUpdate);
void enableSignalDrivenIoOnSocket(int8_t socketFd, void (*handler)(int, siginfo_t *, void *));

#endif // !init
