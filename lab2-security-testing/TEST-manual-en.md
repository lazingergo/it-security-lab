# IT security

## Software security testing



## Educational Objectives

In this lab exercise, we will demonstrate widely used security testing techniques. The static and dynamic testing techniques introduced are effective tools to identify errors in software and thus improve overall security. The exercises involve testing an application previously presented in the input validation exercise. This involves starting from an implementation with bugs and using each method to continuously improve the application. By completing the laboratory exercise, the participant will be able to use the most important testing techniques in software development.



## Background Material

Software testing is an area of high importance during the implementation process. Using appropriate testing
can significantly improve the quality of the finished product, reduce
user problems and prevent other financial losses resulting from bugs.

Software testing has several purposes:

- **_Functional testing_**  checks whether the software works according to the defined specifications.
  In this case, the tests are systematically derived from the requirements, we can determine with a high degree of certainty whether the
  software meets all requirements.

- In contrast, in the case of **_security testing_**, we want to ensure that
  make sure that the application has passed the functional testing
  not checked in the functional test, it works acceptably in all other cases.
  Since exhaustive testing is not possible due to the infinite number of possible cases
  possible, therefore a successful security test is a
  is a much more difficult and skill-intensive activity. Fortunately
  statistics show that the vast majority of security failures are of a similar
  reasons, so the most useful testing methods are
  the most useful testing techniques can achieve effective results in this area too.
  Safety testing techniques can be described as _static_ or _dynamic_.
  analyses.

  - In contrast, in the case of **_security testing_**, we want to ensure the aspects not covered in functional tests work acceptably in those cases. Since exhaustive testing is impossible due to the infinite number of possible scenarios, a successful security test is a much more difficult and skill-intensive activity. Fortunately, statistics show that the vast majority of security failures occur for similar reasons. As a result, the most effective testing techniques can still achieve significant results in this area. Security testing techniques can be categorized as either _static_ or _dynamic_ analyses.

Effective security testing can be greatly enhanced by using a typed language during development. Firstly, static analyzers benefit from type analysis, as it provides more accurate information to support development. However, the use of typed languages has been a long-standing debate in the industry, and to date, there is no clear consensus on the matter.

### Application of Types

#### Is Dynamic Typing Good or Bad?

There is no definitive answer to this question[^1]. The choice depends on the project and its goals when determining whether a language with dynamic typing is the right fit.  Python, for example, is a _dynamically_ typed language, not a _statically_ typed one. This means that the Python interpreter does not determine the types of objects until the code is executed.  

- Dynamic types are mostly used in scripting languages such as Ruby, JavaScript, MATLAB, etc.  

- On the other end of the spectrum, statically typed languages are usually compiled rather than interpreted. Examples of statically typed languages include Fortran, C, C++, and Java.  

Because Python is a dynamically typed language, the Python interpreter does not need to know the type of objects before initializing them. An object's type may depend on a value that is only determined at runtime, so the Python interpreter infers the type dynamically. In Python, dynamic type handling provides programmers with a great deal of flexibility. However, this flexibility also has some drawbacks. Typically, when working with a large codebase, introducing types becomes beneficial, offering several advantages. Some examples of these advantages include:  

#### Type information also serves as documentation

Due to the dynamic nature of types, Python function parameters can accept any object, regardless of its type. The following code snippet is perfectly valid for this reason:  

```python
def append_to_container(c, e):
    # Code of the function
```

When encountering such a code snippet, can we be 100% certain about how to use this function without seeing its implementation? No. Here are two "valid" but incompatible ways of using the function:

```python
# First possibility
c = [1, 2]
e = 3
append_to_container(c, e)
    # c is modified in-place in the function and the
    # function does not return any value

# Second possibility
c = [1, 2]
e = 3
new_c = append_to_container(c, e)
    # c is copied internally by the function and a modified
    # copy is returned.
```

In one case, the function modifies the input parameter locally, while in the other, it returns a new value with the modification applied. The header of the `append_to_container` function does not indicate which of the two behaviors is correct. To clarify this, the function should be documented properly, for example, by using a docstring:  

```python
def append_to_container(c, e):
    """Modify c in-place to append e at the end of the container."""
    # Code of the function
```

But this raises an additional problem: docstrings (and documentations in general)
often become outdated, are not maintained, or simply not written properly and do not contain the right information. The following extract is an example:

