matlab-zmq
==========

What's this all about?
----------------------

This API aims to bring the awesome of ZMQ to MATLAB. This project has grown out @fagg wanting to
better manage large scale numerical computing experiments across a High Performance Cluster. However,
this library can be used in any number of contexts across any number of machines (even 1 is OK).


Getting Started
---------------
+ Make sure you have ZMQ 4.0.x installed. Change settings in `config.m` to point to your installation.
+ Ensure you have mex configured [[1]](http://www.mathworks.com/help/matlab/matlab_external/what-you-need-to-build-mex-files.html) [[2]](http://www.mathworks.com/support/compilers/R2014b/index.html)
+ Run `make.m`
+ Make sure that the `lib` directory is on your MATLAB path.
+ Start hacking.

Stuff Doesn't Work
------------------

- Git pull master - make clean; make; make test. Try again.
- Try the dev branch. See if that works.
- If not, open an issue and include the following information:
  - Versions: MATLAB, ZMQ and Operating System
  - Which version of matlab-zmq you're using (i.e. which branch, commit etc).
  - What you are trying to do - preferably include a succinct code example which illustrates the problem.
  - What doesn't work - please describe behaviour explicitly and include any error messages you encounter.

How can I help?
-----------

Pull requests are most welcome. As a general rule, please base all pull requests in master. If in doubt, contact @fagg.

Development Team
-----------------------

- Ashton Fagg (@fagg)
- Anderson Bravalheri (@abravalheri)

Contributors
------------

This project contains contributions from the following people: 

- Ashton Fagg (@fagg)
- Anderson Bravalheri (@abravalheri)
- Matheus Svolenski (@msvolenski)

