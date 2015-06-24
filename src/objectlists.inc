{ objectlists.inc - Pointer lists organized as a linked list that can be
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
{ This class represents a single entry in @link(AnObjectList).  It extends
  @link(ANode) to allow it to hold an instance of @code(TObject) or
  one of its descendants.  It also provides a flag that allows the list entry
  to free the object to which it refers when the entry itself is freed.
}
type AnObjectListItem = class(ANode)
  protected
    // Refers to the object referenced by the item
    MyReference: TObject;
    // Indicates whether or not the item owns its reference
    myOwnReference: boolean;

  public
    { Construct a new list item that refers to the specified object.

      If @code(takeOwnershipOfObject) is @true, then the list item will assume
      ownership of @code(ThisReference) and, when the list item is freed, it
      will call @code(TObject.free) on @code(ThisReference).
    }
    constructor withReference(const ThisReference: TObject;
      const takeOwnershipOfReference: boolean = false); virtual;

    // Initializer
    function init: boolean; override;

    { Destroy the list item.

      If @link(AnObjectListItem.ownsReference Self.ownsReference) is @true and
      @link(AnObjectListItem.Reference Self.Reference) refers to a valid
      instance of @code(TObject), then this method will free that reference
      before calling the inherited routine.
    }
    destructor destroy; override;

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
    function Find(const ThisReference: TObject): AnObjectListItem; virtual;

    { Calculate the number of bytes required to stream the node, and just the
      node.

      This method builds upon the behavior inherited from
      @link(ANode.selfStreamingLength): it calls the inherited routine, then
      checks to see whether @link(AnObjectListItem.Reference Self.Reference)
      is valid (not @nil) and whether it implements @link(CanStream).  If both
      are true, this method calls
      @link(CanStream.streamingLength Self.Reference.streamingLength) and adds
      the result of that call to the total returned.
    }
    function selfStreamingLength: TStreamIOSize; override;

    { Write the node, and just the node, to the specified stream.  For more
      information on this method, see @link(ANode.writeSelfTo).

      This method checks to see whether
      @link(AnObjectListItem.Reference Self.Reference) is valid (not @nil) and
      whether it implements @link(CanStream).  If both are true, then it calls
      @link(CanStream.writeTo Self.Reference.writeTo).

      The inherited routine is called in any case.

      @return(The total number of bytes written to @code(Dest).)
    }
    function writeSelfTo(const Dest: AStream): TStreamIOSize; override;

    { Construct a shallow copy of the other object.

      This method overrides the behavior inherited from
      @link(AnObject.shallowCopyFrom): it calls that method, then checks to see
      whether @code(Other) is an instance of @link(AnObjectListItem).  If so,
      it copies the value of @link(AnObjectListItem.ownsReference) from
      @code(Other) to @code(Self), overwriting the value in @code(Self).

      Note that this method does NOT copy any sibling or child nodes and so
      cannot be used to create a full copy of any descendant of @link(ANode).
      The copy will NOT be automatically placed in the list to which
      @code(Other) belongs, if any, but the caller is free to do so.
    }
    function shallowCopyFrom(const Other: AnObject): boolean; override;

    // CanPrint implementation -------------------------------------------------
    { Construct and return a string representation of the node.

      If @code(AnObjectListItem.Reference Self.Reference) is not @nil, and the
      object indicated by the reference implements @link(CanPrint), then this
      method will call @link(CanPrint.toString Self.Reference.toString) and
      return the result.  Otherwise, it returns a string representation of
      @link(AnObjectListItem.Reference Self.Reference), using
      @link(plitStringRepresentation) to control the format of the string.
    }
    function toString: AnsiString; override;

    // Properties --------------------------------------------------------------
    { Retrieve the object reference associated with the node. }
    function Reference: TObject; virtual; overload;

    { Set the object reference associated with the node.

      @return(The old reference associated with the node.)
    }
    function SetReference(const NewReference: TObject): TObject; virtual;
      overload;

    { Determine whether or not the list item assumes ownership of its reference.

      If this method returns @true, then the list item will call
      @code(TObject.free) to free its reference when the item itself is
      destroyed.  This behavior can be changed by calling
      @link(AnObjectListItem.setOwnsReference).
    }
    function ownsReference: boolean; virtual;

    { Set whether or not the list item assumes ownership of its reference.

      If @code(flag) is @true, then the list item will call
      @code(TObject.free) to free its reference when the item itself is
      destroyed.  Otherwise, the object will not be freed when the list item
      is freed.

      @return(The previous value of @link(AnObjectListItem.ownsReference).)
    }
    function setOwnsReference(const flag: boolean): boolean; virtual;
end;

// AnObjectList ****************************************************************
{ This class represents a list of one or more objects.  It extends
  @link(ALinkedList) to allow instances of @code(TObject) and its descendants to
  be stored within the list.  As with @code(ALinkedList), @code(AnObjectList)
  may be accessed as though it was an indexed array, or it may be used like a
  stack (LIFO).
}
type AnObjectList = class(ALinkedList)
  protected
    // Indicate whether or not the list owns its objects
    myOwnReferences: boolean;

  public
    { Construct a new pointer list that contains the specified values.

      This method calls @link(AnObjectList.pushSeveral) to insert the values in
      @code(References) into the list after the new instance has been
      constructed and initialized.  If @code(References) is empty, then an empty
      list is created by this routine.

      If @code(takeOwnershipOfReferences) is @true, then the list will assume
      ownership of the objects in @code(References) and they will be freed when
      the list itself is freed.  The value of @code(takeOwnershipOfReferences)
      is assigned to @link(AnObjectList.ownsReferences Self.ownsReferences).
    }
    constructor withReferences(const References: array of TObject;
      const takeOwnershipOfReferences: boolean = false); virtual;

    { Construct a new object list by expanding the values provided in
      @code(thisList).

      @code(thisList) must be a list of numeric values separated by
      @code(thisDelimiter).  The numeric values are assumed to be valid pointer
      references.

      If @code(thisDelimiter) is provided by the caller, then it is used to
      split the values provided in @code(thisList); otherwise, the delimiter
      specified by @link(llstDefaultDelimiter) is used.  See
      @link(AnObjectList.explode) for more information.

      If @code(thisList) is an empty string, then an empty list is created by
      this routine.

      If @code(takeOwnershipOfReferences) is @true, then the list will assume
      ownership of the objects in @code(thisList) and they will be freed when
      the list itself is freed.  The value of @code(takeOwnershipOfReferences)
      is assigned to @link(AnObjectList.ownsReferences Self.ownsReferences).
    }
    constructor from(const thisList: string;  const thisDelimiter: string = '';
      const takeOwnershipOfReferences: boolean = false); virtual;

    // Initializer
    function init: boolean; override;

    { Adds @code(ThisReference) to the end of the list.

      This method constructs a new instance of @link(AnObjectListItem) and then
      calls @link(ALinkedList.addItem) to add the item to the list.

      @return(The index of the new item.  This index is not guaranteed to
      remain the same for this item if other items are added or removed at
      locations PRIOR to the index returned.)
    }
    function add(const ThisReference: TObject): TIndexAbsolute; virtual;

    { Add several items to the end of the list.

      This method iterates through each item in @code(References) and calls
      @link(AnObjectList.add) to add each item to the list.

      @return(The total number of items added to the list.)
    }
    function addSeveral(const References: array of TObject): TCensus; virtual;

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
      list.  Calling @link(AnObjectList.pop) immediately after this routine will
      return @code(ThisReference).

      This method constructs a new instance of @link(AnObjectListItem) and then
      calls @link(ALinkedList.pushItem) to push the item onto the list.
    }
    procedure push(const ThisReference: TObject); virtual;

    { Push several items onto the list.

      This method iterates through each item in @code(References) and calls
      @link(AnObjectList.push) to push each string value onto the list.
    }
    procedure pushSeveral(const References: array of TObject); virtual;

    { Pops the last item from the list.  This will be the reference most
      recently added to the list by a call to @link(AnObjectList.add) or
      @link(AnObjectList.push).

      If the list is empty, this routine returns @nil.
    }
    function Pop: TObject; virtual;

    { Inserts the specified object reference at the given @code(index).  If an
      item already exists at that index, it is "bumped aside" in favor of the
      new item.

      This method constructs a new instance of @link(AnObjectListItem) and then
      calls @link(ALinkedList.insertItemAt) to insert the item into the list.

      The first item in the list is always at index zero (0).  The last item
      in the list is always at index (@link(ALinkedList.census) @code(-1)).
      If @code(index) specifies a value that is greater than the number of items
      in the list, then @code(ThisReference) is inserted at the end of the list.
    }
    procedure insertAt(const index: TIndexRelative;
      const ThisReference: TObject); virtual;

    { Performs a recursive search to determine whether or not the specified
      object exists within the list.  For very large lists, this search can
      be inefficient (and can cause stack overflows, due to the recursive
      nature of the search).

      This method calls @link(AnObjectListItem.find) on the last item in the
      list, which causes the entire list to be searched recursively.

      @return(@true if @code(ThisReference) is found in the list; @false if
        not.)
    }
    function hasReference(const ThisReference: TObject): boolean; virtual;

    { Performs a linear search to find the index of the specified object.  The
      search will begin at @code(afterIndex).

      Because this method performs a linear search, it can be inefficient on
      very large lists.

      @return(The index of the specified object reference, if found; @code(-1)
      otherwise.  The index returned by this routine will always be an offset
      relative to the beginning of the list (i.e., a positive integer) if the
      pointer value is found.)
    }
    function indexOf(const ThisReference: TObject;
      const afterIndex: TIndexRelative = 0): TIndexRelative; virtual;

    // Properties --------------------------------------------------------------
    { Retrieve the value of the item at the specified index.

      This method calls @link(ALinkedList.itemAt) to retrieve the
      @link(AnObjectListItem) instance at the specified index; it then returns
      the value of @link(AnObjectListItem.Reference).

      Index zero (0) represents the first node in the list, while index
      (@link(ALinkedList.census) @code(- 1)) represents the last node in the
      list.  If @code(index) specifies a value that is greater than the number
      of nodes in the list, then this routine will remove the last item in
      the list.

      If an invalid index is passed to this routine, it will return @nil.
    }
    function Item(const thisIndex: TIndexRelative): TObject; virtual;

    { Set the value of the item at the specified index.

      This method calls @link(ALinkedList.itemAt) to retrieve the
      @link(AnObjectListItem) instance at the specified index; it then calls
      @link(AnObjectListItem.SetReference) on that instance.

      If @code(index) specifies an index that is greater than or equal to the
      value of @link(ALinkedList.census Self.census), then this method will
      insert a new item that contains @code(ThisReference) into the list.

      @return(The previous value of the string stored at the specified index,
      if any.  @nil will be returned if an invalid index is passed to this
      routine.)
    }
    function SetItem(const thisIndex: TIndexRelative;
      const ThisReference: TObject): TObject; virtual;

    { Determine whether or not the list assumes ownership of the objects that
      it contains.

      If this function returns @true, then the list will free all of the objects
      that it contains when the list itself is destroyed.  To change this
      behavior, call @link(AnObjectList.setOwnsReferences).
    }
    function ownsReferences: boolean; virtual;

    { Set whether or not the list assumes ownership of the objects that it
      contains.

      If @code(flag) is @true, then the list will free all of the objects that
      it contains when the list itself is destroyed.  Otherwise, they are not
      freed.

      @return(The previous value of @link(AnObjectList.ownsReferences).)
    }
    function setOwnsReferences(const flag: boolean): boolean; virtual;
