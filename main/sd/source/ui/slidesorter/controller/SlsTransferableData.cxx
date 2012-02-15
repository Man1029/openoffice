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



// MARKER(update_precomp.py): autogen include statement, do not remove
#include "precompiled_sd.hxx"

#include "controller/SlsTransferableData.hxx"

#include "SlideSorterViewShell.hxx"
#include "View.hxx"

namespace sd { namespace slidesorter { namespace controller {

SdTransferable* TransferableData::CreateTransferable (
    SdDrawDocument* pSrcDoc, 
    ::sd::View* pWorkView, 
    sal_Bool bInitOnGetData,
    SlideSorterViewShell* pViewShell,
    const ::std::vector<Representative>& rRepresentatives)
{
    SdTransferable* pTransferable = new SdTransferable (pSrcDoc, pWorkView, bInitOnGetData);
    ::boost::shared_ptr<TransferableData> pData (new TransferableData(pViewShell, rRepresentatives));
    pTransferable->AddUserData(pData);
    return pTransferable;
}




::boost::shared_ptr<TransferableData> TransferableData::GetFromTransferable (const SdTransferable* pTransferable)
{
    ::boost::shared_ptr<TransferableData> pData;
    for (sal_Int32 nIndex=0,nCount=pTransferable->GetUserDataCount(); nIndex<nCount; ++nIndex)
    {
        pData = ::boost::dynamic_pointer_cast<TransferableData>(pTransferable->GetUserData(nIndex));
        if (pData)
            return pData;
    }
    return ::boost::shared_ptr<TransferableData>();
}




TransferableData::TransferableData (
    SlideSorterViewShell* pViewShell,
    const ::std::vector<Representative>& rRepresentatives)
    : mpViewShell(pViewShell),
      maRepresentatives(rRepresentatives)
{
    if (mpViewShell != NULL)
        StartListening(*mpViewShell);
}




TransferableData::~TransferableData (void)
{
    if (mpViewShell != NULL)
        EndListening(*mpViewShell);
}




void TransferableData::DragFinished (sal_Int8 nDropAction)
{
	if (mpViewShell != NULL)
		mpViewShell->DragFinished(nDropAction);
    /*
    for (CallbackContainer::const_iterator
             iCallback(maDragFinishCallbacks.begin()),
             iEnd(maDragFinishCallbacks.end());
         iCallback!=iEnd;
         ++iCallback)
    {
        if (*iCallback)
            (*iCallback)(nDropAction);
    }
    maDragFinishCallbacks.clear();
    */
}




void TransferableData::Notify (SfxBroadcaster&, const SfxHint& rHint)
{
    if (rHint.ISA(SfxSimpleHint) && mpViewShell!=NULL)
    {
        SfxSimpleHint& rSimpleHint (*PTR_CAST(SfxSimpleHint, &rHint));
        if (rSimpleHint.GetId() == SFX_HINT_DYING)
        {
            // This hint may come either from the ViewShell or from the
            // document (registered by SdTransferable).  We do not know
            // which but both are sufficient to disconnect from the
            // ViewShell.
            EndListening(*mpViewShell);
            mpViewShell = NULL;
        }
    }
}




const ::std::vector<TransferableData::Representative>& TransferableData::GetRepresentatives (void) const
{
    return maRepresentatives;
}




SlideSorterViewShell* TransferableData::GetSourceViewShell (void) const
{
    return mpViewShell;
}

} } } // end of namespace ::sd::slidesorter::controller
