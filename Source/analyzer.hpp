//============================================================================
//
//  analyzer.hpp
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

#ifndef _ANALYZER_HPP_
#define _ANALYZER_HPP_

#include "util/io.hpp"

namespace WaveletAnalyzer {

class Analyzer : public Util::IO {

public:
    Analyzer();
    ~Analyzer();

public:
    bool   Open(const char *name);
    void   Close(void);
    size_t Read(void *data, size_t size);
    size_t Write(const void *data, size_t size);
    size_t Tell(void);

private:
    void Main(void);

};

}  // namespace WaveletAnalyzer

#endif /* _ANALYZER_HPP_ */
