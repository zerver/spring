/*
Open Asset Import Library (assimp)
----------------------------------------------------------------------

Copyright (c) 2006-2012, assimp team
All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the 
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY 
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

----------------------------------------------------------------------
*/

/** @file  LogAux.h
 *  @brief Common logging usage patterns for importer implementations
 */
#ifndef INCLUDED_AI_LOGAUX_H
#define INCLUDED_AI_LOGAUX_H

#include "TinyFormatter.h"

namespace Assimp {

template <class TDeriving>
class LogFunctions 
{

public:

	// ------------------------------------------------------------------------------------------------
	static void ThrowException(const std::string& msg)
	{
		throw DeadlyImportError(log_prefix+msg);
	}

	// ------------------------------------------------------------------------------------------------
	static void LogWarn(const Formatter::format& message)	{
		if (!DefaultLogger::isNullLogger()) {
			DefaultLogger::get()->warn(log_prefix+(std::string)message);
		}
	}

	// ------------------------------------------------------------------------------------------------
	static void LogError(const Formatter::format& message)	{
		if (!DefaultLogger::isNullLogger()) {
			DefaultLogger::get()->error(log_prefix+(std::string)message);
		}
	}

	// ------------------------------------------------------------------------------------------------
	static void LogInfo(const Formatter::format& message)	{
		if (!DefaultLogger::isNullLogger()) {
			DefaultLogger::get()->info(log_prefix+(std::string)message);
		}
	}

	// ------------------------------------------------------------------------------------------------
	static void LogDebug(const Formatter::format& message)	{
		if (!DefaultLogger::isNullLogger()) {
			DefaultLogger::get()->debug(log_prefix+(std::string)message);
		}
	}

	// https://sourceforge.net/tracker/?func=detail&atid=1067632&aid=3358562&group_id=226462
#if !defined(__GNUC__) || !defined(__APPLE__) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 3)

	// ------------------------------------------------------------------------------------------------
	static void LogWarn  (const char* message) {
		if (!DefaultLogger::isNullLogger()) {
			LogWarn(Formatter::format(message));
		}
	}

	// ------------------------------------------------------------------------------------------------
	static void LogError  (const char* message) {
		if (!DefaultLogger::isNullLogger()) {
			LogError(Formatter::format(message));
		}
	}

	// ------------------------------------------------------------------------------------------------
	static void LogInfo  (const char* message) {
		if (!DefaultLogger::isNullLogger()) {
			LogInfo(Formatter::format(message));
		}
	}

	// ------------------------------------------------------------------------------------------------
	static void LogDebug  (const char* message) {
		if (!DefaultLogger::isNullLogger()) {
			LogDebug(Formatter::format(message));
		}
	}

#endif

private:

	static const char* log_prefix;

};

} // ! Assimp
#endif
