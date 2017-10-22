/*
 *   Copyright (C) 2011 by Jonathan Naylor G4KLX
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

#include "Defs.h"


class CRemoteLinkData {
public:
	CRemoteLinkData(const std::string& callsign, PROTOCOL protocol, bool linked, DIRECTION direction, bool dongle);
	~CRemoteLinkData();

	std::string getCallsign() const;
	int32_t     getProtocol() const;
	int32_t     isLinked() const;
	int32_t     getDirection() const;
	int32_t     isDongle() const;

private:
	std::string  m_callsign;
	PROTOCOL     m_protocol;
	bool         m_linked;
	DIRECTION    m_direction;
	bool         m_dongle;
};