end;
{$ENDIF}
{$IFDEF ReadImplementation}
// AnObjectListItem ************************************************************
// Constructor
constructor AnObjectListItem.withReference(const ThisReference: TObject;
  const takeOwnershipOfReference: boolean = false);

begin
  inherited new;

  MyReference := ThisReference;
  myOwnReference := takeOwnershipOfReference;
end;

// Initializer
function AnObjectListItem.init: boolean;

begin
  result := inherited init;

  if result then
  begin
    MyReference := nil;
    myOwnReference := false;
  end;
end;

// Destructor
destructor AnObjectListItem.destroy;

begin
  // Free our reference, if indicated
  if (myOwnReference) and (MyReference <> nil) then
    SysUtils.freeAndNil(MyReference);

  inherited destroy;
end;

// Find the specified object reference
function AnObjectListItem.Find(const ThisReference: TObject): AnObjectListItem;

begin
  Result := nil;

  // Check ourselves first
  if MyReference = ThisReference then
  begin
    Result := Self;
    exit;
  end;

  // Query the older siblings next
  if (MyPrevious <> nil) and (MyPrevious is AnObjectListItem) then
    Result := AnObjectListItem(MyPrevious).Find(ThisReference);

  if Result <> nil then
    exit;

  // If the item is still not found, then check children
  if (MyChild <> nil) and (MyChild is AnObjectListItem) then
    Result := AnObjectListItem(MyChild).Find(ThisReference);

  if Result <> nil then
    exit;

  // Finally, search younger siblings
  if (MyNext <> nil) and (MyNext is AnObjectListItem) then
    Result := AnObjectListItem(MyNext).find(ThisReference);
