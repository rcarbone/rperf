The Rock Container Interface (RCI) is specification for containers application programming.
It provides an interface with implementation independence.

Overview
========
The Rock Container Interface is a specification that defines an independent interface for container applications.
Programs using RCI can be run over a variety of implementation, such as the

 Unordered maps
 .... 

 or any other implementation C/C++ that provides the data structures and algorithms with insert, lookup, and delete routines.

RCI provides similar functionality as the xxx interface,
however it is implementation independent in contrast to the xxx interface which is heavily biased toward the STL intreface.

RCI consists of link library application programming interface (API), header files,
and the rules and restrictions elaborating how RCI processes work and interact.


Implementation independence
===========================
RCI is Implementation independent. However, applications still need to be aware of the desired implementation in order to select it.
Applications can achieve this implementation independence mapping the interface provided by the RCI library over real implementations.


RCI-to-STL equivalents
======================
The following table shows approximate equivalence between the RCI and STL interfaces:

|    RCI    |   STL  |   Notes   |
| --------  | ------ | --------- |
| rci_alloc | new    |           |
| rci_free  | delete |           |
| ......... | ...... |           |


Implementations
===============
