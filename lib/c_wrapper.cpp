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
#include "musicbrainz.h"

extern "C"
{
#include "mb_c.h"

musicbrainz_t mb_New(void)
{
    return (musicbrainz_t)new MusicBrainz();
}

void mb_Delete(musicbrainz_t o)
{
    delete (MusicBrainz *)o; 
}

int mb_SetServer(musicbrainz_t o, char *serverAddr, short serverPort)
{
    MusicBrainz *obj = (MusicBrainz *)o;
    return (int)obj->SetServer(string(serverAddr), serverPort);
}

int mb_SetProxy(musicbrainz_t o,char *proxyAddr, short proxyPort)
{
    MusicBrainz *obj = (MusicBrainz *)o;
    return (int)obj->SetServer(string(proxyAddr), proxyPort);
}

int mb_SetDevice(musicbrainz_t o, char *device)
{
    MusicBrainz *obj = (MusicBrainz *)o;
    return (int)obj->SetDevice(string(device));
}

int mb_GetWebSubmitURL(musicbrainz_t o, char *url, int urlLen)
{
    MusicBrainz *obj = (MusicBrainz *)o;
    string   urlString;
    int      ret;

    ret = (int)obj->GetWebSubmitURL(urlString);
    if (ret)
    {
       strncpy(url, urlString.c_str(), urlLen - 1);
       url[urlLen - 1] = 0;
    }
    return ret;
}

int mb_Query(musicbrainz_t o, char *xmlObject)
{
    MusicBrainz *obj = (MusicBrainz *)o;
    return (int)obj->Query(string(xmlObject));
}

int mb_QueryWithArgs(musicbrainz_t o, char *xmlObject, char **args)
{
    MusicBrainz *obj = (MusicBrainz *)o;
    vector<string>           *argList;
    string                    temp;
    int                       ret;

    argList = new vector<string>;
    for(; *args; args++)
    {
        temp = string(*args);
        argList->push_back(temp);
    }
        
    ret = obj->Query(string(xmlObject), argList);

    delete argList;

    return (int)ret;
}

void mb_GetQueryError(musicbrainz_t o, char *error, int maxErrorLen)
{
    MusicBrainz *obj = (MusicBrainz *)o;
    string                    err;

    obj->GetQueryError(err);

    strncpy(error, err.c_str(), maxErrorLen);
    error[maxErrorLen - 1] = 0;
}

int mb_GetResultData(musicbrainz_t o, char *resultName, 
                      char *data, int maxDataLen)
{
    MusicBrainz *obj = (MusicBrainz *)o;
    string   value;

    value = obj->Data(string(resultName));
    if (value.length() == 0)
       return 0;

    strncpy(data, value.c_str(), maxDataLen);
    data[maxDataLen - 1] = 0;

    return 1;
}

int mb_GetResultInt(musicbrainz_t o, char *resultName)
{
    MusicBrainz *obj = (MusicBrainz *)o;
    return obj->DataInt(string(resultName));
}

int mb_Select(musicbrainz_t o, char *selectQuery)
{
    MusicBrainz *obj = (MusicBrainz *)o;
    return obj->Select(string(selectQuery));
}

int mb_DoesResultExist(musicbrainz_t o, char *resultName)
{
    MusicBrainz *obj = (MusicBrainz *)o;
    return obj->DoesResultExist(string(resultName));
}

int mb_GetResultRDF(musicbrainz_t o,char *xml, int maxXMLLen)
{
    MusicBrainz *obj = (MusicBrainz *)o;
    string   xmlString;

    if (!obj->GetResultRDF(xmlString))
       return 0;

    strncpy(xml, xmlString.c_str(), maxXMLLen);
    xml[maxXMLLen - 1] = 0;

    return 1;
}

int mb_GetResultRDFLen(musicbrainz_t o)
{
    MusicBrainz *obj = (MusicBrainz *)o;
    string   xmlString;

    if (!obj->GetResultRDF(xmlString))
       return 0;

    return xmlString.length();
}

int mb_GetNumItems(musicbrainz_t o)
{
    MusicBrainz *obj = (MusicBrainz *)o;

    return obj->GetNumItems();
}

}