end;

// Construct a shallow copy from another item
function AnObjectListItem.shallowCopyFrom(const Other: AnObject): boolean;

begin
  // Call the inherited routine first
  result := inherited shallowCopyFrom(Other);
  if result = false then
    exit;

  // Copy the pointer reference
  result := false;

  if (Other <> nil) and (Other is AnObjectListItem) then
  begin
    MyReference := AnObjectListItem(Other).Reference;
    result := true;
  end;
end;

// Determine the streaming length of the item
function AnObjectListItem.selfStreamingLength: TStreamIOSize;

begin
  // Call the inherited routine
  result := inherited SelfStreamingLength;

  // See if the object we reference implments CanStream
  if (MyReference <> nil) and (MyReference is AnObject) then
    if AnObject(MyReference).implements('CanStream') then
      with MyReference as CanStream do
        inc(result, streamingLength);
end;

// Write the item to the specified stream
function AnObjectListItem.writeSelfTo(const Dest: AStream): TStreamIOSize;

begin
  result := 0;
  if Dest = nil then
    exit;

  // See if the object we reference implements CanStream
  if (MyReference <> nil) and (MyReference is AnObject) then
    if AnObject(MyReference).implements('CanStream') then
      with MyReference as CanStream do
        inc(result, writeTo(Dest));

  { Call the inherited routine (see ANode.writeSelfTo for why we do it in this
    order)
  }
  inc(result, inherited writeSelfTo(Dest));
