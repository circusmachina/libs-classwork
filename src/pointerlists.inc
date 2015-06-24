{ pointerlists.inc - Pointer lists organized as a linked list that can be
    accessed like an indexed array or as a stack (LIFO).

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
// APointerListItem ************************************************************
{ This class represents a single entry in @link(APointerList).  It extends
  @link(ANode) to allow each one to hold a pointer value.  The list item also
  supports streaming.
}
type APointerListItem = class(ANode)
  protected
    { Stores the value of the list item. }
    myReference: pointer;

  public
    { Construct a new list item that has the specified pointer value. }
    constructor withReference(const thisReference: pointer); virtual;

    { Initializer }
    function init: boolean; override;

    { Search the node, its children, and its older siblings for the specified
      value.

      This method performs a sequential search through each node and so can
      be inefficient for large lists.  It begins by recursively calling itself
      on any older siblings.  If the value is not found, then it searches the
      current node and its children.

      If this method is called on the last item in @link(APointerList), it will
      result in the entire list being searched.

      @return(A reference to the first node that contains the specified pointer
      value, if found; @nil if the value was not found.)
    }
    function Find(const thisReference: pointer): APointerListItem; virtual;

    { Construct a shallow copy of the other object.

      This method overrides the behavior inherited from
      @link(AnObject.shallowCopyFrom): it calls that method, then checks to see
      whether @code(Other) is an instance of @link(APointerListItem).  If so,
      it copies the value of @link(APointerListItem.reference) from @code(Other)
      to @code(Self), overwriting the value in @code(Self).

      Note that this method does NOT copy any sibling or child nodes and so
      cannot be used to create a full copy of any descendant of @link(ANode).
      The copy will NOT be automatically placed in the list to which
      @code(Other) belongs, if any, but the caller is free to do so.
    }
    function shallowCopyFrom(const Other: AnObject): boolean; override;

    { Calculate the number of bytes required to stream the node, and just the
      node.

      This method builds upon the behavior inherited from
      @link(ANode.selfStreamingLength): it calls the inherited routine, then
      calculates the number of bytes required to stream its text and adds that
      to the result, which is returned to the caller.
    }
    function selfStreamingLength: TStreamIOSize; override;

    { Write the node, and just the node, to the specified stream.  For more
      information on this method, see @link(ANode.writeSelfTo).

      This writes the value of its reference, then calls the inherited routine.

      @return(The total number of bytes written to @code(Dest).)
    }
    function writeSelfTo(const Dest: AStream): TStreamIOSize; override;

  // CanStream implementation --------------------------------------------------
    { Read the node from the specified stream.  For more information on this
      method, see @link(ANode.readFrom).

      This method reads the value of its pointer reference, then calls the
      inherited routine.

      @return(The total number of bytes read from @code(Source).)
    }
    function readFrom(const Source: AStream): TStreamIOSize; override;

  // CanPrint implementation ---------------------------------------------------
    { Construct and return a string representation of the node.

      In accordance with the criteria described in
      @link(APrintingObject.toString), this method returns a string
      representation of @link(APointerListItem.reference Self.reference).  The
      format of the string is controlled by @link(plitStringRepresentation).
    }
    function toString: AnsiString; override;

  // Properties ----------------------------------------------------------------
    { Retrieve the pointer value associated with the node. }
    function reference: pointer; virtual;

    { Set the pointer value associated with the node.

      @return(The old pointer value associated with the node.)
    }
    function setReference(const newReference: pointer): pointer;
end;

// APointerList ****************************************************************
{ This class represents a list of one or more pointers.  It extends
  @link(ALinkedList) to allow pointer values to be stored within the list.  As
  with @code(ALinkedList), @code(APointerList) may be accessed as though it was
  an indexed array, or it may be used like a stack (LIFO).
}
type APointerList = class(ALinkedList)
  public
    { Construct a new pointer list that contains the specified values.

      This method calls @link(APointerList.pushSeveral) to insert the values in
      @code(references) into the list after the new instance has been
      constructed and initialized.  If @code(references) is empty, then an empty
      list is created by this routine.
    }
    constructor withReferences(const references: array of pointer); virtual;

    { Construct a new pointer list by expanding the values provided in
      @code(thisList).

      @code(thisList) must be a list of numeric values separated by
      @code(thisDelimiter).  The numeric values are assumed to be valid pointer
      references.

      If @code(thisDelimiter) is provided by the caller, then it is used to
      split the values provided in @code(thisList); otherwise, the delimiter
      specified by @link(llstDefaultDelimiter) is used.  See
      @link(APointerList.explode) for more information.

      If @code(thisList) is an empty string, then an empty list is created by
      this routine.
    }
    constructor from(const thisList: string;  const thisDelimiter: string = '');
      virtual;

    { Adds @code(thisReference) to the end of the list.

      This method constructs a new instance of @link(APointerListItem) and then
      calls @link(ALinkedList.addItem) to add the item to the list.

      @return(The index of the new item.  This index is not guaranteed to
      remain the same for this item if other items are added or removed at
      locations PRIOR to the index returned.)
    }
    function add(const thisReference: pointer): TIndexAbsolute; virtual;

    { Add several items to the end of the list.

      This method iterates through each item in @code(references) and calls
      @link(APointerList.add) to add each item to the list.

      @return(The total number of items added to the list.)
    }
    function addSeveral(const references: array of pointer): TCensus; virtual;

    { Add the items specified in @code(thisList) to the list.

      This method assumes that @code(thisList) contains one or more numeric
      values that are separated by a delimiter of some kind (e.g., a comma).  It
      uses the value of @code(delimiter) to split the string into its component
      values; these values are then added as individual items to the list.  If
      @code(delimiter) is not supplied, or if it is an empty string, then
      the value of @link(llstDefaultDelimiter) is used.  The assumption is that
      the numeric values in the list represent valid pointer values.
      @bold(USE THIS METHOD WITH CARE) -- or don't use it at all.  It is defined
      merely for the sake of completeness.

      @return(The total number of items added to the list.)
    }
    function explode(thisList: string; delimiter: string = ''): TCensus;
      override;

    { Pushes the specified pointer value onto the list, as the last item in the
      list.  Calling @link(APointerList.pop) immediately after this routine will
      return @code(thisReference).

      This method constructs a new instance of @link(APointerListItem) and then
      calls @link(ALinkedList.pushItem) to push the item onto the list.
    }
    procedure push(const thisReference: pointer); virtual;

    { Push several items onto the list.

      This method iterates through each item in @code(references) and calls
      @link(APointerList.push) to push each string value onto the list.
    }
    procedure pushSeveral(const references: array of pointer); virtual;

    { Pops the last item from the list.  This will be the reference most
      recently added to the list by a call to @link(APointerList.add) or
      @link(APointerList.push).

      If the list is empty, this routine returns @nil.
    }
    function pop: pointer; virtual;

    { Inserts the specified pointer value at the given @code(index).  If an item
      already exists at that index, it is "bumped aside" in favor of the new
      item.

      This method constructs a new instance of @link(APointerListItem) and then
      calls @link(ALinkedList.insertItemAt) to insert the item into the list.

      The first item in the list is always at index zero (0).  The last item
      in the list is always at index (@link(ALinkedList.census) @code(-1)).
      If @code(index) specifies a value that is greater than the number of items
      in the list, then @code(thisReference) is inserted at the end of the list.
    }
    procedure insertAt(const index: TIndexRelative;
      const thisReference: pointer); virtual;

    { Performs a recursive search to determine whether or not the specified
      value exists within the list.  For very large lists, this search can
      be inefficient (and can cause stack overflows, due to the recursive
      nature of the search).

      This method calls @link(APointerListItem.find) on the last item in the
      list, which causes the entire list to be searched recursively.

      @return(@true if @code(thisReference) is found in the list; @false if
        not.)
    }
    function hasReference(const thisReference: pointer): boolean; virtual;
      overload;

    { Performs a linear search to find the index of the specified value.  The
      search will begin at @code(afterIndex).

      Because this method performs a linear search, it can be inefficient on
      very large lists.

      @return(The index of the specified pointer value, if found; @code(-1)
      otherwise.  The index returned by this routine will always be an offset
      relative to the beginning of the list (i.e., a positive integer) if the
      pointer value is found.)
    }
    function indexOf(const thisReference: pointer;
      const afterIndex: TIndexRelative = 0): TIndexRelative; virtual;

     // Properties --------------------------------------------------------------
    { Retrieve the value of the item at the specified index.

      This method calls @link(ALinkedList.itemAt) to retrieve the
      @link(APointerListItem) instance at the specified index; it then returns
      the value of @link(APointerListItem.reference).

      Index zero (0) represents the first node in the list, while index
      (@link(ALinkedList.census) @code(- 1)) represents the last node in the
      list.  If @code(index) specifies a value that is greater than the number
      of nodes in the list, then this routine will remove the last item in
      the list.

      If an invalid index is passed to this routine, it will return @nil.
    }
    function item(const thisIndex: TIndexRelative): pointer; virtual;

    { Set the value of the item at the specified index.

      This method calls @link(ALinkedList.itemAt) to retrieve the
      @link(APointerListItem) instance at the specified index; it then calls
      @link(APointerListItem.setReference) on that instance.

      If @code(index) specifies an index that is greater than or equal to the
      value of @link(ALinkedList.census Self.census), then this method will
      insert a new item that contains @code(thisReference) into the list.

      @return(The previous value of the string stored at the specified index,
      if any.  @nil will be returned if an invalid index is passed to this
      routine.)
    }
    function setItem(const thisIndex: TIndexRelative;
      const thisReference: pointer): pointer; virtual;
end;
{$ENDIF}
{$IFDEF ReadResourceStrings}
  { This string controls the format of the string returned by
    @link(APointerListItem.toString).  @code(SysUtils.format) is called
    recursively in order to process this string.

    The first time that @code(SysUtils.format) is called, the double-percent
    signs @code(%%) are replaced with a single percent sign, which will be used
    in the subsequent call.  The first integer placeholder is filled with a
    value that is twice the size, in bytes, of a @code(pointer); this ensures
    that when, the second call to @code(SysUtils.format) is made, the resulting
    hexadecimal value will be padded on the left with zeroes, if necessary, so
    that the representation has the appropriate number of hexadecimal digit
    places.

    The second time that @code(SysUtils.format) is called, it is fed the output
    of the previous call, and the second integer placeholder is filled with a
    hexadecimal representation of the value returned by
    @link(APointerListItem.reference) -- the memory address to which the
    reference points.

    This format of this string is designed to produce a representation that can
    be converted by @code(SysUtils.strToInt64).
  }
  plitStringRepresentation = '$%%.%dX';
{$ENDIF}
{$IFDEF ReadImplementation}
// APointerListItem ************************************************************
// Constructor
constructor APointerListItem.withReference(const thisReference: pointer);

begin
  inherited new;

  myReference := thisReference;
end;

// Initializer
function APointerListItem.init: boolean;

begin
  result := inherited init;

  if result then
  begin
    myReference := nil;
  end;
end;

// Find the specified pointer value
function APointerListItem.Find(const thisReference: pointer): APointerListItem;

begin
  Result := nil;

  // Check ourselves first
  if myReference = thisReference then
  begin
    Result := Self;
    exit;
  end;

  // Query the older siblings next
  if (MyPrevious <> nil) and (MyPrevious is APointerListItem) then
    Result := APointerListItem(MyPrevious).Find(thisReference);

  if Result <> nil then
    exit;

  // If the item is still not found, then check children
  if (MyChild <> nil) and (MyChild is APointerListItem) then
    Result := APointerListItem(MyChild).Find(thisReference);

  if Result <> nil then
    exit;

  // Finally, search younger siblings
  if (MyNext <> nil) and (MyNext is APointerListItem) then
    Result := APointerListItem(MyNext).Find(thisReference);
end;

// Construct a shallow copy from another item
function APointerListItem.shallowCopyFrom(const Other: AnObject): boolean;

begin
  // Call the inherited routine first
  result := inherited shallowCopyFrom(Other);
  if result = false then
    exit;

  // Copy the pointer reference
  result := false;

  if (Other <> nil) and (Other is APointerListItem) then
  begin
    myReference := APointerListItem(Other).reference;
    result := true;
  end;
end;

// Determine the streaming length of the item
function APointerListItem.selfStreamingLength: TStreamIOSize;

begin
  // Call the inherited routine as well
  result := inherited SelfStreamingLength + sizeof(myReference);
end;

// Write the item to the specified stream
function APointerListItem.writeSelfTo(const Dest: AStream): TStreamIOSize;

begin
  result := 0;
  if Dest = nil then
    exit;

  // Write the value of our pointer reference
  inc(result, Dest.write(sizeof(myReference), myReference));
  { Call the inherited routine (see ANode.writeSelfTo for why we do it in this
    order)
  }
  inc(result, inherited writeSelfTo(Dest));
end;

// CanStream implementation ----------------------------------------------------
// Read the item from the specified stream
function APointerListItem.readFrom(const Source: AStream): TStreamIOSize;

begin
  result := 0;
  if Source = nil then
    exit;

  // Read the value of our pointer reference
  inc(result, Source.read(sizeof(myReference), myReference));
  { Call the inherited routine (see ANode.readFrom for why we do it in this
    order)
  }
  inc(result, inherited readFrom(Source));
end;

// CanPrint implementation -----------------------------------------------------
// Construct a string representation of the item
function APointerListItem.toString: AnsiString;

begin
  result := SysUtils.format(SysUtils.format(plitStringRepresentation, [
      sizeof(pointer) * 2
    ]), [
    ptrUint(myReference)
  ]);
end;

// Properties ------------------------------------------------------------------
// Retrieve the pointer value stored in the item
function APointerListItem.reference: pointer;

begin
  result := myReference;
end;

// Set the pointer value stored by the item
function APointerListItem.setReference(const newReference: pointer): pointer;

begin
  result := myReference;
  myReference := newReference;
end;

// APointerList ****************************************************************
// Constructor
constructor APointerList.withReferences(const references: array of pointer);

begin
  inherited new;

  Self.addSeveral(references);
end;

// Constructor
constructor APointerList.from(const thisList: string;
  const thisDelimiter: string = '');

begin
  inherited new;

  Self.explode(thisList, thisDelimiter);
end;

// Add the specified reference to the list
function APointerList.add(const thisReference: pointer): TIndexAbsolute;

var
  // Refers to the new entry
  NewEntry: APointerListItem;

begin
  // Construct a new item
  NewEntry := APointerListItem.withReference(thisReference);

  // Add the item
  result := Self.addItem(NewEntry);
end;

// Add the specified references to the list
function APointerList.addSeveral(const references: array of pointer): TCensus;

var
  // Loop counter
  i: longword;

begin
  result := System.length(references);

  if result = 0 then
    exit;

  // Loop through the references
  for i := 0 to (result - 1) do
    Self.add(references[i]);
end;

// Explode a delimited list of pointer references
function APointerList.explode(thisList: string;
  delimiter: string = ''): TCensus;

var
  // Stores an item retrieved from the list
  thisItem: string;
  // Stores the numeric value of an address
  address: ptrInt;

begin
  result := 0;

  if thisList = '' then
    exit;

  if delimiter = '' then
    delimiter := llstDefaultDelimiter;

  // Loop through the string
  while System.length(thisList) > 0 do
  begin
    thisItem := Charstring.chomp(thisList, delimiter);

    if thisItem <> '' then
    begin
      // Try to convert the item to a numeric address
      address := 0;

    {$IFDEF CPU64}
      if SysUtils.tryStrToInt64(thisItem, address) then
    {$ELSE}
      if SysUtils.tryStrToInt(thisItem, address) then
    {$ENDIF}
      begin
        Self.add(pointer(address));
        inc(result);
      end;
    end

    else
      break;
  end;
end;

// Push the specified item onto the list
procedure APointerList.push(const thisReference: pointer);

var
  // Refers to the new entry
  NewEntry: APointerListItem;

begin
  NewEntry := APointerListItem.withReference(thisReference);

  Self.pushItem(NewEntry);
end;

// Push several items onto the list
procedure APointerList.pushSeveral(const references: array of pointer);

var
  // Loop counter
  i: longword;

begin
  if System.length(references) > 0 then
    for i := 0 to (System.length(references) - 1) do
      Self.push(references[i]);
end;

// Pop the last item from the list
function APointerList.pop: pointer;

var
  // Refers to the last entry in the list
  LastEntry: APointerListItem;

begin
  // Pop the entry
  LastEntry := APointerListItem(Self.PopItem);

  if LastEntry <> nil then
  begin
    // Return the value before freeing the entry
    result := LastEntry.reference;

    LastEntry.free;
  end;
end;

// Insert the specified item into the list at the specified index
procedure APointerList.insertAt(const index: TIndexRelative;
  const thisReference: pointer);

var
  // Refers to the new entry
  NewEntry: APointerListItem;

begin
  NewEntry := APointerListItem.withReference(thisReference);

  Self.insertItemAt(index, NewEntry);
end;

// Perform a recursive search to find the specified reference
function APointerList.hasReference(const thisReference: pointer): boolean;

begin
  result := false;

  if (MyLastItem <> nil) and (MyLastItem is APointerListItem) then
    result := APointerListItem(MyLastItem).Find(thisReference) <> nil;
end;

// Find the index of the specified reference
function APointerList.indexOf(const thisReference: pointer;
  const afterIndex: TIndexRelative = 0): TIndexRelative;

var
  // Refers to the current item
  ThisItem: APointerListItem;
  // Stores the current index
  currentIndex: longint;

begin
  result := -1;

  ThisItem := APointerListItem(Self.itemAt(afterIndex));

  currentIndex := afterIndex;

  { Ensure we only use indices relative to the beginning of the list }
  if currentIndex < 0 then
    inc(currentIndex, myCensus + 1);

  if currentIndex < 0 then
    currentIndex := 0;

  while ThisItem <> nil do
  begin
    if ThisItem.reference = thisReference then
    begin
      result := currentIndex;
      break;
    end;

    ThisItem := APointerListItem(ThisItem.Next);
    inc(currentIndex);
  end;
end;

// Properties ------------------------------------------------------------------
// Retrieve the value of the item at the specified index
function APointerList.item(const thisIndex: TIndexRelative): pointer;

var
  // Refers to the entry in question
  ThisEntry: APointerListItem;

begin
  result := nil;

  // Retrieve the entry in question
  ThisEntry := APointerListItem(Self.itemAt(thisIndex));

  if ThisEntry <> nil then
    result := ThisEntry.reference;
end;

// Set the value of the item at the specified index
function APointerList.setItem(const thisIndex: TIndexRelative;
  const thisReference: pointer): pointer;

var
  // Refers to the entry in question
  ThisEntry: APointerListItem;

begin
  result := nil;

  // Retrieve the entry in question
  ThisEntry := APointerListItem(Self.itemAt(thisIndex));

  // If the entry exists, set its value
  if ThisEntry <> nil then
    result := ThisEntry.setReference(thisReference)

  // Otherwise, insert a new item
  else begin
    ThisEntry := APointerListItem.withReference(thisReference);
    Self.addItem(ThisEntry);
  end;
end;

{$ENDIF}
