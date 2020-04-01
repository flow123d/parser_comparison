# parser_comparison
Comparison of BParser with other state of the art parsers and agains direct C++ implementation.


## Building
- clone repository to your desktop
- setup submodules:
  
      git submodule init
      git submodule update
  
- execute 'make' commands defined in makefile:

      # build grammar.o file (bparser)
      make grammar
      # build *.o files (muparser)
      make muparser-all
      # build tests
      make test_exprtk
      make test_muparser
      make test_bparser
  
- these comands print output to csv files stored in directory 'output'
- outputs to graphs are printed by python scripts stored in directory 'python'
