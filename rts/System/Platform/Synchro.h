/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#ifndef SYNCHRO_H
#define SYNCHRO_H

#include <boost/version.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/detail/atomic_count.hpp>


namespace Threading {

typedef boost::recursive_mutex RecursiveMutex;

class RecursiveScopedLock {
	char sl_lock[sizeof(boost::recursive_mutex::scoped_lock)];
public:
	RecursiveScopedLock(boost::recursive_mutex& m, bool locked = true) {
	// NOTE the "new (...) ..." syntax is called "placement new", and is
	// described here: http://en.wikipedia.org/wiki/Placement_syntax
#if (BOOST_VERSION >= 103500)
		if (locked) {
			new (sl_lock) boost::recursive_mutex::scoped_lock(m);
		} else {
			new (sl_lock) boost::recursive_mutex::scoped_lock(m, boost::defer_lock);
		}
#else
		new (sl_lock) boost::recursive_mutex::scoped_lock(m, locked);
#endif
	}
	virtual ~RecursiveScopedLock() {
#if (BOOST_VERSION >= 103500)
		((boost::recursive_mutex::scoped_lock*)sl_lock)->~unique_lock();
#else
		((boost::recursive_mutex::scoped_lock*)sl_lock)->~scoped_lock();
#endif
	}
};

typedef boost::mutex Mutex;

class ScopedLock {
	char sl_lock[sizeof(boost::mutex::scoped_lock)];
public:
	ScopedLock(boost::mutex& m, bool locked = true) {
#if (BOOST_VERSION >= 103500)
		if (locked) {
			new (sl_lock) boost::mutex::scoped_lock(m);
		} else {
			new (sl_lock) boost::mutex::scoped_lock(m, boost::defer_lock);
		}
#else
		new (sl_lock) boost::mutex::scoped_lock(m, locked);
#endif
	}
	virtual ~ScopedLock() {
#if (BOOST_VERSION >= 103500)
		((boost::mutex::scoped_lock*)sl_lock)->~unique_lock();
#else
		((boost::mutex::scoped_lock*)sl_lock)->~scoped_lock();
#endif
	}
};

class AtomicCount : public boost::detail::atomic_count {
public:
	static const long hlongmax = LONG_MAX/2;
	long refval;

	AtomicCount(long ref) : boost::detail::atomic_count(hlongmax), refval(ref) {}
	~AtomicCount() {} // workaround because boost::detail::atomic_count has no destructor

	void operator=(long ref) {
		this->~AtomicCount();
		new (this) AtomicCount(ref); // LOL!
	}

    long operator++()
    {
        return refval + (hlongmax - boost::detail::atomic_count::operator--());
    }

    long operator--()
    {
        return  refval + (hlongmax - boost::detail::atomic_count::operator++());
    }

    operator long() const
    {
		return  refval + (hlongmax - boost::detail::atomic_count::operator long());
    }
};

}

#endif // SYNCHRO_H
