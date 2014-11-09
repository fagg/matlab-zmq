## Coding Guidelines

* Use 4 spaces for identation
* Variables should be javaCase with functions_underscored_like_so

### C Code

* [K&R style](http://en.wikipedia.org/wiki/Indent_style#K.26R_style) for bracing
* Traditional C style for comments (i.e. `/*` and `*/`) no matter how long they are.
* One-line conditionals are discouraged, e.g.
```c
/* bad: */
if (this) do_that();
else do_something_else();

/* good: */
if (this)
    do_that();
else
    do_something_else();

/* good: */
if (this) {
    do_that();
} else {
    do_something_else();
}
```

### MATLAB Code

* Condition statements must be wrapped in braces.
  This makes the code easier to read in that it makes control flow stand out, e.g.
```matlab
if (foo == 1) % rather than if foo == 1
```
* One-line conditionals are discouraged, e.g.
```matlab
% bad:
if (this), do_that(), end;

% good:
if (this)
   do_that();
end
```
