# rperf - Benchmark Tools for HashTable Implementations

* [rspeed]   a performance tool to evaluate/compare/sort time elapsed in execution of test scenarios.
* [rbattle]  a match race tool to evaluate relative ranking in execution of test scenarios.
* [librht]   an abstract C library to be mapped over real hash table implementations


The following 11 different test scenarios can be currently run over 48 different hashtable implementations:
* grow_seq    - Populate an empty container (sequential keys)
* grow_rnd    - Populate an empty container (random keys)
* hit_seq     - Search one-by-one all existing items (sequential keys)
* hit_rnd     - Search one-by-one all existing items (random keys)
* miss_seq    - Search for non existing items (sequential keys)
* miss_rnd    - Search for non existing items (random keys)
* delete_seq  - Delete one-by-one all existing items (sequential keys)
* delete_rnd  - Delete one-by-one all existing items (random keys)
* replace_seq - Replace one-by-one all existing items (sequential keys)
* replace_rnd - Replace one-by-one all existing items (random keys)
* kbench      - Delete if found, add otherwise (non-unique keys)


# Results for the impatients

rspeed, ver. 0.1.0 built on Mar 25 2018 15:40:31

Evaluate time elapsed repeating 10 times the same test, each acting with 100000 unique items

Running on : Linux nuc 3.2.0-4-amd64 #1 SMP Debian 3.2.96-2 x86_64

Local time : Sat Apr  7 10:20:22 2018

Dataset  : unique sequential string keys (char *) and generic pointers (void *) as values

Times    : wall-clock time evaluated at nsecs resolution and rendered in a more human-readable format

Rendering: results are sorted by less average time elapsed


