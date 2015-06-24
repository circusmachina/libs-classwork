{ objectmappings.inc: A class to map string keys to object instances.

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
// AnObjectMappingEntry ********************************************************
{ This class represents an entry in an object mapping, which is an object
  reference that is indexed by a string key.

  Unlike a typical dictionary, the item here is not associated with a
  hash lookup table; instead, it is treated as a node in a binary tree.
}
type AnObjectMappingEntry = class(AMappingEntry)
  protected
    { Stores the object to which the entry refers }
    MyReference: AnObject;
    { Indicates whether or not the entry owns its reference }
    myOwnReference: boolean;

  public
    { Construct a new entry that associates the specified name with the
      specified object reference.  The value of @code(thisName) will be hashed
      to determine the sort key of the dictionary entry when it is inserted
      into a dictionary.

      If @code(takeOwnershipOfReference) is @true, then the mapping entry will
      assume ownership of @code(ThisReference) and will free the object
      reference when the mapping entry itself is freed.  Otherwise, the caller
      is responsible for freeing @code(ThisReference) when it is no longer
      required, but NOT before freeing the instance of
      @link(AnObjectMappingEntry).

    }
    constructor named(const thisName: string; const ThisReference: AnObject;
      const takeOwnershipOfReference: boolean = true); reintroduce; virtual;

    { Initializer }
    function init: boolean; override;

    { Construct a shallow copy of the other object.

      This method overrides the behavior inherited from
      @link(AStringLeaf.shallowCopyFrom): it calls that routine, then checks to
      see whether @code(Other) is an instance of @link(AnObjectMappingEntry).
      If so, it copies the value of @link(AnObjectMappingEntry.Reference) from
      @code(Other) to @code(Self), overwriting the value in @code(Self).  If
      @code(AnObjectMappingEntry.ownsReference Self.ownsReference) is @true,
      then the reference held by @code(Self) is first freed before the new one
      is copied from @code(Other).

      Note that this method does NOT copy any sibling or child nodes and so
      cannot be used to create a full copy of any descendant of @link(ANode).
      Likewise, it does NOT copy the left or right subtrees and so cannot be
      used to create a full copy of any descendant of @link(ABinaryLeaf).
      The copy will NOT be automatically placed in the binary tree to which
      @code(Other) belongs, if any, but the caller is free to do so, so long
      as the node's sort key does not match one that already exists in the
      tree.
    }
    function shallowCopyFrom(const Other: AnObject): boolean; override;

    { Calculate the number of bytes required to stream the node, and just the
      node.

      This method builds upon the behavior inherited from
      @link(AStringLeaf.selfStreamingLength): it calls the inherited method,
      then adds the number of bytes required to stream
      @link(AnObjectMappingEntry.Reference Self.Reference), so long as
      @link(AnObjectMappingEntry.Reference Self.Reference) is an instance of
      an object which implements the @link(CanStream) interface.
    }
    function selfStreamingLength: TStreamIOSize; override;

    { Write the node, and just the node, to the specified stream.

      This method builds upon the behavior inherited from
      @link(AStringLeaf.writeSelfTo): it first calls the inherited routine,
      then writes the value of
      @link(AnObjectMappingEntry.Reference Self.Reference), so long as
      @link(AnObjectMappingEntry.Reference Self.Reference) is an instance of an
      object which implements the @link(CanStream) interface.

      @return(The total number of bytes written to @code(Dest).)
    }
    function writeSelfTo(const Dest: AStream): TStreamIOSize; override;

    // Properties --------------------------------------------------------------
    { Determine whether or not the entry owns its object reference.

      If this method returns @true, then the entry assumes ownership of the
      object to which @link(AnObjectMappingEntry.Reference) refers, and will
      free that reference when the entry itself is freed.  Otherwise, the
      mapping entry does not assume ownership of its object reference and, to
      prevent memory leaks, the caller is responsible for freeing the reference
      when it is no longer required.
    }
    function ownsReference: boolean; virtual;

    { Set whether or not the entry owns its object reference.

      If @code(flag) is @true, then the entry assumes ownership of the
      object to which @link(AnObjectMappingEntry.Reference) refers, and will
      free that reference when the entry itself is freed.  Otherwise, the
      mapping entry does not assume ownership of its object reference and, to
      prevent memory leaks, the caller is responsible for freeing the reference
      when it is no longer required.

      @return(The previous value of
        @link(AnObjectMappingEntry.ownsReference Self.ownsReference).)
    }
    function setOwnsReference(const flag: boolean): boolean; virtual;

    { Retrieve the reference held by the entry.

      The reference returned by this routine should NOT be freed by the caller
      without first ensuring that @link(AnObjectMappingEntry.ownsReference) is
      @false.
    }
    function Reference: AnObject; virtual;

    { Set the reference held by the entry.

      If @code(takeOwnershipOfReference) is @true, then the mapping entry will
      assume ownership of @code(NewReference) and will free the object reference
      when the mapping entry itself is freed.  Otherwise, the caller is
      responsible for freeing @code(NewReference) when it is no longer
      required, but NOT before freeing the instance of
      @link(AnObjectMappingEntry).

      @return(The previous reference held by the entry, if
        @link(AnObjectMappingEntry.ownsReference) is @false.  In such cases,
        the caller is responsible for freeing this reference.  Otherwise, this
        method returns @nil.)
    }
    function SetReference(const NewReference: AnObject;
      const takeOwnershipOfReference: boolean = true): AnObject; virtual;

    { Retrieve the value of the entry as a string.

      If @link(AnObjectMappingEntry.Reference Self.Reference) represents an
      instance of a class that implements the @link(CanPrint) interface, this
      method calls @link(CanPrint.toString) on the reference and returns the
      result.  Otherwise, it returns the value obtained by calling
      @link(AnObject.name) on the reference.
    }
    function valueAsString: string; override;
end;

// AnObjectMapping *************************************************************
{ This class represents an object mapping, which is a list of object references
  that are index by string keys.  The overall functionality of the mapping is
  similar to that provided by Python and other languages that implement such
  objects; however, the dictionary is implemented as a binary tree rather than a
  table of hashed objects.

  As with instances of @link(ABinaryTree), the mapping is capable of
  looking up entries and streaming those entries to and from arbitrary streams.
  The mapping also supports condensing itself into a delimited string and
  iterating over its keys and values.
}
type AnObjectMapping = class(AMapping)
  public
    { Construct a list of all references maintained by the mapping.

      This method proceeds sequentially through all entries in the mapping,
      obtaining the reference held by each.  The references are inserted into an
      instance of @link(AnObjectList) which is constructed for this purpose.

      @return(An instance of @link(AnObjectList) that contains the references
      held by all mapping entries.  This list should be freed by the caller when
      it is no longer required.  If there are no entries in the mapping, an
      empty object list is returned.)
    }
    function References: AnObjectList; virtual;

    { Add the specified key and value to the mapping.

      As with other instances of @link(ABinaryTree), @link(AnObjectMapping) does
      not allow duplicate entries; consequently, if @code(thisKey) already
      exists in the dictionary, its value will be updated with
      @code(ThisReference).  Otherwise, a new entry is created.

      If @code(takeOwnershipOfReference) is @true, the entry specified will
      assume ownership of @code(ThisReference) and free it when the entry itself
      is freed.

      @return(A reference to the mapping entry that has been created and
      inserted or, if the entry already existed, the existing entry.  The caller
      should NOT attempt to free this reference; that will be done when the
      dictionary is destroyed.)
    }
    function Add(const thisKey: string;
      const ThisReference: AnObject;
      const takeOwnershipOfReference: boolean = true): AnObjectMappingEntry;
      reintroduce; virtual;

  // Properties ----------------------------------------------------------------
    { Retrieve the reference associated with the given name.

      This method retrieves the instance of @link(AnObjectMappingEntry) that has
      the specified @code(key); it then returns the reference obtained by
      calling @link(AnObjectMappingEntry.Reference) on that entry.

      If no entry that matches @code(key) is found in the dictionary, this
      routine returns the value of @code(DefaultReference).
    }
    function ReferenceNamed(const key: string;
      const DefaultReference: AnObject = nil): AnObject; virtual;

    { Set the reference associated with the given name.

      This method retrieves the instance of @link(AnObjectMappingEntry) that has
      the specified @code(key); it then sets the value by calling
      @link(AnObjectMappingEntry.SetReference) on that entry.

      If no entry that matches @code(key) is found in the dictionary, then this
      method will add a new entry.

      If @code(takeOwnershipOfReference) is @true, then the mapping entry will
      assume ownership of @code(NewReference) and will free the object reference
      when the mapping entry itself is freed.  Otherwise, the caller is
      responsible for freeing @code(NewReference) when it is no longer
      required, but NOT before freeing the instance of
      @link(AnObjectMappingEntry).

      @return(The previous reference maintained by the named entry, if the
        @link(AnObjectMappingEntry.ownsReference) is @true.  In such cases, the
        caller is responsible for freeing the reference returned.  Otherwise,
        this method returns @nil.)
    }
    function SetReferenceNamed(const key: string;
      const NewReference: AnObject;
      const takeOwnershipOfReference: boolean = true): AnObject; virtual;

    { Retrieve the reference at the specified index in the dictionary.

      Internally, this method calls @link(ABinaryTree.LeafAtIndex) on itself; if
      that method returns a node, then @link(AnObjectMappingEntry.Reference) is
      called on it.

      @return(The reference maintained by the node at the specified
        @code(index), if found; @nil otherwise.)
    }
    function ReferenceAt(const thisIndex: TIndexAbsolute): AnObject; virtual;

    { Set the reference maintained by the entry at the specified index.

      Internally, this method calls @link(ABinaryTree.LeafAtIndex) on itself; if
      that method returns a node, then @link(AnObjectMappingEntry.SetReference)
      is called on it.

      If @code(takeOwnershipOfReference) is @true, then the mapping entry will
      assume ownership of @code(NewReference) and will free the object reference
      when the mapping entry itself is freed.  Otherwise, the caller is
      responsible for freeing @code(NewReference) when it is no longer
      required, but NOT before freeing the instance of
      @link(AnObjectMappingEntry).

      @return(The previous reference maintained by the named entry, if the
        @link(AnObjectMappingEntry.ownsReference) is @true.  In such cases, the
        caller is responsible for freeing the reference returned.  Otherwise,
        this method returns @nil.)
    }
    function SetReferenceAt(const ThisIndex: TIndexAbsolute;
      const NewReference: AnObject;
      const takeOwnershipOfReference: boolean = true): AnObject; virtual;
end;
{$ENDIF}
{$IFDEF ReadImplementation}
// AnObjectMappingEntry ********************************************************
{ Construct and initialize a new mapping entry using the given name and object
  reference.
}
constructor AnObjectMappingEntry.named(const thisName: string;
  const ThisReference: AnObject;
  const takeOwnershipOfReference: boolean = true);

begin
  inherited named(thisName);
  MyReference := ThisReference;
  myOwnReference := takeOwnershipOfReference;
end;

// Initializer
function AnObjectMappingEntry.init: boolean;

begin
  result := inherited init;

  if result then
  begin
    MyReference := nil;
    myOwnReference := true;
  end;
end;

// Construct a shallow copy of the other object.
function AnObjectMappingEntry.shallowCopyFrom(const Other: AnObject): boolean;

begin
  // Call the inherited routine first
  result := inherited shallowCopyFrom(Other);

  // Check to see whether the other object is compatible with us
  if not(Other is AnObjectMappingEntry) then
    result := false;

  if result then
  begin
    // Free the exiting reference, if needed
    if (MyReference <> nil) and (myOwnReference) then
      SysUtils.freeAndNil(MyReference);

    MyReference := AnObjectMappingEntry(Other).Reference;
  end;
end;

// Calculate the number of bytes required to stream the node
function AnObjectMappingEntry.selfStreamingLength: TStreamIOSize;

begin
  // Call the inherited routine
  result := inherited selfStreamingLength;

  // Include ourselves if we can
  if (MyReference <> nil) and (MyReference is CanStream) then
    with MyReference as CanStream do
      inc(result, streamingLength);
end;

// Write the node, and just the node, to the specified stream
function AnObjectMappingEntry.writeSelfTo(const Dest: AStream): TStreamIOSize;

begin
  result := 0;

  if Dest <> nil then
  begin
    result := inherited writeSelfTo(Dest);

    // Write ourselves if we can
    if (MyReference <> nil) and (MyReference is CanStream) then
      with MyReference as CanStream do
        inc(result, writeTo(Dest));
  end;
end;

// Properties ------------------------------------------------------------------
// Determine whether or not the entry owns its reference
function AnObjectMappingEntry.ownsReference: boolean;

begin
  result := myOwnReference;
end;

// Set whether or not the entry owns its reference
function AnObjectMappingEntry.setOwnsReference(const flag: boolean): boolean;

begin
  result := myOwnReference;
  myOwnReference := flag;
end;

// Retrieve the reference maintained by the entry
function AnObjectMappingEntry.Reference: AnObject;

begin
  Result := MyReference;
end;

// Set the value of the entry
function AnObjectMappingEntry.SetReference(const NewReference: AnObject;
  const takeOwnershipOfReference: boolean = true): AnObject;

begin
  // Free the previous reference, if needed
  if (MyReference <> nil) and (myOwnReference) then
    SysUtils.freeAndNil(MyReference);

  Result := MyReference;
  MyReference := NewReference;
  myOwnReference := takeOwnershipOfReference;
end;

// Retrieve the value of the entry as a string
function AnObjectMappingEntry.valueAsString: string;

begin
  result := '';

  if MyReference <> nil then
  begin
    if MyReference is CanPrint then
      with MyReference as CanPrint do
        result := toString

    else
      result := MyReference.name;
  end;
end;

// AnObjectMapping *************************************************************
// Retrieve a list of references in the mapping
function AnObjectMapping.References: AnObjectList;

var
  // Refers to an entry from the mapping
  ThisEntry: AnObjectMappingEntry;

begin
  Result := AnObjectList.new;

  ThisEntry := AnObjectMappingEntry(MyRoot);

  while ThisEntry <> nil do
  begin
    Result.push(ThisEntry.Reference);

    ThisEntry := AnObjectMappingEntry(ThisEntry.Next);
  end;
end;

// Create a new entry with the specified key and reference
function AnObjectMapping.Add(const thisKey: string;
  const ThisReference: AnObject;
  const takeOwnershipOfReference: boolean = true): AnObjectMappingEntry;

var
  // Refers to the new entry
  NewEntry: AnObjectMappingEntry;

begin
  NewEntry := AnObjectMappingEntry.named(thisKey, ThisReference,
    takeOwnershipOfReference);

  if NewEntry <> nil then
  begin
    Result := AnObjectMappingEntry(Self.InsertLeaf(NewEntry));

    if Result <> NewEntry then
    begin
      Result.SetReference(ThisReference, takeOwnershipOfReference);
      NewEntry.free;
    end;
  end;
end;

// Properties ------------------------------------------------------------------
// Retrieve the named reference
function AnObjectMapping.ReferenceNamed(const key: string;
  const DefaultReference: AnObject = nil): AnObject;

var
  // Refers to the named item
  ThisEntry: AnObjectMappingEntry;

begin
  Result := DefaultReference;

  ThisEntry := AnObjectMappingEntry(Self.FetchLeaf(key));

  if ThisEntry <> nil then
    Result := ThisEntry.Reference;
end;

// Set the named reference
function AnObjectMapping.SetReferenceNamed(const key: string;
  const NewReference: AnObject;
  const takeOwnershipOfReference: boolean = true): AnObject;

var
  // Refers to the named item
  ThisEntry: AnObjectMappingEntry;

begin
  Result := nil;

  ThisEntry := AnObjectMappingEntry(Self.FetchLeaf(key));
  if ThisEntry <> nil then
    Result := ThisEntry.SetReference(NewReference, takeOwnershipOfReference)

  else
    Self.Add(key, NewReference, takeOwnershipOfReference);
end;

// Retrieve the reference at the specified index
function AnObjectMapping.ReferenceAt(const thisIndex: TIndexAbsolute): AnObject;

var
  // Used to loop through the dictionary
  ThisItem: AnObjectMappingEntry;

begin
  Result := nil;

  ThisItem := AnObjectMappingEntry(Self.LeafAtIndex(thisIndex));

  if ThisItem <> nil then
    Result := ThisItem.Reference;
end;

// Set the reference at the specified index
function AnObjectMapping.SetReferenceAt(const thisIndex: TIndexAbsolute;
  const NewReference: AnObject;
  const takeOwnershipOfReference: boolean = true): AnObject;

var
  // Used to loop through the dictionary
  ThisItem: AnObjectMappingEntry;

begin
  Result := nil;

  ThisItem := AnObjectMappingEntry(Self.LeafAtIndex(thisIndex));

  if ThisItem <> nil then
    Result := ThisItem.SetReference(NewReference, takeOwnershipOfReference);
end;
{$ENDIF}

