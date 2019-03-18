'''Wrapper for messageDefinitions.h

Generated with:
ctypesgen.py --cpp=clang -E -a -o ../hapticControl/python/messageDefinitions.py ../hapticControl/src/network/messageDefinitions.h

Do not modify this file.
'''

__docformat__ =  'restructuredtext'

# Begin preamble

import ctypes, os, sys
from ctypes import *

_int_types = (c_int16, c_int32)
if hasattr(ctypes, 'c_int64'):
    # Some builds of ctypes apparently do not have c_int64
    # defined; it's a pretty good bet that these builds do not
    # have 64-bit pointers.
    _int_types += (c_int64,)
for t in _int_types:
    if sizeof(t) == sizeof(c_size_t):
        c_ptrdiff_t = t
del t
del _int_types

class c_void(Structure):
    # c_void_p is a buggy return type, converting to int, so
    # POINTER(None) == c_void_p is actually written as
    # POINTER(c_void), so it can be treated as a real pointer.
    _fields_ = [('dummy', c_int)]

def POINTER(obj):
    p = ctypes.POINTER(obj)

    # Convert None to a real NULL pointer to work around bugs
    # in how ctypes handles None on 64-bit platforms
    if not isinstance(p.from_param, classmethod):
        def from_param(cls, x):
            if x is None:
                return cls()
            else:
                return x
        p.from_param = classmethod(from_param)

    return p

class UserString:
    def __init__(self, seq):
        if isinstance(seq, basestring):
            self.data = seq
        elif isinstance(seq, UserString):
            self.data = seq.data[:]
        else:
            self.data = str(seq)
    def __str__(self): return str(self.data)
    def __repr__(self): return repr(self.data)
    def __int__(self): return int(self.data)
    def __long__(self): return long(self.data)
    def __float__(self): return float(self.data)
    def __complex__(self): return complex(self.data)
    def __hash__(self): return hash(self.data)

    def __cmp__(self, string):
        if isinstance(string, UserString):
            return cmp(self.data, string.data)
        else:
            return cmp(self.data, string)
    def __contains__(self, char):
        return char in self.data

    def __len__(self): return len(self.data)
    def __getitem__(self, index): return self.__class__(self.data[index])
    def __getslice__(self, start, end):
        start = max(start, 0); end = max(end, 0)
        return self.__class__(self.data[start:end])

    def __add__(self, other):
        if isinstance(other, UserString):
            return self.__class__(self.data + other.data)
        elif isinstance(other, basestring):
            return self.__class__(self.data + other)
        else:
            return self.__class__(self.data + str(other))
    def __radd__(self, other):
        if isinstance(other, basestring):
            return self.__class__(other + self.data)
        else:
            return self.__class__(str(other) + self.data)
    def __mul__(self, n):
        return self.__class__(self.data*n)
    __rmul__ = __mul__
    def __mod__(self, args):
        return self.__class__(self.data % args)

    # the following methods are defined in alphabetical order:
    def capitalize(self): return self.__class__(self.data.capitalize())
    def center(self, width, *args):
        return self.__class__(self.data.center(width, *args))
    def count(self, sub, start=0, end=float("inf")):
        return self.data.count(sub, start, end)
    def decode(self, encoding=None, errors=None): # XXX improve this?
        if encoding:
            if errors:
                return self.__class__(self.data.decode(encoding, errors))
            else:
                return self.__class__(self.data.decode(encoding))
        else:
            return self.__class__(self.data.decode())
    def encode(self, encoding=None, errors=None): # XXX improve this?
        if encoding:
            if errors:
                return self.__class__(self.data.encode(encoding, errors))
            else:
                return self.__class__(self.data.encode(encoding))
        else:
            return self.__class__(self.data.encode())
    def endswith(self, suffix, start=0, end=float("inf")):
        return self.data.endswith(suffix, start, end)
    def expandtabs(self, tabsize=8):
        return self.__class__(self.data.expandtabs(tabsize))
    def find(self, sub, start=0, end=float("inf")):
        return self.data.find(sub, start, end)
    def index(self, sub, start=0, end=float("inf")):
        return self.data.index(sub, start, end)
    def isalpha(self): return self.data.isalpha()
    def isalnum(self): return self.data.isalnum()
    def isdecimal(self): return self.data.isdecimal()
    def isdigit(self): return self.data.isdigit()
    def islower(self): return self.data.islower()
    def isnumeric(self): return self.data.isnumeric()
    def isspace(self): return self.data.isspace()
    def istitle(self): return self.data.istitle()
    def isupper(self): return self.data.isupper()
    def join(self, seq): return self.data.join(seq)
    def ljust(self, width, *args):
        return self.__class__(self.data.ljust(width, *args))
    def lower(self): return self.__class__(self.data.lower())
    def lstrip(self, chars=None): return self.__class__(self.data.lstrip(chars))
    def partition(self, sep):
        return self.data.partition(sep)
    def replace(self, old, new, maxsplit=-1):
        return self.__class__(self.data.replace(old, new, maxsplit))
    def rfind(self, sub, start=0, end=float("inf")):
        return self.data.rfind(sub, start, end)
    def rindex(self, sub, start=0, end=float("inf")):
        return self.data.rindex(sub, start, end)
    def rjust(self, width, *args):
        return self.__class__(self.data.rjust(width, *args))
    def rpartition(self, sep):
        return self.data.rpartition(sep)
    def rstrip(self, chars=None): return self.__class__(self.data.rstrip(chars))
    def split(self, sep=None, maxsplit=-1):
        return self.data.split(sep, maxsplit)
    def rsplit(self, sep=None, maxsplit=-1):
        return self.data.rsplit(sep, maxsplit)
    def splitlines(self, keepends=0): return self.data.splitlines(keepends)
    def startswith(self, prefix, start=0, end=float("inf")):
        return self.data.startswith(prefix, start, end)
    def strip(self, chars=None): return self.__class__(self.data.strip(chars))
    def swapcase(self): return self.__class__(self.data.swapcase())
    def title(self): return self.__class__(self.data.title())
    def translate(self, *args):
        return self.__class__(self.data.translate(*args))
    def upper(self): return self.__class__(self.data.upper())
    def zfill(self, width): return self.__class__(self.data.zfill(width))

