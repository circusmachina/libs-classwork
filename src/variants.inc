{ variants.inc: Utility routines for handling variants and TVarRecs.

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
{ This function converts the value in @code(value) to a @code(Variant).

  This function converts @code(value) as follows:
  @unorderedList(
    @item(If @code(value.vtype) is @code(vtBoolean), the resulting variant is
      cast as a @code(WordBool).)
    @item(If @code(value.vtype) is @code(vtInteger), the resulting variant is
      cast as a @code(longint).)
    @item(If @code(value.vtype) is @code(vtInt64), the resulting variant is
      cast as a @code(int64).)
    @item(If @code(value.vtype) is @code(vtQWord), the resulting variant is
      cast as a @code(qword).)
    @item(If @code(value.vtype) is @code(vtExtended), the resulting variant is
      cast as a @code(double), with some potential loss of precision, since
      @code(Variant)s cannot store @code(extended) values.)
    @item(If @code(value.vtype) is @code(vtCurrency), the resulting variant is
      cast as a @code(currency) type.)
    @item(If @code(value.vtype) is @code(vtChar), @code(vtWideChar),
      @code(vtString), @code(vtPChar), @code(vtPWideChar), @code(vtAnsiString),
      @code(vtWideString), or @code(vtUnicodeString), the resulting variant is
      cast as an @code(AnsiString).)
    @item(If @code(value.vtype) is @code(vtObject), @code(vtClass), or
      @code(vtInterface), the resulting variant is cast as a @code(pointer).)
    @item(If @code(value.vtype) is @code(vtVariant), the resulting variant is
      set to the value of @code(value.vVariant).)
  )

  In all other cases, the resulting variant is set to @code(Variants.Null).
}
function TVarRecToVariant(const value: TVarRec): Variant;

{ This function converts @code(item) to a string and returns the result.

  If @code(item) represents a boolean value, the string returned will be either
  @link(boolValueTrueString) or @link(boolValueFalseString), depending on the
  value of the item.

  If @code(item) represents an instance of @link(AnObject) that implements
  the @link(CanPrint) interface, the string returned is that obtained by calling
  @link(CanPrint.toString) on the item.  Otherwise, if @code(item) represents
  @link(AnObject), the string returned is that obtained by calling
  @link(AnObject.name).
}
function TVarRecToString(const item: TVarRec): string;
{$ENDIF}
{$IFDEF ReadImplementation}
// Convert a TVarRec to a Variant
function TVarRecToVariant(const value: TVarRec): Variant;

begin
  result := Variants.null;

  case value.vType of
    vtInteger:
      result := value.vInteger;

    vtBoolean:
      result := value.vBoolean;

    vtChar:
      result := value.vChar;

    vtWideChar:
      result := System.wideCharLenToString(@value.vWideChar, 1);

    vtExtended:
      result := double(value.vExtended^);

    vtString:
      result := value.vString^;

    vtPointer:
      result := value.vPointer;

    vtPChar:
      result := value.vPChar^;

    vtObject:
      result := pointer(value.VObject);

    vtClass:
      result := pointer(value.VClass);

    vtPWideChar:
      result := System.wideCharToString(value.vpWideChar);

    vtAnsiString:
      result := AnsiString(value.vAnsiString);

    vtCurrency:
      result := value.vCurrency^;

    vtVariant:
      result := value.vVariant^;

    vtInterface:
      result := value.vInterface;

    vtWideString:
      result := WideString(value.vWideString);

    vtInt64:
      result := value.vInt64^;

    vtUnicodeString:
      result := UnicodeString(value.vUnicodeString);

    vtQWord:
      result := value.vQWord^;
  end;
end;

// Convert a TVarRec to a string
function TVarRecToString(const item: TVarRec): string;

begin
  result := '';

  case item.vtype of
    vtInteger:
      result := intToStr(item.vinteger);

    vtBoolean:
      if item.vboolean then
        result := boolValueTrueString

      else
        result := boolValueFalseString;

    vtChar:
      result := item.vchar;

    vtExtended:
      result := floatToStr(item.vextended^);

    vtString:
      result := item.vstring^;

    vtPointer:
      result := intToHex(int64(item.vpointer), sizeof(int64) * 2);

    vtPChar:
      result := item.vpchar^;

    vtObject:
      if (item.VObject <> nil) and (item.VObject is AnObject) then
        if AnObject(item.VObject) is CanPrint then
          with item.VObject as CanPrint do
            result := toString

        else
          result := AnObject(item.VObject).name;

    vtClass:
      if item.VClass <> nil then
        result := item.VClass.className;

    vtAnsiString:
      result := AnsiString(item.vAnsiString);

    vtWideString:
      result := WideString(item.vWideString);

    vtUnicodeString:
      result := UnicodeString(item.vUnicodeString);

    vtInt64:
      result := intToStr(item.vInt64^);

    vtQWord:
      result := intToStr(item.vQWord^);
  end;
end;
{$ENDIF}
