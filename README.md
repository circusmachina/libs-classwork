**classwork.pp**: A library of basic Pascal classes useful in other projects.  
Copyright (C) 2011-2015 by [Michael Malicoat][Author].  
[Home page][] | [Source][] | [Source documentation][] |
[Bug tracker][]

This is a library of basic classes that are designed to serve as the foundation
of other, larger projects.  The classes and routines defined by the library
are designed to work across multiple platforms and have (as of this writing) 
been tested on Windows 7 and Ubuntu.

The library continues to be refined.  It presently provides classes and
routines for managing:

* [ASCII and UTF-8 strings][Charstring]
* [Nodes][ANode] and the [lists][ALinkedList] that manage them
* [Binary trees][ABinaryTree] and [dictionaries][ADictionary]
* [Streaming input and output][AStream], including [binary file streams]
  [AFileStream] and [stream modifiers][AStreamWrapper]
* [Logging][ALog] to arbitrary streams
* [Iteration][AnIterator]
* [Vectors][AVector] (used here in the C sense rather than the math sense)
* [Basic application support][AnApplication], including the ability to 
  extract [version information][AFileVersionInfoResource] from the executable
* [Simple parsers][AMiniParser]

All of my other libraries and projects routinely make use of the functionality
provided by the classes in this library, and it is my hope that you may find
them just as useful.

The documentation seen here was generated with a modified version of [PasDoc][].
The [classes hierarchy graph](GVClasses.svg) was generated using [GraphViz][].

## License ##
This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 3 of the License or, at your option, any later 
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABLITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program.  If not, see <http://www.gnu.org/licenses/>.

## References ##
_(Not visible in the parsed HTML output because they've turned into the 
various links used in the text above.)_

[Author]: http://www.circusmachina.com/gladius/Profiles/Michael
[Home page]: http://docs.circusmachina.com/libs/classwork/
[Source]: https://github.com/circusmachina/libs/classwork
[Source documentation]: http://docs.circusmachina.com/libs/classwork/
[Bug tracker]: http://monkeywrench.circusmachina.com

[PasDoc]: http://pasdoc.sourceforge.net
[GraphViz]: http://www.graphviz.org

[Charstring]: http://docs.circusmachina.com/libs/classwork/html/classwork.Charstring.html
[ANode]: http://docs.circusmachina.com/libs/classwork/html/classwork.ANode.html
[ALinkedList]: http://docs.circusmachina.com/libs/classwork/html/classwork.ALinkedList.html
[ABinaryTree]: http://docs.circusmachina.com/libs/classwork/html/classwork.ABinaryTree.html
[ADictionary]: http://docs.circusmachina.com/libs/classwork/html/classwork.ADictionary.html
[AStream]: http://docs.circusmachina.com/libs/classwork/html/classwork.AStream.html
[AFileStream]: http://docs.circusmachina.com/libs/classwork/html/classwork.AFileStream.html
[AStreamWrapper]: http://docs.circusmachina.com/libs/classwork/html/classwork.AStreamWrapper.html
[ALog]: http://docs.circusmachina.com/libs/classwork/html/classwork.ALog.html
[AnIterator]: http://docs.circusmachina.com/libs/classwork/html/classwork.AnIterator.html
[AVector]: http://docs.circusmachina.com/libs/classwork/html/classwork.AVector.html
[AnApplication]: http://docs.circusmachina.com/libs/classwork/html/classwork.AnApplication.html
[AFileVersionInfoResource]: http://docs.circusmachina.com/libs/classwork/html/classwork.AFileVersionInfoResource.html
[AMiniParser]: http://docs.circusmachina.com/libs/classwork/html/classwork.AMiniParser.html