class MutableString(UserString):
    """mutable string objects

    Python strings are immutable objects.  This has the advantage, that
    strings may be used as dictionary keys.  If this property isn't needed
    and you insist on changing string values in place instead, you may cheat
    and use MutableString.

    But the purpose of this class is an educational one: to prevent
    people from inventing their own mutable string class derived
    from UserString and than forget thereby to remove (override) the
    __hash__ method inherited from UserString.  This would lead to
    errors that would be very hard to track down.

    A faster and better solution is to rewrite your program using lists."""
    def __init__(self, string=""):
        self.data = string
    def __hash__(self):
        raise TypeError("unhashable type (it is mutable)")
    def __setitem__(self, index, sub):
        if index < 0:
            index += len(self.data)
        if index < 0 or index >= len(self.data): raise IndexError
        self.data = self.data[:index] + sub + self.data[index+1:]
    def __delitem__(self, index):
        if index < 0:
            index += len(self.data)
        if index < 0 or index >= len(self.data): raise IndexError
        self.data = self.data[:index] + self.data[index+1:]
    def __setslice__(self, start, end, sub):
        start = max(start, 0); end = max(end, 0)
        if isinstance(sub, UserString):
            self.data = self.data[:start]+sub.data+self.data[end:]
        elif isinstance(sub, basestring):
            self.data = self.data[:start]+sub+self.data[end:]
        else:
            self.data =  self.data[:start]+str(sub)+self.data[end:]
    def __delslice__(self, start, end):
        start = max(start, 0); end = max(end, 0)
        self.data = self.data[:start] + self.data[end:]
    def immutable(self):
        return UserString(self.data)
    def __iadd__(self, other):
        if isinstance(other, UserString):
            self.data += other.data
        elif isinstance(other, basestring):
            self.data += other
        else:
            self.data += str(other)
        return self
    def __imul__(self, n):
        self.data *= n
        return self

class String(MutableString, Union):

    _fields_ = [('raw', POINTER(c_char)),
                ('data', c_char_p)]

    def __init__(self, obj=""):
        if isinstance(obj, (str, unicode, UserString)):
            self.data = str(obj)
        else:
            self.raw = obj

    def __len__(self):
        return self.data and len(self.data) or 0

    def from_param(cls, obj):
        # Convert None or 0
        if obj is None or obj == 0:
            return cls(POINTER(c_char)())

        # Convert from String
        elif isinstance(obj, String):
            return obj

        # Convert from str
        elif isinstance(obj, str):
            return cls(obj)

        # Convert from c_char_p
        elif isinstance(obj, c_char_p):
            return obj

        # Convert from POINTER(c_char)
        elif isinstance(obj, POINTER(c_char)):
            return obj

        # Convert from raw pointer
        elif isinstance(obj, int):
            return cls(cast(obj, POINTER(c_char)))

        # Convert from object
        else:
            return String.from_param(obj._as_parameter_)
    from_param = classmethod(from_param)

def ReturnString(obj, func=None, arguments=None):
    return String.from_param(obj)

# As of ctypes 1.0, ctypes does not support custom error-checking
# functions on callbacks, nor does it support custom datatypes on
# callbacks, so we must ensure that all callbacks return
# primitive datatypes.
#
# Non-primitive return values wrapped with UNCHECKED won't be
# typechecked, and will be converted to c_void_p.
def UNCHECKED(type):
    if (hasattr(type, "_type_") and isinstance(type._type_, str)
        and type._type_ != "P"):
        return type
    else:
        return c_void_p

# ctypes doesn't have direct support for variadic functions, so we have to write
# our own wrapper class
class _variadic_function(object):
    def __init__(self,func,restype,argtypes):
        self.func=func
        self.func.restype=restype
        self.argtypes=argtypes
    def _as_parameter_(self):
        # So we can pass this variadic function as a function pointer
        return self.func
    def __call__(self,*args):
        fixed_args=[]
        i=0
        for argtype in self.argtypes:
            # Typecheck what we can
            fixed_args.append(argtype.from_param(args[i]))
            i+=1
        return self.func(*fixed_args+list(args[i:]))

# End preamble

_libs = {}
_libdirs = []

# Begin loader

# ----------------------------------------------------------------------------
# Copyright (c) 2008 David James
# Copyright (c) 2006-2008 Alex Holkner
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#  * Neither the name of pyglet nor the names of its
#    contributors may be used to endorse or promote products
#    derived from this software without specific prior written
#    permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
# LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
# ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
# ----------------------------------------------------------------------------

