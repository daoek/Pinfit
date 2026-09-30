# MISRA-oriented generated C

Pinfit uses MISRA C:2012 as a guideline, not as a compliance target: the generated skeleton
follows most of its rules where that costs nothing, so a MISRA review starts from a clean base. This
page explains what that means concretely, and — just as importantly — what it does not mean.

## What the generator does

### Single point of exit

Generated functions compute into `pinfit_result` and return once, at the end:

```c title="drivers/Interface/common_iic_I.h (excerpt)"
static inline common_iic_status_t common_iic_write(const common_iic_interface_t * const interface, uint32_t length)
{
    common_iic_status_t pinfit_result = COMMON_IIC_INVALID_PARAM;

    if (interface != NULL)
    {
        if ((interface->context != NULL) && (interface->write != NULL))
        {
            pinfit_result = interface->write(interface->context, length);
        }
        else
        {
            pinfit_result = COMMON_IIC_NOT_INITIALIZED;
        }
    }

    return pinfit_result;
}
```

In a non-`void` user region, **assign to `pinfit_result`** rather than returning early, so the
function keeps its single return.

### Pointers are checked where they cross an interface

The dispatch wrapper above checks the interface, its context and the function pointer before
calling through; a bind function checks its `interface` argument before writing to it; a StateSmith
machine's public functions check `context`. An interface implementation can rely on this: it is
only reached through a wrapper that already checked the context.

!!! warning "Not every public function checks its `context`"

    Functions that take their own module's context directly do **not** test it for `NULL` before
    using it: an observer's `_init`, `_subscribe`, `_unsubscribe` and `_publish_*`, a builtin state
    machine's `_init`, `_tick`, `_go_to_state` and `_on_<EVENT>`, and an adapter's `_set_target`.
    Passing a null context to one of them is undefined behaviour, the same as for a hand-written C
    function with that signature. Check it at the call site, or record it for your checker.

### No silent invalid return values

Every non-`void` interface function should supply `invalidReturn` - on the function, on
`invalidReturns` for its return type, or as the interface-level default. A fabricated `-1` is not a
valid value of an enum, a pointer, or an unsigned type, so Pinfit never spreads one scalar across
unrelated return types. Name nothing at all and the guard falls back to a zero initializer,
`(flash_command_t){0}`: it compiles for any type, but for an enum whose `0` value means success it
reports success from a failed guard. Name a real sentinel for those.
`uninitializedReturn` covers the "nothing bound yet" case in the same way.

### Unused parameters are consumed explicitly

Stub bodies you have not filled in yet still compile cleanly:

```c title="ra_iic.c (excerpt)"
    (void)module;
    (void)slave_address;
```

Set [`format.suppressUnusedWarnings: false`](project-configuration.md#suppressunusedwarnings) if
your standard forbids those casts.

### Braces and blocks

Every `if`, `else` and loop body that Pinfit generates is braced, including single-statement
bodies. A `case` that holds a user region gets its own `{ ... }` block with an explicit `break;` —
every `@PinfitSwitch` case, every state-machine event and tick case, and a command table's `default`.
The purely mechanical switches — a builtin state machine's `_go_to_state()` and a command table's
opcode dispatch — use unbraced `case X: call(); break;` bodies.

## What it does not mean

!!! warning "Compliance is a property of the translation unit, not of the generator"

    MISRA compliance applies to the **complete** translation unit — your configured types, your
    expressions, your includes, and everything inside your user regions. Pinfit generating
    conforming skeletons does not make the resulting file conforming.

    Confirm it with your project's MISRA checker and deviation policy, the same as any other
    source file.

### The known deviation: Rule 11.5

The generic interface pattern converts the `void *context` of the function-pointer table to the
concrete module context type:

```c title="ra_iic.c (excerpt)"
static common_iic_status_t ra_iic_common_iic_write(void *context, uint32_t length)
{
    ra_iic_context_t *module = (ra_iic_context_t *)context;   /* conversion from void* */
```

That is the mechanism by which one interface serves any number of implementations without the
interface knowing any of them. Projects enforcing advisory **Rule 11.5** (a conversion from a
pointer to object to a pointer to a different object type) need to record this as a **design
deviation**, once, for the generated dispatch pattern.

It is the only deviation inherent to the generated structure. Everything else a checker flags is
either in your user regions or in the types you chose.

## The StateSmith engine is out of scope

None of the above applies to [`engine: statesmith`](../generators/state-machine.md#the-statesmith-engine)
state machines' `door_sm.h`/`door_sm.c` - that code comes from StateSmith, a separate tool with its
own generation style, and needs its own review and deviations if your project requires MISRA
compliance. Pinfit's MISRA-oriented claims cover only what Pinfit itself generates: for a statesmith
machine, that's `door.h`/`door.c` and `door_hooks.h`/`door_hooks.c`, generated the same
single-return, null-checked way as everything else on this page.

## Practical review advice

- **Keep the YAML in review.** Reviewers can read one spec file instead of three generated ones,
  and the generated diff shows exactly what the change produced.
- **Run the checker on generated output, not on the YAML.** Pinfit has no view of your checker's
  configuration or deviations.
- **Re-run after changing `format`.** `indent`, `functionNaming` and `publicVariables` change every
  generated file, so re-baseline any checker report that records line numbers.
