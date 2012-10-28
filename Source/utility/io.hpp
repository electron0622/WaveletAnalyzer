//============================================================================
//
//  utility/io.hpp
//
//  Copyright (C) 2012  Sato Takaaki.
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//============================================================================

#ifndef _UTILITY_IO_HPP_
#define _UTILITY_IO_HPP_

namespace WaveletAnalyzer {

namespace Utility {

class IO {

public:
    virtual ~IO();

public:
    virtual bool   Open(const char *name);
    virtual bool   Open(const wchar_t *name);
    virtual void   Close(void);
    virtual size_t Read(void *data, size_t size);
    virtual size_t Write(const void *data, size_t size);
    virtual bool   Seek(size_t offset);
    virtual size_t Tell(void);

};

}  // namespace Utility

}  // namespace WaveletAnalyzer

#endif /* _UTILITY_IO_HPP_ */