import os.path, re, sys, glob
import platform
import ctypes
import ctypes.util

def _environ_path(name):
    if name in os.environ:
        return os.environ[name].split(":")
    else:
        return []

class LibraryLoader(object):
    def __init__(self):
        self.other_dirs=[]

    def load_library(self,libname):
        """Given the name of a library, load it."""
        paths = self.getpaths(libname)

        for path in paths:
            if os.path.exists(path):
                return self.load(path)

        raise ImportError("%s not found." % libname)

    def load(self,path):
        """Given a path to a library, load it."""
        try:
            # Darwin requires dlopen to be called with mode RTLD_GLOBAL instead
            # of the default RTLD_LOCAL.  Without this, you end up with
            # libraries not being loadable, resulting in "Symbol not found"
            # errors
            if sys.platform == 'darwin':
                return ctypes.CDLL(path, ctypes.RTLD_GLOBAL)
            else:
                return ctypes.cdll.LoadLibrary(path)
        except OSError(e):
            raise ImportError(e)

    def getpaths(self,libname):
        """Return a list of paths where the library might be found."""
        if os.path.isabs(libname):
            yield libname
        else:
            # FIXME / TODO return '.' and os.path.dirname(__file__)
            for path in self.getplatformpaths(libname):
                yield path

            path = ctypes.util.find_library(libname)
            if path: yield path

    def getplatformpaths(self, libname):
        return []

# Darwin (Mac OS X)

class DarwinLibraryLoader(LibraryLoader):
    name_formats = ["lib%s.dylib", "lib%s.so", "lib%s.bundle", "%s.dylib",
                "%s.so", "%s.bundle", "%s"]

    def getplatformpaths(self,libname):
        if os.path.pathsep in libname:
            names = [libname]
        else:
            names = [format % libname for format in self.name_formats]

        for dir in self.getdirs(libname):
            for name in names:
                yield os.path.join(dir,name)

    def getdirs(self,libname):
        '''Implements the dylib search as specified in Apple documentation:

        http://developer.apple.com/documentation/DeveloperTools/Conceptual/
            DynamicLibraries/Articles/DynamicLibraryUsageGuidelines.html

        Before commencing the standard search, the method first checks
        the bundle's ``Frameworks`` directory if the application is running
        within a bundle (OS X .app).
        '''

        dyld_fallback_library_path = _environ_path("DYLD_FALLBACK_LIBRARY_PATH")
        if not dyld_fallback_library_path:
            dyld_fallback_library_path = [os.path.expanduser('~/lib'),
                                          '/usr/local/lib', '/usr/lib']

        dirs = []

        if '/' in libname:
            dirs.extend(_environ_path("DYLD_LIBRARY_PATH"))
        else:
            dirs.extend(_environ_path("LD_LIBRARY_PATH"))
            dirs.extend(_environ_path("DYLD_LIBRARY_PATH"))

        dirs.extend(self.other_dirs)
        dirs.append(".")
        dirs.append(os.path.dirname(__file__))

        if hasattr(sys, 'frozen') and sys.frozen == 'macosx_app':
            dirs.append(os.path.join(
                os.environ['RESOURCEPATH'],
                '..',
                'Frameworks'))

        dirs.extend(dyld_fallback_library_path)

        return dirs

# Posix

class PosixLibraryLoader(LibraryLoader):
    _ld_so_cache = None

    def _create_ld_so_cache(self):
        # Recreate search path followed by ld.so.  This is going to be
        # slow to build, and incorrect (ld.so uses ld.so.cache, which may
        # not be up-to-date).  Used only as fallback for distros without
        # /sbin/ldconfig.
        #
        # We assume the DT_RPATH and DT_RUNPATH binary sections are omitted.

        directories = []
        for name in ("LD_LIBRARY_PATH",
                     "SHLIB_PATH", # HPUX
                     "LIBPATH", # OS/2, AIX
                     "LIBRARY_PATH", # BE/OS
                    ):
            if name in os.environ:
                directories.extend(os.environ[name].split(os.pathsep))
        directories.extend(self.other_dirs)
        directories.append(".")
        directories.append(os.path.dirname(__file__))

        try: directories.extend([dir.strip() for dir in open('/etc/ld.so.conf')])
        except IOError: pass

        unix_lib_dirs_list = ['/lib', '/usr/lib', '/lib64', '/usr/lib64']
        if sys.platform.startswith('linux'):
            # Try and support multiarch work in Ubuntu
            # https://wiki.ubuntu.com/MultiarchSpec
            bitage = platform.architecture()[0]
            if bitage.startswith('32'):
                # Assume Intel/AMD x86 compat
                unix_lib_dirs_list += ['/lib/i386-linux-gnu', '/usr/lib/i386-linux-gnu']
            elif bitage.startswith('64'):
                # Assume Intel/AMD x86 compat
                unix_lib_dirs_list += ['/lib/x86_64-linux-gnu', '/usr/lib/x86_64-linux-gnu']
            else:
                # guess...
                unix_lib_dirs_list += glob.glob('/lib/*linux-gnu')
        directories.extend(unix_lib_dirs_list)

        cache = {}
        lib_re = re.compile(r'lib(.*)\.s[ol]')
        ext_re = re.compile(r'\.s[ol]$')
        for dir in directories:
            try:
                for path in glob.glob("%s/*.s[ol]*" % dir):
                    file = os.path.basename(path)

                    # Index by filename
                    if file not in cache:
                        cache[file] = path

                    # Index by library name
                    match = lib_re.match(file)
                    if match:
                        library = match.group(1)
                        if library not in cache:
                            cache[library] = path
            except OSError:
                pass

        self._ld_so_cache = cache

    def getplatformpaths(self, libname):
        if self._ld_so_cache is None:
            self._create_ld_so_cache()

        result = self._ld_so_cache.get(libname)
        if result: yield result

        path = ctypes.util.find_library(libname)
        if path: yield os.path.join("/lib",path)