```python
# Example of a useless docstring. The docstring only repeats
# informations already in function or arguments names.
def append_to_container(c, e):
"""Append element e to the contained c."""
    # Code of the function
```

The biggest disadvantage of using docstrings, however, is that they
cannot be analysed by automated tools such as integrated
integrated development environments (IDEs) or code analysis programs, unless
they conform to very strict formatting rules (e.g., [Sphinx format](https://sphinx-rtd-tutorial.readthedocs.io/en/latest/docstrings.html#the-sphinx-docstring-format),
[Google format](https://sphinxcontrib-napoleon.readthedocs.io/en/latest/example_google.html)
vagy [Numpy format](https://sphinxcontrib-napoleon.readthedocs.io/en/latest/example_numpy.html#example-numpy)).

#### Many features of the IDE are based on types

For the following functionalities, it is particularly useful to include types in the code:  

- Displaying documentation in tooltips.  
- Showing errors or warnings when a type does not match the expected type.  
- Using the "Go to type definition" feature when exploring a new codebase.  

#### Errors Become Clearer  

If you know in advance what type a variable should be, you can check at runtime whether the assigned object matches the expected type.  

```python
def count_bit_number(i):
    return i.bit_length()

a = count_bit_number(3) # result is 2
b = count_bit_number(1.0) # error
```

The last line of the above code fragment causes an error:

```python
AttributeError: 'float' object has no attribute 'bit_length'
```

where, if Python had been aware that `count_bit_number`
function accepts only integers as input, the error message would be something like
would have been something like,

```python
TypeError: 'count_bit_number' got a 'float', expected 'int'
```

which is much easier to understand.

#### Adding types to Python

In the previous section, we explored when and why it is beneficial to use types. However, instead of enforcing strict type management in Python (which could have led to various unintended consequences and complications), the Python developers introduced type annotations as part of [PEP 484](https://peps.python.org/pep-0484/).  

Importantly, the term _annotation_ highlights that the Python interpreter does not enforce type information. Type annotations serve as hints for developers and external tools, but they are not processed or enforced by the interpreter.  

For a good summary of the use of type annotations, see [mypy library](https://mypy.readthedocs.io/en/stable/cheat_sheet_py3.html)
and the official [python documentation](https://docs.python.org/3/library/typing.html).

### Static testing

_Static_ analyzers do not execute programs; they only "read" and interpret source code or machine code at the static level. The analysis is performed on a memory representation of the instructions. These techniques scale well and can efficiently handle large codebases.  
However, their disadvantage is that they lack access to runtime information, which can lead to false positives. This means they may flag issues in code fragments that would never be executed in real-life scenarios or contain bugs that could never actually be exploited.  

#### Mypy

[Mypy](https://github.com/python/mypy) is a static type checker for Python that can identify errors based on type information. Type checkers help ensure that variables and functions are used correctly in the code. This allows certain issues to be detected without executing the code, using static analysis.  
The example below demonstrates a possible error that Mypy can easily detect:  

```python
number = input("What is your favourite number?")
# error: Unsupported operand types for + ("str" and "int")
print("It is", number + 1)
```

The addition of type hints does not interfere with the execution of the program. This type information should be thought of as comments. The Python interpreter can always run the application, even if Mypy reports an error. However, in such cases, a runtime issue is likely to occur.  

Mypy is designed to be introduced incrementally, meaning it is not necessary to annotate the entire codebase at once. This makes it easy to use in practice. It supports features such as type inference, generics, callable types, tuple types, union types, structural subtyping, and more.  
Using Mypy makes programs easier to understand and maintain.  

### Dynamic Testing  

_Dynamic_ tests analyze software during execution. Because of this, they have access to runtime information, allowing for a much more accurate analysis. However, due to the nature of dynamic testing, there are a few important considerations:  

- They are difficult to scale, as tests often need to be manually written and executed.  
- If not all possible execution paths are tested with different inputs, we cannot determine the security of untested code fragments.  
- Like static analysis, dynamic testing can produce false negatives, meaning there is no guarantee that all vulnerabilities will be detected.  

#### Pytest  

Pytest is a Python-based testing framework for writing and executing test code. It offers several advantages:  

- Pytest is free and open-source.  
- It is simple and easy to use.  
- Pytest can run multiple tests in parallel, reducing the overall execution time of the test suite.  
- It automatically discovers test files following the naming conventions `test_*.py` or `*_test.py`.  
- Pytest allows running the entire test suite, executing only a subset of tests, or skipping specific tests during execution.  

When building tests, filenames should start with "test" or end with `test_example.py` or `example_test.py` in order to be automatically discovered by Pytest. If the tests are defined as methods within a class, the class name should start with "Test" (e.g., `TestClass`). In this case, the class should not be called `__init__`. Within the class, the names of methods must also begin with "test_". Methods whose names do not match this pattern will not be executed as tests.  

```python
# Function to be tested in operations.py
def sum(a, b):
    """Returns the sum of the two number"""
    return a + b
```

```python
# Test case definition in test_example.py
import pytest

def test_sum():
    assert sum(1, 2) == 3
```

You can then run the tests with `pytest` or with
each test separately, for example by `pytest test_example.py`
command.

![Use of Pytest \label{pytest}](figures/pytest2.png)
_Use of Pytest_

This will cause Pytest to automatically discover the tests in the folder or within the specified file. It will look for files starting with `test` and functions starting with `test` inside them. In the image above[^2], you can see that Pytest marks a successful test with a green dot and a failed test with a red "F." Additionally, it indicates how many tests have passed or failed.  

```python
import pytest
@pytest.fixture
def client():
    """Configures the app for testing

    Sets app config variable ``TESTING`` to ``True``

    :return: App for testing
    """

    #app.config['TESTING'] = True
    client = app.test_client()

    yield client
```

The `@pytest.fixture` annotation in the example above tells Pytest to create the following function (using the `yield` command) to set up an application for testing. In this case, the function doesn't do much, but it could be used to create temporary database files or set up configurations for testing (e.g., the `app.config` line commented out)[^3].  

[^1]: <https://cerfacs.fr/coop/python-typing>
[^2]: <https://circleci.com/blog/testing-flask-framework-with-pytest/#c-consent-modal>
[^3]: [https://codethechange.stanford.edu/guides/guide_flask_unit_testing](https://codethechange.stanford.edu/guides/guide_flask_unit_testing.html)



## Setting up the Environment

### Using a Virtual Machine

Using the VirtualBox application, import the released OVA file and start the VM. No further preparation is required using the provided VM.

### Setting up the environment

If not using the published VM, the following steps can be followed to prepare the home environment.

1. Check if Python 3.7 or later is installed: `python3 --version` If not, install it according to the python tutorial provided.
2. Install the necessary Python packages
   - `tkinter`: This is usually pre-installed with Python. If not:
     - **macOS:** install the Tcl/Tk libraries using Homebrew: `brew install python-tk`
     - **Ubuntu:** install it with `sudo apt install python3-tk` or `sudo apt-get install python3-tk`
   - `Pillow`: Required for image processing, this will be installed inside the virtual environment.
   - `mypy`: Package used for static testing, this will installed inside the virtual environment.
   - `pytest`: Package for dynamic testing, this will installed inside the virtual environment.

```
brew info python-tk
==> python-tk@3.13: stable 3.13.1

brew info tcl-tk
==> tcl-tk: stable 9.0.1
```

3. Setting up a virtual environment

   - Create a virtual environment to manage dependencies:

```bash
python3 -m venv myenv # create
source myenv/bin/activate # activate
```

- Once activated, you will see (myenv) in the terminal prompt, indicating that you are now issuing commands from within it. If you give a name other than myenv, of course it will be displayed and you will have to look for the /bin folder inside it when activating.

4. Installing Python packages
   - Install the python package `Pillow` for image processing in the virtual environment: `pip3 install Pillow`
   - Install the python package `mypy` for static testing in the virtual environment: `pip3 install mypy`
   - Install the python package `pytest` for dynamic testing in the virtual environment: `pip3 install pytest`

### Setting up the code

Download the initial code from: https://software.crysys.hu/it-security/security-testing




## Tasks

The lab exercise consists of four tasks. The aim is to demonstrate and test both a static and a dynamic testing technique.  

### 1. Demo Task

Demonstrate the use of `mypy` and `pytest` through the example application.

Open the `demo.py` file. This file contains a simplified bank account class, where you can deposit money, withdraw money, and ensure the balance doesn't go negative. (At this point, we are not concerned with the correctness of the operations, such as allowing the balance to go negative when withdrawing money, etc.)

The 3 lines at the end of the file also demonstrate how to use the class.  

##### Using Mypy

- Run `mypy` to see that without type information, it doesn't provide any useful insights:

  ```bash
  mypy --config-file mypy.ini demo.py
  ```

- Add an obviously bad function call at the end of the file to show that this issue isn’t obvious from the code itself; it would only be caught at runtime. The bad function call should be:

  ```python
  account.deposit("test")
  ```

- Run mypy again to see that it doesn’t catch this problem:

  ```bash
  mypy --config-file mypy.ini demo.py
  ```

- Run mypy again to see that it doesn’t catch this problem:

- Let's provide the class with type information to improve the situation. By using the class definition and the example of usage, we can piece together the correct annotations. Don't forget to import the `Tuple` class to ensure it works properly! A correct solution for the annotation is:  

  ```python
  from typing import Tuple

      class BankAccount:

          def __init__(self,
                       owner: str,
                       account_number:Tuple[int, int, int],
                       initial_balance: int = 0) -> None:
              self.owner = owner
              self.account_number = account_number
              self.balance = initial_balance

          def deposit(self, amount: int) -> None:
              self.balance += amount

          def withdraw(self, amount: int) -> None:
              self.balance -= amount

          def is_overdrawn(self) -> bool:
              return self.balance < 0

          def __str__(self) -> str:
              return f"Balance of {self.owner} (account number: {self.account_number}) is: {self.balance}"
  ```

- Run `mypy` to check if everything has been entered correctly. If so, the check will run without errors:

  ```bash
  mypy --config-file mypy.ini demo.py
  ```

- Now, put the incorrect function call that was added earlier back into the code!

- Run `mypy` again to see that if everything is correct, `mypy` can find and report such an error:

  ```bash
  mypy --config-file mypy.ini demo.py
  ```

- Let's delete the erroneous line from the code now, so that it doesn't interfere with the rest of the process!

##### Using Pytest

Open the `test_demo.py` file:

- So far, we've been performing static analysis, but now we are moving on to dynamic testing, which means we will start running the code! (Until now, we've only used `mypy`, which just reads our code.)

- The class we've been working with so far is imported from the other file.

- There are 4 test cases in the file, each testing some functions related to the bank account.

- The first test is the `test_deposit` function:

  - The first line contains a log message to indicate which case is being tested.

  - Then, we create an example bank account for Uncle Scrooge with a balance of 10.

  - We deposit an additional 5 into the bank account.

  - For clarity, we print the current balance of the account (this line calls the `__str__` method in the background).

  - We check that after the operation, the account balance matches the expected value.

- The rest of the tests follow the same logic, but in the next ones, we withdraw money from the account and check whether the balance is positive or negative after each operation.

- Let's run the tests and see what happens and what information we see in the console:

  ```bash
  pytest test_demo.py
  ```

### 2. Task: Mypy

In this exercise, the objective is to use the `CIFF` class with type information that will aid in its development. To do this, follow these steps:

- In the `ciff.py` file, specify the parameters and variables of the `__init__` function along with its variables using type information (including the setter and getter functions).

- From the project folder, run the `mypy` application with the following command:

  ```bash
  mypy --config-file mypy.ini src/ciff.py
  ```

- If mypy reports an error, fix the code until the check runs without errors.

**Moodle question**: After fixing the error reported by mypy, what is the correct line?

### 3. Task: Pytest

In this exercise, you will perform dynamic testing using `pytest`. The test is performed using the dynamic test function `pytest`:

- First, run the tests and evaluate whether the output is correct or incorrect. Tests can be run in this way:

  ```bash
  pytest test_ciff.py
  ```

- As you can see, one test returned an incorrect result: `invalid5.ciff` file. The application considered the file to be correct when, in fact, it is not. Correct the error in the file `ciff.py` by implementing the missing check based on the error message.

- Re-run the tests and check that all the tests now give correct results!

**Moodle question**: Which three member variables should have been checked to solve the incorrect test? (Give only the names of the variables, separated by commas. For example: `a, b, c`)

### 4. Task: Pytest Cont'd

As a continuation of the previous task:

- Examine the log messages generated during the execution of the test cases. Follow the steps described in the next section. If you find any case that returns a correct result but is not correct, or if you find a case that runs correctly but does not actually work, look for the problem in the `ciff.py` file and implement additional checks.

- After fixing all the problems, run the `pytest` command in the same way as before, and check that all tests are now working correctly!

**Moodle question**: What is the name of the `.ciff` file which, although it gives correct test results, runs incorrectly?
