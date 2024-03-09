#! /bin/sh
#
# Copyright (C) 2002-2024 Free Software Foundation, Inc.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

# This program invokes the Python rewrite of gnulib-tool.
# It is meant to behave identically to gnulib-tool, just faster.

progname=$0

# func_exit STATUS
# exits with a given status.
# This function needs to be used, rather than 'exit', when a 'trap' handler is
# in effect that refers to $?.
func_exit ()
{
  (exit $1); exit $1
}

# func_fatal_error message
# outputs to stderr a fatal error message, and terminates the program.
# Input:
# - progname                 name of this program
func_fatal_error ()
{
  echo "$progname: *** $1" 1>&2
  echo "$progname: *** Stop." 1>&2
  func_exit 1
}

# func_readlink SYMLINK
# outputs the target of the given symlink.
if (type readlink) > /dev/null 2>&1; then
  func_readlink ()
  {
    # Use the readlink program from GNU coreutils.
    readlink "$1"
  }
else
  func_readlink ()
  {
    # Use two sed invocations. A single sed -n -e 's,^.* -> \(.*\)$,\1,p'
    # would do the wrong thing if the link target contains " -> ".
    LC_ALL=C ls -l "$1" | sed -e 's, -> ,#%%#,' | sed -n -e 's,^.*#%%#\(.*\)$,\1,p'
  }
fi

# func_gnulib_dir
# locates the directory where the gnulib repository lives
# Input:
# - progname                 name of this program
# Sets variables
# - self_abspathname         absolute pathname of gnulib-tool
# - gnulib_dir               absolute pathname of gnulib repository
func_gnulib_dir ()
{
  case "$progname" in
    /* | ?:*) self_abspathname="$progname" ;;
    */*) self_abspathname=`pwd`/"$progname" ;;
    *)
      # Look in $PATH.
      # Iterate through the elements of $PATH.
      # We use IFS=: instead of
      #   for d in `echo ":$PATH:" | sed -e 's/:::*/:.:/g' | sed -e 's/:/ /g'`
      # because the latter does not work when some PATH element contains spaces.
      # We use a canonicalized $pathx instead of $PATH, because empty PATH
      # elements are by definition equivalent to '.', however field splitting
      # according to IFS=: loses empty fields in many shells:
      #   - /bin/sh on OSF/1 and Solaris loses all empty fields (at the
      #     beginning, at the end, and in the middle),
      #   - /bin/sh on IRIX and /bin/ksh on IRIX and OSF/1 lose empty fields
      #     at the beginning and at the end,
      #   - GNU bash, /bin/sh on AIX and HP-UX, and /bin/ksh on AIX, HP-UX,
      #     Solaris lose empty fields at the end.
      # The 'case' statement is an optimization, to avoid evaluating the
      # explicit canonicalization command when $PATH contains no empty fields.
      self_abspathname=
      if test "$PATH_SEPARATOR" = ";"; then
        # On Windows, programs are searched in "." before $PATH.
        pathx=".;$PATH"
      else
        # On Unix, we have to convert empty PATH elements to ".".
        pathx="$PATH"
        case :$PATH: in
          *::*)
            pathx=`echo ":$PATH:" | sed -e 's/:::*/:.:/g' -e 's/^://' -e 's/:\$//'`
            ;;
        esac
      fi
      saved_IFS="$IFS"
      IFS="$PATH_SEPARATOR"
      for d in $pathx; do
        IFS="$saved_IFS"
        test -z "$d" && d=.
        if test -x "$d/$progname" && test ! -d "$d/$progname"; then
          self_abspathname="$d/$progname"
          break
        fi
      done
      IFS="$saved_IFS"
      if test -z "$self_abspathname"; then
        func_fatal_error "could not locate the gnulib-tool program - how did you invoke it?"
      fi
      ;;
  esac
  while test -h "$self_abspathname"; do
    # Resolve symbolic link.
    linkval=`func_readlink "$self_abspathname"`
    test -n "$linkval" || break
    case "$linkval" in
      /* | ?:* ) self_abspathname="$linkval" ;;
      * ) self_abspathname=`echo "$self_abspathname" | sed -e 's,/[^/]*$,,'`/"$linkval" ;;
    esac
  done
  gnulib_dir=`echo "$self_abspathname" | sed -e 's,/[^/]*$,,'`
}

func_gnulib_dir

# Setting the PYTHONPATH environment variable is needed to avoid an error:
#   from . import constants          =>   ImportError: attempted relative import with no known parent package
#   from pygnulib import constants   =>   ModuleNotFoundError: No module named 'pygnulib'
# Explanation: https://stackoverflow.com/questions/16981921/relative-imports-in-python-3
exec env PYTHONPATH="$gnulib_dir" python3 "$gnulib_dir/pygnulib/main.py" "$@"