# Windows

class _WindowsLibrary(object):
    def __init__(self, path):
        self.cdll = ctypes.cdll.LoadLibrary(path)
        self.windll = ctypes.windll.LoadLibrary(path)

    def __getattr__(self, name):
        try: return getattr(self.cdll,name)
        except AttributeError:
            try: return getattr(self.windll,name)
            except AttributeError:
                raise

class WindowsLibraryLoader(LibraryLoader):
    name_formats = ["%s.dll", "lib%s.dll", "%slib.dll"]

    def load_library(self, libname):
        try:
            result = LibraryLoader.load_library(self, libname)
        except ImportError:
            result = None
            if os.path.sep not in libname:
                for name in self.name_formats:
                    try:
                        result = getattr(ctypes.cdll, name % libname)
                        if result:
                            break
                    except WindowsError:
                        result = None
            if result is None:
                try:
                    result = getattr(ctypes.cdll, libname)
                except WindowsError:
                    result = None
            if result is None:
                raise ImportError("%s not found." % libname)
        return result

    def load(self, path):
        return _WindowsLibrary(path)

    def getplatformpaths(self, libname):
        if os.path.sep not in libname:
            for name in self.name_formats:
                dll_in_current_dir = os.path.abspath(name % libname)
                if os.path.exists(dll_in_current_dir):
                    yield dll_in_current_dir
                path = ctypes.util.find_library(name % libname)
                if path:
                    yield path

# Platform switching

# If your value of sys.platform does not appear in this dict, please contact
# the Ctypesgen maintainers.

loaderclass = {
    "darwin":   DarwinLibraryLoader,
    "cygwin":   WindowsLibraryLoader,
    "win32":    WindowsLibraryLoader
}

loader = loaderclass.get(sys.platform, PosixLibraryLoader)()

def add_library_search_dirs(other_dirs):
    loader.other_dirs = other_dirs

load_library = loader.load_library

del loaderclass

# End loader

add_library_search_dirs([])

# No libraries

# No modules

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 82
class struct_anon_1(Structure):
    pass

struct_anon_1.__slots__ = [
    'serial_no',
    'msg_type',
    'reserved',
    'timestamp',
]
struct_anon_1._fields_ = [
    ('serial_no', c_int),
    ('msg_type', c_int),
    ('reserved', c_double),
    ('timestamp', c_double),
]

MSG_HEADER = struct_anon_1 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 82

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 86
class struct_anon_2(Structure):
    pass

struct_anon_2.__slots__ = [
    'header',
]
struct_anon_2._fields_ = [
    ('header', MSG_HEADER),
]

M_CONTROL_SIMULATION_STARTED = struct_anon_2 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 86

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 90
class struct_anon_3(Structure):
    pass

struct_anon_3.__slots__ = [
    'header',
]
struct_anon_3._fields_ = [
    ('header', MSG_HEADER),
]

M_CONTROL_SIMULATION_FINISHED = struct_anon_3 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 90

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 104
class struct_anon_4(Structure):
    pass

struct_anon_4.__slots__ = [
    'header',
    'posX',
    'posY',
    'posZ',
    'velX',
    'velY',
    'velZ',
    'forceX',
    'forceY',
    'forceZ',
    'collisions',
]
struct_anon_4._fields_ = [
    ('header', MSG_HEADER),
    ('posX', c_double),
    ('posY', c_double),
    ('posZ', c_double),
    ('velX', c_double),
    ('velY', c_double),
    ('velZ', c_double),
    ('forceX', c_double),
    ('forceY', c_double),
    ('forceZ', c_double),
    ('collisions', (c_char * 128) * 4),
]

M_HAPTIC_DATA_STREAM = struct_anon_4 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 104

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 108
class struct_anon_5(Structure):
    pass

struct_anon_5.__slots__ = [
    'header',
]
struct_anon_5._fields_ = [
    ('header', MSG_HEADER),
]

M_HAPTICS_TOOL_CONNECTED = struct_anon_5 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 108

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 114
class struct_anon_6(Structure):
    pass

struct_anon_6.__slots__ = [
    'header',
    'objectName',
    'enabled',
]
struct_anon_6._fields_ = [
    ('header', MSG_HEADER),
    ('objectName', c_char * 128),
    ('enabled', c_int),
]

M_HAPTICS_SET_ENABLED = struct_anon_6 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 114

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 119
class struct_anon_7(Structure):
    pass

struct_anon_7.__slots__ = [
    'header',
    'objectName',
]
struct_anon_7._fields_ = [
    ('header', MSG_HEADER),
    ('objectName', c_char * 128),
]

