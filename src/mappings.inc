{ mappings.inc: An abstract class that defines mappings, which associate a
    string key with some type of value.

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
// AMappingEntry ***************************************************************
{ This class represents a mapping entry, which is an item that is indexed
  by a string key and which maps that key to a value.  The base class places
  no restrictions on the type of value that can be represented by the entry; it
  stipulates only that descendant classes must provide a way to represent that
  value as a string.
}
type AMappingEntry = class(AStringLeaf)
  public
    { Implode the entry into a string that contains a key/value pair, where
      the key and value are separated by @code(delimiter).

      This method calls @link(AStringLeaf.name Self.name) to obtain the value
      of the entry's string key and @link(AMappingEntry.valueAsString) to
      obtain the entry's present value.

      If @code(delimiter) is not specified by the caller, or if it is an
      empty string, then the value of @link(mapDefaultDelimiter) is used.
    }
    function implode(delimiter: string = ''): AnsiString; virtual;

    // CanPrint implementation -------------------------------------------------
    { Construct and return a string representation of the entry, suitable for
      printing to a text-based device, such as a console.

      In accordance with the criteria described by
      @link(APrintingObject.toString), the string representation returned by
      this method will include the name of the entry (returned by a call to
      @link(AStringLeaf.name ADictionaryEntry.name)) and a string
      representation of its value (returned by a call to
      @code(Variants.varToStr)), separated by the delimiter specified in
      @link(mapDefaultDelimiter).  This string is obtained by calling
      @link(AMappingEntry.implode).
    }
    function toString: AnsiString; override;

    // Properties --------------------------------------------------------------
    { Return the value of the mapping entry as a string.  This method must
      be implemented by descendant classes.
    }
    function valueAsString: string; virtual; abstract;
end;

// AMapping ********************************************************************
{ This class represents a mapping, which is a list of items that are indexed
  by a string key.  The overall functionality of the mapping is similar to
  that provided by Python and other languages that implement such objects;
  however, the maping is implemented as a binary tree rather than a table
  of hashed objects and buckets.
}
type AMapping = class(ABinaryTree)
  public
    // Initializer
    function init: boolean; override;

    { Condense the mapping and its entries into a doubly-delimited string.

      This method iterates through the items in the mapping, calling
      @link(AMappingEntry.implode) on each one.  The resulting string
      will consist of key/value pairs, in which the keys and values are
      separated by @code(keyValueDelimiter) and the key/value pairs are
      separated from each other by @code(pairsDelimiter).

      If @code(keyValueDelimiter) is not specified by the caller, or if it is an
      empty string, then the value of @link(mapDefaultDelimiter) is used.  If
      @code(pairsDelimiter) is not specified by the caller, or if it is an empty
      string, then value of @link(llstDefaultDelimiter) is used.

      @return(A doubly-delimited string that contains representations of all
      dictionary entries.)
    }
    function implode(pairsDelimiter: string = '';
      keyValueDelimiter: string = ''): AnsiString; virtual;

    { Construct a list of all string keys in the mapping.

      This method proceeds sequentially through all entries in the mapping,
      obtaining the name of each.  The names are inserted into an instance of
      @link(AStringList) which is constructed for this purpose.

      @return(An instance of @link(AStringList) that contains the names of all
      mapping entries.  This list should be freed by the caller when it is
      no longer required.  If there are no entries in the mapping, an empty
      string list is returned.)
    }
    function Keys: AStringList; virtual;

    { Construct a list of all values in the mapping, represented as strings.

      This method iterates sequentially through all entries in the mapping,
      calling @link(AMappingEntry.valueAsString) on each.  The values are
      inserted into an instance of @link(AStringList) which is constructed
      for this purpose.

      @return(An instance of @link(AStringList) that contains the values of
        all mapping entries, represented as strings.  This list should be
        freed by the caller when it is no longer required.  If there are no
        entries in the mapping, an empty string list is returned.)
    }
    function ValuesAsStrings: AStringList; virtual;

    { Construct a delimited string that contains the names of all entries in
      the mapping.  The names are separated from each other by the string
      specified in @code(delimiter).  If @code(delimiter) is not provided by
      the caller, or if it is an empty string, then the value of
      @link(llstDefaultDelimiter) is used.
    }
    function implodeKeys(delimiter: string = ''): AnsiString; virtual;

    { Construct a delimited string that contains the values of all entries in
      the dictionary.  The values are separated from each other by the string
      specified in @code(delimiter).  If @code(delimiter) is not provided by
      the caller, or if it is an empty string, then the value of
      @link(llstDefaultDelimiter) is used.
    }
    function implodeValues(delimiter: string = ''): AnsiString; virtual;

    // Properties --------------------------------------------------------------
    { Retrieve the entry associated with the specified string key.  This method
      is synonymous to a call to @link(ABinaryTree.fetchLeaf), except that it
      returns an instance of @link(AMappingEntry).
    }
    function Entry(const key: string): AMappingEntry; virtual;

    { Retrieve the key at the specified index in the mapping.

      Internally, this method calls @link(ABinaryTree.LeafAtIndex) on itself; if
      that method returns a node, then
      @link(AStringLeaf.name AMappingEntry.name) is called on it.

      @return(The name of the node at the specified @code(index), if found;
      an empty string otherwise.)
    }
    function keyAt(const thisIndex: TIndexAbsolute): string; virtual;
end;

{$ENDIF}
{$IFDEF ReadResourceStrings}
  { This string is used as the default key/value pair delimiter when none is
    provided to @link(AMapping.implode) and
    @link(AMappingEntry.implode).
  }
  mapDefaultDelimiter = '=';
{$ENDIF}
{$IFDEF ReadImplementation}
// AMappingEntry ***************************************************************
// Implode the entry
function AMappingEntry.implode(delimiter: string = ''): AnsiString;

begin
  if delimiter = '' then
    delimiter := mapDefaultDelimiter;

  result := Self.name + delimiter + Self.valueAsString;
end;

// CanPrint implementation -----------------------------------------------------
{* Creates a string representation of the dictionary item.  This representation
   will contain the key and value of the item separated by the delimiter
   specified by mapeDefaultDelimiter.
*}
function AMappingEntry.toString: AnsiString;

begin
  result := Self.implode;
end;

// AMapping ********************************************************************
// Initializer
function AMapping.init: boolean;

begin
  result := inherited init;

  if result then
  begin
    MyLeafType := AMappingEntry;
  end;
end;

{* Condenses the mapping and its entries into a string.  The string is
   constructed as a delimited list of values, where each value is a key/value
   pair.
*}
function AMapping.implode(pairsDelimiter: string = '';
  keyValueDelimiter: string = ''): AnsiString;

var
  // Refers to a list of entries constructed from the dictionary entries
  EntryList: AStringList;
  // Refers to an entry from the dictionary
  ThisEntry: AMappingEntry;

begin
  result := '';

  if MyChild <> nil then
  begin
    EntryList := AStringList.new;
    ThisEntry := AMappingEntry(MyRoot);

    while ThisEntry <> nil do
    begin
      EntryList.push(ThisEntry.implode(keyValueDelimiter));

      ThisEntry := AMappingEntry(ThisEntry.Next);
    end;

    // Now implode the string list
    result := EntryList.implode(pairsDelimiter);
    EntryList.free;
  end;
end;

{* Retrieves a list of dictionary keys.

   \return A list of dictionary keys.  If the dictionary contains no entries,
   this routine will return an empty list.
*}
function AMapping.Keys: AStringList;

var
  // Refers to an entry from the dictionary
  ThisEntry: AMappingEntry;

begin
  Result := AStringList.new;

  ThisEntry := AMappingEntry(MyRoot);

  while ThisEntry <> nil do
  begin
    Result.push(ThisEntry.name);

    ThisEntry := AMappingEntry(ThisEntry.Next);
  end;
end;

// Retrieve a list of mapping values, represented as strings
function AMapping.ValuesAsStrings: AStringList;

var
  // Refers to an entry from the dictionary
  ThisEntry: AMappingEntry;

begin
  Result := AStringList.new;

  ThisEntry := AMappingEntry(MyRoot);

  while ThisEntry <> nil do
  begin
    Result.push(ThisEntry.valueAsString);

    ThisEntry := AMappingEntry(ThisEntry.Next);
  end;
end;

{* Condenses the list of mapping keys into a string.  This routine is
   provided primarily for convenience, as it simply calls AMapping.keys()
   and then calls AStringList.implode() on the resulting list.

   \return A string containing a list of all keys in the dictionary, delimited
   by the character or characters specified by \p thisDelimiter.  If there are
   no entries in the dictionary, this routine returns an empty string.
*}
function AMapping.implodeKeys(delimiter: string = ''): AnsiString;

var
  // Refers to the list of keys
  KeyList: AStringList;

begin
  result := '';

  if delimiter = '' then
    delimiter := llstDefaultDelimiter;

  KeyList := Self.keys;

  if KeyList <> nil then
  begin
    result := KeyList.implode(delimiter);
    KeyList.free;
  end;
end;

// Condenses the list of dictionary values into a string
function AMapping.implodeValues(delimiter: string = ''): AnsiString;

var
  // Refers to a list of entries constructed from the dictionary entries
  EntryList: AStringList;
  // Refers to an entry from the dictionary
  ThisEntry: AMappingEntry;

begin
  result := '';

  if MyChild <> nil then
  begin
    EntryList := AStringList.new;
    ThisEntry := AMappingEntry(MyRoot);

    while ThisEntry <> nil do
    begin
      EntryList.push(ThisEntry.valueAsString);

      ThisEntry := AMappingEntry(ThisEntry.Next);
    end;

    // Now implode the string list
    result := EntryList.implode(delimiter);
    EntryList.free;
  end;
end;

// Properties ------------------------------------------------------------------
// Retrieve the entry specified by the named key
function AMapping.Entry(const key: string): AMappingEntry;

begin
  Result := AMappingEntry(Self.fetchLeaf(key));
end;

// Retrieve the key at the specified offset
function AMapping.keyAt(const thisIndex: TIndexAbsolute): string;

var
  // Used to loop through the dictionary
  ThisItem: AMappingEntry;

begin
  result := '';

  ThisItem := AMappingEntry(Self.leafAtIndex(thisIndex));

  if ThisItem <> nil then
    result := ThisItem.name;
end;
{$ENDIF}
