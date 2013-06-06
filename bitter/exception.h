// Copyright 2013, Sebastian Jeltsch (sjeltsch@kip.uni-heidelberg.de)
// Distributed under the terms of the LGPLv3 or newer.

#pragma once

#include <exception>
#include <string>

namespace bit {

class bitter_exception :
	public virtual std::exception
{
public:
	bitter_exception(std::string const& msg) :
		_msg(msg) {}

	virtual ~bitter_exception() throw () {}

	virtual const char *what() const throw()
	{
		return _msg.c_str();
	}

protected:
	std::string const _msg;
};

} // namespace bit