M_HAPTICS_EFFECT_MAGNET = struct_anon_7 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 119

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 124
class struct_anon_8(Structure):
    pass

struct_anon_8.__slots__ = [
    'header',
    'objectName',
]
struct_anon_8._fields_ = [
    ('header', MSG_HEADER),
    ('objectName', c_char * 128),
]

M_HAPTICS_EFFECT_STICKSLIP = struct_anon_8 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 124

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 129
class struct_anon_9(Structure):
    pass

struct_anon_9.__slots__ = [
    'header',
    'objectName',
]
struct_anon_9._fields_ = [
    ('header', MSG_HEADER),
    ('objectName', c_char * 128),
]

M_HAPTICS_EFFECT_SURFACE = struct_anon_9 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 129

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 134
class struct_anon_10(Structure):
    pass

struct_anon_10.__slots__ = [
    'header',
    'objectName',
]
struct_anon_10._fields_ = [
    ('header', MSG_HEADER),
    ('objectName', c_char * 128),
]

M_HAPTICS_EFFECT_VIBRATION = struct_anon_10 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 134

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 139
class struct_anon_11(Structure):
    pass

struct_anon_11.__slots__ = [
    'header',
    'objectName',
]
struct_anon_11._fields_ = [
    ('header', MSG_HEADER),
    ('objectName', c_char * 128),
]

M_HAPTICS_EFFECT_VISCOSITY = struct_anon_11 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 139

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 145
class struct_anon_12(Structure):
    pass

struct_anon_12.__slots__ = [
    'header',
    'objectName',
    'stiffness',
]
struct_anon_12._fields_ = [
    ('header', MSG_HEADER),
    ('objectName', c_char * 128),
    ('stiffness', c_double),
]

M_HAPTICS_SET_STIFFNESS = struct_anon_12 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 145

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 149
class struct_anon_13(Structure):
    pass

struct_anon_13.__slots__ = [
    'header',
]
struct_anon_13._fields_ = [
    ('header', MSG_HEADER),
]

M_HAPTICS_BOUNDING_PLANE = struct_anon_13 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 149

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 155
class struct_anon_14(Structure):
    pass

struct_anon_14.__slots__ = [
    'header',
    'objectName',
    'enabled',
]
struct_anon_14._fields_ = [
    ('header', MSG_HEADER),
    ('objectName', c_char * 128),
    ('enabled', c_int),
]

M_GRAPHICS_SET_ENABLED = struct_anon_14 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 155

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 160
class struct_anon_15(Structure):
    pass

struct_anon_15.__slots__ = [
    'header',
    'glFloatColor',
]
struct_anon_15._fields_ = [
    ('header', MSG_HEADER),
    ('glFloatColor', c_float),
]

M_GRAPHICS_COLOR_FTOB = struct_anon_15 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 160

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 165
class struct_anon_16(Structure):
    pass

struct_anon_16.__slots__ = [
    'header',
    'glByteColor',
]
struct_anon_16._fields_ = [
    ('header', MSG_HEADER),
    ('glByteColor', c_char),
]

M_GRAPHICS_COLOR_BTOF = struct_anon_16 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 165

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 172
class struct_anon_17(Structure):
    pass

struct_anon_17.__slots__ = [
    'header',
    'a_eye',
    'a_at',
    'a_up',
]
struct_anon_17._fields_ = [
    ('header', MSG_HEADER),
    ('a_eye', c_double * 3),
    ('a_at', c_double * 3),
    ('a_up', c_double * 3),
]

M_GRAPHICS_LOOKAT = struct_anon_17 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 172

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 179
class struct_anon_18(Structure):
    pass

struct_anon_18.__slots__ = [
    'header',
    'scale',
    'lengthScale',
    'thicknessScale',
]
struct_anon_18._fields_ = [
    ('header', MSG_HEADER),
    ('scale', c_double),
    ('lengthScale', c_double),
    ('thicknessScale', c_double),
]

M_GRAPHICS_DRAW_FRAME = struct_anon_18 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 179

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 189
class struct_anon_19(Structure):
    pass

struct_anon_19.__slots__ = [
    'header',
    'xMin',
    'xMax',
    'yMin',
    'yMax',
    'zMin',
    'zMax',
]
struct_anon_19._fields_ = [
    ('header', MSG_HEADER),
    ('xMin', c_double),
    ('xMax', c_double),
    ('yMin', c_double),
    ('yMax', c_double),
    ('zMin', c_double),
    ('zMax', c_double),
]

M_GRAPHICS_DRAW_WIRE_BOX = struct_anon_19 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 189

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 196
class struct_anon_20(Structure):
    pass

struct_anon_20.__slots__ = [
    'header',
    'radius',
    'numSlices',
    'numStacks',
]
struct_anon_20._fields_ = [
    ('header', MSG_HEADER),
    ('radius', c_double),
    ('numSlices', c_double),
    ('numStacks', c_double),
]

M_GRAPHICS_DRAW_SPHERE = struct_anon_20 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 196

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 203
class struct_anon_21(Structure):
    pass

struct_anon_21.__slots__ = [
    'header',
    'arrowStart',
    'arrowTip',
    'a_width',
]
struct_anon_21._fields_ = [
    ('header', MSG_HEADER),
    ('arrowStart', c_double * 3),
    ('arrowTip', c_double * 3),
    ('a_width', c_double),
]

