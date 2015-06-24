{ math.inc: Various number routines not found in the RTL.

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
{ Clamp @code(value) to a number between @code(min) and @code(max).

  If @code(value) is less than @code(min), then @code(value) is adjusted to the
  value of @code(min).  If @code(value) is greater than @code(max), then it is
  adjusted to the value of @code(max).  Otherwise, @code(value) remains
  unchanged.
}
procedure clamp(var value: longint; const min, max: longint); overload;

{ Clamp @code(value) to a number between @code(min) and @code(max).

  If @code(value) is less than @code(min), then @code(value) is adjusted to the
  value of @code(min).  If @code(value) is greater than @code(max), then it is
  adjusted to the value of @code(max).  Otherwise, @code(value) remains
  unchanged.
}
procedure clamp(var value: longword; const min, max: longword); overload;

{ Clamp @code(value) to a number between @code(min) and @code(max).

  If @code(value) is less than @code(min), then @code(value) is adjusted to the
  value of @code(min).  If @code(value) is greater than @code(max), then it is
  adjusted to the value of @code(max).  Otherwise, @code(value) remains
  unchanged.
}
procedure clamp(var value: int64; const min, max: int64); overload;

{ Clamp @code(value) to a number between @code(min) and @code(max).

  If @code(value) is less than @code(min), then @code(value) is adjusted to the
  value of @code(min).  If @code(value) is greater than @code(max), then it is
  adjusted to the value of @code(max).  Otherwise, @code(value) remains
  unchanged.
}
procedure clamp(var value: qword; const min, max: qword); overload;

{ Clamp @code(value) to a number between @code(min) and @code(max).

  If @code(value) is less than @code(min), then @code(value) is adjusted to the
  value of @code(min).  If @code(value) is greater than @code(max), then it is
  adjusted to the value of @code(max).  Otherwise, @code(value) remains
  unchanged.
}
procedure clamp(var value: real; const min, max: real); overload;

{ Clamp @code(value) to a number between @code(min) and @code(max).

  If @code(value) is less than @code(min), then @code(value) is adjusted to the
  value of @code(min).  If @code(value) is greater than @code(max), then it is
  adjusted to the value of @code(max).  Otherwise, @code(value) remains
  unchanged.
}
procedure clamp(var value: double; const min, max: double); overload;
{$ENDIF}
{$IFDEF ReadImplementation}
// Clamp the given value to the given range
procedure clamp(var value: longint; const min, max: longint);

begin
  if value < min then
    value := min

  else if value > max then
    value := max;
end;

// Clamp the given value to the given range
procedure clamp(var value: longword; const min, max: longword);

begin
  if value < min then
    value := min

  else if value > max then
    value := max;
end;

// Clamp the given value to the given range
procedure clamp(var value: int64; const min, max: int64);

begin
  if value < min then
    value := min

  else if value > max then
    value := max;
end;

// Clamp the given value to the given range
procedure clamp(var value: qword; const min, max: qword);

begin
  if value < min then
    value := min

  else if value > max then
    value := max;
end;

// Clamp the given value to the given range
procedure clamp(var value: real; const min, max: real);

begin
  if value < min then
    value := min

  else if value > max then
    value := max;
end;

// Clamp the given value to the given range
procedure clamp(var value: double; const min, max: double);

begin
  if value < min then
    value := min

  else if value > max then
    value := max;
end;
{$ENDIF}
