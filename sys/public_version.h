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

#ifndef PUBLIC_VERSION_H_
#define PUBLIC_VERSION_H_

// Dokan version legacy to 1.3.1.1000 included
#define DOKAN_DRIVER_VERSION_1 0x0000190

// Dokan version from 1.4.0.1000 included
// Changelog:
// * IRP buffer size correctness checks were added.
//   In V1 the userland could in some case (i.e. QuerySecurity) send
//   a smaller buffer than EVENT_INFORMATION.
//
// Note: No changes for EVENT_START
#define DOKAN_DRIVER_VERSION_2 0x0000200

// Current driver version
// The define has to be updated when a new driver version is created
#define DOKAN_DRIVER_VERSION DOKAN_DRIVER_VERSION_2

// All EVENT_START members by version
#define EVENT_START_MEMBER_BASE ULONG UserVersion;

#define EVENT_START_MEMBER_V1                                                  \
  ULONG DeviceType;                                                            \
  ULONG Flags;                                                                 \
  WCHAR MountPoint[260];                                                       \
  WCHAR UNCName[64];                                                           \
  ULONG IrpTimeout;

// Versioned EVENT_START structure
// None of them should be used outside this file
// but instead the latest version EVENT_START

// The base of all EVENT_START only including the version supported by userland
// This is only used to fetch version number sent by userland at start.
typedef struct _EVENT_START_BASE {
  EVENT_START_MEMBER_BASE
} EVENT_START_BASE, *PEVENT_START_BASE;

// EVENT_START compatible with DOKAN_DRIVER_VERSION_1
typedef struct _EVENT_START_V1 {
  EVENT_START_MEMBER_BASE
  EVENT_START_MEMBER_V1
} EVENT_START_V1, *PEVENT_START_V1;

// EVENT_START compatible with DOKAN_DRIVER_VERSION_2
typedef EVENT_START_V1 EVENT_START_V2, *PEVENT_START_V2;

// Current EVENT_START
// The define has to be updated when a new driver version is created
// EVENT_START is the only struct that has be used outside this file
typedef EVENT_START_V2 EVENT_START, *PEVENT_START;

#endif