/* --------------------------------------------------------------------------

   MusicBrainz -- The Intenet music metadatabase

   Copyright (C) 2000 Robert Kaye
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
   
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

     $Id$

----------------------------------------------------------------------------*/
#ifndef _MUSICBRAINZ_H_
#define _MUSICBRAINZ_H_

#include "errors.h"
#include "queries.h"

#include <string>
#include <vector>

using namespace std;

class XQL;
class MusicBrainz
{
    public:

                MusicBrainz(void);
       virtual ~MusicBrainz(void);

       bool     SetServer      (const string &serverAddr, short serverPort);
       bool     SetProxy       (const string &proxyAddr, short proxyPort);
       bool     SetDevice      (const string &device);

       bool     Query          (const string &xmlObject, 
                                vector<string> *args = NULL);
       void     GetQueryError  (string &ErrorText);
       int      GetNumItems    (void);
       bool     GetWebSubmitURL(string &url);

       bool     Select         (const string &selectQuery);
       bool     DoesResultExist(const string &resultName, int Index = -1);
       bool     GetResultData  (const string &resultName, int Index, 
                                string &data);
       bool     GetResultRDF   (string &RDFObject);

       const string &Data      (const string &resultName, int Index = -1);
       int           DataInt   (const string &resultName, int Index = -1);

    private:

       void     SubstituteArgs(string &xml, vector<string> *args);
       void     SetError(Error ret);
       int      SplitResponse(const string &inputXml);

       int      GetNumXMLResults(void);
       bool     SelectXMLResult(int index);

       string          m_error, m_empty; 
       string          m_server, m_proxy;
       short           m_serverPort, m_proxyPort;
       string          m_device, m_selectQuery; 
       vector<string>  m_xmlList;
       int             m_xmlIndex, m_numItems;
       XQL            *m_xql;
       vector<int>     m_indexes;
};

#endif