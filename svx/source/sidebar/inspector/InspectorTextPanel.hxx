/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */
#pragma once

#include <sfx2/sidebar/IContextChangeReceiver.hxx>
#include <sfx2/weldutils.hxx>
#include <vcl/EnumContext.hxx>
#include <sfx2/sidebar/PanelLayout.hxx>

namespace svx
{
namespace sidebar
{
class InspectorTextPanel : public PanelLayout, public ::sfx2::sidebar::IContextChangeReceiver
{
public:
    virtual ~InspectorTextPanel() override;
    virtual void dispose() override;

    static VclPtr<vcl::Window> Create(vcl::Window* pParent,
                                      const css::uno::Reference<css::frame::XFrame>& rxFrame);

    virtual void HandleContextChange(const vcl::EnumContext& rContext) override;

    InspectorTextPanel(vcl::Window* pParent,
                       const css::uno::Reference<css::frame::XFrame>& rxFrame);

private:
    std::unique_ptr<weld::TreeView> mxListBoxStyles; // To dump all the properties

    vcl::EnumContext maContext;
};
}
} // end of namespace svx::sidebar

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
