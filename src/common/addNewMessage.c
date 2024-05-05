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

#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <signal.h>
#include <unistd.h>

void addNewMessage(AllMessages *allMessages, char *message, uint16_t size, uint8_t owner) {
  char *text = malloc(size);
  memcpy(text, message, size);

  Message *newMessage = malloc(sizeof(Message));
  memset(newMessage, 0, sizeof(Message));
  newMessage->size = size;
  newMessage->status = PEER_NOT_RECEIVED;
  newMessage->string = text;
  newMessage->nextMessage = NULL;

  if(allMessages->sizeInChars > 0) {
    allMessages->lastMessage->nextMessage = newMessage;
    newMessage->previousMessage = allMessages->lastMessage;
  } else {
    newMessage->previousMessage = NULL;
    allMessages->currentStartingMessage = newMessage;
    allMessages->currentStartingMessageCharPosition = 0;
  }

  allMessages->lastMessage = newMessage;
  allMessages->sizeInChars += size;

  if(owner == MESSAGE_FROM_MYSELF) {
    newMessage->code = allMessages->messagesByCode.currentSize-allMessages->messagesByCode.availableSpace;
    if(allMessages->messagesByCode.availableSpace > 0) {
      allMessages->messagesByCode.array[allMessages->messagesByCode.currentSize-allMessages->messagesByCode.availableSpace] = newMessage;
      allMessages->messagesByCode.availableSpace--;
    } else {
      Message **newArray = malloc(sizeof(Message)*allMessages->messagesByCode.currentSize+DEFAULT_SIZE_FOR_MESSAGES_BY_CODE_ARRAY);
      memcpy(newArray, allMessages->messagesByCode.array, allMessages->messagesByCode.currentSize);
      free(allMessages->messagesByCode.array);
      allMessages->messagesByCode.array = newArray;
      allMessages->messagesByCode.currentSize = allMessages->messagesByCode.currentSize+DEFAULT_SIZE_FOR_MESSAGES_BY_CODE_ARRAY;

      allMessages->messagesByCode.array[allMessages->messagesByCode.currentSize-allMessages->messagesByCode.availableSpace] = newMessage;
      allMessages->messagesByCode.availableSpace = DEFAULT_SIZE_FOR_MESSAGES_BY_CODE_ARRAY-1;
    }
    allMessages->messagesByCode.length++;
  } else {
    newMessage->status = RECEIVED;
  }

  if(!allMessages->socketOutputStatus.isThereAnythingBeingSent && allMessages->socketOutputStatus.isThereAnySpaceOnTheSocketSendBuffer) {
    static union sigval sigVal;
    // sigVal.sival_int = 3;
    sigqueue(getpid(), SIGRTMIN, sigVal);
  }
}
