{ refcntobjects.inc - Reference-counted objects.  Not used extensively by the
   author, but included for the sake of utility.

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
{ This class represents a reference-counted object; that is, an object that
  maintains a count of the number of times it has been explicitly referenced.
  When that count reaches zero, the object is automatically destroyed.

  When the object is created, its reference count is initially one
  @code(1), since the caller that created the object obtains the first
  reference to it.  The count is incremented every time
  @link(AReferenceCountedObject.Reference) is called, and decremented each
  time @code(AReferenceCountedObject.free) is called.  When the count reaches
  zero, the object is destroyed and ANY REMAINING REFERENCES BECOME INVALID.
  Ideally, there should be no remaining references at this point, but care must
  nonetheless be exercised when using reference-counted objects.  When the
  object is passed from one routine to another, the reference obtained by
  calling @link(AReferenceCountedObject.Reference) should be used.  Routines
  that receive a reference should explicitly free it (by calling
  @code(TObject.free) on it) when it is no longer required.

  The reference counting scheme used by this class is thread-safe; adjustments
  to the reference count are protected by a mutex that is local to the instance.
}
type AReferenceCountedObject = class(AnObject)
  protected
    // Refers to the critical section of code which handles reference counts
    myReferenceChange: TRTLCriticalSection;

    // Stores the reference count of the object
    myReferenceCount: longword;

    { Adjust the reference counter by the specified amount.

      This method increments or decrements
      @link(AReferenceCountedObject.referenceCount Self.referenceCount) by
      the amount specified by @code(amount), in a thread-safe manner.

      This method should be called instead of directly operating on the
      reference count of the instance.
    }
    procedure adjustReferenceCount(const amount: smallint); virtual;

    { Set the reference counter to the specified value.

      This method sets the value of
      @link(AReferenceCountedObject.referenceCount Self.referenceCount) in a
      thread-safe manner.

      This method should be called instead of directly operating on the
      reference count of the instance.

      @return(The previous value of
        @link(AReferenceCountedObject.referenceCount Self.referenceCount).)
    }
    function setReferenceCount(const newValue: longword): longword; virtual;

  public
    // Initializer
    function init: boolean; override;

    { Destructor.

      This method releases the local mutex that is used to protect adjustments
      to the reference count of the instance.
    }
    destructor destroy; override;

    { Free the instance.

      This method overrides the behavior inherited from
      @code(TObject.freeInstance): it decrements
      @link(AReferenceCountedObject.referenceCount Self.referenceCount) and then
      checks to see whether the counter has reached zero (@code(0)).  If so, it
      calls the inherited routine.

      Overriding @code(TObject.freeInstance) should prevent the instance from
      being freed even if its destructor is called directly.

      This method decrements the reference count of the instance.
    }
    procedure freeInstance; override;

    { Obtain a reference to the object.

      This method increments the reference counter associated with the object
      and then returns a reference to it.  The reference returned should be
      freed by the caller when it is no longer needed.
    }
    function Reference: AReferenceCountedObject; virtual;

    // Properties --------------------------------------------------------------
    { Retrieve the current reference count of the object. }
    function referenceCount: longword; virtual;
end;
{$ENDIF}
{$IFDEF ReadImplementation}
// AReferenceCountedObject *****************************************************
// Initializer
function AReferenceCountedObject.init: boolean;

begin
  result := inherited init;

  if result then
  begin
    { Setting this here doesn't matter because no routine besides the caller
      should yet have a reference to the instance (since `init` is called from
      inside the constructor).
    }
    myReferenceCount := 1;
    System.initCriticalSection(myReferenceChange);
  end;
end;

// Destructor
destructor AReferenceCountedObject.destroy;

begin
  // Release the mutex
  System.doneCriticalSection(myReferenceChange);
end;

// Free the instance
procedure AReferenceCountedObject.freeInstance;

begin
  // Decrement the reference count
  if myReferenceCount > 0 then
    Self.adjustReferenceCount(-1);

  // Has the reference count reached zero?
  if myReferenceCount = 0 then
    inherited freeInstance;
end;

// Adjust the reference count by the specified amount
procedure AReferenceCountedObject.adjustReferenceCount(const amount: smallint);

begin
  if amount <> 0 then
  begin
    // Lock the value
    System.enterCriticalSection(myReferenceChange);

      // Adjust the value
      myReferenceCount := myReferenceCount + amount;

    // Unlock the value
    System.leaveCriticalSection(myReferenceChange);
  end;
end;

// Set the reference count to the specified amount
function AReferenceCountedObject.setReferenceCount(const newValue: longword): longword;

begin
  result := myReferenceCount;

  if newValue <> myReferenceCount then
  begin
    // Lock the value
    System.enterCriticalSection(myReferenceChange);

      // Adjust the value
      myReferenceCount := newValue;

    // Unlock the value
    System.leaveCriticalSection(myReferenceChange);
  end;
end;

// Obtain a reference to the object
function AReferenceCountedObject.Reference: AReferenceCountedObject;

begin
  // Increment the reference count
  Self.adjustReferenceCount(1);
  // Return the reference
  Result := Self;
end;

// Properties ------------------------------------------------------------------
// Retrieve the current reference count of the object
function AReferenceCountedObject.referenceCount: longword;

begin
  result := myReferenceCount;
end;

{$ENDIF}