M_GRAPHICS_DRAW_ARROW = struct_anon_21 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 203

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 211
class struct_anon_22(Structure):
    pass

struct_anon_22.__slots__ = [
    'header',
    'innerRadius',
    'outerRadius',
    'sides',
    'rings',
]
struct_anon_22._fields_ = [
    ('header', MSG_HEADER),
    ('innerRadius', c_double),
    ('outerRadius', c_double),
    ('sides', c_int),
    ('rings', c_int),
]

M_GRAPHICS_DRAW_SOLID_TORUS = struct_anon_22 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 211

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 221
class struct_anon_23(Structure):
    pass

struct_anon_23.__slots__ = [
    'header',
    'meshObjectName',
    'lengthX',
    'lengthY',
    'position',
    'rotation',
    'color',
]
struct_anon_23._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('lengthX', c_double),
    ('lengthY', c_double),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_PLANE = struct_anon_23 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 221

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 234
class struct_anon_24(Structure):
    pass

struct_anon_24.__slots__ = [
    'header',
    'meshObjectName',
    'lengthX',
    'lengthY',
    'position',
    'rotation',
    'colorTopLeft',
    'colorTopRight',
    'colorBottomLeft',
    'colorBottomRight',
]
struct_anon_24._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('lengthX', c_double),
    ('lengthY', c_double),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('colorTopLeft', c_float * 4),
    ('colorTopRight', c_float * 4),
    ('colorBottomLeft', c_float * 4),
    ('colorBottomRight', c_float * 4),
]

M_GRAPHICS_CREATE_PLANE2 = struct_anon_24 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 234

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 246
class struct_anon_25(Structure):
    pass

struct_anon_25.__slots__ = [
    'header',
    'meshObjectName',
    'lengthX',
    'lengthY',
    'numSidesX',
    'numSidesY',
    'position',
    'rotation',
    'color',
]
struct_anon_25._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('lengthX', c_double),
    ('lengthY', c_double),
    ('numSidesX', c_uint),
    ('numSidesY', c_uint),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_double * 4),
]

M_GRAPHICS_CREATE_MAP = struct_anon_25 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 246

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 257
class struct_anon_26(Structure):
    pass

struct_anon_26.__slots__ = [
    'header',
    'meshObjectName',
    'radiusX',
    'radiusY',
    'numSlices',
    'position',
    'rotation',
    'color',
]
struct_anon_26._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('radiusX', c_double),
    ('radiusY', c_double),
    ('numSlices', c_uint),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_DISK = struct_anon_26 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 257

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 269
class struct_anon_27(Structure):
    pass

struct_anon_27.__slots__ = [
    'header',
    'meshObjectName',
    'lengthX',
    'lengthY',
    'radiusCorners',
    'numSegmentsPerCorner',
    'position',
    'rotation',
    'color',
]
struct_anon_27._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('lengthX', c_double),
    ('lengthY', c_double),
    ('radiusCorners', c_double),
    ('numSegmentsPerCorner', c_int),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_PANEL = struct_anon_27 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 269

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 287
class struct_anon_28(Structure):
    pass

struct_anon_28.__slots__ = [
    'header',
    'meshObjectName',
    'lengthX',
    'lengthY',
    'cornerTopLeftRadius',
    'cornerTopRightRadius',
    'cornerBottomLeftRadius',
    'cornerBottomRightRadius',
    'numSegmentsPerCorner',
    'position',
    'rotation',
    'colorTopLeft',
    'colorTopRight',
    'colorBottomLeft',
    'colorBottomRight',
]
struct_anon_28._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('lengthX', c_double),
    ('lengthY', c_double),
    ('cornerTopLeftRadius', c_double),
    ('cornerTopRightRadius', c_double),
    ('cornerBottomLeftRadius', c_double),
    ('cornerBottomRightRadius', c_double),
    ('numSegmentsPerCorner', c_int),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('colorTopLeft', c_float * 4),
    ('colorTopRight', c_float * 4),
    ('colorBottomLeft', c_float * 4),
    ('colorBottomRight', c_float * 4),
]

M_GRAPHICS_CREATE_PANEL2 = struct_anon_28 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 287

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 298
class struct_anon_29(Structure):
    pass

struct_anon_29.__slots__ = [
    'header',
    'meshObjectName',
    'lengthX',
    'lengthY',
    'lengthZ',
    'position',
    'rotation',
    'color',
]
struct_anon_29._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('lengthX', c_double),
    ('lengthY', c_double),
    ('lengthZ', c_double),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_BOX = struct_anon_29 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 298

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 308
class struct_anon_30(Structure):
    pass

struct_anon_30.__slots__ = [
    'header',
    'meshObjectName',
    'size',
    'resolution',
    'position',
    'rotation',
    'color',
]
struct_anon_30._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('size', c_double),
    ('resolution', c_int),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_TEAPOT = struct_anon_30 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 308

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 323
class struct_anon_31(Structure):
    pass

struct_anon_31.__slots__ = [
    'header',
    'meshObjectName',
    'height',
    'radius',
    'numSides',
    'numHeightSegments',
    'numRings',
    'includeTop',
    'includeBottom',
    'position',
    'rotation',
    'color',
]
struct_anon_31._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('height', c_double),
    ('radius', c_double),
    ('numSides', c_uint),
    ('numHeightSegments', c_uint),
    ('numRings', c_uint),
    ('includeTop', c_int),
    ('includeBottom', c_int),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_CYLINDER = struct_anon_31 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 323

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 336
class struct_anon_32(Structure):
    pass

