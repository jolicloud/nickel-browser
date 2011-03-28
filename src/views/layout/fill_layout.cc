// Copyright (c) 2011 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "views/layout/fill_layout.h"

#include "base/logging.h"

namespace views {

FillLayout::FillLayout() {
}

FillLayout::~FillLayout() {
}

void FillLayout::Layout(View* host) {
  if (!host->has_children())
    return;

  View* frame_view = host->GetChildViewAt(0);
  frame_view->SetBounds(0, 0, host->width(), host->height());
}

gfx::Size FillLayout::GetPreferredSize(View* host) {
  DCHECK(host->child_count() == 1);
  return host->GetChildViewAt(0)->GetPreferredSize();
}

}  // namespace views