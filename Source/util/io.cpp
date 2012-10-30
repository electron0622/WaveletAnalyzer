//============================================================================
//
//  util/io.cpp
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

#include <stddef.h>
#include "io.hpp"

namespace WaveletAnalyzer {

namespace Util {

IO::~IO() {
}

bool IO::Open(const char *name) {
    return false;
}

bool IO::Open(const wchar_t *name) {
    return false;
}

void IO::Close(void) {
    return;
}

size_t IO::Read(void *data, size_t size) {
    return 0;
}

size_t IO::Write(const void *data, size_t size) {
    return 0;
}

bool IO::Seek(size_t offset) {
    return false;
}

size_t IO::Tell(void) {
    return 0;
}

}  // namespace Util

}  // namespace WaveletAnalyzer