struct_anon_32.__slots__ = [
    'header',
    'meshObjectName',
    'height',
    'innerRadius',
    'outerRadius',
    'numSides',
    'numHeightSegments',
    'position',
    'rotation',
    'color',
]
struct_anon_32._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('height', c_double),
    ('innerRadius', c_double),
    ('outerRadius', c_double),
    ('numSides', c_uint),
    ('numHeightSegments', c_uint),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_PIPE = struct_anon_32 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 336

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 347
class struct_anon_33(Structure):
    pass

struct_anon_33.__slots__ = [
    'header',
    'meshObjectName',
    'radius',
    'numSlices',
    'numStacks',
    'position',
    'rotation',
    'color',
]
struct_anon_33._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('radius', c_double),
    ('numSlices', c_uint),
    ('numStacks', c_uint),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_SPHERE = struct_anon_33 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 347

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 360
class struct_anon_34(Structure):
    pass

struct_anon_34.__slots__ = [
    'header',
    'meshObjectName',
    'radiusX',
    'radiusY',
    'radiusZ',
    'numSlices',
    'numStacks',
    'position',
    'rotation',
    'color',
]
struct_anon_34._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('radiusX', c_double),
    ('radiusY', c_double),
    ('radiusZ', c_double),
    ('numSlices', c_uint),
    ('numStacks', c_uint),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_ELLIPSOID = struct_anon_34 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 360

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 372
class struct_anon_35(Structure):
    pass

struct_anon_35.__slots__ = [
    'header',
    'meshObjectName',
    'innerRadius',
    'outerRadius',
    'numSides',
    'numRings',
    'position',
    'rotation',
    'color',
]
struct_anon_35._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('innerRadius', c_double),
    ('outerRadius', c_double),
    ('numSides', c_uint),
    ('numRings', c_uint),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_RING = struct_anon_35 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 372

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 387
class struct_anon_36(Structure):
    pass

struct_anon_36.__slots__ = [
    'header',
    'meshObjectName',
    'innerRadius0',
    'innerRadius1',
    'outerRadius',
    'coverageAngleDeg',
    'includeExtremityFaces',
    'numSides',
    'numRings',
    'position',
    'rotation',
    'color',
]
struct_anon_36._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('innerRadius0', c_double),
    ('innerRadius1', c_double),
    ('outerRadius', c_double),
    ('coverageAngleDeg', c_double),
    ('includeExtremityFaces', c_int),
    ('numSides', c_uint),
    ('numRings', c_uint),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_RING_SECTION = struct_anon_36 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 387

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 403
class struct_anon_37(Structure):
    pass

struct_anon_37.__slots__ = [
    'header',
    'meshObjectName',
    'height',
    'radiusBototm',
    'radiusTop',
    'numSides',
    'numHeightSegments',
    'numRings',
    'includeBottom',
    'includeTop',
    'position',
    'rotation',
    'color',
]
struct_anon_37._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('height', c_double),
    ('radiusBototm', c_double),
    ('radiusTop', c_double),
    ('numSides', c_uint),
    ('numHeightSegments', c_uint),
    ('numRings', c_uint),
    ('includeBottom', c_int),
    ('includeTop', c_int),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_CONE = struct_anon_37 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 403

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 414
class struct_anon_38(Structure):
    pass

struct_anon_38.__slots__ = [
    'header',
    'meshObjectName',
    'height',
    'baseSize',
    'includeBottom',
    'position',
    'rotation',
    'color',
]
struct_anon_38._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('height', c_double),
    ('baseSize', c_double),
    ('includeBottom', c_int),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_SQUARE_PYRAMID = struct_anon_38 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 414

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 428
class struct_anon_39(Structure):
    pass

struct_anon_39.__slots__ = [
    'header',
    'meshObjectName',
    'a_length',
    'radiusShaft',
    'lengthTip',
    'radiusTip',
    'includeTipsAtBothExtremities',
    'numSides',
    'position',
    'rotation',
    'color',
]
struct_anon_39._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('a_length', c_double),
    ('radiusShaft', c_double),
    ('lengthTip', c_double),
    ('radiusTip', c_double),
    ('includeTipsAtBothExtremities', c_int),
    ('numSides', c_uint),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_ARROW = struct_anon_39 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 428

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 445
class struct_anon_40(Structure):
    pass

struct_anon_40.__slots__ = [
    'header',
    'meshObjectName',
    'innerRadius0',
    'innerRadius1',
    'outerRadius',
    'lengthTip',
    'radiusTip',
    'coverageAngleDeg',
    'includeTipsAtBothExtremities',
    'numSides',
    'numRings',
    'position',
    'rotation',
    'color',
]
struct_anon_40._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('innerRadius0', c_double),
    ('innerRadius1', c_double),
    ('outerRadius', c_double),
    ('lengthTip', c_double),
    ('radiusTip', c_double),
    ('coverageAngleDeg', c_double),
    ('includeTipsAtBothExtremities', c_int),
    ('numSides', c_uint),
    ('numRings', c_uint),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_CIRCULAR_ARROW = struct_anon_40 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 445

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 455
class struct_anon_41(Structure):
    pass

