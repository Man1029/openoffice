/**************************************************************
 * 
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 * 
 *************************************************************/



#include "precompiled_sd.hxx"

#include "controller/SlsProperties.hxx"
#include <vcl/svapp.hxx>

namespace sd { namespace slidesorter { namespace controller {

Properties::Properties (void)
    : mbIsHighlightCurrentSlide(false),
      mbIsShowSelection(true),
      mbIsShowFocus(true),
      mbIsCenterSelection(false),
      mbIsSmoothSelectionScrolling(true),
      mbIsSuspendPreviewUpdatesDuringFullScreenPresentation(true),
      maBackgroundColor(Application::GetSettings().GetStyleSettings().GetWindowColor()),
      maTextColor(Application::GetSettings().GetStyleSettings().GetActiveTextColor()),
      maSelectionColor(Application::GetSettings().GetStyleSettings().GetHighlightColor()),
      maHighlightColor(Application::GetSettings().GetStyleSettings().GetMenuHighlightColor()),
      mbIsUIReadOnly(false),
      mbIsOnlyPreviewTriggersMouseOver(true),
      mbIsHighContrastModeActive(
          Application::GetSettings().GetStyleSettings().GetHighContrastMode())
{
}




Properties::~Properties (void)
{
}




void Properties::HandleDataChangeEvent (void)
{
    maBackgroundColor = Application::GetSettings().GetStyleSettings().GetWindowColor();
    maTextColor = Application::GetSettings().GetStyleSettings().GetActiveTextColor();
    maSelectionColor = Application::GetSettings().GetStyleSettings().GetHighlightColor();
    maHighlightColor = Application::GetSettings().GetStyleSettings().GetMenuHighlightColor();
    mbIsHighContrastModeActive 
        = Application::GetSettings().GetStyleSettings().GetHighContrastMode();
}




bool Properties::IsHighlightCurrentSlide (void) const
{
    return mbIsHighlightCurrentSlide;
}




void Properties::SetHighlightCurrentSlide (const bool bIsHighlightCurrentSlide)
{
    mbIsHighlightCurrentSlide = bIsHighlightCurrentSlide;
}




bool Properties::IsShowSelection (void) const
{
    return mbIsShowSelection;
}




void Properties::SetShowSelection (const bool bIsShowSelection)
{
    mbIsShowSelection = bIsShowSelection;
}




bool Properties::IsShowFocus (void) const
{
    return mbIsShowFocus;
}




void Properties::SetShowFocus (const bool bIsShowFocus)
{
    mbIsShowFocus = bIsShowFocus;
}




bool Properties::IsCenterSelection (void) const
{
    return mbIsCenterSelection;
}




void Properties::SetCenterSelection (const bool bIsCenterSelection)
{
    mbIsCenterSelection = bIsCenterSelection;
}




bool Properties::IsSmoothSelectionScrolling (void) const
{
    return mbIsSmoothSelectionScrolling;
}




void Properties::SetSmoothSelectionScrolling (const bool bIsSmoothSelectionScrolling)
{
    mbIsSmoothSelectionScrolling = bIsSmoothSelectionScrolling;
}




bool Properties::IsSuspendPreviewUpdatesDuringFullScreenPresentation (void) const
{
    return mbIsSuspendPreviewUpdatesDuringFullScreenPresentation;
}




void Properties::SetSuspendPreviewUpdatesDuringFullScreenPresentation (const bool bFlag)
{
    mbIsSuspendPreviewUpdatesDuringFullScreenPresentation = bFlag;
}




Color Properties::GetBackgroundColor (void) const
{
    return maBackgroundColor;
}




void Properties::SetBackgroundColor (const Color& rColor)
{
    maBackgroundColor = rColor;
}



Color Properties::GetTextColor (void) const
{
    return maTextColor;
}




void Properties::SetTextColor (const Color& rColor)
{
    maTextColor = rColor;
}




Color Properties::GetSelectionColor (void) const
{
    return maSelectionColor;
}




void Properties::SetSelectionColor (const Color& rColor)
{
    maSelectionColor = rColor;
}




Color Properties::GetHighlightColor (void) const
{
    return maHighlightColor;
}




void Properties::SetHighlightColor (const Color& rColor)
{
    maHighlightColor = rColor;
}




bool Properties::IsUIReadOnly (void) const
{
    return mbIsUIReadOnly;
}




void Properties::SetUIReadOnly (const bool bIsUIReadOnly)
{
    mbIsUIReadOnly = bIsUIReadOnly;
}




bool Properties::IsOnlyPreviewTriggersMouseOver (void) const
{
    return mbIsOnlyPreviewTriggersMouseOver;
}




void Properties::SetOnlyPreviewTriggersMouseOver (const bool bFlag)
{
    mbIsOnlyPreviewTriggersMouseOver = bFlag;
}




bool Properties::IsHighContrastModeActive (void) const
{
    return mbIsHighContrastModeActive;
}


} } } // end of namespace ::sd::slidesorter::controller
