/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 *
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is Netscape
 * Communications Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s):
 *  Brian Ryner <bryner@netscape.com>
 */

#include "nsIComponentManager.h"
#include "nsIServiceManager.h"
#include "nsSSLSocketProvider.h"
#include "nsNSSIOLayer.h"

nsSSLSocketProvider::nsSSLSocketProvider()
{
  NS_INIT_REFCNT();
}

nsresult
nsSSLSocketProvider::Init()
{
  nsresult rv = NS_OK;
  return rv;
}

nsSSLSocketProvider::~nsSSLSocketProvider()
{
}

NS_IMPL_THREADSAFE_ISUPPORTS2(nsSSLSocketProvider, nsISocketProvider,
                              nsISSLSocketProvider);

NS_METHOD
nsSSLSocketProvider::Create(nsISupports *aOuter, REFNSIID aIID, void **aResult)
{
  nsresult rv;
  nsSSLSocketProvider* inst;
  
  if (!aResult) {
    rv = NS_ERROR_NULL_POINTER;
    return rv;
  }
  *aResult = nsnull;
  if (aOuter) {
    rv = NS_ERROR_NO_AGGREGATION;
    return rv;
  }
  
  NS_NEWXPCOM(inst, nsSSLSocketProvider);
  if (!inst) {
    rv = NS_ERROR_OUT_OF_MEMORY;
    return rv;
  }
  NS_ADDREF(inst);
  rv = inst->QueryInterface(aIID, aResult);
  NS_RELEASE(inst);
  
  return rv;
}


NS_IMETHODIMP
nsSSLSocketProvider::NewSocket(const char *host,
                               PRInt32 port,
                               const char *proxyHost,
                               PRInt32 proxyPort,
                               PRFileDesc **_result,
                               nsISupports **securityInfo)
{
  nsresult rv = nsSSLIOLayerNewSocket(host,
                                      port,
                                      proxyHost,
                                      proxyPort,
                                      _result,
                                      securityInfo,
                                      PR_FALSE);
  return (NS_FAILED(rv)) ? NS_ERROR_SOCKET_CREATE_FAILED : NS_OK;
}

// Add the SSL IO layer to an existing socket
NS_IMETHODIMP
nsSSLSocketProvider::AddToSocket(const char *host,
                                 PRInt32 port,
                                 const char *proxyHost,
                                 PRInt32 proxyPort,
                                 PRFileDesc *aSocket,
                                 nsISupports **securityInfo)
{
  nsresult rv = nsSSLIOLayerAddToSocket(host,
                                        port,
                                        proxyHost,
                                        proxyPort,
                                        aSocket,
                                        securityInfo,
                                        PR_FALSE);
  
  return (NS_FAILED(rv)) ? NS_ERROR_SOCKET_CREATE_FAILED : NS_OK;
}
