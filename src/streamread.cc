/*
 * Copyright (C) 2007-2008 by the Widelands Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include "streamread.h"

#include "wexception.h"

#include <cassert>

StreamRead::~StreamRead()
{
}

/**
 * Read a number of bytes from the stream.
 *
 * If the requested number of bytes couldn't be read, this function
 * fails by throwing an exception.
 */
void StreamRead::DataComplete(void * const data, const size_t size)
{
	size_t read = Data(data, size);

	if (read != size)
		throw wexception("Stream ended unexpectedly (%u bytes read, %u expected)", read, size);
}

int8_t StreamRead::Signed8() {
	int8_t x;
	DataComplete(&x, 1);
	return x;
}

uint8_t StreamRead::Unsigned8() {
	uint8_t x;
	DataComplete(&x, 1);
	return x;
}

int16_t StreamRead::Signed16() {
	int16_t x;
	DataComplete(&x, 2);
	return Little16(x);
}

uint16_t StreamRead::Unsigned16() {
	uint16_t x;
	DataComplete(&x, 2);
	return Little16(x);
}

int32_t StreamRead::Signed32() {
	int32_t x;
	DataComplete(&x, 4);
	return Little32(x);
}

uint32_t StreamRead::Unsigned32() {
	uint32_t x;
	DataComplete(&x, 4);
	return Little32(x);
}

std::string StreamRead::String()
{
	std::string x;
	char ch;

	for (;;) {
		DataComplete(&ch, 1);

		if (ch == 0)
			break;

		x += ch;
	}

	return x;
}

bool StreamRead::ReadLine(char * buf, const char * const buf_end) {
	assert(buf < buf_end);

	if (EndOfFile()) return false;
	for (char c; Data(&c, 1) and c != '\n';) {
		if (c == '\r') continue;
		*buf = c;
		if (c == 0) throw Null_In_Line();
		if (++buf == buf_end) {
			buf[-1] = 0;
			throw Buffer_Overflow();
		}
	}
	*buf = 0;
	return true;
}
