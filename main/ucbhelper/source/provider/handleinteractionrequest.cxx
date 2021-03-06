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
#include "precompiled_ucbhelper.hxx"
#include "ucbhelper/handleinteractionrequest.hxx"
#include "com/sun/star/task/XInteractionAbort.hpp"
#include "com/sun/star/task/XInteractionHandler.hpp"
#include "com/sun/star/task/XInteractionRetry.hpp"
#include "com/sun/star/ucb/CommandFailedException.hpp"
#include "com/sun/star/ucb/XCommandEnvironment.hpp"
#include "com/sun/star/uno/Reference.hxx"
#include "com/sun/star/uno/RuntimeException.hpp"
#include "cppuhelper/exc_hlp.hxx"
#include "osl/diagnose.h"
#include "rtl/ustring.hxx"
#ifndef _UCBHELPER_INTERACTIONREQUEST_HXX
#include "ucbhelper/interactionrequest.hxx"
#endif
#include "ucbhelper/simpleauthenticationrequest.hxx"
#include "ucbhelper/simpleinteractionrequest.hxx"
#include "ucbhelper/simplecertificatevalidationrequest.hxx"
#ifndef INCLUDED_UTILITY
#include <utility>
#define INCLUDED_UTILITY
#endif

using namespace com::sun::star;

namespace {

void
handle(uno::Reference< task::XInteractionRequest > const & rRequest,
       uno::Reference< ucb::XCommandEnvironment > const & rEnvironment)
    SAL_THROW((uno::Exception))
{
    OSL_ENSURE(rRequest.is(), "specification violation");
    uno::Reference< task::XInteractionHandler > xHandler;
    if (rEnvironment.is())
        xHandler = rEnvironment->getInteractionHandler();
    if (!xHandler.is())
        cppu::throwException(rRequest->getRequest());
    xHandler->handle(rRequest.get());
}

}

namespace ucbhelper {

sal_Int32
handleInteractionRequest(
    rtl::Reference< ucbhelper::SimpleInteractionRequest > const & rRequest,
    uno::Reference< ucb::XCommandEnvironment > const & rEnvironment,
    bool bThrowOnAbort)
    SAL_THROW((uno::Exception))
{
    handle(rRequest.get(), rEnvironment);
    sal_Int32 nResponse = rRequest->getResponse();
    switch (nResponse)
    {
    case ucbhelper::CONTINUATION_UNKNOWN:
        cppu::throwException(rRequest->getRequest());
        break;

    case ucbhelper::CONTINUATION_ABORT:
        if (bThrowOnAbort)
            throw ucb::CommandFailedException(
                      rtl::OUString(), 0, rRequest->getRequest());
        break;
    }
    return nResponse;
}

std::pair< sal_Int32,
           rtl::Reference< ucbhelper::InteractionSupplyAuthentication > >
handleInteractionRequest(
    rtl::Reference< ucbhelper::SimpleAuthenticationRequest > const & rRequest,
    uno::Reference< ucb::XCommandEnvironment > const & rEnvironment,
    bool bThrowOnAbort)
    SAL_THROW((uno::Exception))
{
    handle(rRequest.get(), rEnvironment);
    rtl::Reference< ucbhelper::InteractionContinuation >
        xContinuation(rRequest->getSelection());
    if (uno::Reference< task::XInteractionAbort >(
                xContinuation.get(), uno::UNO_QUERY).
            is())
        if (bThrowOnAbort)
            throw ucb::CommandFailedException(
                      rtl::OUString(), 0, rRequest->getRequest());
        else
            return std::make_pair(
                       ucbhelper::CONTINUATION_ABORT,
                       rtl::Reference<
                           ucbhelper::InteractionSupplyAuthentication >());
    else if (uno::Reference< task::XInteractionRetry >(
                     xContinuation.get(), uno::UNO_QUERY).
                 is())
        return std::make_pair(
                   ucbhelper::CONTINUATION_ABORT,
                   rtl::Reference<
                       ucbhelper::InteractionSupplyAuthentication >());
    else
        return std::make_pair(
                   ucbhelper::CONTINUATION_UNKNOWN,
                   rtl::Reference<
                       ucbhelper::InteractionSupplyAuthentication >(
                           rRequest->getAuthenticationSupplier()));
}

}

namespace ucbhelper {

sal_Int32
handleInteractionRequest(
    rtl::Reference< ucbhelper::SimpleCertificateValidationRequest > const & rRequest,
    uno::Reference< ucb::XCommandEnvironment > const & rEnvironment,
    bool bThrowOnAbort)
    SAL_THROW((uno::Exception))
{
    handle(rRequest.get(), rEnvironment);
    sal_Int32 nResponse = rRequest->getResponse();
    switch (nResponse)
    {
    case ucbhelper::CONTINUATION_UNKNOWN:
        cppu::throwException(rRequest->getRequest());
        break;

    case ucbhelper::CONTINUATION_ABORT:
        if (bThrowOnAbort)
            throw ucb::CommandFailedException(
                      rtl::OUString(), 0, rRequest->getRequest());
        break;
    }
    return nResponse;
}

}

