/*
 *   Copyright (C) 2009,2014 by Jonathan Naylor G4KLX
 *   Copyright (c) 2017 by Thomas A. Early N7TAE
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#pragma once

#include <string>
#include <cstdint>
#include <cstdio>

#include "HeaderData.h"
#include "AMBEData.h"

enum DVTFR_TYPE {
	DVTFR_NONE,
	DVTFR_HEADER,
	DVTFR_DATA
};

class CDVTOOLFileReader {
public:
	CDVTOOLFileReader();
	~CDVTOOLFileReader();

	std::string  getFileName() const;
	unsigned int getRecords() const;

	bool         open(const std::string& fileName);

	DVTFR_TYPE   read();

	CHeaderData* readHeader();
	CAMBEData*   readData();

	void         close();

private:
	std::string    m_fileName;
	FILE          *m_file;
	uint32_t       m_records;
	DVTFR_TYPE     m_type;
	unsigned char* m_buffer;
	unsigned int   m_length;
	unsigned char  m_seqNo;
};
