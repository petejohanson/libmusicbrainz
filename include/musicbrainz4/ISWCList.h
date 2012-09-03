/* --------------------------------------------------------------------------

   libmusicbrainz4 - Client library to access MusicBrainz

   Copyright (C) 2012 Andrew Hawkins

   This file is part of libmusicbrainz4.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   libmusicbrainz4 is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this library.  If not, see <http://www.gnu.org/licenses/>.

     $Id$

----------------------------------------------------------------------------*/

#ifndef _MUSICBRAINZ4_ISWC_LIST_H
#define _MUSICBRAINZ4_ISWC_LIST_H

#include <string>
#include <iostream>

#include "musicbrainz4/ListImpl.h"

#include "musicbrainz4/xmlParser.h"

namespace MusicBrainz4
{
	class CISWC;
	class CISWCListPrivate;

	class CISWCList: public CListImpl<CISWC>
	{
	public:
		CISWCList(const XMLNode& Node=XMLNode::emptyNode());
		CISWCList(const CISWCList& Other);
		CISWCList& operator =(const CISWCList& Other);
		virtual ~CISWCList();

		virtual CISWCList *Clone();

		virtual std::ostream& Serialise(std::ostream& os) const;
		static std::string GetElementName();

	protected:
		virtual void ParseAttribute(const std::string& Name, const std::string& Value);
		virtual void ParseElement(const XMLNode& Node);

	private:
		CISWCListPrivate * const m_d;
	};
}

#endif
