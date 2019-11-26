# Overview

this project is only for boost library primitive feasibility and investigation test code.
just to check if the libraries works as expected with some perspective such as cpu resource, memory footprint, system calls and system traps.

# Rules

eash test program needs to be categorizes into correspoding to boost category, for example boost::interprocess, interprocess is one of the category here.

each category folder must have Makefile/CMake to build whole directory programs and enable user to run the test program.

# Category

- [boost::interprocess](./boost_interprocess)

  boost::interprocess related test programs.

# How to Run

```
git clone https://github.com/fujitatomoya/boost_primitives
cd boost_primitives/boost_interprocess
make
make run
```

# Authors

* **Tomoya Fujita** --- Tomoya.Fujita@sony.com

# License

Apache 2.0