end;

// CanPrint implementation -----------------------------------------------------
// Construct a string representation of the item
function AnObjectListItem.toString: AnsiString;

begin
  // See if the object we reference implements CanPrint
  if (MyReference <> nil) and (MyReference is AnObject) then
    if AnObject(MyReference).implements('CanPrint') then
      with MyReference as CanPrint do
      begin
        result := toString;
        exit;
      end;

  // Otherwise, return a string representation of the object reference
  result := SysUtils.format(SysUtils.format(plitStringRepresentation, [
      sizeof(pointer) * 2
    ]), [
    ptrUint(MyReference)
  ]);
end;

// Properties ------------------------------------------------------------------
// Retrieve the object reference used by the item
function AnObjectListItem.Reference: TObject;

begin
  result := MyReference;
end;

// Set the object reference used by the item
function AnObjectListItem.SetReference(const NewReference: TObject): TObject;

begin
  Result := MyReference;
  MyReference := NewReference;
end;

// Retrieve whether or not the item owns its reference
function AnObjectListItem.ownsReference: boolean;

begin
  result := myOwnReference;
end;

// Set whether or not the item owns its reference
function AnObjectListItem.setOwnsReference(const flag: boolean): boolean;

begin
  result := myOwnReference;
  myOwnReference := flag;
end;

// AnObjectList ****************************************************************
// Constructor
constructor AnObjectList.withReferences(const References: array of TObject;
  const takeOwnershipOfReferences: boolean = false);

begin
  inherited new;

  myOwnReferences := takeOwnershipOfReferences;
  Self.addSeveral(References);
end;

// Constructor
constructor AnObjectList.from(const thisList: string;
  const thisDelimiter: string = '';
  const takeOwnershipOfReferences: boolean = false);

begin
  inherited new;

  myOwnReferences := takeOwnershipOfReferences;
  Self.explode(thisList, thisDelimiter);
end;

// Initializer
function AnObjectList.init: boolean;

begin
  result := inherited init;

  if result then
  begin
    myOwnReferences := false;
  end;
end;

// Add the specified reference to the list
function AnObjectList.add(const ThisReference: TObject): TIndexAbsolute;

var
  // Refers to the new entry
  NewEntry: AnObjectListItem;

begin
  // Construct a new item
  NewEntry := AnObjectListItem.withReference(ThisReference, myOwnReferences);

  // Add the item
  result := Self.addItem(NewEntry);
