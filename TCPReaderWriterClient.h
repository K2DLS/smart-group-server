/*
 *   Copyright (C) 2010,2011,2012,2013 by Jonathan Naylor G4KLX
 *   Copyright (c) 2017 by Thomas A. Early
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
#include <netdb.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>

class CTCPReaderWriterClient {
public:
	CTCPReaderWriterClient(const std::string& address, unsigned int port, const std::string& localAddress = std::string(""));
	CTCPReaderWriterClient(int fd);
	CTCPReaderWriterClient();
	~CTCPReaderWriterClient();

	bool open(const std::string& address, unsigned int port, const std::string& localAddress = std::string(""));
	bool open();

	int  read(unsigned char* buffer, unsigned int length, unsigned int secs, unsigned int msecs = 0U);
	int readLine(std::string& line, unsigned int secs);
	bool write(const unsigned char* buffer, unsigned int length);
	bool writeLine(const std::string& line);

	void close();

private:
	std::string    m_address;
	unsigned short m_port;
	std::string    m_localAddress;
	int            m_fd;
};