struct_anon_41.__slots__ = [
    'header',
    'meshObjectName',
    'controlPoints',
    'numDivisions',
    'position',
    'rotation',
    'color',
]
struct_anon_41._fields_ = [
    ('header', MSG_HEADER),
    ('meshObjectName', c_char * 128),
    ('controlPoints', c_double * 3),
    ('numDivisions', c_int),
    ('position', c_double * 3),
    ('rotation', c_double * 9),
    ('color', c_float * 4),
]

M_GRAPHICS_CREATE_BEZIER_PATCH = struct_anon_41 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 455

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 463
class struct_anon_42(Structure):
    pass

struct_anon_42.__slots__ = [
    'header',
    'objectName',
    'radius',
    'localPosition',
    'color',
]
struct_anon_42._fields_ = [
    ('header', MSG_HEADER),
    ('objectName', c_char * 128),
    ('radius', c_double),
    ('localPosition', c_double * 3),
    ('color', c_float * 4),
]

M_GRAPHICS_SHAPE_SPHERE = struct_anon_42 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 463

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 472
class struct_anon_43(Structure):
    pass

struct_anon_43.__slots__ = [
    'header',
    'objectName',
    'innerRadius',
    'outerRadius',
    'localPosition',
    'color',
]
struct_anon_43._fields_ = [
    ('header', MSG_HEADER),
    ('objectName', c_char * 128),
    ('innerRadius', c_double),
    ('outerRadius', c_double),
    ('localPosition', c_double * 3),
    ('color', c_float * 4),
]

M_GRAPHICS_SHAPE_TORUS = struct_anon_43 # /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 472

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 1
try:
    DEFAULT_IP = 'localhost:10000'
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 2
try:
    MAX_PACKET_LENGTH = 8192
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 3
try:
    MAX_STRING_LENGTH = 128
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 6
try:
    CONTROL_SIMULATION_STARTED = 10
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 7
try:
    CONTROL_SIMULATION_FINISHED = 11
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 10
try:
    HAPTIC_DATA_STREAM = 1000
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 11
try:
    HAPTICS_TOOL_CONNECTED = 1001
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 12
try:
    HAPTICS_SET_ENABLED = 1002
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 13
try:
    HAPTICS_EFFECT_MAGNET = 1003
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 14
try:
    HAPTICS_EFFECT_STICKSLIP = 1004
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 15
try:
    HAPTICS_EFFECT_SURFACE = 1005
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 16
try:
    HAPTICS_EFFECT_VIBRATION = 1006
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 17
try:
    HAPTICS_EFFECT_VISCOSITY = 1007
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 18
try:
    HAPTICS_SET_STIFFNESS = 1008
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 19
try:
    HAPTICS_BOUNDING_PLANE = 1009
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 22
try:
    GRAPHICS_SET_ENABLED = 2000
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 37
try:
    GRAPHICS_COLOR_FTOB = 2015
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 38
try:
    GRAPHICS_COLOR_BTOF = 2016
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 39
try:
    GRAPHICS_LOOKAT = 2017
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 40
try:
    GRAPHICS_DRAW_FRAME = 2018
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 41
try:
    GRAPHICS_DRAW_WIRE_BOX = 2019
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 42
try:
    GRAPHICS_DRAW_SPHERE = 2020
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 43
try:
    GRAPHICS_DRAW_ARROW = 2021
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 44
try:
    GRAPHICS_DRAW_SOLID_TORUS = 2022
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 45
try:
    GRAPHICS_CREATE_PLANE = 2023
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 46
try:
    GRAPHICS_CREATE_PLANE2 = 2024
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 47
try:
    GRAPHICS_CREATE_MAP = 2025
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 48
try:
    GRAPHICS_CREATE_DISK = 2026
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 49
try:
    GRAPHICS_CREATE_PANEL = 2027
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 50
try:
    GRAPHICS_CREATE_PANEL2 = 2028
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 51
try:
    GRAPHICS_CREATE_BOX = 2029
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 52
try:
    GRAPHICS_CREATE_TEAPOT = 2030
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 53
try:
    GRAPHICS_CREATE_CYLINDER = 2031
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 54
try:
    GRAPHICS_CREATE_PIPE = 2032
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 55
try:
    GRAPHICS_CREATE_SPHERE = 2033
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 56
try:
    GRAPHICS_CREATE_ELLIPSOID = 2034
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 57
try:
    GRAPHICS_CREATE_RING = 2035
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 58
try:
    GRAPHICS_CREATE_RING_SECTION = 2036
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 59
try:
    GRAPHICS_CREATE_CONE = 2037
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 60
try:
    GRAPHICS_CREATE_SQUARE_PYRAMID = 2038
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 61
try:
    GRAPHICS_CREATE_ARROW = 2039
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 62
try:
    GRAPHICS_CREATE_CIRCULAR_ARROW = 2040
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 63
try:
    GRAPHICS_CREATE_BEZIER_PATCH = 2041
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 72
try:
    GRAPHICS_SHAPE_SPHERE = 2050
except:
    pass

# /home/mfl24/Documents/chaiProjects/hapticControl/src/network/messageDefinitions.h: 73
try:
    GRAPHICS_SHAPE_TORUS = 2051
except:
    pass

# No inserted files

