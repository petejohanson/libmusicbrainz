/* --------------------------------------------------------------------------

   MusicBrainz -- The Intenet music metadatabase

   Copyright (C) 2000 Robert Kaye
   Copyright (C) 1999 Marc E E van Woerkom
   
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

#if !defined(_CDI_LINUX_H_)
#define _CDI_LINUX_H_


#define OS "Linux"

// 
//  Linux CD-audio declarations
//

#include <linux/cdrom.h>

typedef char* MUSICBRAINZ_DEVICE;



// 
//  Linux specific prototypes
// 

int ReadTOCHeader(int fd, 
                  int& first, 
                  int& last);

int ReadTOCEntry(int fd, 
                 int track, 
                 int& lba);

#endif