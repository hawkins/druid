# Goals
1. Informative, helpful, and intuitive compiler errors.
1. Reasonable predictability from source code to low-level implementation.
1. C-like syntax, control, and power. Useful additions without the clutter of C++.
1. Native compilation to native && web assemblies.

# Things wrong with C
- Things decay to pointers too quickly (i.e. struct parameters to functions and sizeof()).
- struct.something_i_want vs struct->something_i_want is unnecessary.
- No "official" string library.
- No "official" build system, creating need for numerous build/configuration programs each of which usually have complicated syntax of their own.

# Example Syntax
```C
#summon stdio
#summon math
#summon dataCube

status entry_point()
{
    if(g:count)
    {
        // do stuff
    }
    
    stdout("Hello world!\n");
    stdout("Fibb of %d: %d\n", 9, math::fib(9));
    
    data_cube = conjure(DataCube);
    data_cube:print_data();
    banish(data_cube);

    return(status:success);
}
```

# g:
Global state available at the entry point of a program, largely taking the place of `int argc, char** argv` and (hopefully) including certain environment variables, directory && OS information, etc.