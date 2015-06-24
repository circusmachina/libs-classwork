{ commontypes.inc: Common types used by the library.

  This file is part of classwork.pp, a library of basic and useful classes.

  This program is free software: you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the
  Free Software Foundation; either version 3 of the License or, at your
  option, any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABLITY
  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
  License for more details.

  You should have received a copy of the GNU General Public License along
  with this program.  If not, see <http://www.gnu.org/licenses/>.
}
{$IFDEF ReadInterface}

{ Define this to force the use of 64-bit types.  Otherwise, 64-bit types are
  only used on 64-bit targets.
}
{DEFINE TYPESIZE := 64}

{$IF defined(CPU64)}
  {$DEFINE TYPESIZE := 64}
{$ELSEIF defined(CPU32)}
  {$DEFINE TYPESIZE := 32}
{$ENDIF}

{ This type is used to represent an absolute index, which is a way of referring
  to the position of one item in a sequence relative to the beginning of the
  sequence.  The index is zero-based, with the first item in the sequence always
  positioned at index zero (@code(0)).

  This type is defined to make code flexible and easy to change.
}
type
{$IF TYPESIZE = 64}
  TIndexAbsolute = qword;
{$ELSE}
  TIndexAbsolute = longword;
{$ENDIF}

{ This type is used to represent a relative index, which is a way of referring
  to the position of one item in a sequence relative to another item in the
  sequence.
  
  Typically, values greater than or equal to zero are used to represent 
  a position relative to the beginning of the sequence.  The first item in the
  sequence can thus be found at index zero (@code(0)).  Values less than zero
  are used to represent a position relative to the end of the sequence.  The
  last item in the sequence can be found at index @code(-1).

  This type is defined to make code flexible and easy to change.
}
type
{$IF TYPESIZE = 64}
  TIndexRelative = int64;
{$ELSE}
  TIndexRelative = longint;
{$ENDIF}

{ This type is used to represent a census, which is a way of storing a total
  count of items.

  This type is defined to make code flexible and easy to change.
}
type
{$IF TYPESIZE = 64}
  TCensus = int64;
{$ELSE}
  TCensus = longint;
{$ENDIF}

{ This type is used to represent a sort key, which is used to arrange items in
  a binary tree or list.  It is defined to make code flexible and easy to
  change or update.
}
type
{$IF TYPESIZE = 64}
  TSortKey = int64;
{$ELSE}
  TSortKey = longint;
{$ENDIF}
{$ENDIF}
