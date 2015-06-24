{ singletons.inc - A class that can only be constructed once, that maintains its
    own reference count, and that frees itself when the reference count reaches
    zero.

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
// ASingleton ******************************************************************
{ This class represents a singleton, which is a class that can only have one
  instance throughout the lifetime of a program.  The class makes use of
  class variables to maintain a reference to the first (and only) instance of
  the class, as well as a reference count to ensure that the instance is
  freed when the reference count reaches zero.

  The class is designed so that code can simply attempt to construct a new
  instance of the class (via @link(AnObject.new ASingleton.new) or another
  mechanism).  If an instance has already been constructed, that same
  instance is returned to the caller; otherwise, a new instance is created.
  When the caller is finished with the instance, it can be "freed" (via
  @code(TObject.free)).  Only if the reference count of the instance has reached
  zero (@code(0)), will the instance actually be freed.

  In neither case does the caller need to handle construction and destruction
  of the instance in any way that differs from the way other class instances are
  created and destroyed.
}
type ASingleton = class(AReferenceCountedObject)
  protected
    // Refers to the first and only instance of the class
    class var MyInstance: ASingleton;

  public
    { Class constructor.

      This method is defined to ensure that
      @link(ASingleton.Instance) is @nil before an attempt is made to construct
      an instance of the class.  This step is necessary because FreePascal's
      runtime does not initialize the memory occupied by a class, so random
      junk data could potentially exist in the location used by the pointer,
      which would confuse our routine.

      This method is called automatically by the runtime when the program first
      runs.
    }
    class constructor create;

    { Class destructor.

      This method is defined to ensure that, in the event of programmer error
      which fails to release the final reference to the class instance, the
      instance is still freed -- preventing memory leaks.

      This method is called automatically by the runtime just prior to the end
      of the program.
    }
    class destructor destroy;

    { Allocate memory for a new instance of the class.

      This method overrides the behavior inherited from
      @code(TObject.NewInstance): it first checks to see whether
      @link(ASingleton.Instance Self.Instance) is @nil; if not, then an
      instance has already been allocated and a reference to that instance
      is returned.  If the instance has not yet been allocated, then the
      inherited routine is called.

      The reference count of the instance is incremented by this method.
    }
    class function NewInstance: TObject; override;

    // Initializer
    function init: boolean; override;

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

    // Properties --------------------------------------------------------------
    { Retrieve a reference to the only instance of the singleton.

      If an instance of the singleton has not yet been created, this method
      returns @nil.  Otherwise, it returns a reference to the singleton.

      The reference count of the instance is NOT changed by this method.
    }
    class function Instance: TObject; virtual;
end;

type ASingletonClass = class of ASingleton;
{$ENDIF}
{$IFDEF ReadImplementation}
// ASingleton ******************************************************************
// Class constructor
class constructor ASingleton.create;

begin
  // Ensure the class variables have appropriate values
  MyInstance := nil;
end;

// Class destructor
class destructor ASingleton.destroy;

begin
  // Ensure any instance of the class is destroyed
  if MyInstance <> nil then
  begin
    // Reset the reference count and force the instance to be destroyed
    MyInstance.myReferenceCount := 1;
    MyInstance.free;
  end;
end;

// Construct a new instance of the singleton
class function ASingleton.NewInstance: TObject;

begin
  // Check whether or not we already have an instance
  if MyInstance <> nil then
    // We do: increment the reference counter and return it
    MyInstance.Reference

  else
    MyInstance := ASingleton(inherited NewInstance);

  Result := MyInstance;
end;

// Free an instance of the singleton
procedure ASingleton.freeInstance;

begin
  // Decrement the reference count
  Self.adjustReferenceCount(-1);

  // Have we reached zero?
  if myReferenceCount = 0 then
  begin
    MyInstance := nil;
    inherited freeInstance;
  end;
end;

// Initializer
function ASingleton.init: boolean;

begin
  result := inherited init;

  if result then
  begin
    // Reference count always starts at one
    myReferenceCount := 1;
  end;
end;

// Properties ------------------------------------------------------------------
// Retrieve a reference to the instance without altering the reference count
class function ASingleton.Instance: TObject;

begin
  Result := Self.Instance;
end;

{$ENDIF}