end;

// Add the specified references to the list
function AnObjectList.addSeveral(const References: array of TObject): TCensus;

var
  // Loop counter
  i: longword;

begin
  result := System.length(References);

  if result = 0 then
    exit;

  // Loop through the references
  for i := 0 to (result - 1) do
    Self.add(References[i]);
end;

// Explode a delimited list of pointer references
function AnObjectList.explode(thisList: string;
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
        Self.add(TObject(pointer(address)));
        inc(result);
      end;
    end

    else
      break;
  end;
end;

// Push the specified item onto the list
procedure AnObjectList.push(const ThisReference: TObject);

var
  // Refers to the new entry
  NewEntry: AnObjectListItem;

begin
  NewEntry := AnObjectListItem.withReference(ThisReference, myOwnReferences);

  Self.pushItem(NewEntry);
end;

// Push several items onto the list
procedure AnObjectList.pushSeveral(const References: array of TObject);

var
  // Loop counter
  i: longword;

begin
  if System.length(References) > 0 then
    for i := 0 to (System.length(References) - 1) do
      Self.push(References[i]);
end;

// Pop the last item from the list
function AnObjectList.Pop: TObject;

var
  // Refers to the last entry in the list
  LastEntry: AnObjectListItem;

begin
  // Pop the entry
  LastEntry := AnObjectListItem(Self.PopItem);

  if LastEntry <> nil then
  begin
    // Return the value before freeing the entry
    result := LastEntry.Reference;

    { Ignore the 'ownsReferences' value here or the return value will be
      useless
    }
    LastEntry.setOwnsReference(false);
    LastEntry.free;
  end;
end;

// Insert the specified item into the list at the specified index
procedure AnObjectList.insertAt(const index: TIndexRelative;
  const ThisReference: TObject);

var
  // Refers to the new entry
  NewEntry: AnObjectListItem;

begin
  NewEntry := AnObjectListItem.withReference(thisReference, myOwnReferences);

  Self.insertItemAt(index, NewEntry);
end;

// Perform a recursive search to find the specified reference
function AnObjectList.hasReference(const ThisReference: TObject): boolean;

begin
  result := false;

  if (MyLastItem <> nil) and (MyLastItem is AnObjectListItem) then
    result := AnObjectListItem(MyLastItem).Find(ThisReference) <> nil;
end;

// Find the index of the specified reference
function AnObjectList.indexOf(const ThisReference: TObject;
  const afterIndex: TIndexRelative = 0): TIndexRelative;

var
  // Refers to the current item
  ThisItem: AnObjectListItem;
  // Stores the current index
  currentIndex: longint;

begin
  result := -1;

  ThisItem := AnObjectListItem(Self.itemAt(afterIndex));

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

    ThisItem := AnObjectListItem(ThisItem.Next);
    inc(currentIndex);
  end;
end;

// Properties ------------------------------------------------------------------
// Retrieve the reference at the specified index
function AnObjectList.Item(const thisIndex: TIndexRelative): TObject;

var
  // Refers to the entry in question
  ThisEntry: AnObjectListItem;

begin
  result := nil;

  // Retrieve the entry in question
  ThisEntry := AnObjectListItem(Self.itemAt(thisIndex));

  if ThisEntry <> nil then
    result := ThisEntry.Reference;
end;

// Set the reference at the specified index
function AnObjectList.SetItem(const thisIndex: TIndexRelative;
  const ThisReference: TObject): TObject;

var
  // Refers to the entry in question
  ThisEntry: AnObjectListItem;

begin
  result := nil;

  // Retrieve the entry in question
  ThisEntry := AnObjectListItem(Self.ItemAt(thisIndex));

  // If the entry exists, set its value
  if ThisEntry <> nil then
    result := ThisEntry.SetReference(ThisReference)

  // Otherwise, insert a new item
  else begin
    ThisEntry := AnObjectListItem.withReference(ThisReference, myOwnReferences);
    Self.addItem(ThisEntry);
  end;
end;

// Determine whether or not the list owns its references
function AnObjectList.ownsReferences: boolean;

begin
  result := myOwnReferences;
end;

// Set whether or not the list owns its references
function AnObjectList.setOwnsReferences(const flag: boolean): boolean;

begin
  result := myOwnReferences;
  myOwnReferences := flag;
end;

{$ENDIF}