| Rank | Implementation        | mark |  #1 |  #2 |  #3 |  #4 |  #5 |  #6 |  #7 |  #8 |  #9 | #10 | #11 |
| ---- | --------------------- | ---- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
|   1  | ulib::align-hash++    |  506 |   4 |   1 |   1 |   2 |   1 |   3 |   2 |   1 |   8 |   1 |   1 |
|   2  | ulib::open-hash++     |  487 |   5 |  13 |   4 |   1 |   3 |   2 |   8 |   2 |   2 |   2 |   2 |
|   3  | rdestl::hash_map      |  472 |   7 |  12 |   5 |   6 |   2 |   5 |   6 |   3 |   1 |   7 |   5 |
|   4  | amtl::hashmap         |  458 |   9 |   9 |   9 |   8 |   6 |   1 |  10 |   7 |   7 |   4 |   3 |
|   5  | tommyds/dynamic       |  442 |   6 |   6 |   7 |  11 |   9 |   8 |   9 |  10 |   4 |  10 |   9 |
|   6  | glib/ghash            |  441 |   3 |  11 |   2 |  10 |   4 |  17 |   4 |  11 |  11 |  11 |   6 |
|   7  | gcc/hashtab           |  441 |   2 |   5 |   6 |   9 |   8 |   4 |   3 |   4 |  10 |   8 |  31 |
|   8  | tommyds/fixed         |  440 |   1 |   8 |   3 |   7 |   5 |   7 |   1 |   5 |   3 |   9 | --- |
|   9  | libevent/ht           |  425 |  11 |  14 |   8 |   5 |  10 |   6 |  15 |  19 |   5 |   6 |   7 |
|  10  | google/c-dense        |  417 |  13 |   2 |  14 |   3 |  14 |   9 |  27 |   6 |  19 |   3 |   4 |
|  11  | emilib::HashMap       |  403 |  14 |  10 |  10 |   4 |   7 |  12 |  13 |   8 |  21 |  14 |  15 |
|  12  | tommyds/linear        |  393 |   8 |  15 |  11 |  16 |  12 |  14 |  11 |  24 |   6 |  13 |   8 |
|  13  | ulib/chain-hash       |  359 |  15 |  18 |  12 |  12 |  11 |  10 |   5 |  14 |  16 |  17 | --- |
|  14  | apr/hashtable         |  351 |  20 |  16 |  18 |  20 |  15 |  21 |   7 |  12 |   9 |  15 |  27 |
|  15  | google::densehash     |  333 |  17 |  19 |  13 |  25 |  13 |  11 |  17 |  15 |  26 |  30 |  12 |
|  16  | Qt::QHash             |  303 |  24 |  37 |  21 |  17 |  30 |  13 |  22 |  31 |  15 |   5 |  13 |
|  17  | ccan/htable           |  300 |  30 |  30 |  17 |  24 |  19 |  20 |  23 |  16 |  12 |  12 |  28 |
|  18  | libghthash/hashtable  |  298 |  18 |  26 |  15 |  19 |  16 |  16 |  14 |  32 |  14 |  21 | --- |
|  19  | hashit/overflow       |  287 |  16 |   3 |  16 |  14 |  17 |  15 |  16 |   9 |  48 | --- | --- |
|  20  | mct::closed           |  281 |  35 |  27 |  27 |  13 |  25 |  18 |  28 |  18 |  32 |  16 |  11 |
|  21  | mct::linked           |  281 |  23 |  20 |  26 |  15 |  23 |  19 |  30 |  25 |  24 |  28 |  17 |
|  22  | ulib/open-hash        |  278 |  29 |  32 |  24 |  28 |  24 |  25 |  24 |  13 |  17 |  19 |  18 |
|  23  | libCDS/HashMap        |  263 |  25 |  43 |  34 |  18 |  18 |  22 |  20 |  23 |  13 |  18 |  34 |
|  24  | eastl::hashmap        |  250 |  28 |  38 |  22 |  23 |  22 |  26 |  21 |  26 |  23 |  23 |  29 |
|  25  | calg/hashtable        |  247 |  31 |  36 |  25 |  27 |  20 |  23 |  19 |  28 |  18 |  20 |  37 |
|  26  | ulib::chain-hash++    |  231 |  21 |   4 |  19 |  22 |  27 |  39 |  26 |  21 |  37 |  42 | --- |
|  27  | apr-no-pool/hashtable |  228 |  34 |  42 |  31 |  33 |  26 |  28 |  12 |  20 |  20 |  24 |  33 |
|  28  | klib/khash            |  223 |  19 |  25 |  29 |  39 |  41 |  44 |  18 |  35 |  22 |  22 |  14 |
|  29  | Tcl C-API/hashtable   |  212 |  12 |  35 |  35 |  36 |  33 |  24 |  36 |  22 |  29 |  32 |  25 |
|  30  | klib::khash++         |  201 |  42 |  28 |  39 |  31 |  31 |  33 |  33 |  17 |  30 |  26 |  20 |
|  31  | uthash/hashtable      |  198 |  26 |  23 |  37 |  29 |  35 |  29 |  35 |  30 |  36 |  29 |  24 |
|  32  | clark/hashtable       |  194 |  44 |  21 |  40 |  34 |  36 |  27 |  31 |  27 |  28 |  27 |  22 |
|  33  | hashit/open           |  188 |  10 |   7 |  23 |  21 |  37 |  40 |  41 |  29 |  47 |  46 | --- |
|  34  | std::unordered_map    |  187 |  32 |  24 |  30 |  26 |  28 |  38 |  34 |  39 |  31 |  39 |  23 |
|  35  | hashit/chain          |  180 |  33 |  17 |  38 |  40 |  39 |  31 |  25 |  34 |  27 |  25 | --- |
|  36  | judy/JSL              |  169 |  22 |  34 |  20 |  35 |  21 |  30 |  39 |  40 |  42 |  43 |  36 |
|  37  | PeterMoore/st         |  161 |  27 |  47 |  33 |  30 |  29 |  32 |  29 |  37 |  33 |  38 |  35 |
|  38  | redis/dict            |  155 |  40 |  29 |  36 |  32 |  38 |  35 |  32 |  38 |  25 |  33 |  38 |
|  39  | boost::unordered_map  |  141 |  38 |  31 |  42 |  37 |  40 |  36 |  38 |  42 |  34 |  36 |  16 |
|  40  | rigtorp::hashmap      |  137 |  41 |  33 |  28 |  42 |  32 |  41 |  37 |  36 |  38 |  40 |  26 |
|  41  | google/c-sparse       |  126 |  43 |  22 |  41 |  38 |  45 |  34 |  43 |  33 |  43 |  31 |  32 |
|  42  | tbb::unordered_map    |  125 |  37 |  40 |  32 |  41 |  34 |  37 |  42 |  43 |  40 |  41 |  19 |
|  43  | ulib/align-hash       |   85 |  45 |  48 |  46 |  48 |  48 |  48 |  40 |  41 |  35 |  37 |  10 |
|  44  | stb/sdict             |   70 |  36 |  46 |  43 |  43 |  42 |  42 |  48 |  48 |  45 |  47 |  21 |
|  45  | oddou::hashmap        |   61 |  48 |  44 |  45 |  44 |  43 |  43 |  47 |  44 |  39 |  34 |  39 |
|  46  | Python C-API/dict     |   54 |  39 |  39 |  44 |  45 |  46 |  45 |  45 |  45 |  44 |  44 |  41 |
|  47  | google::sparsehash    |   46 |  46 |  41 |  48 |  46 |  44 |  47 |  46 |  46 |  46 |  45 |  30 |
|  48  | libcfu/hashtable      |   45 |  47 |  45 |  47 |  47 |  47 |  46 |  44 |  47 |  41 |  35 |  40 |


