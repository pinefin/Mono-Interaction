# Single Header Mono Interaction

These docs are using a offline/test version of Escape From Tarkov as an example. 

This was not developed on the live version of the game.

## Configuration

### ENCRYPT_STR
Used for encrypting strings. Made streamlined with just a single line that needs to be swapped out.

If you intend to encrypt your strings, uncomment and swap the macro with your own!


### GET_MODULE_BASE `GET_MODULE_BASE(str)`
Used for getting a module's base address.

Swap this out to your custom function for getting a module base.


### GET_EXPORT_ADDR `GET_EXPORT_ADDR(base, str)`
Used for getting a export fromm a provided module base address.

Swap this out to your custom function for getting a export in a module.

## Examples/Usage

### Getting the root domain

We need the domain to attach thread, get base images etc.
This is how we get it.

If you have ATTACH_THREAD_AUTOMATICALLY, it will attach the thread upon calling this function.

```auto Domain = Mono_Domain::GetRootDomain();```


### Attaching the thread

Attaches you to the mono thread to safely call mono functions without issues.

```Domain->AttachThread();```


### Opening a module

Opening a module for us to use.

```auto AssemblyCSharp_Assembly = Domain->OpenAssembly("Assembly-CSharp"); //Adding .dll is not needed```

### Getting the image of an assembly

Gets the image base of the module that we just opened.

```auto AssemblyCSharp = AssemblyCSharp_Assembly->GetImage();```

### Getting a Class

Getting a class for us to use

```auto GameWorldClass = AssemblyCSharp->GetClassFromName("EFT", "GameWorld");```

### Getting a method object

Getting the method object, this is NOT a callable address, it's an object.

`auto GameWorldUpdate_Uncompiled = GameWorldClass->GetMethod("Update", -1);`

### Getting the actual method

This will compile the method. After compiling, it will be a callable address.
If a method is already JIT compiled (from either the game or yourself) it won't be compiled anymore.

`auto GameWorldUpdate = GameWorldUpdate_Uncompiled->CompileMethod<void(*)(uint64_t GameWorld>()`

You can also do...

`auto GameWorldUpdate = GameWorldClass->GetCompiledMethod<void(*)(uint64_t GameWorld)>("Update", -1);`

