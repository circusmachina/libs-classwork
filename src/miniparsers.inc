{ miniparsers.inc: A simple parser.

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
{ This enumeration defines the type of tokens that are recognized by instances
  of @link(AMiniScanner).
}
type AMiniTokenType = (
  // No token
  MT_NONE,
  // The end of the source
  MT_EOS,
  // An integer value
  MT_INTEGER,
  // A floating-point value
  MT_FLOAT,
  // A recognized piece of text or special character
  MT_RECOGNIZED,
  // An unrecognized token
  MT_UNRECOGNIZED
);

type
  // Points to an instance of @link(TMiniToken).
  PMiniToken = ^TMiniToken;

  { This type represents a token that is parsed by an instance of
    @link(AMiniParser).

    The @code(tokenType) member indicates the type of token found by the
    scanner and will correspond to one of the values in the
    @link(AMiniTokenType) enumeration: @unorderedList(
      @item(@code(MT_RECOGNIZED): Indicates that a recognized token was
        found by the scanner.  The @code(text) member will contain the literal
        text of the token as read by the scanner.)

      @item(@code(MT_INTEGER): Indicates a whole number was found by the
        scanner.  The @code(integerValue) member will contained the parsed
        value of this number.)

      @item(@code(MT_FLOAT): Indicates a fractional number was found by the
        scanner.  The @code(floatValue) member will contain the parsed value
        of this number.)

      @item(@code(MT_UNRECOGNIZED): Indicates that an unrecognized token was
        encountered by the scanner.  The @code(text) member will contain the
        literal text of the token as read by the scanner.)
      )
  }
  TMiniToken = record
    tokenType: AMiniTokenType;
    text: string;
    integerValue: int64;
    floatValue: double;
  end;

{ This class represents a miniature scanner.  It is primarily designed to scan
  text input streams for custom format specifiers, but can be used wherever a
  full-fledged scanner and tokenizer is not required.

  The scanner tokenizes by reading a single character from the source stream.
  It then compares this character with those provided in its lists of
  recognized digits, letters, and special characters.  If a match is found, it
  continues collecting characters until a different character is found.  The
  collected token is then checked to see whether it is a valid integer or
  floating-point value, and if it matches the list of recognized tokens.
}
type AMiniScanner = class(AnIterator)
  protected
    // Refers to the source
    MySource: ATextInputStream;
    // Indicates whether or not the scanner owns its source
    myOwnSource: boolean;
    // Stores a list of characters that represent digits
    myRecognizedDigits: string;
    // Stores a list of characters that represent letters
    myRecognizedLetters: string;
    // Stores a list of characters that represent special characters
    myRecognizedSpecialCharacters: string;
    // Refers to a list of recognized tokens
    MyRecognizedTokens: AStringTree;
    // Stores the  previous token read from the source
    myPreviousToken: TMiniToken;
    // Stores the most recent token read from the source
    myCurrentToken: TMiniToken;

    { Collect a numeric token from the source.

      This method is called automatically by @link(AMiniScanner.nextToken) when
      it encounters a character in the source that matches one of the characters
      in @link(AMiniScanner.recognizedDigits Self.recognizedDigits).  The
      routine collects characters until it encounters a character which is not
      a recognized digit.
    }
    procedure collectNumericToken; virtual;

    { Collect a "word".

      This method is called automatically by @link(AMiniScanner.nextToken) when
      it encounters a character in the source that matches one of the characters
      in @link(AMiniScanner.recognizedLetters Self.recognizedLetters).  The
      routine collects characters until it encounters a character which is not a
      recognized letter.
    }
    procedure collectWordToken; virtual;

    { Collect a sequence of special characters.

      This method is called automatically by @link(AMiniScanner.nextToken) when
      it encounters a character in the source that matches one of the characters
      in @link(AMiniScanner.recognizedSpecialCharacters Self.recognizedSpecialCharacters).
      The routine collects characters until it encounters a character which is
      not a recognized special character.  It then returns characters to the
      source until it finds a match in
      @link(AMiniScanner.RecognizedTokens Self.RecognizedTokens) or until there
      is only one character left.
    }
    procedure collectSpecialToken; virtual;

    { Collect an unrecognized token.

      This method is called automatically by @link(AMiniScanner.nextToken) when
      it encounters a character in the source that does not match any of those
      specified in @link(AMiniScanner.recognizedDigits Self.recognizedDigits),
      @link(AMiniScanner.recognizedLetters Self.recognizedLetters), and
      @link(AMiniScanner.recognizedSpecialCharacters Self.recognizedSpecialCharacters).
      It collects characters until it encountes one which IS defined in one of
      the known lists.
    }
    procedure collectUnrecognizedToken; virtual;

  public
    { Construct a new instance of @link(AMiniScanner) that will scan the
      specified stream for tokens.

      If @code(takeOwnershipOfTarget) is @true, then the scanner will assume
      ownership of @code(ThisTarget) and will free the stream when the scanner
      is, itself, freed.
    }
    constructor forSource(const ThisTarget: ATextInputStream;
      const takeOwnershipOfTarget: boolean = true); virtual;

    { Construct a new instance of @link(AMiniScanner) that will scan the
      specified string for tokens.

      This method will construct an instance of @link(AStringStream) for use
      as the scanner's @link(AMiniScanner.Source Source).
    }
    constructor forString(const thisString: string); virtual;

    // Initializer
    function init: boolean; override;

    // Destructor
    destructor destroy; override;

    { Retrieve the next token from the source. }
    function nextToken: PMiniToken; virtual;

    { Increment the iterator.

      This method simply calls @link(AMiniScanner.nextToken).
    }
    procedure next; override;

    { Determine whether or not there are more tokens to retrieve from the
      source.

      This method checks to see whether the end of the source stream has been
      reached and returns @false if so.  Otherwise it returns @true.
    }
    function continues: boolean; override;

    // Properties --------------------------------------------------------------
    { Retrieve a reference to the source stream being scanned.

      The caller should NOT attempt to free the reference returned by this
      stream.
    }
    function Source: ATextInputStream; virtual;

    { Retrieve whether or not the scanner assumes ownership of its source
      stream.

      If this method returns @true, then the mini-scanner instance assumes
      ownership of its source stream and will free the stream when it is,
      itself, freed.  To change this behavior, call
      @link(AMiniScanner.setOwnsSource).
    }
    function ownsSource: boolean; virtual;

    { Set whether or not the scanner assumes ownership of its source stream.

      If @code(flag) is @true, then the mini-scanner instance will assume
      ownership of its source stream and will free the stream when it is,
      itself, freed.  Otherwise, the caller will need to free
      @link(AMiniScanner.Source) when it is no longer required.

      @return(The previous value of @link(AMiniScanner.ownsSource).)
    }
    function setOwnsSource(const flag: boolean): boolean; virtual;

    { Retrieve the list of characters that are recognized as digits by the
      scanner.
    }
    function recognizedDigits: string; virtual;

    { Set the list of characters that are recognized as digits by the scanner.

      @return(The previous value of @link(AMiniScanner.recognizedDigits).)
    }
    function setRecognizedDigits(const newValue: string): string; virtual;

    { Retrieve the list of characters that are recognized as letters by the
      scanner.
    }
    function recognizedLetters: string; virtual;

    { Set the list of characters that are recognized as letters by the scanner.

      @return(The previous value of @link(AMiniScanner.recognizedLetters).)
    }
    function setRecognizedLetters(const newValue: string): string; virtual;

    { Retrieve the list of characters that are recognized as special characters
      by the scanner.
    }
    function recognizedSpecialCharacters: string; virtual;

    { Set the list of characters that are recognized as special characters by
      the scanner.

      @return(The previous value of
        @link(AMiniScanner.recognizedSpecialCharacters).)
    }
    function setRecognizedSpecialCharacters(const newValue: string): string;
      virtual;

    { Retrieve a reference to the list of recognized tokens.  Tokens in this
      list are any combination of letters, digits, and/or special characters
      that, when combined, form a token that should be recognized and acted upon
      by the parser driving the mini-scanner instance.

      The caller should NOT attempt to free the reference returned by this
      routine.
    }
    function RecognizedTokens: AStringTree; virtual;

    { Retrieve the previous token read from the source. }
    function previousToken: PMiniToken; virtual;

    { Retrieve the most recent token read from the source. }
    function currentToken: PMiniToken; virtual;
end;

// AMiniParser *****************************************************************
type
  AMiniParser = class;

  { This type represents a callback routine which is invoked by an instance of
    @link(AMiniParser) when its scanner returns a token.

    @param(Parser refers to the instance of @link(AMiniParser) which has parsed
      the token.)
    @param(token refers to the most recent token parsed by the parser.  This is
      the token that prompted the callback to be invoked.)
    @param(prevToken refers to the token parsed by the parser just prior to the
      current token.  It is provided in case the callback routine wants to act
      upon the current token only if the previous token was a specific value.)
    @param(abortParsing indicates whether or not the mini-parser instance should
      continue to parse.  It can be set to @true by the callback to abort
      further processing of the source.  Under normal circumstances, the
      callback routine will set it to @false when it has finished handling the
      given @code(token).)

    The callback should return a value indicative of the number of errors
    encountered while handling the token.  A return value of zero (@code(0))
    therefore indicates success.
  }
  AMiniParserCallback = function(const Parser: AMiniParser;
    const token: PMiniToken; const prevToken: PMiniToken;
    var abortParsing: boolean): integer of object;

  { This class represents a miniature parser.  The purpose of the class is to
    provide a quick way to parse custom formatting strings.

    The parser creates an instance of @link(AMiniScanner) which is used to
    scan the string in question.  The scanner returns tokens which the parser
    will act upon by calling one of three callback routines.  These callback
    routines can be set by the calling routine.  This behavior allows the
    caller to customize the parser's response to a token without the need to
    define a custom descendant of this class (though, of course, that is also
    possible).

    The three callback routines defined by this class are: @unorderedList(
      @item(@link(AMiniParser.recognizedTokenCallback recognizedTokenCallback),
        invoked when the scanner returns a @link(TMiniToken) instance whose
        @code(tokenType) member is @link(AMiniTokenType MT_RECOGNIZED).)

      @item(@link(AMiniParser.numericTokenCallback numericTokenCallback),
        invoked when the scanner returns a @link(TMiniToken) instance whose
        @code(tokenType) member is @link(AMiniTokenType MT_INTEGER) or
        @link(AMiniTokenType MT_FLOAT).)

      @item(@link(AMiniParser.unrecognizedTokenCallback unrecognizedTokenCallback),
        invoked when the scanner returns a @link(TMiniToken) instance whose
        @code(tokenType) member is @link(AMiniTokenType MT_UNRECOGNIZED).)
      )
  }
  AMiniParser = class(ANode)
  protected
    // Refers to the scanner used to scan for tokens
    MyScanner: AMiniScanner;
    // Refers to the callback used when a recognized token is found
    myRecognizedTokenCallback: AMiniParserCallback;
    // Refers to the callback used when a numeric token is found
    myNumericTokenCallback: AMiniParserCallback;
    // Refers to the callback used when an unrecognized token is found
    myUnrecognizedTokenCallback: AMiniParserCallback;

  public
    // Initializer
    function init: boolean; override;

    // Destructor
    destructor destroy; override;

    { Retrieve the next token from the source. }
    procedure nextToken; virtual;

    { Retrieve the previous token read from the source. }
    function previousToken: PMiniToken; virtual;

    { Retrieve the most recent token read from the source. }
    function currentToken: PMiniToken; virtual;

    { Parse the source. }
    function parse: int64; virtual;

    // Properties --------------------------------------------------------------
    { Retrieve a reference to the scanner used to obtain tokens.

      The caller should NOT attempt to free the reference returned by this
      routine.
    }
    function Scanner: AMiniScanner; virtual;

    { Retrieve a reference to the list of recognized tokens.

      This function is a pass-through function and returns the value returned
      by a call to
      @link(AMiniScanner.RecognizedTokens Self.Scanner.RecognizedTokens).

      The caller should NOT attempt to free the reference returned by this
      routine.
    }
    function RecognizedTokens: AStringTree; virtual;

    { Retrieve a reference to the recognized token callback.  This is the method
      called when a token is found that matches a value in
      @link(AMiniParser.RecognizedTokens Self.RecognizedTokens).
    }
    function recognizedTokenCallback: AMiniParserCallback; virtual;

    { Set the recognized token callback.  This is the method that will be called
      when a token is found that matches a value in
      @link(AMiniParser.RecognizedTokens Self.RecognizedTokens).

      @return(The previous value of @link(AMiniParser.recognizedTokenCallback).
        The caller can save this reference in order to "chain up" to the
        previous handler.)
    }
    function setRecognizedTokenCallback(const newCallback: AMiniParserCallback): AMiniParserCallback;
      virtual;

    { Retrieve a reference to the numeric token callback.  This is the method
      called when an integer or floating-point token is encountered.
    }
    function numericTokenCallback: AMiniParserCallback; virtual;

    { Set the numeric token callback.  This is the method that will be called
      when an integer or floating-point token is encountered.

      @return(The previous value of @link(AMiniParser.numericTokenCallback).
        The caller can save this reference in order to "chain up" to the
        previous handler.)
    }
    function setNumericTokenCallback(const newCallback: AMiniParserCallback): AMiniParserCallback;
      virtual;

    { Retrieve a reference to the unrecognized token callback.  This is the
      method that will be called when an unrecognized token is encountered.
    }
    function unrecognizedTokenCallback: AMiniParserCallback; virtual;

    { Set the unrecognized token callback.  This is the method that will be
      called when an unrecognized token is encountered.

      @return(The previous value of
        @link(AMiniParser.unrecognizedTokenCallback).  The caller can save this
        reference in order to "chain up" to the previous handler.)
    }
    function setUnrecognizedTokenCallback(const newCallback: AMiniParserCallback): AMiniParserCallback;
      virtual;
end;
{$ENDIF}
{$IFDEF ReadImplementation}
// AMiniScanner ****************************************************************
// Constructor
constructor AMiniScanner.forSource(const ThisTarget: ATextInputStream;
  const takeOwnershipOfTarget: boolean = true);

begin
  inherited new;

  MySource := ThisTarget;
  myOwnSource := takeOwnershipOfTarget;
end;

// Constructor
constructor AMiniScanner.forString(const thisString: string);

begin
  inherited new;

  if thisString <> '' then
  begin
    MySource := ATextInputStream.around(AStringStream.new(thisString));
    myOwnSource := true;
  end;
end;

// Initializer
function AMiniScanner.init: boolean;

begin
  result := inherited init;

  if result then
  begin
    MySource := nil;
    myOwnSource := false;
    myRecognizedDigits := charsDigits;
    myRecognizedLetters := charsUppercaseLetters + charsLowercaseLetters;
    myRecognizedSpecialCharacters := '';

    MyRecognizedTokens := AStringTree.new;

    MemorySpan.fill(@myPreviousToken, sizeof(myPreviousToken), 0);
    MemorySpan.fill(@myCurrentToken, sizeof(myCurrentToken), 0);

    result := MyRecognizedTokens <> nil;
  end;
end;

// Destructor
destructor AMiniScanner.destroy;

begin
  // Free the list of recognized tokens
  if MyRecognizedTokens <> nil then
    SysUtils.freeAndNil(MyRecognizedTokens);

  // Free the source stream, if indicated
  if (MySource <> nil) and (myOwnSource) then
    SysUtils.freeAndNil(MySource);

  inherited destroy;
end;

// Collect a numeric token from the source
procedure AMiniScanner.collectNumericToken;

var
  // Refers to a character read from the source
  ch: string;

begin
  ch := '';
  myCurrentToken.tokenType := MT_UNRECOGNIZED;
  myCurrentToken.text := '';
  myCurrentToken.integerValue := 0;
  myCurrentToken.floatValue := 0;

  while MySource.hasEnded = false do
  begin
    // Read a character from the stream
    ch := '';
    MySource.readCharacters(1, ch);

    // Check it
    if Charstring.positionOf(ch, myRecognizedDigits) = 0 then
      break;

    // Add the character to the token string
    myCurrentToken.text := myCurrentToken.text + ch;
  end;

  // Put back the non-digit character
  MySource.rewindBy(System.length(ch));

  // Check to see if we have a valid numeric value
  if SysUtils.tryStrToInt64(myCurrentToken.text,
    myCurrentToken.integerValue) then
    myCurrentToken.tokenType := MT_INTEGER

  else if SysUtils.tryStrToFloat(myCurrentToken.text,
    myCurrentToken.floatValue) then
    myCurrentToken.tokenType := MT_FLOAT;
end;

// Collect a word token from the source
procedure AMiniScanner.collectWordToken;

var
  // Refers to a character read from the source
  ch: string;

begin
  ch := '';
  myCurrentToken.tokenType := MT_UNRECOGNIZED;
  myCurrentToken.text := '';

  while MySource.hasEnded = false do
  begin
    // Read a character from the stream
    ch := '';
    MySource.readCharacters(1, ch);

    // Check it
    if Charstring.positionOf(ch, myRecognizedLetters) = 0 then
      break;

    // Add the character to the token string
    myCurrentToken.text := myCurrentToken.text + ch;
  end;

  // Put back the non-letter character
  MySource.rewindBy(System.length(ch));

  // Check to see if the token is recognized
  if MyRecognizedTokens.hasLeafWithKey(myCurrentToken.text) then
    myCurrentToken.tokenType := MT_RECOGNIZED;
end;

// Collect a special token from the source
procedure AMiniScanner.collectSpecialToken;

var
  // Refers to a character read from the source
  ch: string;

begin
  ch := '';

  myCurrentToken.tokenType := MT_UNRECOGNIZED;
  myCurrentToken.text := '';

  while MySource.hasEnded = false do
  begin
    // Read a character from the stream
    ch := '';
    MySource.readCharacters(1, ch);

    // Check it
    if Charstring.positionOf(ch, myRecognizedSpecialCharacters) = 0 then
      break;

    // Add the character to the token string
    myCurrentToken.text := myCurrentToken.text + ch;
  end;

  // Put back the non-special character
  MySource.rewindBy(System.length(ch));

  repeat
    // Check to see if the token is recognized
    if MyRecognizedTokens.hasLeafWithKey(myCurrentToken.text) then
    begin
      // Yes, so the token type and exit
      myCurrentToken.tokenType := MT_RECOGNIZED;
      break;
    end;

    // Strip the rightmost character and try again
    ch := Charstring.character(myCurrentToken.text,
      Charstring.census(myCurrentToken.text));
    System.setLength(myCurrentToken.text,
      System.length(myCurrentToken.text) - System.length(ch));

    // Put the character back into the source
    MySource.rewindBy(System.length(ch));
  until Charstring.census(myCurrentToken.text) = 1;

  // Check one last time on the single character
  if myCurrentToken.tokenType = MT_UNRECOGNIZED then
    if MyRecognizedTokens.hasLeafWithKey(myCurrentToken.text) then
      myCurrentToken.tokenType := MT_RECOGNIZED;
end;

// Collect an unrecognized token from the source
// Collect a word token from the source
procedure AMiniScanner.collectUnrecognizedToken;

var
  // Refers to a character read from the source
  ch: string;

begin
  ch := '';
  myCurrentToken.tokenType := MT_UNRECOGNIZED;
  myCurrentToken.text := '';

  while MySource.hasEnded = false do
  begin
    // Read a character from the stream
    ch := '';
    MySource.readCharacters(1, ch);

    // Check it
    if Charstring.positionOf(ch, myRecognizedDigits) = 0 then
      break;

    if Charstring.positionOf(ch, myRecognizedLetters) = 0 then
      break;

    if Charstring.positionOf(ch, myRecognizedSpecialCharacters) = 0 then
      break;

    // Add the character to the token string
    myCurrentToken.text := myCurrentToken.text + ch;
  end;

  // Put back the recognized character
  MySource.rewindBy(System.length(ch));
end;

// Get the next token from the source
function AMiniScanner.nextToken: PMiniToken;

var
  // Stores a character read from the stream
  ch: string;

begin
  result := @myCurrentToken;

  // Do nothing if we've reached the end of the stream and we already know it
  if (MySource.hasEnded) and (myCurrentToken.tokenType = MT_EOS) then
    exit;

  // Store the current token as the previous token
  // Empty the string to ensure no memory leaks
  myPreviousToken.text := '';
  MemorySpan.copy(@myCurrentToken, @myPreviousToken, sizeof(myCurrentToken));

  // Clear the current token
  // Empty the string to ensure no memory leaks
  myCurrentToken.text := '';
  MemorySpan.fill(@myCurrentToken, sizeof(myCurrentToken), 0);

  // If we've reached the end of the source...
  if MySource.hasEnded then
  begin
    myCurrentToken.tokenType := MT_EOS;
    exit;
  end;

  // Read a character from the source and then put it back
  ch := '';
  MySource.readCharacters(1, ch);
  MySource.rewindBy(System.length(ch));

  // Check the character type and read the token
  if Charstring.positionOf(ch, myRecognizedLetters) > 0 then
    Self.collectWordToken

  else if Charstring.positionOf(ch, myRecognizedSpecialCharacters) > 0 then
    Self.collectSpecialToken

  else if Charstring.positionOf(ch, myRecognizedDigits) > 0 then
    Self.collectNumericToken

  else
    Self.collectUnrecognizedToken;
end;

// Increment the iterator
procedure AMiniScanner.next;

begin
  Self.nextToken;
end;

// Determine whether or not there are more tokens to retrieve
function AMiniScanner.continues: boolean;

begin
  result := (MySource.hasEnded = false);
end;

// Properties ------------------------------------------------------------------
// Retrieve a reference to the source being scanned
function AMiniScanner.Source: ATextInputStream;

begin
  Result := MySource;
end;

// Retrieve whether or not the scanner owns its source
function AMiniScanner.ownsSource: boolean;

begin
  result := myOwnSource;
end;

// Set whether or not the scanner owns its source
function AMiniScanner.setOwnsSource(const flag: boolean): boolean;

begin
  result := myOwnSource;
  myOwnSource := flag;
end;

// Retrieve the characters that are recognized as digits
function AMiniScanner.recognizedDigits: string;

begin
  result := myRecognizedDigits;
end;

// Set the characters that are recognized as digits
function AMiniScanner.setRecognizedDigits(const newValue: string): string;

begin
  result := myRecognizedDigits;
  myRecognizedDigits := newValue;
end;

// Retrieve the characters that are recognized as letters
function AMiniScanner.recognizedLetters: string;

begin
  result := myRecognizedLetters;
end;

// Set the characters that are recognized as letters
function AMiniScanner.setRecognizedLetters(const newValue: string): string;

begin
  result := myRecognizedLetters;
  myRecognizedLetters := newValue;
end;

// Retrieve the characters that are recognized as special characters
function AMiniScanner.recognizedSpecialCharacters: string;

begin
  result := myRecognizedSpecialCharacters;
end;

// Set the characters that are recognized as special characters
function AMiniScanner.setRecognizedSpecialCharacters(const newValue: string): string;

begin
  result := myRecognizedSpecialCharacters;
  myRecognizedSpecialCharacters := newValue;
end;

// Retrieve a reference to the list of recognized tokens
function AMiniScanner.RecognizedTokens: AStringTree;

begin
  Result := MyRecognizedTokens;
end;

// Retrieve a reference to the second most recent token collected
function AMiniScanner.previousToken: PMiniToken;

begin
  result := @myPreviousToken;
end;

// Retrieve a reference to the most recent token collected
function AMiniScanner.currentToken: PMiniToken;

begin
  result := @myCurrentToken;
end;

// AMiniParser *****************************************************************
// Initializer
function AMiniParser.init: boolean;

begin
  result := inherited init;

  if result then
  begin
    MyScanner := nil;

    myRecognizedTokenCallback := nil;
    myNumericTokenCallback := nil;
    myUnrecognizedTokenCallback := nil;
  end;
end;

// Destructor
destructor AMiniParser.destroy;

begin
  // Free our scanner
  if MyScanner <> nil then
    SysUtils.freeAndNil(MyScanner);

  inherited destroy;
end;

// Retrieve the next token from the scanner
procedure AMiniParser.nextToken;

begin
  if MyScanner <> nil then
    MyScanner.nextToken;
end;

// Retrieve the previous token from the scanner
function AMiniParser.previousToken: PMiniToken;

begin
  result := nil;

  if MyScanner <> nil then
    result := MyScanner.previousToken;
end;

// Retrieve the current token from the scanner
function AMiniParser.currentToken: PMiniToken;

begin
  result := nil;

  if MyScanner <> nil then
    result := MyScanner.currentToken;
end;

// Parse the source
function AMiniParser.parse: int64;

var
  // Indicates whether or not parsing should abort
  abortParsing: boolean;

begin
  result := -1;

  if MyScanner = nil then
    exit;

  // Get the first token from the scanner
  Self.nextToken;

  // Parsing loop
  while Self.currentToken^.tokenType <> MT_EOS do
  begin
    abortParsing := false;

    case Self.currentToken^.tokenType of
      MT_RECOGNIZED:
        if myRecognizedTokenCallback <> nil then
          result := result + myRecognizedTokenCallback(Self, Self.currentToken,
            Self.previousToken, abortParsing);

      MT_INTEGER, MT_FLOAT:
        if myNumericTokenCallback <> nil then
          result := result + myNumericTokenCallback(Self, Self.currentToken,
            Self.previousToken, abortParsing);

      else
        if myUnrecognizedTokenCallback <> nil then
          result := result + myUnrecognizedTokenCallback(Self,
            Self.currentToken, Self.previousToken, abortParsing);
    end;

    if abortParsing then
      break;

    // Get the next token
    Self.nextToken;
  end;
end;

// Properties ------------------------------------------------------------------
// Retrieve a reference to the scanner used by the parser
function AMiniParser.Scanner: AMiniScanner;

begin
  Result := MyScanner;
end;

// Retrieve a reference to the list of recognized tokens
function AMiniParser.RecognizedTokens: AStringTree;

begin
  Result := nil;

  if MyScanner <> nil then
    Result := MyScanner.RecognizedTokens;
end;

// Retrieve a reference to the recognized token callback
function AMiniParser.recognizedTokenCallback: AMiniParserCallback;

begin
  result := myRecognizedTokenCallback;
end;

// Set the recognized token callback
function AMiniParser.setRecognizedTokenCallback(const newCallback: AMiniParserCallback): AMiniParserCallback;

begin
  result := myRecognizedTokenCallback;
  myRecognizedTokenCallback := newCallback;
end;

// Retrieve a reference to the numeric token callback
function AMiniParser.numericTokenCallback: AMiniParserCallback;

begin
  result := myNumericTokenCallback;
end;

// Set the numeric token callback
function AMiniParser.setNumericTokenCallback(const newCallback: AMiniParserCallback): AMiniParserCallback;

begin
  result := myNumericTokenCallback;
  myNumericTokenCallback := newCallback;
end;

// Retrieve a reference to the unrecognized token callback
function AMiniParser.unrecognizedTokenCallback: AMiniParserCallback;

begin
  result := myUnrecognizedTokenCallback;
end;

// Set the unrecognized token callback
function AMiniParser.setUnrecognizedTokenCallback(const newCallback: AMiniParserCallback): AMiniParserCallback;

begin
  result := myUnrecognizedTokenCallback;
  myUnrecognizedTokenCallback := newCallback;
end;

{$ENDIF}
