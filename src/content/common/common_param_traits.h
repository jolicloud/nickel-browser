// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file is used to define IPC::ParamTraits<> specializations for a number
// of types so that they can be serialized over IPC.  IPC::ParamTraits<>
// specializations for basic types (like int and std::string) and types in the
// 'base' project can be found in ipc/ipc_message_utils.h.  This file contains
// specializations for types that are used by the content code, and which need
// manual serialization code.  This is usually because they're not structs with
// public members..

#ifndef CONTENT_COMMON_COMMON_PARAM_TRAITS_H_
#define CONTENT_COMMON_COMMON_PARAM_TRAITS_H_
#pragma once

#include "base/platform_file.h"
#include "base/ref_counted.h"
#include "googleurl/src/gurl.h"
#include "ipc/ipc_message_utils.h"
#include "net/url_request/url_request_status.h"
// !!! WARNING: DO NOT ADD NEW WEBKIT DEPENDENCIES !!!
//
// That means don't add #includes to any file in 'webkit/' or
// 'third_party/WebKit/'. Chrome Frame and NACL build parts of base/ and
// content/common/ for a mini-library that doesn't depend on webkit.

// TODO(erg): The following headers are historical and only work because
// their definitions are inlined, which also needs to be fixed.
#include "webkit/glue/resource_type.h"

// Forward declarations.
class GURL;

namespace net {
class HttpResponseHeaders;
class HostPortPair;
class UploadData;
}

namespace webkit_glue {
struct ResourceDevToolsInfo;
struct ResourceLoadTimingInfo;
}

namespace IPC {

template <>
struct ParamTraits<GURL> {
  typedef GURL param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, void** iter, param_type* p);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<ResourceType::Type> {
  typedef ResourceType::Type param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, void** iter, param_type* p);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<net::URLRequestStatus> {
  typedef net::URLRequestStatus param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, void** iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<scoped_refptr<net::UploadData> > {
  typedef scoped_refptr<net::UploadData> param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, void** iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template<>
struct ParamTraits<net::HostPortPair> {
  typedef net::HostPortPair param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, void** iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<scoped_refptr<net::HttpResponseHeaders> > {
  typedef scoped_refptr<net::HttpResponseHeaders> param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, void** iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<webkit_glue::ResourceLoadTimingInfo> {
  typedef webkit_glue::ResourceLoadTimingInfo param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, void** iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<scoped_refptr<webkit_glue::ResourceDevToolsInfo> > {
  typedef scoped_refptr<webkit_glue::ResourceDevToolsInfo> param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, void** iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct ParamTraits<base::PlatformFileInfo> {
  typedef base::PlatformFileInfo param_type;
  static void Write(Message* m, const param_type& p);
  static bool Read(const Message* m, void** iter, param_type* r);
  static void Log(const param_type& p, std::string* l);
};

template <>
struct SimilarTypeTraits<base::PlatformFileError> {
  typedef int Type;
};

}  // namespace IPC

#endif  // CONTENT_COMMON_COMMON_PARAM_TRAITS_H_