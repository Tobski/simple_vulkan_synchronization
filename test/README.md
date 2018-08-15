# Tests

`tests.c` defines a number of unit tests to test that various scenarios
produce the desired output.
Tests are based on the common synchronization examples on the Vulkan-Docs
wiki: https://github.com/KhronosGroup/Vulkan-Docs/wiki/Synchronization-Examples.

## Building

On a unix based system these tests can be built using:

`gcc -o tests tests.c -lvulkan`

## Running

Running is straightforward:

`./tests`

The executable will write out the tests that are run, whether they pass or
fail, and what caused them to fail if they did.

## Adding tests

If you'd like to add a test, just define a new test in main() as per those
that already exist.
