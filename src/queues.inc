{ queues.inc: First-in, first-out queues implemented as linked lists.

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
// ALinkedQueue ****************************************************************
{ This class alters the behavior of @link(ALinkedList) so that it can be used
  as a queue (first in, first out).  It does this by modifying the behavior of
  @link(ALinkedList.PopItem) so that it returns the first item in the list,
  instead of the last.  Of course, like its parent, instances of
  @link(ALinkedQueue) can also be accessed as indexed arrays, if desired.
}
type ALinkedQueue = class(ALinkedList)
  public
    { Remove the first node from the list and return it to the caller.

      This method overrides the behavior inherited from
      @link(ALinkedList.PopItem) so that it returns the first item in the list,
      instead of the last.

      If there are no items in the list, this method returns @nil.
    }
    function PopItem: ANode; override;
end;

// AStringQueue ****************************************************************
{ This class alters the behavior of @link(AStringList) so that it can be used
  as a queue (first in, first out).  It does this by modifying the behavior of
  @link(AStringList.pop) so that it returns the first string in the list,
  instead of the last.  Of course, like its parent, instances of
  @link(AStringQueue) can also be accessed as indexed arrays, if desired.
}
type AStringQueue = class(AStringList)
  public
    { Remove the first string from the list and return it to the caller.

      This method overrides the behavior inherited from
      @link(AStringList.pop) so that it returns the first string in the list,
      instead of the last.

      If there are no items in the list, this method returns an empty string.
    }
    function pop: string; override;
end;

// AVariantQueue ***************************************************************
{ This class alters the behavior of @link(AVariantList) so that it can be used
  as a queue (first in, first out).  It does this by modifying the behavior of
  @link(AVariantList.pop) so that it returns the first value in the list,
  instead of the last.  Of course, like its parent, instances of
  @link(AVariantQueue) can also be accessed as indexed arrays, if desired.
}
type AVariantQueue = class(AVariantList)
  public
    { Remove the first value from the list and return it to the caller.

      This method overrides the behavior inherited from
      @link(AVariantList.pop) so that it returns the first value in the list,
      instead of the last.

      If there are no items in the list, this method returns
      @code(Variants.null).
    }
    function pop: Variant; override;
end;

// APointerQueue ***************************************************************
{ This class alters the behavior of @link(APointerList) so that it can be used
  as a queue (first in, first out).  It does this by modifying the behavior of
  @link(APointerList.pop) so that it returns the first value in the list,
  instead of the last.  Of course, like its parent, instances of
  @link(APointerQueue) can also be accessed as indexed arrays, if desired.
}
type APointerQueue = class(APointerList)
  public
    { Remove the first reference from the list and return it to the caller.

      This method overrides the behavior inherited from
      @link(APointerList.pop) so that it returns the first reference in the
      list, instead of the last.

      If there are no items in the list, this method returns @nil.
    }
    function pop: pointer; override;
end;

// AnObjectQueue ***************************************************************
{ This class alters the behavior of @link(AnObjectList) so that it can be used
  as a queue (first in, first out).  It does this by modifying the behavior of
  @link(AnObjectList.Pop) so that it returns the first object in the list,
  instead of the last.  Of course, like its parent, instances of
  @link(AnObjectQueue) can also be accessed as indexed arrays, if desired.
}
type AnObjectQueue = class(AnObjectList)
  public
    { Remove the first object from the list and return it to the caller.

      This method overrides the behavior inherited from
      @link(AnObjectList.Pop) so that it returns the first object in the list,
      instead of the last.

      If there are no items in the list, this method returns @nil.
    }
    function Pop: TObject; override;
end;
{$ENDIF}

{$IFDEF ReadImplementation}
// ALinkedQueue ****************************************************************
// Pop the first node from the list
function ALinkedQueue.PopItem: ANode;

begin
  Result := nil;

  if MyFirstItem <> nil then
  begin
    Result := MyFirstItem;
    MyFirstItem := MyFirstItem.Next;

    // Detach the last item from the list
    Result.detach;
    dec(myCensus);
  end;

  if MyFirstItem = nil then
    MyLastItem := nil;
end;

// AStringQueue ****************************************************************
// Pop the first string from the list
function AStringQueue.pop: string;

var
  // Refers to the item retrieved from the list
  ThisItem: AStringListItem;

begin
  result := '';
  ThisItem := AStringListItem(Self.RemoveItemAt(0));

  if ThisItem <> nil then
  begin
    result := ThisItem.text;
    ThisItem.free;
  end;
end;

// AVariantQueue ***************************************************************
// Pop the first value from the list
function AVariantQueue.pop: Variant;

var
  // Refers to the item retrieved from the list
  ThisItem: AVariantListItem;

begin
  result := Variants.null;
  ThisItem := AVariantListItem(Self.RemoveItemAt(0));

  if ThisItem <> nil then
  begin
    result := ThisItem.value;
    ThisItem.free;
  end;
end;

// APointerQueue ***************************************************************
// Pop the first reference from the list
function APointerQueue.pop: pointer;

var
  // Refers to the item retrieved from the list
  ThisItem: APointerListItem;

begin
  result := nil;
  ThisItem := APointerListItem(Self.RemoveItemAt(0));

  if ThisItem <> nil then
  begin
    result := ThisItem.reference;
    ThisItem.free;
  end;
end;

// AnObjectQueue ***************************************************************
// Pop the first object from the list
function AnObjectQueue.Pop: TObject;

var
  // Refers to the item retrieved from the list
  ThisItem: AnObjectListItem;

begin
  Result := nil;
  ThisItem := AnObjectListItem(Self.RemoveItemAt(0));

  if ThisItem <> nil then
  begin
    Result := ThisItem.Reference;
    // Ignore the ownership flag
    ThisItem.setOwnsReference(false);
    ThisItem.free;
  end;
end;

{$ENDIF}
