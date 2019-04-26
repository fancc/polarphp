//===--- PlatformKinds.def - Swift PlatformKind Metaprogramming -*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2017 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
// See https://swift.org/LICENSE.txt for license information
// See https://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
// This source file is part of the polarphp.org open source project
//
// Copyright (c) 2017 - 2018 polarphp software foundation
// Copyright (c) 2017 - 2018 zzu_softboy <zzu_softboy@163.com>
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See https://polarphp.org/LICENSE.txt for license information
// See https://polarphp.org/CONTRIBUTORS.txt for the list of polarphp project authors
//
// Created by polarboy on 2019/04/26.
//===----------------------------------------------------------------------===//
//
// This file defines macros used for macro-metaprogramming with platform kinds.
//
//===----------------------------------------------------------------------===//

/// AVAILABILITY_PLATFORM(X, PrettyName)
///   X - The name of the platform
///   PrettyName - A string with the platform name for pretty printing
#ifndef AVAILABILITY_PLATFORM
#define AVAILABILITY_PLATFORM(X, PrettyName)
#endif

// Reordering these platforms will break serialization.
AVAILABILITY_PLATFORM(iOS, "iOS")
AVAILABILITY_PLATFORM(tvOS, "tvOS")
AVAILABILITY_PLATFORM(watchOS, "watchOS")
AVAILABILITY_PLATFORM(OSX, "OS X")
AVAILABILITY_PLATFORM(iOSApplicationExtension, "iOS application extension")
AVAILABILITY_PLATFORM(tvOSApplicationExtension, "tvOS application extension")
AVAILABILITY_PLATFORM(watchOSApplicationExtension, "watchOS application extension")
AVAILABILITY_PLATFORM(OSXApplicationExtension, "OS X application extension")

#undef AVAILABILITY_PLATFORM
