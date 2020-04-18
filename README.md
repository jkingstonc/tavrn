# Tavrn - Welcome to the tavrn 'me laddies!
Tavrn is a compiled programming langage

# Philosophy:
Its's hard to find a language that is fast, provides low level access and most importantly... easy to use! C++ is good for large projects requiring speed, but is so hard to use: cryptic error messages, endless errors. Java is fast to use, but is restrictive: It has such an emphasis on OOP that it becomes a struggle to write procedural programs. 

An important part of the philosophy is safety, I believe that the programmer should not have restrictions imposed upon them by the language itself. It is the programmers job to manage the memory, if they wish to write a memory-safety library then they can! (C++ smart pointers). Cough Cough Rust... Although their ownership system is great, it imposes restrictions on the programmer and to be honest is hard to learn for newbies.

The goal of this project is to develop an easy-to-use, procedural language that allows the programmer to write fast, efficient code.


# Desirable Features:

# Syntax:
## Variables:
Variables are assigned using the  `var` keyword

```
var x : i32;    // Create a variable x, initialised to 0
var x : i32 5;  // Create a variable x, initialised to 5
var x : 5;      // Create a variable x, and infer the type of 5
```

## Functions
```
fun add : i32 (a : i32, b : i32){ 
    return a + b;
}
```
What happens if we want default arguments? Well we can provide default values and when we call the function, we can specify which arguments we want (We can also let the compiler infer this).
```
fun add : i32 (a : i32 5, b : i32 10){
    return a + b;
}

add(1);         // Returns 1 + 10 = 11
add(a=1, b=2);  // Returns 1 + 2 = 3
```
We can also call functions without passing paramaters!
```
add;            // Returns 5 + 10 = 15
```



## Structs
The language is a procedural language and so will have NO objects. Structures can however object-like methods but these are not stored in memory near the structure.

```
struct vec : {
    x : i32;
    y : i32;
    z : i32;
    
    add : vec (other : vec) {
        return vec(this.x + vec.x, this.y + vec.y, this.z + vec.z); 
    }
}
```
What happens if a struct member is another struct, such as a linked list?
```
struct node : {
    next :! node null;
}
```

We use the `!` symbol to indicate that this is strictly assigned, meaning when the node is deleted (either off the stack or the heap), then all its members are also deleted. We can also provide the member with a default value if not initialised, here we use `null`.

We also may need to tell the compiler that a value 


## Pointers


## Strict assignment
As noted earlier, we can tell variables that when they are deleted (either off the stack or heap), then their members should.

Here, we create a variable x and strictly assign it a heap allocated vector. When x goes out of scope, the vector will automatically be deleted
```
{
    var x :! new vec;
}
```
Although this seems like it violates my philosphoy of memory freedom, it does't. as x is a primitive and thus a stack assigned variable, we know it will be de-allocated from the stack when the scope ends. And becuase strict allocation works with either the stack or the heap, the deallocation of the variable triggers the deallocation of the vector.

## Avoiding Null Pointers
Say we want to dereference a pointer, however we can't guarantee it's null, what do we do? We mark the variable that could be null with a `?`

```
fun couldBeNull : (v :? *vector){

}
```

## Exception handling
Try-catches are clunky and cause excessive code writing, they have their place when needing to handle many-functions-deep errors but not for simple functions. We know this may cause an exception below, we need to handle it.
```
fun causesException : string (){
    return io::readFile("log.txt");
}
var file : causesException;     // Will return an exception struct if needed
```

We can then handle this exception below. The `~` operator indicates the variable may be an error, the compiler needs to know this so it can wrap the result with an exception struct. When calling the function that causes the exception, we then indicate again that this result could be an error. The `#catch` dev-directive will call the exception handler if the file variable is wrapped with an exception. Note, the catch directive will do the following: If the file contains a valid error, it will hadle it. If not, it will assign the proper value back to `file` so we don't get any unwanted side effects with `file` not being a string. Essentially the compiler will cast the file to the proper value.
```
fun causesException :~ string (){
    return io::readFile("log.txt");
}

fun exceptionHandler : (e : exception) {
    io::println(e.message);
}

var file :~ causesexception;
#catch(file, @exceptionHandler);
```

## Minor stuff

### Wildcard Type

### Type Checking