Each implementation under test is simply a plugin
(a shared object dynamically loaded/unloaded for the purpose of the tests).

Currently supported implementations.

|  Library                        |  Module               | Author
| ------------------------------- | --------------------- | -------------------------------------------------- |
|  Qt Toolkit                     | Qt::QHash             | The Qt Company Ltd.                                |
|  amtl                           | amtl::hashmap         | David Anderson and AlliedModders LLC               |
|  Apache apr                     | apr-no-pool/hashtable | Apache Software Foundation (ASF)                   |
|  Apache apr/C API               | apr/hashtable         | Apache Software Foundation (ASF)                   |
|  Boost C++ Libraries            | boost::unordered_map  | Jeremy B. Maitin-Shepard and Daniel James          |
|  Google libcash                 | google/c-dense        | Craig Silverstein                                  |
|  Google libcash                 | google/c-sparse       | Craig Silverstein                                  |
|  C Algorithms                   | calg/hashtable        | Simon Howard (fraggle@gmail.com)                   |
|  ccan                           | ccan/htable           | Rusty Russell (rusty@rustcorp.com.au)              |
|  cwc22                          | clark/hashtable       | Christopher Clark (christopher.clark@cl.cam.ac.uk  |
|  mct                            | mct::closed           | Paul Pogonyshev (pogonyshev@gmail.net)             |
|  Google Hash-Map                | google::densehash     | Google Inc.                                        |
|  eastl                          | eastl::hashmap        | Electronic Arts Inc.                               |
|  emilib                         | emilib::HashMap       | Emil Ernerfeldt (emil.ernerfeldt@gmail.com)        |
|  glib                           | glib/ghash            | Peter Mattis, Spencer Kimball and Josh MacDonald   |
|  libghthash                     | libghthash/hashtable  | Simon Kagstrom (ska@bth.se)                        |
|  hashit                         | hashit/chain          | David Gomez (david@pleyades.net)                   |
|  hashit                         | hashit/open           | David Gomez (david@pleyades.net)                   |
|  hashit                         | hashit/overflow       | David Gomez (david@pleyades.net)                   |
|  GNU libiberty                  | gcc/hashtab           | Vladimir Makarov (vmakarov@cygnus.com)             |
|  judy                           | judy/JSL              | Doug Baskinks (dougbaskins@yahoo.com)              |
|  klib                           | klib::khash++         | Heng Li (lh3@me.com)                               |
|  klib                           | klib/khash            | Heng Li (lh3@me.com)                               |
|  LibCDS                         | libCDS/HashMap        | ZongXian Shen (andy.zsshen@gmail.com)              |
|  libcfu                         | libcfu/hashtable      | Don Owens (don@owensnet.com)                       |
|  libevent                       | libevent/ht           | Niels Provos (provos@umich.edu)                    |
|  mct                            | mct::linked           | Paul Pogonyshev (pogonyshev@gmail.net)             |
|  oddou                          | oddou::hashmap        | Vivien Oddou <x@x.x>                               |
|  Python C/API                   | Python C-API/dict     | Guido van Rossum and the Python community          |
|  rdestl                         | rdestl::hash_map      | Maciej Sinilo                                      |
|  Redis                          | redis/dict            | Salvatore Sanfilippo (antirez@gmail.com)           |
|  rigtorp                        | rigtorp::hashmap      | Erik Rigtorp <erik@rigtorp.se>                     |
|  Google Hash-Map                | google::sparsehash    | Google Inc.                                        |
|  st                             | PeterMoore/st         | Peter Moore @ UCB                                  |
|  stb                            | stb/sdict             | Sean T. Barrett (sean@nothings.org)                |
|  Intel Threading Building Block | tbb::unordered_map    | Intel Corporation                                  |
|  Tcl C-API                      | Tcl C-API/hashtable   | John Ousterhout and the Tcl community              |
|  tommyds                        | tommyds/dynamic       | Andrea Mazzoleni (amadvance@gmail.com)             |
|  tommyds                        | tommyds/fixed         | Andrea Mazzoleni (amadvance@gmail.com)             |
|  tommyds                        | tommyds/linear        | Andrea Mazzoleni (amadvance@gmail.com)             |
|  ulib                           | ulib::align-hash++    | Zilong Tan (eric.zltan@gmail.com)                  |
|  ulib                           | ulib/align-hash       | Zilong Tan (eric.zltan@gmail.com)                  |
|  ulib                           | ulib::chain-hash++    | Zilong Tan (eric.zltan@gmail.com)                  |
|  ulib                           | ulib/chain-hash       | Zilong Tan (eric.zltan@gmail.com)                  |
|  ulib                           | ulib::open-hash++     | Zilong Tan (eric.zltan@gmail.com)                  |
|  ulib                           | ulib/open-hash        | Zilong Tan (eric.zltan@gmail.com)                  |
|  GNU C++ Library                | std::unordered_map    | Silicon Graphics Computer Systems, Inc.            |
|  uthash                         | uthash/hashtable      | Troy D. Hanson                                     |

===================================


How to add an implementation
============================
Please remember that each implementation under test is simply a plugin.
So, go to the implementations/ directory and create a new one with
the name of the implementation.

In this directory copy the following 3 files from an already available implementation:

 1.  Makefile
 2.  README.c
 3.
    a. glue.c (C)
    b. glue.cpp (C++)

and edit these files in order to meet the requirements.

- In file [Makefile] the only required definition to customize should be NAME and optionally
  the CFLAGS and C++FLAGS.

  Do not edit anything below the separator because the Makefile is automatically
  updated when needed.

- In file [README.c] the section to customize is that contains Plugin definitions

- In file [glue.c] / [glue.cpp] must be implemented each function rsuite_xxx() in order to map the
  virtual interface defined in rht.h over the real implementation


When you have done please run in the directory with the new defined implementation
the following commands:
> make       (to compile everything)
> make ut    (to run the unit tests)
> make run   (to run the test suite)
> make x     (to run both units tests and the test suite)
> make leaks (to check for errors and memory leaks)
> make help  (provides you all the available rules)


Please not do forget at the end of the process to add the name of the new directory
in implementations/Makefile.


How to add a test scenario
==========================

1. add in builtins/rlsuite.h the declaration of the function
2. add in builtins/suite.c an item in the builtins static table
3. add in builtins/suite-code.c the implementation of the scenario

4. add in etc/plugin-rl.c
  a. an item in enum rlsuite_id_t
  b. a case in function run_this()
  c. a function to handle the call

5. update README.c foreach plugin


That's all folks!


How to add an Unit Test
=======================

1. add in builtins/unit.c
   a. the local Unit Test identifier
   b. the external reference of the function which implements the Unit Test
   c. an item to the table of builtins
2. add in builtins/unit-code.c the implementation of the Unit Test

That's all folks!
