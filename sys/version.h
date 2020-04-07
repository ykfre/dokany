/*
  Dokan : user-mode file system library for Windows

  Copyright (C) 2020 Google, Inc.

  http://dokan-dev.github.io

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3 of the License, or (at your option) any
later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along
with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef VERSION_H_
#define VERSION_H_

#include <minwindef.h>
#include <wdm.h>

#include "dokan.h"
#include "irp_buffer_helper.h"

inline BOOL IsUserVersionCompatible(ULONG UserVersion) {
  return UserVersion >= DOKAN_DRIVER_VERSION_1 &&
         UserVersion <= DOKAN_DRIVER_VERSION_2;
}

NTSTATUS GetEventStart(__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp,
                       __out PEVENT_START eventStart) {
  PEVENT_START_BASE baseEventStart = NULL;
  ULONG userVersion;

  DOKAN_INIT_LOGGER(logger, DeviceObject->DriverObject, 0);

  GET_IRP_BUFFER_OR_RETURN(Irp, baseEventStart);

  userVersion = baseEventStart->UserVersion;
  if (!IsUserVersionCompatible(userVersion)) {
    return DokanLogError(
        &logger, STATUS_INVALID_PARAMETER,
        L"Driver version check in event start failed. Received %lx",
        userVersion);
  }

  // Convert here all compatible version of EVENT_START
  // supported by IsUserVersionCompatible to the current one
  if (userVersion >= DOKAN_DRIVER_VERSION_1 &&
      userVersion <= DOKAN_DRIVER_VERSION_2) {
    PEVENT_START_V2 eventStartTmp = NULL;
    GET_IRP_BUFFER_OR_RETURN(Irp, eventStartTmp);
    RtlCopyMemory(eventStart, eventStartTmp, sizeof(EVENT_START_V2));
  } else {
    ASSERT("EVENT_START convertion missing");
  }

  return STATUS_SUCCESS;
}

#endif